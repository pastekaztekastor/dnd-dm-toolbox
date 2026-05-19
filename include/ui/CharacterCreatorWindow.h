#pragma once

#include "../dnd/Character.h"
#include "../dnd/Dice.h"
#include "../database/DatabaseManager.h"
#include <memory>
#include <vector>

namespace UI {

// Forward declarations
class DiceRollerWindow;
class ContextualHelpWindow;

class CharacterCreatorWindow {
private:
    std::shared_ptr<DnD::Character> character;
    DiceRollerWindow* diceRoller; // Pointer to the global dice roller
    ContextualHelpWindow* helpWindow; // Pointer to the help window
    std::shared_ptr<Database::DatabaseManager> database; // Database connection
    bool isOpen;

    // Données chargées depuis la DB
    std::vector<Database::RaceData> dbRaces;
    std::vector<Database::ClassData> dbClasses;

    // Character creation state
    int creationStep; // 0 = basic info, 1 = race, 2 = class, 3 = abilities, 4 = skills, 5 = review

    // Ability score generation
    enum class AbilityMethod {
        PointBuy,
        StandardArray,
        Roll
    };
    AbilityMethod abilityMethod;
    std::vector<int> rolledScores;
    int pointBuyPoints;

    // Temporary data for UI
    char nameBuffer[256];
    char backgroundBuffer[256];
    int selectedRace;
    int selectedClass;
    int selectedAlignment;

    // Point buy costs
    int GetPointBuyCost(int score) const {
        if (score <= 8) return 0;
        if (score <= 13) return score - 8;
        return (score - 8) + (score - 13);
    }

    int CalculatePointBuyTotal() const {
        int total = 0;
        total += GetPointBuyCost(character->abilityScores.strength);
        total += GetPointBuyCost(character->abilityScores.dexterity);
        total += GetPointBuyCost(character->abilityScores.constitution);
        total += GetPointBuyCost(character->abilityScores.intelligence);
        total += GetPointBuyCost(character->abilityScores.wisdom);
        total += GetPointBuyCost(character->abilityScores.charisma);
        return total;
    }

    // Formater l'affichage d'une race avec ses bonus
    std::string FormatRaceLabel(const Database::RaceData& race) const {
        std::string label = race.nom;
        std::vector<std::string> bonuses;

        if (race.bonus_forces       > 0) bonuses.push_back("+" + std::to_string(race.bonus_forces)       + " FOR");
        if (race.bonus_dexterite    > 0) bonuses.push_back("+" + std::to_string(race.bonus_dexterite)    + " DEX");
        if (race.bonus_constitution > 0) bonuses.push_back("+" + std::to_string(race.bonus_constitution) + " CON");
        if (race.bonus_intelligence > 0) bonuses.push_back("+" + std::to_string(race.bonus_intelligence) + " INT");
        if (race.bonus_sagesse      > 0) bonuses.push_back("+" + std::to_string(race.bonus_sagesse)      + " SAG");
        if (race.bonus_charisme     > 0) bonuses.push_back("+" + std::to_string(race.bonus_charisme)     + " CHA");

        if (!bonuses.empty()) {
            label += " (";
            for (size_t i = 0; i < bonuses.size(); ++i) {
                if (i > 0) label += ", ";
                label += bonuses[i];
            }
            label += ")";
        }

        return label;
    }

    // Formater l'affichage d'une classe avec son dé de vie
    std::string FormatClassLabel(const Database::ClassData& cls) const {
        return cls.nom + " (DV: d" + std::to_string(cls.dee_de_vie) + ")";
    }

public:
    CharacterCreatorWindow()
        : character(std::make_shared<DnD::Character>())
        , diceRoller(nullptr)
        , helpWindow(nullptr)
        , database(std::make_shared<Database::DatabaseManager>())
        , isOpen(false)
        , creationStep(0)
        , abilityMethod(AbilityMethod::PointBuy)
        , pointBuyPoints(27)
        , selectedRace(0)
        , selectedClass(0)
        , selectedAlignment(4) // True Neutral
    {
        nameBuffer[0] = '\0';
        backgroundBuffer[0] = '\0';

        if (database->Connect("dnd_toolbox.db")) {
            // Charger les races et classes depuis la DB
            dbRaces   = database->Races()->LoadAll();
            dbClasses = database->Classes()->LoadAll();
        }
    }

    void SetDiceRoller(DiceRollerWindow* roller) { diceRoller = roller; }
    void SetHelpWindow(ContextualHelpWindow* help) { helpWindow = help; }
    void Show() { isOpen = true; }
    void Hide() { isOpen = false; }
    bool IsOpen() const { return isOpen; }

    void Render();

    std::shared_ptr<DnD::Character> GetCharacter() const { return character; }
    void Reset() {
        character = std::make_shared<DnD::Character>();
        creationStep = 0;
        nameBuffer[0] = '\0';
        backgroundBuffer[0] = '\0';
        rolledScores.clear();
    }

private:
    void RenderBasicInfo();
    void RenderRaceClass();  // Étape 1 : Race et Classe ensemble (pour l'instant)
    void RenderAbilities();
    void RenderSkills();
    void RenderReview();
};

} // namespace UI
