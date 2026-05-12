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
        STEP_RACE_SELECTION,      // 0: Grille de tuiles des races
        STEP_RACE_DETAILS,        // 1: Détails + variantes + nom
        STEP_CLASS_SELECTION,     // 2: Grille de tuiles des classes (TODO)
        STEP_CLASS_DETAILS,       // 3: Détails + compétences (TODO)
        STEP_STATS,               // 4: Génération caractéristiques (TODO)
        STEP_BACKGROUND,          // 5: Histoire + alignement (TODO)
        STEP_SPELLS,              // 6: Sorts si applicable (TODO)
        STEP_SUMMARY              // 7: Résumé final (TODO)
    };

    // ============================================================
    // État du plugin
    // ============================================================

    // Navigation
    CreationStep currentStep = STEP_RACE_SELECTION;
    std::vector<bool> stepCompleted;  // Track validation de chaque étape

    // Données de création
    DnD::Character character;  // Personnage en cours de création
    std::vector<Database::RaceData> availableRaces;
    std::map<std::string, std::vector<Database::RaceData>> allSubRaces;
    Database::RaceData selectedRace;
    Database::RaceData selectedSubRace;
    int selectedRaceIndex = -1;
    int selectedSubRaceIndex = -1;
    int selectedVariantIndex = -1;

    // Informations personnelles (Étape 1)
    char firstName[128] = "";
    char lastName[128] = "";
    int age = 0;

    // ============================================================
    // Méthodes de rendu
    // ============================================================

    /**
     * @brief Affiche le fil d'Ariane (breadcrumb) des étapes
     */
    void RenderBreadcrumb();

    /**
     * @brief Étape 1: Panneau latéral avec sélection race et détails
     */
    void RenderRaceGrid();

    /**
     * @brief Valide l'étape actuelle
     * @return true si l'étape est valide et on peut passer à la suivante
     */
    bool ValidateCurrentStep();

    /**
     * @brief Retourne le timestamp actuel en secondes (pour les logs)
     */
    double GetCurrentTime();
};
