#pragma once

#include "../../include/core/ToolBase.h"
#include "ClasseManagerRepository.h"
#include "tab/ClasseManagerTab_Classe.h"
#include "tab/ClasseManagerTab_Evolution.h"
#include "tab/ClasseManagerTab_Aptitude.h"
#include "tab/ClasseManagerTab_Equipement.h"
#include "tab/ClasseManagerTab_Arbre.h"
#include <string>
#include <vector>
#include <memory>

// Plugin "classe_manager" : cycle de vie + liste des classes + dispatch des
// tabs (Classe, Évolution, Arbre, Aptitude, Équipement).
// TODO : corps des méthodes à écrire ensemble.

class ClasseManagerTool : public Core::ToolBase {
public:
    ClasseManagerTool();
    ~ClasseManagerTool() override;

    std::string GetID()          const override { return "classe_manager"; }
    std::string GetName()        const override { return "Gestionnaire de classes DD 5.5"; }
    std::string GetCategory()    const override { return "Gestionnaire"; }
    std::string GetDescription() const override { return "Gestionnaire des classes de Donjon et Dragon 5.5 du Manuel du joueur"; }

    void OnCreate()  override;
    void OnDestroy() override;
    void OnSave(sqlite3* db) override;
    void OnLoad(sqlite3* db) override;
    void Render()    override;
    void OnEvent(const Core::Event& event) override;

private:
    // DB
    sqlite3* pluginDb = nullptr;
    std::unique_ptr<ClasseManager::ClasseManagerRepository> repo;

    // Tabs (possèdent leur propre état d'édition)
    std::unique_ptr<ClasseManager::ClasseTabClasse>     tabClasse;
    std::unique_ptr<ClasseManager::ClasseTabEvolution>  tabEvolution;
    std::unique_ptr<ClasseManager::ClasseTabAptitude>   tabAptitude;
    std::unique_ptr<ClasseManager::ClasseTabEquipement> tabEquipement;
    std::unique_ptr<ClasseManager::ClasseTabArbre>      tabArbre;

    // State – liste des classes
    std::vector<ClasseManager::ClasseData> classes;
    int  selectedIdx  = -1;
    bool detailLoaded = false;

    // State – chargement de seed
    std::string seedLoadStatus;

    // Helpers
    void LoadClasses();
    void SelectClasse(int idx);

    void RenderLeftPanel();
    void RenderRightPanel();
};
