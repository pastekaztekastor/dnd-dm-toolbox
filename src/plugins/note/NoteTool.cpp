#include "NoteTool.h"
#include "../../include/core/Logger.h"
#include "../../include/core/EventBus.h"
#include "../../include/ui/MarkdownRenderer.h"
#include <imgui.h>
#include <nlohmann/json.hpp>
#include <chrono>
#include <algorithm>
#include <cstring>

NoteTool::NoteTool() {
    // Constructeur
    memset(noteBuffer, 0, sizeof(noteBuffer));
}

NoteTool::~NoteTool() {
    // Destructeur
}

// ============================================================
// LIFECYCLE
// ============================================================

void NoteTool::OnCreate() {
    // Appelé quand l'instance est créée
    // TODO: S'abonner aux événements, initialiser des ressources, etc.

    // Exemple: S'abonner à un événement
    // if (eventBus) {
    //     eventBus->Subscribe("some.event", [this](const Core::Event& e) {
    //         OnEvent(e);
    //     });
    // }
}

void NoteTool::OnDestroy() {
    // Appelé quand l'instance est détruite
    // TODO: Nettoyer les ressources, se désabonner, etc.
}

void NoteTool::OnSave(sqlite3* db) {
    // Sauvegarder l'état de cette instance dans le fichier .dnd
    if (!db) return;

    nlohmann::json state;
    state["note_content"] = std::string(noteBuffer);

    // Utiliser le tool_type comme ID unique (une seule note par campagne)
    std::string noteID = "note_singleton";

    // Sauvegarder dans la DB
    const char* sql = "INSERT OR REPLACE INTO tool_instances (id, tool_type, data) VALUES (?, ?, ?)";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, noteID.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, GetID().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, state.dump().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
}

void NoteTool::OnLoad(sqlite3* db) {
    // Charger l'état depuis le fichier .dnd
    if (!db) return;

    // Utiliser le même ID singleton
    std::string noteID = "note_singleton";

    const char* sql = "SELECT data FROM tool_instances WHERE id = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, noteID.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* jsonStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));

            if (jsonStr && strlen(jsonStr) > 0) {
                auto state = nlohmann::json::parse(jsonStr);

                if (state.contains("note_content")) {
                    std::string content = state["note_content"];
                    strncpy(noteBuffer, content.c_str(), sizeof(noteBuffer) - 1);
                }
            }
        }

        sqlite3_finalize(stmt);
    }
}

// ============================================================
// RENDERING
// ============================================================

static void RenderNoteInput(const char* id, char* buf, size_t bufSize,
                            ImVec2 size, NoteTool* tool, bool needFocus) {
    if (needFocus) ImGui::SetKeyboardFocusHere(0);
    ImGui::InputTextMultiline(id, buf, bufSize, size,
                              ImGuiInputTextFlags_CallbackEdit,
                              NoteTool::InputCb, tool);
}

void NoteTool::Render() {
    if (!isOpen) return;
    if (!acLoaded) LoadCompletions();

    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);

    if (ImGui::Begin(GetWindowTitle().c_str(), &isOpen)) {
        if (ImGui::Button(previewMode ? "Éditer" : "Aperçu"))
            previewMode = !previewMode;
        ImGui::Separator();

        ImVec2 avail = ImGui::GetContentRegionAvail();
        bool needFocus = acApply;  // si on va insérer, on remettra le focus après

        if (!previewMode) {
            RenderNoteInput("##note_content", noteBuffer, sizeof(noteBuffer), avail, this, needFocus);
        } else {
            float half = (avail.x - ImGui::GetStyle().ItemSpacing.x) * 0.5f;
            ImGui::BeginChild("##note_edit", ImVec2(half, avail.y), true);
            RenderNoteInput("##note_content2", noteBuffer, sizeof(noteBuffer), {-1,-1}, this, needFocus);
            ImGui::EndChild();
            ImGui::SameLine();
            ImGui::BeginChild("##note_preview", ImVec2(half, avail.y), true);
            UI::RenderMarkdown(std::string(noteBuffer), [this](const std::string& plugin, const std::string& alias) {
                PublishEvent(plugin + ".open", {{"alias", alias}});
            });
            ImGui::EndChild();
        }

        // ---- Popup d'autocomplétion ----
        ImVec2 inputMin = ImGui::GetItemRectMin();
        ImVec2 inputMax = ImGui::GetItemRectMax();

        // Filtrer les suggestions
        std::vector<const AcItem*> matches;
        if (acActive) {
            std::string filterLow = acFilter;
            std::transform(filterLow.begin(), filterLow.end(), filterLow.begin(), ::tolower);
            for (auto& item : acItems) {
                std::string tokenLow = item.token;
                std::transform(tokenLow.begin(), tokenLow.end(), tokenLow.begin(), ::tolower);
                if (filterLow.empty() || tokenLow.find(filterLow) != std::string::npos)
                    matches.push_back(&item);
            }
        }

        if (acActive && !matches.empty()) {
            // Position approximative : ligne du @ × hauteur de ligne
            float lineH = ImGui::GetTextLineHeightWithSpacing();
            int lineNum = 0;
            for (int i = 0; i < acAtPos && noteBuffer[i]; ++i)
                if (noteBuffer[i] == '\n') ++lineNum;
            float py = inputMin.y + ImGui::GetStyle().FramePadding.y + (lineNum + 1) * lineH;
            py = std::min(py, inputMax.y - 10.0f);  // ne pas déborder en bas

            ImGui::SetNextWindowPos({inputMin.x + ImGui::GetStyle().FramePadding.x, py},
                                    ImGuiCond_Always);
            ImGui::SetNextWindowSizeConstraints({220, 0}, {400, 220});
            ImGui::OpenPopup("##ac_note");
        }

        if (ImGui::BeginPopup("##ac_note",
                ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav |
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove)) {
            if (!acActive || matches.empty()) {
                ImGui::CloseCurrentPopup();
            } else {
                for (size_t i = 0; i < matches.size() && i < 10; ++i) {
                    if (ImGui::Selectable(matches[i]->label.c_str())) {
                        acApplyText = matches[i]->token;
                        acApply     = true;
                        acActive    = false;
                        ImGui::CloseCurrentPopup();
                    }
                }
            }
            ImGui::EndPopup();
        }
    }

    ImGui::End();
}

// ============================================================
// EVENTS
// ============================================================

void NoteTool::OnEvent(const Core::Event& event) {
    // Appelé quand un événement auquel on est abonné est émis
    // TODO: Traiter les événements

    // Exemple:
    // if (event.type == "some.event") {
    //     std::string data = event.data["key"];
    //     // Faire quelque chose avec data
    // }
}

// ============================================================
// MÉTHODES PRIVÉES
// ============================================================

// TODO: Implémenter vos méthodes privées ici

// ============================================================
// EXPORT DU PLUGIN (C ABI) - NE PAS MODIFIER
// ============================================================

extern "C" {
    PLUGIN_API Core::ToolBase* CreateTool() {
        return new NoteTool();
    }

    PLUGIN_API void DestroyTool(Core::ToolBase* tool) {
        delete tool;
    }
}

// ============================================================
// AUTOCOMPLÉTION
// ============================================================

void NoteTool::LoadCompletions() {
    acLoaded = true;
    acItems.clear();
    auto res = CallService("race.getAll", {});
    if (res && res->contains("races")) {
        for (auto& r : (*res)["races"]) {
            std::string alias = r.value("alias", "");
            std::string nom   = r.value("nom",   "");
            if (!alias.empty())
                acItems.push_back({"race." + alias, "@race." + alias + "  — " + nom});
        }
    }
}

int NoteTool::InputCb(ImGuiInputTextCallbackData* data) {
    auto* tool = static_cast<NoteTool*>(data->UserData);
    if (data->EventFlag != ImGuiInputTextFlags_CallbackEdit) return 0;

    // Insérer le token sélectionné : supprimer @filter, insérer @token
    if (tool->acApply) {
        tool->acApply = false;
        int end = tool->acAtPos + 1 + (int)tool->acFilter.size();
        if (tool->acAtPos >= 0 && end <= data->BufTextLen) {
            data->DeleteChars(tool->acAtPos, end - tool->acAtPos);
            std::string ins = "@" + tool->acApplyText;
            data->InsertChars(tool->acAtPos, ins.c_str());
        }
        tool->acActive = false;
        return 0;
    }

    // Détecter si on est en train de taper @xxx
    int cursor = data->CursorPos;
    tool->acAtPos = -1;
    for (int i = cursor - 1; i >= 0; --i) {
        char c = data->Buf[i];
        if (c == '@')  { tool->acAtPos = i; break; }
        if (c == ' ' || c == '\n' || c == '\t') break;
    }
    tool->acActive = (tool->acAtPos >= 0);
    if (tool->acActive)
        tool->acFilter = std::string(data->Buf + tool->acAtPos + 1, cursor - tool->acAtPos - 1);
    else
        tool->acFilter.clear();
    return 0;
}

// ============================================================
// HELPERS
// ============================================================

double NoteTool::GetCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration<double>(duration).count();
}
