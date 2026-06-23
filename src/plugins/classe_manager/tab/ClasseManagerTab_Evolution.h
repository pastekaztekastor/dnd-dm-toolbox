#pragma once

#include "../ClasseManagerRepository.h"
#include <string>

// Tab "Évolution" : classe_evolutions + classe_evolution_valeurs
// (colonnes dynamiques définies par classe_evolution_colonnes).
// TODO : corps des méthodes à écrire ensemble.

namespace ClasseManager {

class ClasseTabEvolution {
public:
    explicit ClasseTabEvolution(ClasseManagerRepository& repo);

    void Render(const ClasseData& c);

    void StartEdit(bool newEntry, const std::string& classeId,
                    const EvolutionData* existing = nullptr);
    void SaveEdit();
    void CancelEdit();

private:
    ClasseManagerRepository& repo;

    bool editMode = false;
    bool isNew    = false;
    EvolutionData editEvo;
    std::string   editError;

    void RenderEvoForm(const ClasseData& c);
};

} // namespace ClasseManager
