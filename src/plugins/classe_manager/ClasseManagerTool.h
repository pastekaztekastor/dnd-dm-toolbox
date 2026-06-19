#pragma once

#include "../../include/core/ToolBase.h"
#include "ClasseManagerRepository.h"
#include <string>
#include <vector>
#include <memory>

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

    // State – list
    std::vector<ClasseManager::ClasseData> classes;
    int  selectedIdx   = -1;
    bool detailLoaded  = false;

    // Arbre d'évolution (positions pré-calculées à la sélection)
    struct TreeNode {
        std::string header;       // "Niv X — NomClasse"  (compartiment UML)
        std::string label;        // corps : noms d'aptitudes séparés par '\n'
        std::string tooltip;      // description_rapide (survol)
        std::string description;  // description complète (clic → panneau bas)
        float x=0, y=0, w=0, h=0;
        float hHeader=0;          // hauteur du compartiment d'en-tête
        int   niveau=0;
        bool  isChoice=false;
        int   colIdx=-1;          // -1 = colonne centrale, 0..N-1 = sous-classe
        std::vector<int> childIdx;    // enfants en éventail (arêtes fourche)
        std::vector<int> seqChildIdx; // enfants en chaîne (arête droite)
    };
    std::vector<TreeNode> treeNodes;
    int  selectedTreeNode = -1;   // nœud sélectionné pour le panneau d'info
    void BuildTree(const ClasseManager::ClasseData& c);

    // State – seed loader
    std::string seedLoadStatus;

    // State – edit classe
    bool editMode  = false;
    bool isNew     = false;
    ClasseManager::ClasseData editClasse;
    std::string editError;

    // State – édition sous-tables (0=aucune 1=évolution 2=aptitude 3=équipement 4=choix_champ 5=choix_valeur)
    int  subEditMode = 0;
    bool subIsNew    = false;
    ClasseManager::EvolutionData       editEvo;
    ClasseManager::AptitudeData        editApt;
    ClasseManager::ChoixEquipementData editEquip;
    ClasseManager::ChoixChampData      editChoixChamp;
    ClasseManager::ChoixValeurData     editChoixValeur;
    std::string subEditError;

    // Helpers
    void LoadClasses();
    void SelectClasse(int idx);
    void StartEdit(bool newClasse);
    void SaveEdit();
    void CancelEdit();

    void StartSubEdit(int tab, bool isNew,
                      const ClasseManager::EvolutionData*       evo   = nullptr,
                      const ClasseManager::AptitudeData*        apt   = nullptr,
                      const ClasseManager::ChoixEquipementData* equip = nullptr,
                      const ClasseManager::ChoixChampData*      cc    = nullptr,
                      const ClasseManager::ChoixValeurData*     cv    = nullptr);
    void SaveSubEdit();
    void CancelSubEdit();

    void RenderLeftPanel();
    void RenderRightPanel();
    void RenderTabClasse(const ClasseManager::ClasseData& c);
    void RenderTabEvolutions(const ClasseManager::ClasseData& c);
    void RenderTabArbre();
    void RenderTabAptitudes(const ClasseManager::ClasseData& c);
    void RenderTabEquipement(const ClasseManager::ClasseData& c);
    void RenderEditPanel();

    void RenderEvoForm();
    void RenderAptForm();
    void RenderEquipForm();
    void RenderChoixChampForm();
    void RenderChoixValeurForm();
};
