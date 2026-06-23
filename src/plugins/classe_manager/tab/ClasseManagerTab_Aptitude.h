#pragma once

#include "../ClasseManagerRepository.h"
#include <string>

// Tab "Aptitude" : classe_aptitudes (y compris sous-classes).
// TODO : corps des méthodes à écrire ensemble.

namespace ClasseManager {

class ClasseTabAptitude {
public:
    explicit ClasseTabAptitude(ClasseManagerRepository& repo);

    void Render(const ClasseData& c);

    void StartEdit(bool newEntry, const std::string& classeId,
                    const AptitudeData* existing = nullptr);
    void SaveEdit();
    void CancelEdit();

private:
    ClasseManagerRepository& repo;

    bool editMode = false;
    bool isNew    = false;
    AptitudeData editApt;
    std::string  editError;

    void RenderAptForm(const ClasseData& c);
};

} // namespace ClasseManager
