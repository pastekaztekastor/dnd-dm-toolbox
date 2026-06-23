#pragma once

#include "../ClasseManagerRepository.h"
#include <string>

// Tab "Classe" : infos générales, présentations (lore), choix de build
// (classe_choix_champ / classe_choix_valeur), création/suppression de classe.
// TODO : corps des méthodes à écrire ensemble.

namespace ClasseManager {

class ClasseTabClasse {
public:
    explicit ClasseTabClasse(ClasseManagerRepository& repo);

    void Render(const ClasseData& c);

    // Création / édition de la classe elle-même
    void StartEdit(bool newClasse, const ClasseData* existing = nullptr);
    void SaveEdit();
    void CancelEdit();
    bool IsEditing() const { return editMode; }
    bool IsNew()     const { return isNew; }
    void RenderEditPanel();

private:
    ClasseManagerRepository& repo;

    bool editMode = false;
    bool isNew    = false;
    ClasseData  editClasse;
    std::string editError;

    // ── Présentation ────────────────────────────────────────────────────────
    int subEditMode = 0; // 0=aucun 1=présentation 2=choix_champ 3=choix_valeur
    bool subIsNew   = false;
    PresentationData editPres;
    ChoixChampData   editChoixChamp;
    ChoixValeurData  editChoixValeur;
    std::string subEditError;

    void StartPresentationEdit(bool newEntry, const std::string& classeId,
                                const PresentationData* existing = nullptr);
    void StartChoixChampEdit(bool newEntry, const std::string& classeId,
                              const ChoixChampData* existing = nullptr);
    void StartChoixValeurEdit(bool newEntry,
                               const ChoixValeurData* existing = nullptr);
    void SaveSubEdit();
    void CancelSubEdit();

    void RenderPresentationForm();
    void RenderChoixChampForm(const ClasseData& c);
    void RenderChoixValeurForm(const ClasseData& c);
};

} // namespace ClasseManager
