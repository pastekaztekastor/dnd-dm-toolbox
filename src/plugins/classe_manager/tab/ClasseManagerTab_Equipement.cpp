#include "ClasseManagerTab_Equipement.h"
#include "../ClasseManagerTool_helpers.h"

// TODO : implémentation à écrire ensemble (cf. ClasseManagerTool_tab_equipement.cpp.old
// pour la logique v1 de référence).

namespace ClasseManager {

ClasseTabEquipement::ClasseTabEquipement(ClasseManagerRepository& repo) : repo(repo) {}

void ClasseTabEquipement::Render(const ClasseData& /*c*/) {}

void ClasseTabEquipement::StartEdit(bool /*newEntry*/, const std::string& /*classeId*/,
                                     const ChoixEquipementData* /*existing*/) {}
void ClasseTabEquipement::SaveEdit() {}
void ClasseTabEquipement::CancelEdit() {}

void ClasseTabEquipement::RenderEquipForm(const ClasseData& /*c*/) {}

} // namespace ClasseManager
