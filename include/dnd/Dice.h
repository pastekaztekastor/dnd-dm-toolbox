#pragma once

#include <random>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

namespace DnD {

// Dice roller utility
class Dice {
private:
    static std::mt19937& GetRNG() {
        static std::random_device rd;
        static std::mt19937 rng(rd());
        return rng;
    }

public:
    // Roll a single die (e.g., d20, d6)
    static int Roll(int sides) {
        if (sides <= 0) return 0;
        std::uniform_int_distribution<int> dist(1, sides);
        return dist(GetRNG());
    }

    // Roll multiple dice and return the sum
    static int Roll(int count, int sides) {
        int total = 0;
        for (int i = 0; i < count; ++i) {
            total += Roll(sides);
        }
        return total;
    }

    // Roll multiple dice and return individual results
    static std::vector<int> RollIndividual(int count, int sides) {
        std::vector<int> results;
        for (int i = 0; i < count; ++i) {
            results.push_back(Roll(sides));
        }
        return results;
    }

    // Roll with modifier (e.g., 2d6+3)
    static int RollWithModifier(int count, int sides, int modifier) {
        return Roll(count, sides) + modifier;
    }

    // Roll with advantage (roll twice, take higher)
    static int RollWithAdvantage(int sides) {
        int roll1 = Roll(sides);
        int roll2 = Roll(sides);
        return std::max(roll1, roll2);
    }

    // Roll with disadvantage (roll twice, take lower)
    static int RollWithDisadvantage(int sides) {
        int roll1 = Roll(sides);
        int roll2 = Roll(sides);
        return std::min(roll1, roll2);
    }

    // Ability score rolling methods
    // 4d6 drop lowest (standard method)
    static int RollAbilityScore() {
        std::vector<int> rolls = RollIndividual(4, 6);
        std::sort(rolls.begin(), rolls.end());
        return rolls[1] + rolls[2] + rolls[3]; // Sum the highest 3
    }

    // Roll a complete set of 6 ability scores
    static std::vector<int> RollAbilityScoreSet() {
        std::vector<int> scores;
        for (int i = 0; i < 6; ++i) {
            scores.push_back(RollAbilityScore());
        }
        return scores;
    }

    // Parse and roll dice notation (e.g., "2d6+3")
    struct DiceRoll {
        int count;
        int sides;
        int modifier;

        int Roll() const {
            return Dice::RollWithModifier(count, sides, modifier);
        }

        std::string ToString() const {
            std::stringstream ss;
            ss << count << "d" << sides;
            if (modifier > 0) ss << "+" << modifier;
            else if (modifier < 0) ss << modifier;
            return ss.str();
        }
    };

    // Parse dice notation string
    static DiceRoll ParseDiceNotation(const std::string& notation) {
        DiceRoll result{1, 20, 0};

        // Simple parser for "XdY+Z" or "XdY-Z" or "XdY"
        size_t dPos = notation.find('d');
        if (dPos != std::string::npos) {
            // Parse count
            if (dPos > 0) {
                result.count = std::stoi(notation.substr(0, dPos));
            }

            // Find modifier
            size_t plusPos = notation.find('+', dPos);
            size_t minusPos = notation.find('-', dPos);

            if (plusPos != std::string::npos) {
                result.sides = std::stoi(notation.substr(dPos + 1, plusPos - dPos - 1));
                result.modifier = std::stoi(notation.substr(plusPos + 1));
            } else if (minusPos != std::string::npos) {
                result.sides = std::stoi(notation.substr(dPos + 1, minusPos - dPos - 1));
                result.modifier = -std::stoi(notation.substr(minusPos + 1));
            } else {
                result.sides = std::stoi(notation.substr(dPos + 1));
            }
        }

        return result;
    }

    // Common dice shortcuts
    static int D4() { return Roll(4); }
    static int D6() { return Roll(6); }
    static int D8() { return Roll(8); }
    static int D10() { return Roll(10); }
    static int D12() { return Roll(12); }
    static int D20() { return Roll(20); }
    static int D100() { return Roll(100); }
};

} // namespace DnD
