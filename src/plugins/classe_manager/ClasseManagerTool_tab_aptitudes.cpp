#include "ClasseManagerTool.h"
#include "ClasseManagerTool_helpers.h"
#include "../../include/ui/MarkdownRenderer.h"
#include "../../include/core/utils.h"
#include <imgui.h>

using namespace ClasseManager;

// ── Formulaire Aptitude ───────────────────────────────────────────────────────

void ClasseManagerTool::RenderAptForm() {
    SubFormHeader("Aptitude", subIsNew);
    ImGui::SameLine();
    if (ImGui::Button("Sauvegarder##a")) SaveSubEdit();
    ImGui::SameLine();
    if (ImGui::Button("Annuler##a"))    CancelSubEdit();
    ImGui::Separator(); ImGui::Spacing();

    if (!subEditError.empty()) {
        ImGui::PushStyleColor(ImGuiCol_Text, {1.f,.35f,.35f,1.f});
        ImGui::TextWrapped("! %s", subEditError.c_str());
        ImGui::PopStyleColor(); ImGui::Spacing();
    }

    InputStr("Nom##a", editApt.nom);
    if (subIsNew) InputStr("Alias / ID##a", editApt.alias);

    ImGui::SetNextItemWidth(70);
    ImGui::InputInt("Niveau d'acquisition##a", &editApt.niveau_acquisition);

    InputStr("Description rapide##a", editApt.description_rapide);

    ImGui::Text("Description");
    InputStrMulti("##desc_a", editApt.description, {-1, 90});

    ImGui::Spacing();
    const auto& champs = classes[selectedIdx].choix_champs;

    // Déclenche un choix (champ)
    ImGui::Text("Déclenche un choix (champ)");
    {
        std::vector<const char*> opts = { "(aucun)" };
        for (auto& cc : champs) opts.push_back(cc.nom.c_str());
        int sel = 0;
        for (int i = 0; i < (int)champs.size(); ++i)
            if (champs[i].id == editApt.choix_champ_id) { sel = i + 1; break; }
        ImGui::SetNextItemWidth(300);
        if (ImGui::Combo("##cchamp_a", &sel, opts.data(), (int)opts.size()))
            editApt.choix_champ_id = (sel == 0) ? "" : champs[sel-1].id;
    }

    // Requiert un choix (valeur)
    ImGui::Text("Requiert un choix (valeur)");
    {
        std::vector<std::string> valIds  = { "" };
        std::vector<std::string> valNoms = { "(aucune)" };
        for (auto& cc : champs)
            for (auto& cv : cc.valeurs) {
                valIds.push_back(cv.id);
                valNoms.push_back(cc.nom + " \xe2\x86\x92 " + cv.nom);
            }
        int sel = 0;
        for (int i = 1; i < (int)valIds.size(); ++i)
            if (valIds[i] == editApt.choix_valeur_id) { sel = i; break; }
        std::vector<const char*> opts;
        for (auto& s : valNoms) opts.push_back(s.c_str());
        ImGui::SetNextItemWidth(300);
        if (ImGui::Combo("##cvaleur_a", &sel, opts.data(), (int)opts.size()))
            editApt.choix_valeur_id = valIds[sel];
    }

    InputStr("Aptitude prérequise (ID)##a", editApt.aptitude_prerequisite_id);

    ImGui::Spacing();
    if (ImGui::Button("Sauvegarder##abot")) SaveSubEdit();
    ImGui::SameLine();
    if (ImGui::Button("Annuler##abot"))    CancelSubEdit();
}

// ── Tab Aptitudes ─────────────────────────────────────────────────────────────

static void RenderAptBadges(const AptitudeData& a, float dimMult = 1.f) {
    if (!a.choix_valeur_id.empty()) {
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, {.9f * dimMult, .4f * dimMult, .4f * dimMult, 1.f});
        ImGui::TextUnformatted("[Var]");
        ImGui::PopStyleColor();
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("%s", a.choix_valeur_id.c_str());
    }
    if (!a.choix_champ_id.empty()) {
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, {.95f * dimMult, .85f * dimMult, .3f * dimMult, 1.f});
        ImGui::TextUnformatted("[Choix]");
        ImGui::PopStyleColor();
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("%s", a.choix_champ_id.c_str());
    }
    if (!a.aptitude_prerequisite_id.empty()) {
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, {.6f * dimMult, .6f * dimMult, .9f * dimMult, 1.f});
        ImGui::TextUnformatted("[Req]");
        ImGui::PopStyleColor();
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("%s", a.aptitude_prerequisite_id.c_str());
    }
}

void ClasseManagerTool::RenderTabAptitudes(const ClasseData& c) {
    if (subEditMode == 2) { RenderAptForm(); return; }

    // ── Groupe modifiable (classe mère) ───────────────────────────────────────
    auto renderEditable = [&](const std::vector<AptitudeData>& apts,
                               const std::string& classeId, const char* header) {
        bool open = ImGui::CollapsingHeader(header, ImGuiTreeNodeFlags_DefaultOpen);
        ImGui::SameLine();
        ImGui::PushID(("add_" + classeId).c_str());
        if (ImGui::SmallButton("+ Ajouter")) {
            AptitudeData blank;
            blank.classe_id = classeId;
            blank.niveau_acquisition = 1;
            StartSubEdit(2, true, nullptr, &blank);
        }
        ImGui::PopID();
        if (!open) return;

        ImGui::Indent(8.f);
        for (auto& a : apts) {
            ImGui::PushID(a.id.c_str());

            // En-tête accordéon : ▸ Niv X — Nom  [Req][Choix]
            char hdr[128];
            snprintf(hdr, sizeof(hdr), "Niv %d \xe2\x80\x94 %s##apt",
                     a.niveau_acquisition, a.nom.c_str());
            bool row = ImGui::TreeNodeEx(hdr,
                ImGuiTreeNodeFlags_SpanAvailWidth);
            RenderAptBadges(a);

            if (row) {
                // Description rapide en italique grisée
                if (!a.description_rapide.empty()) {
                    ImGui::PushStyleColor(ImGuiCol_Text, {.7f,.7f,.7f,1.f});
                    ImGui::TextWrapped("%s", a.description_rapide.c_str());
                    ImGui::PopStyleColor();
                }
                // Description complète (markdown)
                if (!a.description.empty()) {
                    ImGui::Spacing();
                    UI::RenderMarkdown(a.description);
                }
                // Boutons en bas du contenu déplié
                ImGui::Spacing();
                if (ImGui::SmallButton("\xef\x81\x81 Modifier"))
                    StartSubEdit(2, false, nullptr, &a);
                ImGui::SameLine();
                if (Core::ConfirmDeleteButton("X Supprimer", a.id.c_str(), a.nom.c_str())) {
                    repo->BeginTransaction();
                    repo->DeleteAptitude(a.id);
                    repo->Commit();
                    repo->LoadChildData(classes[selectedIdx]);
                    BuildTree(classes[selectedIdx]);
                    ImGui::TreePop(); ImGui::PopID();
                    ImGui::Unindent(8.f);
                    return;
                }
                ImGui::TreePop();
            }

            ImGui::PopID();
        }
        ImGui::Unindent(8.f);
        ImGui::Spacing();
    };

    // ── Groupes informatifs (sous-classes, lecture seule) ─────────────────────
    auto renderInfo = [&](const std::vector<AptitudeData>& apts, const char* header) {
        if (apts.empty()) return;
        bool open = ImGui::CollapsingHeader(header, ImGuiTreeNodeFlags_DefaultOpen);
        if (!open) return;

        ImGui::Indent(8.f);
        for (auto& a : apts) {
            ImGui::PushID(a.id.c_str());

            char hdr[128];
            snprintf(hdr, sizeof(hdr), "Niv %d \xe2\x80\x94 %s##apt",
                     a.niveau_acquisition, a.nom.c_str());
            // Grisé pour indiquer lecture seule
            ImGui::PushStyleColor(ImGuiCol_Text, {.65f,.65f,.65f,1.f});
            bool row = ImGui::TreeNodeEx(hdr, ImGuiTreeNodeFlags_SpanAvailWidth);
            ImGui::PopStyleColor();
            RenderAptBadges(a, 0.8f);

            if (row) {
                if (!a.description_rapide.empty()) {
                    ImGui::PushStyleColor(ImGuiCol_Text, {.55f,.55f,.55f,1.f});
                    ImGui::TextWrapped("%s", a.description_rapide.c_str());
                    ImGui::PopStyleColor();
                }
                if (!a.description.empty()) {
                    ImGui::Spacing();
                    UI::RenderMarkdown(a.description);
                }
                ImGui::TreePop();
            }

            ImGui::PopID();
        }
        ImGui::Unindent(8.f);
        ImGui::Spacing();
    };

    renderEditable(c.aptitudes, c.id, c.nom.c_str());
    for (auto& sc : c.sous_classes)
        renderEditable(sc.aptitudes, sc.id, sc.nom.c_str());
}
