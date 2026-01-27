#pragma once

#include <string>
#include <map>

namespace DnD {

// D&D 5e Ability Scores
enum class Ability {
    Strength,
    Dexterity,
    Constitution,
    Intelligence,
    Wisdom,
    Charisma
};

// Helper functions for abilities
class AbilityHelper {
public:
    // Calculate ability modifier from score
    static int CalculateModifier(int score) {
        return (score - 10) / 2;
    }

    // Get ability name as string
    static std::string ToString(Ability ability) {
        static const std::map<Ability, std::string> names = {
            {Ability::Strength, "Strength"},
            {Ability::Dexterity, "Dexterity"},
            {Ability::Constitution, "Constitution"},
            {Ability::Intelligence, "Intelligence"},
            {Ability::Wisdom, "Wisdom"},
            {Ability::Charisma, "Charisma"}
        };
        return names.at(ability);
    }

    // Get ability short name (3 letters)
    static std::string ToShortString(Ability ability) {
        static const std::map<Ability, std::string> shortNames = {
            {Ability::Strength, "STR"},
            {Ability::Dexterity, "DEX"},
            {Ability::Constitution, "CON"},
            {Ability::Intelligence, "INT"},
            {Ability::Wisdom, "WIS"},
            {Ability::Charisma, "CHA"}
        };
        return shortNames.at(ability);
    }

    // Convert from string to Ability
    static Ability FromString(const std::string& str) {
        static const std::map<std::string, Ability> mapping = {
            {"Strength", Ability::Strength},
            {"Dexterity", Ability::Dexterity},
            {"Constitution", Ability::Constitution},
            {"Intelligence", Ability::Intelligence},
            {"Wisdom", Ability::Wisdom},
            {"Charisma", Ability::Charisma},
            {"STR", Ability::Strength},
            {"DEX", Ability::Dexterity},
            {"CON", Ability::Constitution},
            {"INT", Ability::Intelligence},
            {"WIS", Ability::Wisdom},
            {"CHA", Ability::Charisma}
        };
        return mapping.at(str);
    }
};

// Ability Scores container
struct AbilityScores {
    int strength = 10;
    int dexterity = 10;
    int constitution = 10;
    int intelligence = 10;
    int wisdom = 10;
    int charisma = 10;

    int Get(Ability ability) const {
        switch (ability) {
            case Ability::Strength: return strength;
            case Ability::Dexterity: return dexterity;
            case Ability::Constitution: return constitution;
            case Ability::Intelligence: return intelligence;
            case Ability::Wisdom: return wisdom;
            case Ability::Charisma: return charisma;
        }
        return 10;
    }

    void Set(Ability ability, int value) {
        value = std::max(1, std::min(30, value)); // Clamp between 1 and 30
        switch (ability) {
            case Ability::Strength: strength = value; break;
            case Ability::Dexterity: dexterity = value; break;
            case Ability::Constitution: constitution = value; break;
            case Ability::Intelligence: intelligence = value; break;
            case Ability::Wisdom: wisdom = value; break;
            case Ability::Charisma: charisma = value; break;
        }
    }

    int GetModifier(Ability ability) const {
        return AbilityHelper::CalculateModifier(Get(ability));
    }
};

} // namespace DnD
