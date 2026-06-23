#include "ClasseManagerTab_Aptitude.h"
#include "../ClasseManagerTool_helpers.h"

// TODO : implémentation à écrire ensemble (cf. ClasseManagerTool_tab_aptitudes.cpp.old
// pour la logique v1 de référence).

namespace ClasseManager {

ClasseTabAptitude::ClasseTabAptitude(ClasseManagerRepository& repo) : repo(repo) {}

void ClasseTabAptitude::Render(const ClasseData& /*c*/) {}

void ClasseTabAptitude::StartEdit(bool /*newEntry*/, const std::string& /*classeId*/,
                                   const AptitudeData* /*existing*/) {}
void ClasseTabAptitude::SaveEdit() {}
void ClasseTabAptitude::CancelEdit() {}

void ClasseTabAptitude::RenderAptForm(const ClasseData& /*c*/) {}

} // namespace ClasseManager
