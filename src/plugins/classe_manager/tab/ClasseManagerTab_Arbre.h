#pragma once

#include "../ClasseManagerRepository.h"
#include <string>
#include <vector>

// Tab "Arbre" : rendu de l'arbre d'évolution (lecture seule pour l'instant —
// mis en pause, cf. TODO.md, sera repris avec le moteur de graphe générique).
// TODO : corps des méthodes à écrire ensemble.

namespace ClasseManager {

class ClasseTabArbre {
public:
    explicit ClasseTabArbre(ClasseManagerRepository& repo);

    void BuildTree(const ClasseData& c);
    void Render();

private:
    struct TreeNode {
        std::string header;       // "Niv X — NomClasse"  (compartiment UML)
        std::string label;        // corps : noms d'aptitudes séparés par '\n'
        std::string tooltip;      // description_rapide (survol)
        std::string description;  // description complète (clic -> panneau bas)
        float x = 0, y = 0, w = 0, h = 0;
        float hHeader = 0;        // hauteur du compartiment d'en-tête
        int   niveau = 0;
        bool  isChoice = false;
        int   colIdx = -1;        // -1 = colonne centrale, 0..N-1 = sous-classe
        std::vector<int> childIdx;    // enfants en éventail (arêtes fourche)
        std::vector<int> seqChildIdx; // enfants en chaîne (arête droite)
    };

    ClasseManagerRepository& repo;

    std::vector<TreeNode> treeNodes;
    int selectedTreeNode = -1; // nœud sélectionné pour le panneau d'info
};

} // namespace ClasseManager
