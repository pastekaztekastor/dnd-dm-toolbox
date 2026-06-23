#include "ClasseManagerTab_Classe.h"
#include "../ClasseManagerTool_helpers.h"

// TODO : implémentation à écrire ensemble (cf. ClasseManagerTool_tab_classe.cpp.old,
// ClasseManagerTool_edit_classe.cpp.old et ClasseManagerTool_edit_subtables.cpp.old
// pour la logique v1 de référence — RenderPresentationForm, RenderChoixChampForm,
// RenderChoixValeurForm en sont issus).

namespace ClasseManager {

ClasseTabClasse::ClasseTabClasse(ClasseManagerRepository& repo) : repo(repo) {}

void ClasseTabClasse::Render(const ClasseData& /*c*/) {}

void ClasseTabClasse::StartEdit(bool /*newClasse*/, const ClasseData* /*existing*/) {}
void ClasseTabClasse::SaveEdit() {}
void ClasseTabClasse::CancelEdit() {}
void ClasseTabClasse::RenderEditPanel() {}

void ClasseTabClasse::StartPresentationEdit(bool /*newEntry*/, const std::string& /*classeId*/,
                                             const PresentationData* /*existing*/) {}
void ClasseTabClasse::StartChoixChampEdit(bool /*newEntry*/, const std::string& /*classeId*/,
                                           const ChoixChampData* /*existing*/) {}
void ClasseTabClasse::StartChoixValeurEdit(bool /*newEntry*/,
                                            const ChoixValeurData* /*existing*/) {}
void ClasseTabClasse::SaveSubEdit() {}
void ClasseTabClasse::CancelSubEdit() {}

void ClasseTabClasse::RenderPresentationForm() {}
void ClasseTabClasse::RenderChoixChampForm(const ClasseData& /*c*/) {}
void ClasseTabClasse::RenderChoixValeurForm(const ClasseData& /*c*/) {}

} // namespace ClasseManager
