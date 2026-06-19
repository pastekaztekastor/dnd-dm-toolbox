#include "ClasseManagerTool.h"
#include "ClasseManagerTool_helpers.h"
#include <imgui.h>

using namespace ClasseManager;

void ClasseManagerTool::RenderEditPanel() {
    ImGui::TextColored({.8f, .6f, .2f, 1.f}, isNew ? "[Nouvelle classe]" : "[Modification]");
    ImGui::SameLine();
    if (ImGui::Button("Sauvegarder")) SaveEdit();
    ImGui::SameLine();
    if (ImGui::Button("Annuler")) CancelEdit();
    ImGui::Separator(); ImGui::Spacing();

    if (!editError.empty()) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, .35f, .35f, 1.f));
        ImGui::TextWrapped("! %s", editError.c_str());
        ImGui::PopStyleColor();
        ImGui::Spacing();
    }

    InputStr("Nom##e", editClasse.nom);
    if (isNew) InputStr("Alias / ID##e", editClasse.alias);

    ImGui::SetNextItemWidth(60); ImGui::InputInt("Dé de vie##e", &editClasse.dee_de_vie);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(70); ImGui::InputInt("PV niv 1##e", &editClasse.points_de_vie);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(70); ImGui::InputInt("PV/niv##e", &editClasse.points_de_vie_par_niveau);

    {
        const char* opts[] = { "(aucune)", "INT", "SAG", "CHA" };
        int cur = 0;
        if      (editClasse.caracteristiques_de_sorts == "INT") cur = 1;
        else if (editClasse.caracteristiques_de_sorts == "SAG") cur = 2;
        else if (editClasse.caracteristiques_de_sorts == "CHA") cur = 3;
        if (ImGui::Combo("Caract. de sorts##e", &cur, opts, 4))
            editClasse.caracteristiques_de_sorts = cur ? opts[cur] : "";
    }

    ImGui::Spacing();
    ImGui::Text("Jets de sauvegarde (JSON)");
    InputStr("##js_e", editClasse.jets_de_sauvegarde);
    ImGui::Text("Maîtrises équipement (JSON)");
    InputStr("##me_e", editClasse.equipement_maitrise);
    ImGui::Text("Compétences disponibles (JSON)");
    InputStr("##comp_e", editClasse.competences);

    ImGui::Spacing();
    ImGui::Text("Description");
    InputStrMulti("##desc_e", editClasse.description, {-1, 80});
    ImGui::Text("Aide joueur");
    InputStrMulti("##aide_e", editClasse.aide_joueur,  {-1, 60});

    ImGui::Spacing();
    InputStr("Source##e",  editClasse.source,     0, 120); ImGui::SameLine();
    InputStr("Version##e", editClasse.source_ver, 0, 80);

    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
    if (ImGui::Button("Sauvegarder##bot")) SaveEdit();
    ImGui::SameLine();
    if (ImGui::Button("Annuler##bot")) CancelEdit();
}

void ClasseManagerTool::StartEdit(bool newClasse) {
    isNew = newClasse;
    if (newClasse) {
        editClasse = ClasseData{};
        editClasse.dee_de_vie               = 6;
        editClasse.points_de_vie            = 6;
        editClasse.points_de_vie_par_niveau = 4;
    } else {
        if (selectedIdx < 0) return;
        editClasse = classes[selectedIdx];
    }
    editError.clear();
    editMode = true;
}

void ClasseManagerTool::SaveEdit() {
    editError.clear();
    if (editClasse.nom.empty()) { editError = "Le nom est requis."; return; }
    if (isNew && editClasse.alias.empty()) { editError = "L'alias (ID) est requis."; return; }
    if (repo->ExistsNom(editClasse.nom, isNew ? "" : editClasse.id)) {
        editError = "Ce nom est déjà utilisé : " + editClasse.nom; return;
    }

    repo->BeginTransaction();
    bool ok;
    if (isNew) {
        editClasse.id = editClasse.alias;
        ok = repo->InsertClasse(editClasse);
    } else {
        ok = repo->UpdateClasse(editClasse);
    }
    if (!ok) { repo->Rollback(); editError = "Erreur d'enregistrement."; return; }
    repo->Commit();

    LoadClasses();
    for (int i = 0; i < (int)classes.size(); ++i)
        if (classes[i].id == editClasse.id) { SelectClasse(i); break; }

    editMode = false; isNew = false;
}

void ClasseManagerTool::CancelEdit() {
    editMode = false; isNew = false; editError.clear();
}
