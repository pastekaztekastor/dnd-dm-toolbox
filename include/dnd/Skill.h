#pragma once

#include "Ability.h"
#include <string>
#include <map>
#include <vector>

namespace DnD {

// D&D 5e Skills
enum class Skill {
    Acrobatics,
    AnimalHandling,
    Arcana,
    Athletics,
    Deception,
    History,
    Insight,
    Intimidation,
    Investigation,
    Medicine,
    Nature,
    Perception,
    Performance,
    Persuasion,
    Religion,
    SleightOfHand,
    Stealth,
    Survival
};

// Proficiency level
enum class ProficiencyLevel {
    None,
    Proficient,
    Expert // Double proficiency (like Rogue's Expertise)
};

class SkillHelper {
public:
    // Get the ability associated with a skill
    static Ability GetAbility(Skill skill) {
        static const std::map<Skill, Ability> skillAbilities = {
            {Skill::Acrobatics, Ability::Dexterity},
            {Skill::AnimalHandling, Ability::Wisdom},
            {Skill::Arcana, Ability::Intelligence},
            {Skill::Athletics, Ability::Strength},
            {Skill::Deception, Ability::Charisma},
            {Skill::History, Ability::Intelligence},
            {Skill::Insight, Ability::Wisdom},
            {Skill::Intimidation, Ability::Charisma},
            {Skill::Investigation, Ability::Intelligence},
            {Skill::Medicine, Ability::Wisdom},
            {Skill::Nature, Ability::Intelligence},
            {Skill::Perception, Ability::Wisdom},
            {Skill::Performance, Ability::Charisma},
            {Skill::Persuasion, Ability::Charisma},
            {Skill::Religion, Ability::Intelligence},
            {Skill::SleightOfHand, Ability::Dexterity},
            {Skill::Stealth, Ability::Dexterity},
            {Skill::Survival, Ability::Wisdom}
        };
        return skillAbilities.at(skill);
    }

    // Get skill name as string
    static std::string ToString(Skill skill) {
        static const std::map<Skill, std::string> names = {
            {Skill::Acrobatics, "Acrobatics"},
            {Skill::AnimalHandling, "Animal Handling"},
            {Skill::Arcana, "Arcana"},
            {Skill::Athletics, "Athletics"},
            {Skill::Deception, "Deception"},
            {Skill::History, "History"},
            {Skill::Insight, "Insight"},
            {Skill::Intimidation, "Intimidation"},
            {Skill::Investigation, "Investigation"},
            {Skill::Medicine, "Medicine"},
            {Skill::Nature, "Nature"},
            {Skill::Perception, "Perception"},
            {Skill::Performance, "Performance"},
            {Skill::Persuasion, "Persuasion"},
            {Skill::Religion, "Religion"},
            {Skill::SleightOfHand, "Sleight of Hand"},
            {Skill::Stealth, "Stealth"},
            {Skill::Survival, "Survival"}
        };
        return names.at(skill);
    }

    // Get all skills as a vector
    static std::vector<Skill> GetAllSkills() {
        return {
            Skill::Acrobatics, Skill::AnimalHandling, Skill::Arcana,
            Skill::Athletics, Skill::Deception, Skill::History,
            Skill::Insight, Skill::Intimidation, Skill::Investigation,
            Skill::Medicine, Skill::Nature, Skill::Perception,
            Skill::Performance, Skill::Persuasion, Skill::Religion,
            Skill::SleightOfHand, Skill::Stealth, Skill::Survival
        };
    }

    // Calculate skill bonus
    static int CalculateBonus(int abilityModifier, int proficiencyBonus, ProficiencyLevel proficiency) {
        int bonus = abilityModifier;
        if (proficiency == ProficiencyLevel::Proficient) {
            bonus += proficiencyBonus;
        } else if (proficiency == ProficiencyLevel::Expert) {
            bonus += proficiencyBonus * 2;
        }
        return bonus;
    }

    // Get proficiency bonus for a character level
    static int GetProficiencyBonus(int level) {
        if (level <= 0) return 0;
        if (level <= 4) return 2;
        if (level <= 8) return 3;
        if (level <= 12) return 4;
        if (level <= 16) return 5;
        return 6;
    }
};

} // namespace DnD
