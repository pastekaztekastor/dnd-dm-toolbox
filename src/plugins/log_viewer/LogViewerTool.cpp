#include "LogViewerTool.h"
#include "../../include/core/Logger.h"
#include <imgui.h>
#include <nlohmann/json.hpp>

LogViewerTool::LogViewerTool() {
}

LogViewerTool::~LogViewerTool() {
}

void LogViewerTool::OnCreate() {
    // Rien de spécial à initialiser
}

void LogViewerTool::OnDestroy() {
}

void LogViewerTool::OnSave(sqlite3* db) {
    if (!db) return;

    // Sauvegarder les préférences de filtrage
    nlohmann::json state;
    state["max_logs"] = maxLogsToDisplay;
    state["auto_scroll"] = autoScroll;
    state["filter_tool_name"] = std::string(filterToolName);

    // ID singleton pour log_viewer
    std::string logViewerID = "log_viewer_singleton";

    const char* sql = "INSERT OR REPLACE INTO tool_instances (id, tool_type, data) VALUES (?, ?, ?)";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, logViewerID.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, GetID().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, state.dump().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
}

void LogViewerTool::OnLoad(sqlite3* db) {
    if (!db) return;

    // ID singleton pour log_viewer
    std::string logViewerID = "log_viewer_singleton";

    // Charger les préférences
    const char* sql = "SELECT data FROM tool_instances WHERE id = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, logViewerID.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* jsonStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));

            if (jsonStr && strlen(jsonStr) > 0) {
                auto state = nlohmann::json::parse(jsonStr);

                if (state.contains("max_logs")) maxLogsToDisplay = state["max_logs"];
                if (state.contains("auto_scroll")) autoScroll = state["auto_scroll"];
                if (state.contains("filter_tool_name")) {
                    std::string filter = state["filter_tool_name"];
                    strncpy(filterToolName, filter.c_str(), sizeof(filterToolName) - 1);
                }
            }
        }

        sqlite3_finalize(stmt);
    }
}

void LogViewerTool::Render() {
    if (!isOpen) return;

    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

    if (ImGui::Begin(GetWindowTitle().c_str(), &isOpen)) {
        // Debug: Vérifier que le logger est disponible
        if (!logger) {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "ERREUR: Logger non initialisé !");
            ImGui::TextWrapped("Le système de log n'a pas été injecté dans ce plugin.");
            ImGui::End();
            return;
        }

        RenderFilters();
        ImGui::Separator();
        RenderLogList();
    }

    ImGui::End();
}

void LogViewerTool::RenderFilters() {
    ImGui::Text("Filtres:");
    ImGui::SameLine();

    // Nombre de logs à afficher
    ImGui::SetNextItemWidth(100);
    ImGui::InputInt("##max_logs", &maxLogsToDisplay);
    if (maxLogsToDisplay < 10) maxLogsToDisplay = 10;
    if (maxLogsToDisplay > 10000) maxLogsToDisplay = 10000;

    ImGui::SameLine();
    ImGui::Text("logs");

    ImGui::SameLine(0, 20);

    // Auto-scroll
    ImGui::Checkbox("Auto-scroll", &autoScroll);

    // Filtrer par outil
    ImGui::Checkbox("Filtrer par outil", &filterByTool);
    if (filterByTool) {
        ImGui::SameLine();
        ImGui::SetNextItemWidth(200);
        ImGui::InputText("##filter_tool", filterToolName, sizeof(filterToolName));
    }

    // Bouton pour effacer les filtres
    ImGui::SameLine();
    if (ImGui::Button("Réinitialiser")) {
        filterByTool = false;
        filterToolName[0] = '\0';
        maxLogsToDisplay = 100;
        autoScroll = true;
    }
}

void LogViewerTool::RenderLogList() {
    if (!logger) {
        ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "⚠️  Logger non disponible");
        ImGui::TextWrapped("Le système de log n'a pas pu être initialisé.");
        return;
    }

    // Récupérer les logs
    std::vector<Core::LogEntry> logs;
    try {
        logs = logger->GetRecentLogs(maxLogsToDisplay);
    } catch (const std::exception& e) {
        ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "Erreur lors de la récupération des logs:");
        ImGui::TextWrapped("%s", e.what());
        return;
    }

    // Appliquer le filtre si actif
    std::vector<Core::LogEntry> filteredLogs;
    if (filterByTool && strlen(filterToolName) > 0) {
        std::string filterStr(filterToolName);
        for (const auto& log : logs) {
            if (log.toolType.find(filterStr) != std::string::npos) {
                filteredLogs.push_back(log);
            }
        }
    } else {
        filteredLogs = logs;
    }

    // Afficher le nombre de logs
    ImGui::Text("%zu log(s) affiché(s)", filteredLogs.size());

    ImGui::Separator();

    // Si aucun log, afficher un message
    if (filteredLogs.empty()) {
        ImGui::TextDisabled("Aucun log à afficher.");
        ImGui::TextWrapped("Les logs apparaîtront ici au fur et à mesure que vous utilisez les outils.");
        return;
    }

    // Zone scrollable
    if (ImGui::BeginChild("LogScroll", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar)) {
        for (const auto& log : filteredLogs) {
            // Couleur selon le type d'action
            ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Blanc par défaut

            if (log.actionType.find("error") != std::string::npos) {
                color = ImVec4(1.0f, 0.3f, 0.3f, 1.0f); // Rouge
            } else if (log.actionType.find("warning") != std::string::npos) {
                color = ImVec4(1.0f, 0.8f, 0.3f, 1.0f); // Jaune
            } else if (log.actionType.find("roll") != std::string::npos) {
                color = ImVec4(0.3f, 0.8f, 1.0f, 1.0f); // Bleu
            }

            ImGui::PushStyleColor(ImGuiCol_Text, color);
            ImGui::TextWrapped("%s", log.displayText.c_str());
            ImGui::PopStyleColor();

            // Tooltip avec détails
            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::Text("Outil: %s", log.toolType.c_str());
                ImGui::Text("Instance: %s", log.instanceID.c_str());
                ImGui::Text("Action: %s", log.actionType.c_str());
                ImGui::Text("Timestamp: %.2f", log.timestamp);
                if (!log.data.is_null() && !log.data.empty()) {
                    ImGui::Separator();
                    ImGui::Text("Données:");
                    try {
                        ImGui::TextWrapped("%s", log.data.dump(2).c_str());
                    } catch (...) {
                        ImGui::Text("[Erreur d'affichage des données]");
                    }
                }
                ImGui::EndTooltip();
            }
        }

        // Auto-scroll vers le bas
        if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0f);
        }
    }
    ImGui::EndChild();
}

// Export du plugin
extern "C" {
    PLUGIN_API Core::ToolBase* CreateTool() {
        return new LogViewerTool();
    }

    PLUGIN_API void DestroyTool(Core::ToolBase* tool) {
        delete tool;
    }
}
