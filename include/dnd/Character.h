#pragma once

#include "Ability.h"
#include "Skill.h"
#include <string>
#include <map>
#include <vector>
#include <memory>

namespace DnD {

// Character class types
enum class CharacterClass {
    Barbarian,
    Bard,
    Cleric,
    Druid,
    Fighter,
    Monk,
    Paladin,
    Ranger,
    Rogue,
    Sorcerer,
    Warlock,
    Wizard
};

// Character race types
enum class Race {
    Human,
    Elf,
    Dwarf,
    Halfling,
    Dragonborn,
    Gnome,
    HalfElf,
    HalfOrc,
    Tiefling
};

// Alignment
enum class Alignment {
    LawfulGood,
    NeutralGood,
    ChaoticGood,
    LawfulNeutral,
    TrueNeutral,
    ChaoticNeutral,
    LawfulEvil,
    NeutralEvil,
    ChaoticEvil
};

// Character class
class Character {
public:
    // Basic Info
    std::string name;
    Race race;
    CharacterClass characterClass;
    int level;
    std::string background;
    Alignment alignment;
    int experiencePoints;

    // Ability Scores
    AbilityScores abilityScores;

    // Skills (proficiency level for each skill)
    std::map<Skill, ProficiencyLevel> skillProficiencies;

    // Combat Stats
    int armorClass;
    int initiativeBonus;
    int speed;
    int maxHitPoints;
    int currentHitPoints;
    int temporaryHitPoints;

    // Saving throw proficiencies
    std::map<Ability, bool> savingThrowProficiencies;

    // Meta
    bool isNPC;
    std::string notes;

    Character()
        : name("Unnamed Character")
        , race(Race::Human)
        , characterClass(CharacterClass::Fighter)
        , level(1)
        , background("")
        , alignment(Alignment::TrueNeutral)
        , experiencePoints(0)
        , armorClass(10)
        , initiativeBonus(0)
        , speed(30)
        , maxHitPoints(10)
        , currentHitPoints(10)
        , temporaryHitPoints(0)
        , isNPC(false)
    {
        // Initialize all skills to no proficiency
        for (auto skill : SkillHelper::GetAllSkills()) {
            skillProficiencies[skill] = ProficiencyLevel::None;
        }

        // Initialize saving throws
        savingThrowProficiencies[Ability::Strength] = false;
        savingThrowProficiencies[Ability::Dexterity] = false;
        savingThrowProficiencies[Ability::Constitution] = false;
        savingThrowProficiencies[Ability::Intelligence] = false;
        savingThrowProficiencies[Ability::Wisdom] = false;
        savingThrowProficiencies[Ability::Charisma] = false;
    }

    // Get proficiency bonus based on level
    int GetProficiencyBonus() const {
        return SkillHelper::GetProficiencyBonus(level);
    }

    // Get skill modifier
    int GetSkillModifier(Skill skill) const {
        Ability ability = SkillHelper::GetAbility(skill);
        int abilityMod = abilityScores.GetModifier(ability);
        ProficiencyLevel prof = skillProficiencies.at(skill);
        return SkillHelper::CalculateBonus(abilityMod, GetProficiencyBonus(), prof);
    }

    // Get saving throw modifier
    int GetSavingThrowModifier(Ability ability) const {
        int mod = abilityScores.GetModifier(ability);
        if (savingThrowProficiencies.at(ability)) {
            mod += GetProficiencyBonus();
        }
        return mod;
    }

    // Get initiative modifier
    int GetInitiativeModifier() const {
        return abilityScores.GetModifier(Ability::Dexterity) + initiativeBonus;
    }

    // Calculate passive perception
    int GetPassivePerception() const {
        return 10 + GetSkillModifier(Skill::Perception);
    }

    // HP management
    void TakeDamage(int damage) {
        if (damage <= 0) return;

        // First remove temporary HP
        if (temporaryHitPoints > 0) {
            if (damage <= temporaryHitPoints) {
                temporaryHitPoints -= damage;
                return;
            }
            damage -= temporaryHitPoints;
            temporaryHitPoints = 0;
        }

        // Then remove from current HP
        currentHitPoints = std::max(0, currentHitPoints - damage);
    }

    void Heal(int amount) {
        if (amount <= 0) return;
        currentHitPoints = std::min(maxHitPoints, currentHitPoints + amount);
    }

    bool IsAlive() const {
        return currentHitPoints > 0;
    }

    bool IsConscious() const {
        return currentHitPoints > 0;
    }
};

// Helper class for character creation and race/class info
class CharacterHelper {
public:
    static std::string RaceToString(Race race) {
        static const std::map<Race, std::string> names = {
            {Race::Human, "Humain"},
            {Race::Elf, "Elfe"},
            {Race::Dwarf, "Nain"},
            {Race::Halfling, "Halfelin"},
            {Race::Dragonborn, "Drakéide"},
            {Race::Gnome, "Gnome"},
            {Race::HalfElf, "Demi-Elfe"},
            {Race::HalfOrc, "Demi-Orc"},
            {Race::Tiefling, "Tieffelin"}
        };
        return names.at(race);
    }

    static std::string ClassToString(CharacterClass charClass) {
        static const std::map<CharacterClass, std::string> names = {
            {CharacterClass::Barbarian, "Barbare"},
            {CharacterClass::Bard, "Barde"},
            {CharacterClass::Cleric, "Clerc"},
            {CharacterClass::Druid, "Druide"},
            {CharacterClass::Fighter, "Guerrier"},
            {CharacterClass::Monk, "Moine"},
            {CharacterClass::Paladin, "Paladin"},
            {CharacterClass::Ranger, "Rôdeur"},
            {CharacterClass::Rogue, "Roublard"},
            {CharacterClass::Sorcerer, "Ensorceleur"},
            {CharacterClass::Warlock, "Sorcier"},
            {CharacterClass::Wizard, "Magicien"}
        };
        return names.at(charClass);
    }

    static std::string AlignmentToString(Alignment alignment) {
        static const std::map<Alignment, std::string> names = {
            {Alignment::LawfulGood, "Loyal Bon"},
            {Alignment::NeutralGood, "Neutre Bon"},
            {Alignment::ChaoticGood, "Chaotique Bon"},
            {Alignment::LawfulNeutral, "Loyal Neutre"},
            {Alignment::TrueNeutral, "Neutre Strict"},
            {Alignment::ChaoticNeutral, "Chaotique Neutre"},
            {Alignment::LawfulEvil, "Loyal Mauvais"},
            {Alignment::NeutralEvil, "Neutre Mauvais"},
            {Alignment::ChaoticEvil, "Chaotique Mauvais"}
        };
        return names.at(alignment);
    }

    static std::vector<Race> GetAllRaces() {
        return {
            Race::Human, Race::Elf, Race::Dwarf, Race::Halfling,
            Race::Dragonborn, Race::Gnome, Race::HalfElf,
            Race::HalfOrc, Race::Tiefling
        };
    }

    static std::vector<CharacterClass> GetAllClasses() {
        return {
            CharacterClass::Barbarian, CharacterClass::Bard,
            CharacterClass::Cleric, CharacterClass::Druid,
            CharacterClass::Fighter, CharacterClass::Monk,
            CharacterClass::Paladin, CharacterClass::Ranger,
            CharacterClass::Rogue, CharacterClass::Sorcerer,
            CharacterClass::Warlock, CharacterClass::Wizard
        };
    }

    // Get hit die for a class
    static int GetHitDie(CharacterClass charClass) {
        static const std::map<CharacterClass, int> hitDice = {
            {CharacterClass::Barbarian, 12},
            {CharacterClass::Bard, 8},
            {CharacterClass::Cleric, 8},
            {CharacterClass::Druid, 8},
            {CharacterClass::Fighter, 10},
            {CharacterClass::Monk, 8},
            {CharacterClass::Paladin, 10},
            {CharacterClass::Ranger, 10},
            {CharacterClass::Rogue, 8},
            {CharacterClass::Sorcerer, 6},
            {CharacterClass::Warlock, 8},
            {CharacterClass::Wizard, 6}
        };
        return hitDice.at(charClass);
    }

    // Get racial speed bonus
    static int GetRacialSpeed(Race race) {
        switch (race) {
            case Race::Dwarf:
            case Race::Gnome:
            case Race::Halfling:
                return 25;
            default:
                return 30;
        }
    }
};

} // namespace DnD
