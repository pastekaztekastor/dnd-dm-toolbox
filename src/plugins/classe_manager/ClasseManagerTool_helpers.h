#pragma once
#include <imgui.h>
#include <string>

// Callback de redimensionnement pour les InputText sur std::string
static int str_resize_cb(ImGuiInputTextCallbackData* data) {
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
        auto* str = static_cast<std::string*>(data->UserData);
        str->resize(data->BufTextLen);
        data->Buf = str->data();
    }
    return 0;
}

inline bool InputStr(const char* label, std::string& str,
    ImGuiInputTextFlags flags = 0, float width = -1.f) {
    if (width > 0) ImGui::SetNextItemWidth(width);
    str.reserve(str.size() + 64);
    return ImGui::InputText(label, str.data(), str.capacity(),
        flags | ImGuiInputTextFlags_CallbackResize, str_resize_cb, &str);
}

inline bool InputStrMulti(const char* label, std::string& str, ImVec2 size = {-1, 100}) {
    str.reserve(str.size() + 64);
    return ImGui::InputTextMultiline(label, str.data(), str.capacity(), size,
        ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_WordWrap,
        str_resize_cb, &str);
}

inline void SubFormHeader(const char* titre, bool isNew) {
    ImGui::TextColored({.8f, .6f, .2f, 1.f},
        isNew ? "[Nouveau \xe2\x80\x94 %s]" : "[Modifier \xe2\x80\x94 %s]", titre);
}
