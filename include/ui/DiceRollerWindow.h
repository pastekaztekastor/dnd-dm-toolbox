#pragma once

#include "../dnd/Dice.h"
#include <vector>
#include <string>
#include <memory>

namespace UI {

struct DiceRollResult {
    std::string notation;      // e.g., "2d6+3"
    std::vector<int> rolls;    // Individual dice results
    int modifier;              // Modifier applied
    int total;                 // Final result
    std::string timestamp;     // When the roll was made

    std::string ToString() const {
        std::string result = notation + " = ";
        if (rolls.size() > 1) {
            result += "[";
            for (size_t i = 0; i < rolls.size(); ++i) {
                result += std::to_string(rolls[i]);
                if (i < rolls.size() - 1) result += ", ";
            }
            result += "]";
            if (modifier != 0) {
                result += (modifier > 0 ? " + " : " - ") + std::to_string(std::abs(modifier));
            }
            result += " = ";
        }
        result += std::to_string(total);
        return result;
    }
};

class DiceRollerWindow {
private:
    bool isOpen;
    char diceNotationBuffer[64];
    std::vector<DiceRollResult> history;
    int maxHistorySize;

    // Quick roll buttons
    struct QuickRoll {
        const char* label;
        const char* notation;
    };

    // Preset dice
    std::vector<QuickRoll> quickRolls = {
        {"d4", "1d4"},
        {"d6", "1d6"},
        {"d8", "1d8"},
        {"d10", "1d10"},
        {"d12", "1d12"},
        {"d20", "1d20"},
        {"d100", "1d100"},
        {"2d6", "2d6"},
        {"3d6", "3d6"},
        {"4d6", "4d6"}
    };

    void AddToHistory(const DiceRollResult& result);
    std::string GetCurrentTime() const;

public:
    DiceRollerWindow()
        : isOpen(false)
        , maxHistorySize(50)
    {
        diceNotationBuffer[0] = '\0';
    }

    void Show() { isOpen = true; }
    void Hide() { isOpen = false; }
    bool IsOpen() const { return isOpen; }
    void Toggle() { isOpen = !isOpen; }

    // Roll dice with a specific notation
    void RollDice(const std::string& notation);

    // Render the window
    void Render();

    // Get the last roll result
    const DiceRollResult* GetLastRoll() const {
        return history.empty() ? nullptr : &history.back();
    }

    // Clear history
    void ClearHistory() { history.clear(); }
};

} // namespace UI
