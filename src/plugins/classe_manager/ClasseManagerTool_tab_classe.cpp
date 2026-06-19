#include "ClasseManagerTool.h"
#include "../../include/core/utils.h"
#include <imgui.h>

using namespace ClasseManager;

void ClasseManagerTool::RenderTabClasse(const ClasseData& c) {
    if (editMode)        { RenderEditPanel();      return; }
    if (subEditMode == 4) { RenderChoixChampForm(); return; }
    if (subEditMode == 5) { RenderChoixValeurForm(); return; }

    if (c.classe_parente_id.empty() && ImGui::Button("\xef\x81\x81 Modifier")) StartEdit(false);
    ImGui::Separator(); ImGui::Spacing();

    ImGui::Text("%s", c.nom.c_str());
    if (!c.alias.empty()) { ImGui::SameLine(); ImGui::TextDisabled("  [@classe.%s]", c.alias.c_str()); }
    ImGui::Separator(); ImGui::Spacing();

    ImGui::Text("Dé de vie : d%d   PV niv 1 : %d   PV/niv : %d",
        c.dee_de_vie, c.points_de_vie, c.points_de_vie_par_niveau);

    if (!c.jets_de_sauvegarde.empty() && c.jets_de_sauvegarde != "[]")
        ImGui::TextWrapped("Jets de sauvegarde : %s", c.jets_de_sauvegarde.c_str());

    if (!c.competences.empty() && c.competences != "[]")
        ImGui::TextWrapped("Compétences : %s", c.competences.c_str());

    if (!c.equipement_maitrise.empty() && c.equipement_maitrise != "[]")
        ImGui::TextWrapped("Maîtrises équipement : %s", c.equipement_maitrise.c_str());

    if (!c.caracteristiques_de_sorts.empty())
        ImGui::Text("Caractéristique de sorts : %s", c.caracteristiques_de_sorts.c_str());

    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

    if (!c.description.empty()) {
        ImGui::TextWrapped("%s", c.description.c_str());
        ImGui::Spacing();
    }

    for (auto& p : c.presentations) {
        if (!p.titre.empty())
            ImGui::TextColored({.8f, .7f, .4f, 1.f}, "%s", p.titre.c_str());
        ImGui::TextWrapped("%s", p.texte.c_str());
        ImGui::Spacing();
    }

    if (!c.source.empty())
        ImGui::TextDisabled("Source : %s %s", c.source.c_str(), c.source_ver.c_str());

    // Options de classe (choix_champs) — classe de base uniquement
    if (c.classe_parente_id.empty()) {
        ImGui::Spacing();
        ImGui::SeparatorText("Options de classe");
        if (ImGui::SmallButton("+ Ajouter option"))
            StartSubEdit(4, true);

        for (auto& cc : c.choix_champs) {
            ImGui::Spacing();
            ImGui::PushID(cc.id.c_str());
            ImGui::TextColored({.8f,.7f,.4f,1.f}, "[Niv %d] %s", cc.niveau, cc.nom.c_str());
            ImGui::SameLine();
            ImGui::TextDisabled("  [@classe.%s]", cc.alias.c_str());
            ImGui::SameLine();
            if (ImGui::SmallButton("\xef\x81\x81")) StartSubEdit(4, false, nullptr, nullptr, nullptr, &cc);
            ImGui::SameLine();
            if (Core::ConfirmDeleteButton("X", cc.id.c_str(), cc.nom.c_str())) {
                repo->BeginTransaction();
                repo->DeleteChoixChamp(cc.id);
                repo->Commit();
                repo->LoadChildData(classes[selectedIdx]);
                BuildTree(classes[selectedIdx]);
                ImGui::PopID();
                return;
            }

            ImGui::Indent(12.f);
            for (auto& cv : cc.valeurs) {
                ImGui::PushID(cv.id.c_str());
                ImGui::Bullet();
                ImGui::SameLine();
                ImGui::Text("%s", cv.nom.c_str());
                if (!cv.description.empty() && ImGui::IsItemHovered())
                    ImGui::SetTooltip("%s", cv.description.c_str());
                ImGui::SameLine();
                ImGui::TextDisabled("[%s]", cv.valeur.c_str());
                ImGui::SameLine();
                if (ImGui::SmallButton("\xef\x81\x81")) StartSubEdit(5, false, nullptr, nullptr, nullptr, nullptr, &cv);
                ImGui::SameLine();
                if (Core::ConfirmDeleteButton("X", cv.id.c_str(), cv.nom.c_str())) {
                    repo->BeginTransaction();
                    repo->DeleteChoixValeur(cv.id);
                    repo->Commit();
                    repo->LoadChildData(classes[selectedIdx]);
                    BuildTree(classes[selectedIdx]);
                    ImGui::PopID();
                    ImGui::Unindent(12.f); ImGui::PopID();
                    return;
                }
                ImGui::PopID();
            }
            if (ImGui::SmallButton("+ Valeur")) {
                editChoixValeur = ChoixValeurData{};
                editChoixValeur.champ_id = cc.id;
                StartSubEdit(5, true);
            }
            ImGui::Unindent(12.f);
            ImGui::PopID();
        }
    }
}
