#include "ClasseManagerTool.h"
#include "ClasseManagerTool_helpers.h"
#include "../../include/core/utils.h"
#include <imgui.h>
#include <sstream>
#include <map>

using namespace ClasseManager;

// ── Formulaire Évolution ──────────────────────────────────────────────────────

void ClasseManagerTool::RenderEvoForm() {
    SubFormHeader("Évolution", subIsNew);
    ImGui::SameLine();
    if (ImGui::Button("Sauvegarder##e")) SaveSubEdit();
    ImGui::SameLine();
    if (ImGui::Button("Annuler##e"))    CancelSubEdit();
    ImGui::Separator(); ImGui::Spacing();

    if (!subEditError.empty()) {
        ImGui::PushStyleColor(ImGuiCol_Text, {1.f,.35f,.35f,1.f});
        ImGui::TextWrapped("! %s", subEditError.c_str());
        ImGui::PopStyleColor(); ImGui::Spacing();
    }

    ImGui::SetNextItemWidth(60);
    ImGui::InputInt("Niveau##e", &editEvo.niveau);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(70);
    ImGui::InputInt("Bonus maîtrise##e", &editEvo.bonus_maitrise);
    ImGui::SameLine();
    bool csc = editEvo.choix_sous_classe != 0;
    if (ImGui::Checkbox("Choix de sous-classe##e", &csc))
        editEvo.choix_sous_classe = csc ? 1 : 0;

    ImGui::Text("Aptitudes gagnées (IDs séparés par virgule)");
    InputStr("##ag_e", editEvo.aptitudes_gagnees);

    ImGui::Spacing();
    if (ImGui::CollapsingHeader("Colonnes libres")) {
        const char* typeOpts[] = { "Nombre", "Texte" };
        for (int i = 0; i < 5; ++i) {
            ImGui::PushID(i);
            ImGui::Text("Col %d", i+1);
            ImGui::SameLine(); InputStr("Titre##c", editEvo.cols[i].titre, 0, 130);
            ImGui::SameLine(); ImGui::SetNextItemWidth(80);
            ImGui::Combo("Type##c", &editEvo.cols[i].type, typeOpts, 2);
            if (editEvo.cols[i].type == 0) {
                ImGui::SameLine(); ImGui::SetNextItemWidth(60);
                ImGui::InputInt("Val##c", &editEvo.cols[i].nombre);
            } else {
                ImGui::SameLine(); InputStr("Val##ct", editEvo.cols[i].texte, 0, 150);
            }
            ImGui::PopID();
        }
    }

    if (ImGui::CollapsingHeader("Emplacements de sorts")) {
        for (int i = 0; i < 9; ++i) {
            ImGui::PushID(i);
            char lbl[6]; snprintf(lbl, sizeof(lbl), "S%d##s", i+1);
            ImGui::SetNextItemWidth(50);
            ImGui::InputInt(lbl, &editEvo.sort_slots[i]);
            if (i < 8) ImGui::SameLine();
            ImGui::PopID();
        }
    }

    ImGui::Spacing();
    if (ImGui::Button("Sauvegarder##ebot")) SaveSubEdit();
    ImGui::SameLine();
    if (ImGui::Button("Annuler##ebot"))    CancelSubEdit();
}

// ── Tab Évolutions ────────────────────────────────────────────────────────────

void ClasseManagerTool::RenderTabEvolutions(const ClasseData& c) {
    if (subEditMode == 1) { RenderEvoForm(); return; }

    if (ImGui::Button("+ Ajouter niveau"))
        StartSubEdit(1, true);
    ImGui::Spacing();

    if (c.evolutions.empty()) {
        ImGui::TextDisabled("Aucune donnée d'évolution.");
        return;
    }

    // Dictionnaire id → AptitudeData pour résoudre les noms et détecter les prerequi
    std::map<std::string, const AptitudeData*> aptMap;
    for (auto& a : c.aptitudes)
        aptMap[a.id] = &a;
    for (auto& sc : c.sous_classes)
        for (auto& a : sc.aptitudes)
            aptMap[a.id] = &a;

    // Résout "id1,id2" → "Nom1\nNom2"
    auto resolveApts = [&](const std::string& ids) -> std::string {
        if (ids.empty()) return "";
        std::string result;
        std::istringstream ss(ids);
        std::string tok;
        while (std::getline(ss, tok, ',')) {
            while (!tok.empty() && tok.front() == ' ') tok.erase(tok.begin());
            while (!tok.empty() && tok.back()  == ' ') tok.pop_back();
            if (tok.empty()) continue;
            if (!result.empty()) result += '\n';
            auto it = aptMap.find(tok);
            result += (it != aptMap.end()) ? it->second->nom : tok;
        }
        return result;
    };

    // Vérifie si ce niveau implique un choix dans une voie
    // (aptitude de base OU sous-classe avec prerequi)
    auto hasChoixAtLevel = [&](const EvolutionData& e) -> bool {
        // Base class
        std::istringstream ss(e.aptitudes_gagnees);
        std::string tok;
        while (std::getline(ss, tok, ',')) {
            while (!tok.empty() && tok.front() == ' ') tok.erase(tok.begin());
            while (!tok.empty() && tok.back()  == ' ') tok.pop_back();
            auto it = aptMap.find(tok);
            if (it != aptMap.end() && !it->second->choix_champ_id.empty())
                return true;
        }
        // Sous-classes
        for (auto& sc : c.sous_classes)
            for (auto& a : sc.aptitudes)
                if (a.niveau_acquisition == e.niveau && !a.choix_champ_id.empty())
                    return true;
        return false;
    };

    bool hasCol[5] = {};
    bool hasSorts  = false;
    for (auto& e : c.evolutions) {
        for (int i = 0; i < 5; ++i)
            if (!e.cols[i].titre.empty()) hasCol[i] = true;
        for (int i = 0; i < 9; ++i)
            if (e.sort_slots[i]) hasSorts = true;
    }

    int nbCols = 3; // Niv | BM | Aptitudes
    for (int i = 0; i < 5; ++i) if (hasCol[i]) nbCols++;
    if (hasSorts) nbCols += 9;
    nbCols += 1; // actions

    ImGuiTableFlags tflags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg
                           | ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingFixedFit;
    if (!ImGui::BeginTable("##evo", nbCols, tflags, ImVec2(0, 0))) return;

    ImGui::TableSetupScrollFreeze(0, 1);
    ImGui::TableSetupColumn("Niv", ImGuiTableColumnFlags_WidthFixed, 28.f);
    ImGui::TableSetupColumn("BM",  ImGuiTableColumnFlags_WidthFixed, 28.f);
    ImGui::TableSetupColumn("Aptitudes", ImGuiTableColumnFlags_WidthStretch);
    for (int i = 0; i < 5; ++i) {
        if (!hasCol[i]) continue;
        std::string hdr;
        for (auto& e : c.evolutions)
            if (!e.cols[i].titre.empty()) { hdr = e.cols[i].titre; break; }
        ImGui::TableSetupColumn(hdr.c_str(), ImGuiTableColumnFlags_WidthFixed, 72.f);
    }
    if (hasSorts)
        for (int i = 1; i <= 9; ++i) {
            char lbl[4]; snprintf(lbl, sizeof(lbl), "S%d", i);
            ImGui::TableSetupColumn(lbl, ImGuiTableColumnFlags_WidthFixed, 26.f);
        }
    ImGui::TableSetupColumn("##act", ImGuiTableColumnFlags_WidthFixed, 60.f);
    ImGui::TableHeadersRow();

    int eidx = 0;
    for (auto& e : c.evolutions) {
        ImGui::TableNextRow();

        // Jaune/doré : choix de sous-classe (tu choisis ta voie)
        // Bleu-violet : aptitude(s) avec prerequi (choix dans la voie)
        if (e.choix_sous_classe)
            ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, IM_COL32(80, 68, 10, 140));
        else if (hasChoixAtLevel(e))
            ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, IM_COL32(45, 30, 85, 140));

        ImGui::TableNextColumn(); ImGui::Text("%d", e.niveau);
        ImGui::TableNextColumn(); ImGui::Text("+%d", e.bonus_maitrise);

        // Colonne aptitudes : classe de base + voies au même niveau
        ImGui::TableNextColumn();
        std::string baseNames = resolveApts(e.aptitudes_gagnees);
        bool anyContent = !baseNames.empty();

        if (!baseNames.empty())
            ImGui::TextWrapped("%s", baseNames.c_str());

        for (auto& sc : c.sous_classes) {
            std::string scNames;
            for (auto& a : sc.aptitudes) {
                if (a.niveau_acquisition != e.niveau) continue;
                if (!scNames.empty()) scNames += '\n';
                scNames += a.nom;
                if (!a.choix_champ_id.empty()) scNames += " \xe2\x96\xbc";
            }
            if (scNames.empty()) continue;
            anyContent = true;
            if (!baseNames.empty()) ImGui::Spacing();
            ImGui::PushStyleColor(ImGuiCol_Text, {.55f,.55f,.55f,1.f});
            ImGui::TextUnformatted(sc.nom.c_str());
            ImGui::PopStyleColor();
            ImGui::PushStyleColor(ImGuiCol_Text, {.72f,.72f,.72f,1.f});
            ImGui::TextWrapped("%s", scNames.c_str());
            ImGui::PopStyleColor();
        }

        if (!anyContent)
            ImGui::TextDisabled("\xe2\x80\x94");

        for (int i = 0; i < 5; ++i) {
            if (!hasCol[i]) continue;
            ImGui::TableNextColumn();
            if (e.cols[i].type == 0) ImGui::Text("%d", e.cols[i].nombre);
            else                     ImGui::TextUnformatted(e.cols[i].texte.c_str());
        }
        if (hasSorts)
            for (int i = 0; i < 9; ++i) {
                ImGui::TableNextColumn();
                if (e.sort_slots[i]) ImGui::Text("%d", e.sort_slots[i]);
                else                 ImGui::TextDisabled("\xe2\x80\x94");
            }

        ImGui::TableNextColumn();
        ImGui::PushID(eidx++);
        if (ImGui::SmallButton("\xef\x81\x81"))
            StartSubEdit(1, false, &e);
        ImGui::SameLine();
        char evo_label[32]; std::snprintf(evo_label, sizeof(evo_label), "Niv %d", e.niveau);
        if (Core::ConfirmDeleteButton("X", e.id.c_str(), evo_label)) {
            repo->BeginTransaction();
            repo->DeleteEvolution(e.id);
            repo->Commit();
            repo->LoadChildData(classes[selectedIdx]);
            BuildTree(classes[selectedIdx]);
            ImGui::PopID();
            ImGui::EndTable();
            return;
        }
        ImGui::PopID();
    }
    ImGui::EndTable();
}
