#include "ClasseManagerTab_Evolution.h"
#include "../ClasseManagerTool_helpers.h"

// TODO : implémentation à écrire ensemble (cf. ClasseManagerTool_tab_evolutions.cpp.old
// pour la logique v1 de référence — à adapter aux colonnes dynamiques v2).

namespace ClasseManager {

ClasseTabEvolution::ClasseTabEvolution(ClasseManagerRepository& repo) : repo(repo) {}

void ClasseTabEvolution::Render(const ClasseData& /*c*/) {}

void ClasseTabEvolution::StartEdit(bool /*newEntry*/, const std::string& /*classeId*/,
                                    const EvolutionData* /*existing*/) {}
void ClasseTabEvolution::SaveEdit() {}
void ClasseTabEvolution::CancelEdit() {}

void ClasseTabEvolution::RenderEvoForm(const ClasseData& /*c*/) {}

} // namespace ClasseManager
