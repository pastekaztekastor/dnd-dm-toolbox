#pragma once

#include "../../include/core/ToolBase.h"
#include <string>
#include <vector>

struct ImGuiInputTextCallbackData;

/**
 * @brief Note - Bloque note de votre aventure au format texte (futurement au format md)
 *
 * Fonctionnalités:
 * Simple bloc note en texte
 */
class NoteTool : public Core::ToolBase {
public:
    NoteTool();
    ~NoteTool() override;

    // ============================================================
    // Interface ToolBase (obligatoire)
    // ============================================================

    std::string GetID() const override { return "note"; }
    std::string GetName() const override { return "Note"; }
    std::string GetCategory() const override { return "Outils"; }
    std::string GetDescription() const override { return "Bloque note de votre aventure au format texte (futurement au format md)"; }

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
    // État du plugin
    // ============================================================

    char noteBuffer[1024 * 16];
    bool previewMode = false;

    // Autocomplétion @plugin.alias
    struct AcItem { std::string token; std::string label; };
    std::vector<AcItem> acItems;
    bool        acLoaded    = false;
    bool        acActive    = false;
    int         acAtPos     = -1;   // position du @ dans noteBuffer
    std::string acFilter;           // ce qui est tapé après @
    bool        acApply     = false;
    std::string acApplyText;        // token sélectionné à insérer

    double GetCurrentTime();
    void   LoadCompletions();

public:
    static int InputCb(ImGuiInputTextCallbackData* data);
};
