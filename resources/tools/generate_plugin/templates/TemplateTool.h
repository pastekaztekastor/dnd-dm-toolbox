#pragma once

#include "../../include/core/ToolBase.h"
#include <string>
#include <vector>

/**
 * @brief PLUGIN_NAME_DISPLAY - PLUGIN_DESCRIPTION
 *
 * Fonctionnalités:
 * - TODO: Lister les fonctionnalités
 */
class PLUGIN_CLASS_NAMETool : public Core::ToolBase {
public:
    PLUGIN_CLASS_NAMETool();
    ~PLUGIN_CLASS_NAMETool() override;

    // ============================================================
    // Interface ToolBase (obligatoire)
    // ============================================================

    std::string GetID() const override { return "PLUGIN_ID"; }
    std::string GetName() const override { return "PLUGIN_NAME_DISPLAY"; }
    std::string GetCategory() const override { return "PLUGIN_CATEGORY"; }
    std::string GetDescription() const override { return "PLUGIN_DESCRIPTION"; }

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

    // TODO: Ajouter vos variables d'état ici
    // Exemple:
    // std::string myData;
    // int myCounter = 0;
    // std::vector<MyStruct> myList;

    // ============================================================
    // Méthodes privées
    // ============================================================

    // TODO: Ajouter vos méthodes privées ici
    // Exemple:
    // void DoSomething();
    // void ProcessData();

    /**
     * @brief Retourne le timestamp actuel en secondes (pour les logs)
     */
    double GetCurrentTime();
};
