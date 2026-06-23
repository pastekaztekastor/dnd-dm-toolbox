#include "ClasseManagerTool.h"
#include "../../include/core/EventBus.h"
#include "../../include/core/utils.h"
#include <imgui.h>
#include <nfd.h>
#include <fstream>
#include <sstream>

// TODO : implémentation à écrire ensemble (cf. ClasseManagerTool.cpp.old et
// ClasseManagerTool_lifecycle.cpp.old pour la logique v1 de référence —
// la liste des seeds à charger doit pointer vers seed/seed_*.sql).

using namespace ClasseManager;

ClasseManagerTool::ClasseManagerTool()  {}
ClasseManagerTool::~ClasseManagerTool() {}

// ============================================================
// LIFECYCLE
// ============================================================

void ClasseManagerTool::OnCreate() {}
void ClasseManagerTool::OnDestroy() {}
void ClasseManagerTool::OnSave(sqlite3* /*db*/) {}
void ClasseManagerTool::OnLoad(sqlite3* /*db*/) {}
void ClasseManagerTool::OnEvent(const Core::Event& /*event*/) {}

void ClasseManagerTool::LoadClasses() {}
void ClasseManagerTool::SelectClasse(int /*idx*/) {}

// ============================================================
// RENDER – main
// ============================================================

void ClasseManagerTool::Render() {
    if (!isOpen) return;
    ImGui::SetNextWindowSize(ImVec2(1100, 700), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(GetWindowTitle().c_str(), &isOpen)) { ImGui::End(); return; }

    ImGui::Dummy(ImVec2(0, ImGui::GetWindowHeight() / 2 - 20));
    float textWidth = ImGui::CalcTextSize("Coming soon...").x;
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - textWidth) * 0.5f);
    ImGui::TextDisabled("Coming soon...");

    ImGui::End();
}

void ClasseManagerTool::RenderLeftPanel() {}
void ClasseManagerTool::RenderRightPanel() {}

// ============================================================
// EXPORT C ABI — NE PAS MODIFIER
// ============================================================

extern "C" {
    PLUGIN_API Core::ToolBase* CreateTool() {
        return new ClasseManagerTool();
    }

    PLUGIN_API void DestroyTool(Core::ToolBase* tool) {
        delete tool;
    }
}
