#pragma once

#include "../../include/core/ToolBase.h"
#include "../../include/dnd/Character.h"
#include "../../include/database/DatabaseManager.h"
#include <string>
#include <vector>
#include <map>

/**
 * @brief Créateur de Personnage - Outil de création de personnage D&D 5e
 *
 * Fonctionnalités:
 * - Workflow de création en 8 étapes
 * - Choix race/classe depuis base de données
 * - Génération des caractéristiques
 * - Sélection compétences et sorts
 * - Multi-instance (plusieurs créations simultanées)
 */
class CharacterCreatorTool : public Core::ToolBase {
public:
    CharacterCreatorTool();
    ~CharacterCreatorTool() override;

    // ============================================================
    // Interface ToolBase (obligatoire)
    // ============================================================

    std::string GetID() const override { return "character_creator"; }
    std::string GetName() const override { return "Créateur de Personnage"; }
    std::string GetCategory() const override { return "Création"; }
    std::string GetDescription() const override { return "Outil de création de personnage D&D 5e"; }

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
    // Étapes de création
    // ============================================================

    enum CreationStep {
        STEP_RACE_SELECTION = 0,
        STEP_RACE_DETAILS = 1,              // étape interne (masquée dans le breadcrumb)
        STEP_CLASS_SELECTION = 2,
        STEP_BACKGROUND_SELECTION = 3,
        STEP_STUFF_OPTIONS = 4,
        STEP_PERSONALISATION_OPTIONS = 5,
        STEP_SPELLS_SELECTION = 6,
        STEP_SUMMARY = 7
    };

    // ============================================================
    // État du plugin
    // ============================================================

    // Navigation
    CreationStep currentStep = STEP_RACE_SELECTION;
    std::vector<bool> stepCompleted;

    // Données de création
    DnD::Character character;

    // --- Races ---
    std::vector<Database::RaceData> availableRaces;
    std::map<std::string, std::vector<Database::RaceData>> allSubRaces;
    Database::RaceData selectedRace;
    Database::RaceData selectedSubRace;
    int selectedRaceIndex = -1;
    int selectedSubRaceIndex = -1;
    int selectedVariantIndex = -1;

    // Informations personnelles
    char firstName[128] = "";
    char lastName[128] = "";
    int age = 0;

    // --- Classes ---
    std::vector<Database::ClassData> availableClasses;
    std::map<std::string, std::vector<Database::ClassData>> allSubClasses;
    Database::ClassData selectedClass;
    Database::ClassData selectedSubClass;
    int selectedClassIndex = -1;
    int selectedSubClassIndex = -1;

    // ============================================================
    // Méthodes de rendu
    // ============================================================

    void RenderBreadcrumb();
    void RenderRaceOnglet();
    void RenderClassOnglet();
    void RenderBackgroundOnglet();
    void RenderStuffOptionsOnglet();
    void RenderPersonalisationOptionsOnglet();
    void RenderSpellsSelectionOnglet();
    void RenderSummaryOnglet();

    bool ValidateCurrentStep();
    double GetCurrentTime();
};
