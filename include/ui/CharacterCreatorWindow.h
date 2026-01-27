#pragma once

#include "../dnd/Character.h"
#include "../dnd/Dice.h"
#include <memory>
#include <vector>

namespace UI {

class CharacterCreatorWindow {
private:
    std::shared_ptr<DnD::Character> character;
    bool isOpen;

    // Character creation state
    int creationStep; // 0 = basic info, 1 = race/class, 2 = abilities, 3 = skills, 4 = review

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

public:
    CharacterCreatorWindow()
        : character(std::make_shared<DnD::Character>())
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
    }

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
    void RenderRaceClass();
    void RenderAbilities();
    void RenderSkills();
    void RenderReview();
};

} // namespace UI
