#pragma once

#include "../ClasseManagerRepository.h"
#include <string>

// Tab "Équipement" : classe_choix_equipement.
// TODO : corps des méthodes à écrire ensemble.

namespace ClasseManager {

class ClasseTabEquipement {
public:
    explicit ClasseTabEquipement(ClasseManagerRepository& repo);

    void Render(const ClasseData& c);

    void StartEdit(bool newEntry, const std::string& classeId,
                    const ChoixEquipementData* existing = nullptr);
    void SaveEdit();
    void CancelEdit();

private:
    ClasseManagerRepository& repo;

    bool editMode = false;
    bool isNew    = false;
    ChoixEquipementData editEquip;
    std::string          editError;

    void RenderEquipForm(const ClasseData& c);
};

} // namespace ClasseManager
