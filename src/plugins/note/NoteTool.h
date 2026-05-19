#pragma once

#include "../../include/core/ToolBase.h"
#include <string>
#include <vector>

/**
 * @brief Note - Bloque note de votre aventure au format texte (futurement au format md)
 *
 * Fonctionnalités:
 * Simple bloc note en texte
 */
class NoteTool : public Core::ToolBase {
public:
    NoteTool();
    ~NoteTool() override;

    // ============================================================
    // Interface ToolBase (obligatoire)
    // ============================================================

    std::string GetID() const override { return "note"; }
    std::string GetName() const override { return "Note"; }
    std::string GetCategory() const override { return "Outils"; }
    std::string GetDescription() const override { return "Bloque note de votre aventure au format texte (futurement au format md)"; }

    void OnCreate() override;
    void OnDestroy() override;
    void OnSave(sqlite3* db) override;
    void OnLoad(sqlite3* db) override;
    void Render() override;

    // ============================================================
    // Events (optionnel)
    // ============================================================

    void OnEvent(const Core::Event& event) override;

private:
    // ============================================================
    // État du plugin
    // ============================================================

    char noteBuffer[1024 * 16];  // Buffer de 16KB pour le texte

    // ============================================================
    // Méthodes privées
    // ============================================================

    /**
     * @brief Retourne le timestamp actuel en secondes (pour les logs)
     */
    double GetCurrentTime();
};
