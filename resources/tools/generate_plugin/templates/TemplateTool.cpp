#include "PLUGIN_CLASS_NAMETool.h"
#include "../../include/core/Logger.h"
#include "../../include/core/EventBus.h"
#include <imgui.h>
#include <nlohmann/json.hpp>
#include <chrono>
SOURCE_REPO_INCLUDE_LINE

PLUGIN_CLASS_NAMETool::PLUGIN_CLASS_NAMETool() {
    // Constructeur
    // TODO: Initialiser vos variables si nécessaire
}

PLUGIN_CLASS_NAMETool::~PLUGIN_CLASS_NAMETool() {
    // Destructeur
}

// ============================================================
// LIFECYCLE
// ============================================================

void PLUGIN_CLASS_NAMETool::OnCreate() {
    // Appelé quand l'instance est créée
    // TODO: S'abonner aux événements, initialiser des ressources, etc.

ONCREATE_DB_INIT_LINE
    // Exemple: S'abonner à un événement
    // if (eventBus) {
    //     eventBus->Subscribe("some.event", [this](const Core::Event& e) {
    //         OnEvent(e);
    //     });
    // }
}

void PLUGIN_CLASS_NAMETool::OnDestroy() {
    // Appelé quand l'instance est détruite
    // TODO: Nettoyer les ressources, se désabonner, etc.
ONDESTROY_DB_CLOSE_LINE
}

void PLUGIN_CLASS_NAMETool::OnSave(sqlite3* db) {
    // Sauvegarder l'état de cette instance dans le fichier .dnd
    // TODO: Créer un JSON avec vos données et le sauvegarder

    if (!db) return; // Sécurité: vérifier que la DB existe

    nlohmann::json state;
    // TODO: Remplir le JSON avec vos données
    // state["my_data"] = myData;
    // state["my_counter"] = myCounter;

    // Sauvegarder dans la DB
    const char* sql = "INSERT OR REPLACE INTO tool_instances (id, tool_type, data) VALUES (?, ?, ?)";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, instanceID.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, GetID().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, state.dump().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
}

void PLUGIN_CLASS_NAMETool::OnLoad(sqlite3* db) {
    // Charger l'état depuis le fichier .dnd
    // TODO: Récupérer le JSON et restaurer vos données

    if (!db) return; // Sécurité: vérifier que la DB existe

    const char* sql = "SELECT data FROM tool_instances WHERE id = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, instanceID.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* jsonStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));

            // Sécurité: vérifier que le JSON n'est pas null avant de parser
            if (jsonStr && strlen(jsonStr) > 0) {
                auto state = nlohmann::json::parse(jsonStr);

                // TODO: Restaurer vos données depuis le JSON
                // if (state.contains("my_data")) {
                //     myData = state["my_data"];
                // }
            }
        }

        sqlite3_finalize(stmt);
    }
}

// ============================================================
// RENDERING
// ============================================================

void PLUGIN_CLASS_NAMETool::Render() {
    if (!isOpen) return;

    // Définir la taille par défaut de la fenêtre
    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);

    if (ImGui::Begin(GetWindowTitle().c_str(), &isOpen)) {
        // TODO: Implémenter votre interface ImGui ici

        ImGui::TextWrapped("PLUGIN_DESCRIPTION");
        ImGui::Separator();
        ImGui::Spacing();

        // Interface temporaire
        ImGui::Text("Coming Soon...");
        ImGui::Spacing();
        ImGui::TextDisabled("Ce plugin est en cours de développement.");

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // TODO: Ajouter votre UI ici
        // - ImGui::Text() pour du texte
        // - ImGui::Button() pour des boutons
        // - ImGui::InputText() pour des champs texte
        // - ImGui::BeginChild() / ImGui::EndChild() pour des zones scrollables
        // - ImGui::BeginTable() / ImGui::EndTable() pour des tableaux
        // - etc.

        // Exemple: Bouton simple (décommenter pour tester)
        // if (ImGui::Button("Action")) {
        //     // TODO: Faire quelque chose
        //
        //     // Exemple: Logger une action (vérifier que logger existe)
        //     if (logger) {
        //         Log("action_performed", {{"info", "test"}});
        //     }
        //
        //     // Exemple: Publier un événement (vérifier que eventBus existe)
        //     if (eventBus) {
        //         PublishEvent("PLUGIN_ID.action_done", {{"success", true}});
        //     }
        // }
    }

    ImGui::End();
}

// ============================================================
// EVENTS
// ============================================================

void PLUGIN_CLASS_NAMETool::OnEvent(const Core::Event& event) {
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
        return new PLUGIN_CLASS_NAMETool();
    }

    PLUGIN_API void DestroyTool(Core::ToolBase* tool) {
        delete tool;
    }
}

// ============================================================
// HELPERS
// ============================================================

double PLUGIN_CLASS_NAMETool::GetCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration<double>(duration).count();
}
