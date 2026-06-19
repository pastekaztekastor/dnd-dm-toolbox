#include "ClasseManagerTool.h"
#include "../../include/core/utils.h"
#include <imgui.h>
#include <nfd.h>
#include <fstream>
#include <sstream>

using namespace ClasseManager;

ClasseManagerTool::ClasseManagerTool()  {}
ClasseManagerTool::~ClasseManagerTool() {}

// ============================================================
// RENDER – main
// ============================================================

void ClasseManagerTool::Render() {
    if (!isOpen) return;
    ImGui::SetNextWindowSize(ImVec2(1100, 700), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(GetWindowTitle().c_str(), &isOpen)) { ImGui::End(); return; }

    ImGui::BeginChild("##left", ImVec2(220, 0), true);
    RenderLeftPanel();
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("##right", ImVec2(0, 0), false);
    if (editMode && isNew)
        RenderEditPanel();
    else
        RenderRightPanel();
    ImGui::EndChild();


    ImGui::End();
}

// ============================================================
// LEFT PANEL
// ============================================================

void ClasseManagerTool::RenderLeftPanel() {
    if (!editMode) {
        if (ImGui::Button("+ Nouvelle classe", ImVec2(-1, 0)))
            StartEdit(true);

        if (ImGui::Button("Charger seed SQL...", ImVec2(-1, 0))) {
            nfdchar_t* path = nullptr;
            nfdfilteritem_t filter[1] = { { "Fichier SQL", "sql" } };
            nfdresult_t res = NFD_OpenDialog(&path, filter, 1, nullptr);
            if (res == NFD_OKAY && path) {
                std::ifstream f(path);
                NFD_FreePath(path);
                if (f) {
                    std::stringstream buf; buf << f.rdbuf();
                    char* err = nullptr;
                    int rc = sqlite3_exec(pluginDb, buf.str().c_str(),
                        nullptr, nullptr, &err);
                    if (rc == SQLITE_OK) {
                        LoadClasses();
                        if (!classes.empty()) SelectClasse(0);
                        seedLoadStatus = "Seed chargé avec succès.";
                    } else {
                        seedLoadStatus = err ? std::string("Erreur : ") + err : "Erreur SQL.";
                        if (err) sqlite3_free(err);
                    }
                } else {
                    seedLoadStatus = "Impossible d'ouvrir le fichier.";
                }
            }
        }

        if (!seedLoadStatus.empty()) {
            bool isErr = seedLoadStatus.rfind("Erreur", 0) == 0
                      || seedLoadStatus.rfind("Impossible", 0) == 0;
            ImGui::PushStyleColor(ImGuiCol_Text, isErr
                ? ImVec4(1.f, .4f, .4f, 1.f)
                : ImVec4(.4f, .9f, .4f, 1.f));
            ImGui::TextWrapped("%s", seedLoadStatus.c_str());
            ImGui::PopStyleColor();
            if (ImGui::IsMouseClicked(0)) seedLoadStatus.clear();
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
    }

    for (int i = 0; i < (int)classes.size(); ++i) {
        bool sel = (selectedIdx == i);
        if (ImGui::Selectable(classes[i].nom.c_str(), sel) && !editMode)
            SelectClasse(i);

        if (sel && !classes[i].sous_classes.empty()) {
            ImGui::Indent(14.f);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(.65f, .65f, .65f, 1.f));
            for (auto& sc : classes[i].sous_classes)
                ImGui::TextUnformatted(sc.nom.c_str());
            ImGui::PopStyleColor();
            ImGui::Unindent(14.f);
        }
    }

    if (selectedIdx >= 0 && !editMode) {
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        if (Core::ConfirmDeleteButton("Supprimer classe",
                classes[selectedIdx].id.c_str(),
                classes[selectedIdx].nom.c_str(),
                "Les sous-classes et toutes les données associées\n"
                "seront supprimées (CASCADE). Irréversible.",
                false)) {
            repo->BeginTransaction();
            repo->DeleteClasse(classes[selectedIdx].id);
            repo->Commit();
            LoadClasses();
            selectedIdx = -1; detailLoaded = false;
        }
    }
}

// ============================================================
// RIGHT PANEL
// ============================================================

void ClasseManagerTool::RenderRightPanel() {
    if (selectedIdx < 0 || selectedIdx >= (int)classes.size()) {
        ImGui::TextDisabled("Sélectionnez une classe dans la liste.");
        return;
    }

    const ClasseData& c = classes[selectedIdx];

    if (ImGui::BeginTabBar("##tabs")) {
        if (ImGui::BeginTabItem("Classe"))     { RenderTabClasse(c);     ImGui::EndTabItem(); }
        if (ImGui::BeginTabItem("Évolutions")) { RenderTabEvolutions(c); ImGui::EndTabItem(); }
        if (ImGui::BeginTabItem("Arbre"))      { RenderTabArbre();        ImGui::EndTabItem(); }
        if (ImGui::BeginTabItem("Aptitudes"))  { RenderTabAptitudes(c);  ImGui::EndTabItem(); }
        if (ImGui::BeginTabItem("Équipement")) { RenderTabEquipement(c); ImGui::EndTabItem(); }
        ImGui::EndTabBar();
    }
}

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
