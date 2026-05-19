#include "NoteTool.h"
#include "../../include/core/Logger.h"
#include "../../include/core/EventBus.h"
#include <imgui.h>
#include <nlohmann/json.hpp>
#include <chrono>

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

void NoteTool::Render() {
    if (!isOpen) return;

    // Définir la taille par défaut de la fenêtre
    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);

    if (ImGui::Begin(GetWindowTitle().c_str(), &isOpen)) {
        // Zone de texte multiligne avec retour à la ligne automatique
        ImVec2 textSize = ImVec2(-1, -1);  // Prend tout l'espace disponible
        ImGuiInputTextFlags flags = ImGuiInputTextFlags_WordWrap;
        ImGui::InputTextMultiline("##note_content", noteBuffer, sizeof(noteBuffer), textSize, flags);
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
// HELPERS
// ============================================================

double NoteTool::GetCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration<double>(duration).count();
}
