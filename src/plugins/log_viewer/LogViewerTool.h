#pragma once

#include "../../include/core/ToolBase.h"
#include <string>
#include <vector>

/**
 * @brief Log Viewer - Affichage du journal global de la campagne
 *
 * Ce plugin affiche tous les logs de tous les autres plugins.
 * C'est un outil central qui permet de voir l'historique complet de la session.
 */
class LogViewerTool : public Core::ToolBase {
public:
    LogViewerTool();
    ~LogViewerTool() override;

    // Interface ToolBase
    std::string GetID() const override { return "log_viewer"; }
    std::string GetName() const override { return "Journal de Campagne"; }
    std::string GetCategory() const override { return "Affichage"; }
    std::string GetDescription() const override { return "Affiche l'historique complet des actions"; }

    void OnCreate() override;
    void OnDestroy() override;
    void OnSave(sqlite3* db) override;
    void OnLoad(sqlite3* db) override;
    void Render() override;

private:
    // Filtres
    bool filterByTool = false;
    char filterToolName[256] = "";
    int maxLogsToDisplay = 100;
    bool autoScroll = true;

    // UI helpers
    void RenderFilters();
    void RenderLogList();
};
