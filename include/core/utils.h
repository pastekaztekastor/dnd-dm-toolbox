#pragma once
#include <imgui.h>
#include <cstdio>

namespace Core {

// Affiche un bouton de suppression avec popup de confirmation modale.
// Retourne true uniquement quand l'utilisateur clique "Supprimer" dans la popup.
//
// popup_id  : identifiant unique (typiquement l'ID de l'entité à supprimer)
// item_name : affiché dans la popup : Supprimer "item_name" ?
// detail    : texte secondaire facultatif (avertissement CASCADE, etc.)
// small     : SmallButton si true, Button sinon
inline bool ConfirmDeleteButton(const char* btn_label,
                                const char* popup_id,
                                const char* item_name,
                                const char* detail = nullptr,
                                bool        small  = true)
{
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 0.4f, 0.4f, 1.f));
    bool clicked = small ? ImGui::SmallButton(btn_label) : ImGui::Button(btn_label);
    ImGui::PopStyleColor();

    char modal_id[512];
    std::snprintf(modal_id, sizeof(modal_id),
                  "Confirmation de suppression##%s", popup_id);

    if (clicked)
        ImGui::OpenPopup(modal_id);

    bool confirmed = false;
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(),
                            ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal(modal_id, nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::TextColored(ImVec4(1.f, 0.8f, 0.3f, 1.f),
                           "Confirmation de suppression");
        ImGui::Separator();
        ImGui::Spacing();
        if (item_name && item_name[0])
            ImGui::Text("Supprimer \"%s\" ?", item_name);
        if (detail && detail[0]) {
            ImGui::Spacing();
            ImGui::TextDisabled("%s", detail);
        }
        ImGui::Spacing();
        if (ImGui::Button("Supprimer", ImVec2(140, 0))) {
            confirmed = true;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Annuler", ImVec2(140, 0)))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
    return confirmed;
}

} // namespace Core
