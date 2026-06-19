#include "ClasseManagerTool.h"
#include "ClasseManagerTool_helpers.h"
#include <imgui.h>

using namespace ClasseManager;

void ClasseManagerTool::StartSubEdit(int tab, bool isNewEntry,
    const EvolutionData*       evo,
    const AptitudeData*        apt,
    const ChoixEquipementData* equip,
    const ChoixChampData*      cc,
    const ChoixValeurData*     cv)
{
    subEditMode  = tab;
    subIsNew     = isNewEntry;
    subEditError.clear();
    if (tab == 1) {
        if (evo) editEvo = *evo;
        else {
            editEvo = EvolutionData{};
            if (selectedIdx >= 0 && selectedIdx < (int)classes.size())
                editEvo.classe_id = classes[selectedIdx].id;
            editEvo.bonus_maitrise = 2;
        }
    } else if (tab == 2) {
        if (apt) editApt = *apt;
        else {
            editApt = AptitudeData{};
            if (selectedIdx >= 0 && selectedIdx < (int)classes.size())
                editApt.classe_id = classes[selectedIdx].id;
            editApt.niveau_acquisition = 1;
        }
    } else if (tab == 3) {
        if (equip) editEquip = *equip;
        else {
            editEquip = ChoixEquipementData{};
            if (selectedIdx >= 0 && selectedIdx < (int)classes.size())
                editEquip.classe_id = classes[selectedIdx].id;
            editEquip.choix_num  = 1;
            editEquip.option_num = 1;
        }
    } else if (tab == 4) {
        if (cc) editChoixChamp = *cc;
        else {
            editChoixChamp = ChoixChampData{};
            if (selectedIdx >= 0 && selectedIdx < (int)classes.size())
                editChoixChamp.classe_id = classes[selectedIdx].id;
            editChoixChamp.niveau = 1;
        }
    } else if (tab == 5) {
        if (cv) editChoixValeur = *cv;
        else    editChoixValeur = ChoixValeurData{};
    }
}

void ClasseManagerTool::SaveSubEdit() {
    subEditError.clear();
    if (!repo) return;

    if (subEditMode == 1) {
        if (editEvo.classe_id.empty()) { subEditError = "classe_id manquant."; return; }
        repo->BeginTransaction();
        bool ok;
        if (subIsNew) {
            editEvo.id = editEvo.classe_id + "_lvl_" + std::to_string(editEvo.niveau)
                       + "_" + repo->GenerateId();
            ok = repo->InsertEvolution(editEvo);
        } else {
            ok = repo->UpdateEvolution(editEvo);
        }
        if (!ok) { repo->Rollback(); subEditError = "Erreur d'enregistrement."; return; }
        repo->Commit();

    } else if (subEditMode == 2) {
        if (editApt.nom.empty())                  { subEditError = "Le nom est requis."; return; }
        if (subIsNew && editApt.alias.empty())     { subEditError = "L'alias (ID) est requis."; return; }
        repo->BeginTransaction();
        bool ok;
        if (subIsNew) {
            editApt.id = editApt.alias;
            ok = repo->InsertAptitude(editApt);
        } else {
            ok = repo->UpdateAptitude(editApt);
        }
        if (!ok) { repo->Rollback(); subEditError = "Erreur d'enregistrement."; return; }
        repo->Commit();

    } else if (subEditMode == 3) {
        if (editEquip.description.empty()) { subEditError = "La description est requise."; return; }
        repo->BeginTransaction();
        bool ok;
        if (subIsNew) {
            editEquip.id = editEquip.classe_id + "_equip_" + repo->GenerateId();
            ok = repo->InsertChoixEquipement(editEquip);
        } else {
            ok = repo->UpdateChoixEquipement(editEquip);
        }
        if (!ok) { repo->Rollback(); subEditError = "Erreur d'enregistrement."; return; }
        repo->Commit();

    } else if (subEditMode == 4) {
        if (editChoixChamp.nom.empty())   { subEditError = "Le nom est requis."; return; }
        if (editChoixChamp.alias.empty()) { subEditError = "L'alias est requis."; return; }
        repo->BeginTransaction();
        bool ok;
        if (subIsNew) {
            editChoixChamp.id = editChoixChamp.classe_id + "_champ_" + editChoixChamp.alias;
            ok = repo->InsertChoixChamp(editChoixChamp);
        } else {
            ok = repo->UpdateChoixChamp(editChoixChamp);
        }
        if (!ok) { repo->Rollback(); subEditError = "Erreur d'enregistrement."; return; }
        repo->Commit();

    } else if (subEditMode == 5) {
        if (editChoixValeur.nom.empty()    ) { subEditError = "Le nom est requis."; return; }
        if (editChoixValeur.valeur.empty() ) { subEditError = "La valeur est requise."; return; }
        if (editChoixValeur.champ_id.empty()) { subEditError = "Le champ est requis."; return; }
        repo->BeginTransaction();
        bool ok;
        if (subIsNew) {
            editChoixValeur.id = editChoixValeur.champ_id + "_" + editChoixValeur.valeur;
            ok = repo->InsertChoixValeur(editChoixValeur);
        } else {
            ok = repo->UpdateChoixValeur(editChoixValeur);
        }
        if (!ok) { repo->Rollback(); subEditError = "Erreur d'enregistrement."; return; }
        repo->Commit();
    }

    if (selectedIdx >= 0 && selectedIdx < (int)classes.size()) {
        repo->LoadChildData(classes[selectedIdx]);
        BuildTree(classes[selectedIdx]);
    }
    subEditMode = 0;
}

void ClasseManagerTool::CancelSubEdit() {
    subEditMode = 0; subEditError.clear();
}

// ── Formulaire ChoixChamp ─────────────────────────────────────────────────────

void ClasseManagerTool::RenderChoixChampForm() {
    SubFormHeader("Option de classe", subIsNew);
    ImGui::SameLine();
    if (ImGui::Button("Sauvegarder##cc")) SaveSubEdit();
    ImGui::SameLine();
    if (ImGui::Button("Annuler##cc"))    CancelSubEdit();
    ImGui::Separator(); ImGui::Spacing();

    if (!subEditError.empty()) {
        ImGui::PushStyleColor(ImGuiCol_Text, {1.f,.35f,.35f,1.f});
        ImGui::TextWrapped("! %s", subEditError.c_str());
        ImGui::PopStyleColor(); ImGui::Spacing();
    }

    InputStr("Nom##cc", editChoixChamp.nom);
    if (subIsNew) InputStr("Alias##cc", editChoixChamp.alias);
    ImGui::SetNextItemWidth(70);
    ImGui::InputInt("Niveau##cc", &editChoixChamp.niveau);
    InputStr("Description##cc", editChoixChamp.description);

    ImGui::Spacing();
    if (ImGui::Button("Sauvegarder##ccbot")) SaveSubEdit();
    ImGui::SameLine();
    if (ImGui::Button("Annuler##ccbot"))    CancelSubEdit();
}

// ── Formulaire ChoixValeur ────────────────────────────────────────────────────

void ClasseManagerTool::RenderChoixValeurForm() {
    SubFormHeader("Option — valeur", subIsNew);
    ImGui::SameLine();
    if (ImGui::Button("Sauvegarder##cv")) SaveSubEdit();
    ImGui::SameLine();
    if (ImGui::Button("Annuler##cv"))    CancelSubEdit();
    ImGui::Separator(); ImGui::Spacing();

    if (!subEditError.empty()) {
        ImGui::PushStyleColor(ImGuiCol_Text, {1.f,.35f,.35f,1.f});
        ImGui::TextWrapped("! %s", subEditError.c_str());
        ImGui::PopStyleColor(); ImGui::Spacing();
    }

    // Champ parent
    if (selectedIdx >= 0) {
        const auto& champs = classes[selectedIdx].choix_champs;
        ImGui::Text("Champ parent");
        std::vector<const char*> opts = { "(sélectionner)" };
        for (auto& cc : champs) opts.push_back(cc.nom.c_str());
        int sel = 0;
        for (int i = 0; i < (int)champs.size(); ++i)
            if (champs[i].id == editChoixValeur.champ_id) { sel = i+1; break; }
        ImGui::SetNextItemWidth(260);
        if (ImGui::Combo("##champParent_cv", &sel, opts.data(), (int)opts.size()))
            editChoixValeur.champ_id = (sel == 0) ? "" : champs[sel-1].id;
    }

    InputStr("Nom##cv", editChoixValeur.nom);
    InputStr("Valeur (clé)##cv", editChoixValeur.valeur);
    InputStr("Description##cv", editChoixValeur.description);

    ImGui::Spacing();
    if (ImGui::Button("Sauvegarder##cvbot")) SaveSubEdit();
    ImGui::SameLine();
    if (ImGui::Button("Annuler##cvbot"))    CancelSubEdit();
}
