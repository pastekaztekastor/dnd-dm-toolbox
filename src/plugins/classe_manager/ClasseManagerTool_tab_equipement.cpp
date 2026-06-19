#include "ClasseManagerTool.h"
#include "ClasseManagerTool_helpers.h"
#include "../../include/core/utils.h"
#include <imgui.h>

using namespace ClasseManager;

// ── Formulaire Équipement ─────────────────────────────────────────────────────

void ClasseManagerTool::RenderEquipForm() {
    SubFormHeader("Équipement", subIsNew);
    ImGui::SameLine();
    if (ImGui::Button("Sauvegarder##q")) SaveSubEdit();
    ImGui::SameLine();
    if (ImGui::Button("Annuler##q"))    CancelSubEdit();
    ImGui::Separator(); ImGui::Spacing();

    if (!subEditError.empty()) {
        ImGui::PushStyleColor(ImGuiCol_Text, {1.f,.35f,.35f,1.f});
        ImGui::TextWrapped("! %s", subEditError.c_str());
        ImGui::PopStyleColor(); ImGui::Spacing();
    }

    ImGui::SetNextItemWidth(70); ImGui::InputInt("Choix n°##q",  &editEquip.choix_num);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(70); ImGui::InputInt("Option n°##q", &editEquip.option_num);
    ImGui::Text("Description");
    InputStr("##desc_q", editEquip.description);

    ImGui::Spacing();
    if (ImGui::Button("Sauvegarder##qbot")) SaveSubEdit();
    ImGui::SameLine();
    if (ImGui::Button("Annuler##qbot"))    CancelSubEdit();
}

// ── Tab Équipement ────────────────────────────────────────────────────────────

void ClasseManagerTool::RenderTabEquipement(const ClasseData& c) {
    if (subEditMode == 3) { RenderEquipForm(); return; }

    if (ImGui::Button("+ Ajouter option"))
        StartSubEdit(3, true);
    ImGui::Spacing();

    if (c.choix_equipement.empty()) {
        ImGui::TextDisabled("Aucun équipement de départ défini.");
        return;
    }

    int curChoix = -1;
    for (auto& e : c.choix_equipement) {
        ImGui::PushID(e.id.c_str());

        if (e.choix_num != curChoix) {
            if (curChoix >= 0) ImGui::Spacing();
            ImGui::TextColored({.8f,.7f,.4f,1.f}, "Choix %d", e.choix_num);
            curChoix = e.choix_num;
        }

        if (ImGui::SmallButton("\xef\x81\x81"))
            StartSubEdit(3, false, nullptr, nullptr, &e);
        ImGui::SameLine();
        if (Core::ConfirmDeleteButton("X", e.id.c_str(), e.description.c_str())) {
            repo->BeginTransaction();
            repo->DeleteChoixEquipement(e.id);
            repo->Commit();
            repo->LoadChildData(classes[selectedIdx]);
            ImGui::PopID();
            return;
        }
        ImGui::SameLine();
        ImGui::Text("%d. %s", e.option_num, e.description.c_str());

        ImGui::PopID();
    }
}
