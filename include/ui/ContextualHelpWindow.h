#pragma once

#include <string>
#include <map>

namespace UI {

// Types of help content
enum class HelpCategory {
    Race,
    Class,
    Ability,
    Skill,
    AbilityMethod,
    General
};

struct HelpContent {
    std::string title;
    std::string description;
    std::string advice;      // Conseils pour débutants
    std::string warnings;    // Avertissements (erreurs courantes)
};

class ContextualHelpWindow {
private:
    bool isOpen;
    HelpContent currentHelp;
    bool hasContent;

    // Database of help content
    std::map<std::string, HelpContent> helpDatabase;

    void InitializeHelpDatabase();

public:
    ContextualHelpWindow()
        : isOpen(true)
        , hasContent(false)
    {
        InitializeHelpDatabase();
    }

    void Show() { isOpen = true; }
    void Hide() { isOpen = false; }
    void Toggle() { isOpen = !isOpen; }
    bool IsOpen() const { return isOpen; }

    // Set current help content
    void SetHelp(const std::string& key);
    void ClearHelp();

    void Render();

    // Helper function to create tooltip with help
    static void ShowTooltip(const char* text);

    // Helper to show help icon that updates the help panel when hovered
    void ShowHelpMarker(const std::string& helpKey);
};

} // namespace UI
