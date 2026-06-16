#include "DebugMonitorTool.h"
#include "../../include/core/EventBus.h"
#include <imgui.h>
#include <nlohmann/json.hpp>
#include <chrono>
#include <string>

DebugMonitorTool::DebugMonitorTool() {}
DebugMonitorTool::~DebugMonitorTool() {}

// ============================================================
// LIFECYCLE
// ============================================================

void DebugMonitorTool::OnCreate() {
    LoadCatalog();
}

void DebugMonitorTool::OnDestroy() {
    if (eventBus) {
        for (auto& sub : subscriptions)
            eventBus->Unsubscribe(sub.subID);
    }
    subscriptions.clear();
}

void DebugMonitorTool::OnSave(sqlite3*) {}
void DebugMonitorTool::OnLoad(sqlite3*) {}
void DebugMonitorTool::OnEvent(const Core::Event&) {}

// ============================================================
// RENDERING
// ============================================================

void DebugMonitorTool::Render() {
    if (!isOpen) return;

    ImGui::SetNextWindowSize(ImVec2(860, 620), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(GetWindowTitle().c_str(), &isOpen)) {
        ImGui::End();
        return;
    }

    constexpr float PANEL_H = 190.0f;
    float halfW = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x) * 0.5f;

    if (ImGui::BeginChild("##sbus_panel", ImVec2(halfW, PANEL_H), true)) {
        RenderServiceBus();
    }
    ImGui::EndChild();

    ImGui::SameLine();

    if (ImGui::BeginChild("##ebus_panel", ImVec2(-1.0f, PANEL_H), true)) {
        RenderEventBus();
    }
    ImGui::EndChild();

    RenderConsole();

    ImGui::End();
}

void DebugMonitorTool::LoadCatalog() {
    catalogPlugins.clear();
    auto result = CallService("registry.getManifests", {});
    if (!result.has_value() || !result->contains("plugins")) return;

    for (auto& p : (*result)["plugins"]) {
        auto& svcArr = p["services"];
        if (!svcArr.is_array() || svcArr.empty()) continue;

        PluginServiceEntry entry;
        entry.id   = p.value("id", "");
        entry.name = p.value("name", "");

        for (auto& s : svcArr) {
            ServiceInfo si;
            si.name        = s.value("name", "");
            si.description = s.value("description", "");
            if (s.contains("params") && !s["params"].is_null())
                si.paramsTemplate = s["params"].dump(2);
            else
                si.paramsTemplate = "{}";
            entry.services.push_back(std::move(si));
        }
        catalogPlugins.push_back(std::move(entry));
    }
    selectedPluginIdx  = 0;
    selectedServiceIdx = 0;
    catalogLoaded      = true;
}

void DebugMonitorTool::RenderServiceBus() {
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "ServiceBus");
    ImGui::Separator();

    float w = ImGui::GetContentRegionAvail().x;

    // --- Catalogue : dropdown plugin puis dropdown service ---
    bool hasPlugins = !catalogPlugins.empty();

    if (!catalogLoaded || ImGui::SmallButton("Actualiser##cat")) {
        LoadCatalog();
        hasPlugins = !catalogPlugins.empty();
    }

    if (hasPlugins) {
        // Dropdown plugin
        const char* curPlugin = catalogPlugins[selectedPluginIdx].name.c_str();
        ImGui::SetNextItemWidth(w);
        if (ImGui::BeginCombo("##svcPlugin", curPlugin)) {
            for (int i = 0; i < (int)catalogPlugins.size(); ++i) {
                bool sel = (i == selectedPluginIdx);
                if (ImGui::Selectable(catalogPlugins[i].name.c_str(), sel)) {
                    selectedPluginIdx  = i;
                    selectedServiceIdx = 0;
                    // Pré-remplir params avec le template du premier service
                    auto& svcs = catalogPlugins[i].services;
                    if (!svcs.empty()) {
                        snprintf(svcParams, sizeof(svcParams), "%s",
                                 svcs[0].paramsTemplate.c_str());
                    }
                }
                if (sel) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        // Dropdown service
        auto& svcs = catalogPlugins[selectedPluginIdx].services;
        if (!svcs.empty()) {
            const char* curSvc = svcs[selectedServiceIdx].name.c_str();
            ImGui::SetNextItemWidth(w);
            if (ImGui::BeginCombo("##svcFunc", curSvc)) {
                for (int i = 0; i < (int)svcs.size(); ++i) {
                    bool sel = (i == selectedServiceIdx);
                    if (ImGui::Selectable(svcs[i].name.c_str(), sel)) {
                        selectedServiceIdx = i;
                        snprintf(svcParams, sizeof(svcParams), "%s",
                                 svcs[i].paramsTemplate.c_str());
                    }
                    if (sel) ImGui::SetItemDefaultFocus();
                    if (!svcs[i].description.empty() && ImGui::IsItemHovered()) {
                        ImGui::SetTooltip("%s", svcs[i].description.c_str());
                    }
                }
                ImGui::EndCombo();
            }
        }
    } else {
        ImGui::TextDisabled("(aucun service disponible)");
    }

    // Champ params
    ImGui::TextDisabled("Params (JSON)");
    ImGui::InputTextMultiline("##svcParams", svcParams, sizeof(svcParams), ImVec2(w, 52.0f));

    // Nom du service actif (pour l'appel)
    std::string activeSvcName;
    if (hasPlugins && !catalogPlugins[selectedPluginIdx].services.empty()) {
        activeSvcName = catalogPlugins[selectedPluginIdx].services[selectedServiceIdx].name;
    }

    bool canCall = !activeSvcName.empty();
    if (!canCall) ImGui::BeginDisabled();

    if (ImGui::Button("Call (1:1)##svc")) {
        nlohmann::json params;
        bool ok = true;
        try { params = nlohmann::json::parse(svcParams); }
        catch (const std::exception& e) {
            PushEntry(ConsoleEntry::Kind::Error, std::string("[ERR] JSON invalide : ") + e.what());
            ok = false;
        }
        if (ok) {
            auto result = CallService(activeSvcName, params);
            if (result.has_value()) {
                PushEntry(ConsoleEntry::Kind::Call,
                    "[CALL] " + activeSvcName + " → " + result->dump(),
                    result->dump(2));
            } else {
                PushEntry(ConsoleEntry::Kind::Error,
                    "[ERR] service '" + activeSvcName + "' introuvable ou non chargé");
            }
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Query (tous)##svc")) {
        nlohmann::json params;
        bool ok = true;
        try { params = nlohmann::json::parse(svcParams); }
        catch (const std::exception& e) {
            PushEntry(ConsoleEntry::Kind::Error, std::string("[ERR] JSON invalide : ") + e.what());
            ok = false;
        }
        if (ok) {
            auto results = QueryServices(activeSvcName, params);
            nlohmann::json arr = nlohmann::json::array();
            for (auto& r : results) arr.push_back(r);
            PushEntry(ConsoleEntry::Kind::Query,
                "[QUERY] " + activeSvcName + " → " + std::to_string(results.size()) + " résultat(s)",
                arr.dump(2));
        }
    }

    if (!canCall) ImGui::EndDisabled();
}

void DebugMonitorTool::RenderEventBus() {
    ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.5f, 1.0f), "EventBus");
    ImGui::Separator();

    float w = ImGui::GetContentRegionAvail().x;

    // --- Publier ---
    ImGui::TextDisabled("Type d'événement");
    float btnW = ImGui::CalcTextSize("Publier").x + ImGui::GetStyle().FramePadding.x * 2.0f + 4.0f;
    ImGui::SetNextItemWidth(w - btnW - ImGui::GetStyle().ItemSpacing.x);
    ImGui::InputText("##evtType", evtType, sizeof(evtType));
    ImGui::SameLine();
    if (ImGui::Button("Publier##evt") && evtType[0] != '\0') {
        nlohmann::json data;
        bool ok = true;
        try { data = nlohmann::json::parse(evtData); }
        catch (const std::exception& e) {
            PushEntry(ConsoleEntry::Kind::Error,
                std::string("[ERR] JSON invalide : ") + e.what());
            ok = false;
        }
        if (ok) {
            PublishEvent(evtType, data);
            PushEntry(ConsoleEntry::Kind::Publish,
                "[PUB] " + std::string(evtType) + " → " + data.dump(),
                data.dump(2));
        }
    }

    ImGui::TextDisabled("Data (JSON)");
    ImGui::InputTextMultiline("##evtData", evtData, sizeof(evtData), ImVec2(w, 44.0f));

    // --- Abonnements ---
    ImGui::Spacing();
    ImGui::TextDisabled("Abonnements actifs :");
    ImGui::SameLine();

    float subBtnW = ImGui::CalcTextSize("+").x + ImGui::GetStyle().FramePadding.x * 2.0f + 4.0f;
    ImGui::SetNextItemWidth(w - subBtnW - ImGui::GetStyle().ItemSpacing.x
                            - ImGui::CalcTextSize("Abonnements actifs :").x
                            - ImGui::GetStyle().ItemSpacing.x);
    ImGui::InputText("##subType", subType, sizeof(subType));
    ImGui::SameLine();
    if (ImGui::SmallButton("+##sub") && subType[0] != '\0' && eventBus) {
        bool already = false;
        for (auto& s : subscriptions)
            if (s.eventType == subType) { already = true; break; }

        if (!already) {
            std::string captured = subType;
            int subID = eventBus->Subscribe(captured, [this, captured](const Core::Event& e) {
                PushEntry(ConsoleEntry::Kind::EventIn,
                    "[EVT] " + captured + " ← " + e.data.dump(),
                    e.data.dump(2));
            });
            subscriptions.push_back({ captured, subID });
            subType[0] = '\0';
        }
    }

    int toRemove = -1;
    for (int i = 0; i < (int)subscriptions.size(); ++i) {
        ImGui::TextDisabled("  • %s", subscriptions[i].eventType.c_str());
        ImGui::SameLine();
        std::string btn = "x##unsub" + std::to_string(i);
        if (ImGui::SmallButton(btn.c_str()))
            toRemove = i;
    }
    if (toRemove >= 0) {
        if (eventBus) eventBus->Unsubscribe(subscriptions[toRemove].subID);
        subscriptions.erase(subscriptions.begin() + toRemove);
    }
}

void DebugMonitorTool::RenderConsole() {
    ImGui::Spacing();
    ImGui::Checkbox("Auto-scroll", &autoScroll);
    ImGui::SameLine();
    if (ImGui::Button("Vider##console")) console.clear();
    ImGui::SameLine();
    ImGui::TextDisabled("(%zu / %d entrées)", console.size(), MAX_CONSOLE_ENTRIES);

    // Légende couleurs
    ImGui::SameLine(0, 20.0f);
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "CALL");
    ImGui::SameLine(0, 4.0f);
    ImGui::TextColored(ImVec4(0.8f, 0.5f, 1.0f, 1.0f), "QUERY");
    ImGui::SameLine(0, 4.0f);
    ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.4f, 1.0f), "PUB");
    ImGui::SameLine(0, 4.0f);
    ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.5f, 1.0f), "EVT");
    ImGui::SameLine(0, 4.0f);
    ImGui::TextColored(ImVec4(1.0f, 0.35f, 0.35f, 1.0f), "ERR");

    ImGui::Separator();

    ImVec2 consoleSize = ImGui::GetContentRegionAvail();
    if (!ImGui::BeginChild("##console_log", consoleSize, false,
                           ImGuiWindowFlags_HorizontalScrollbar)) {
        ImGui::EndChild();
        return;
    }

    for (auto& entry : console) {
        ImVec4 color;
        switch (entry.kind) {
            case ConsoleEntry::Kind::Call:    color = {0.4f, 0.8f, 1.0f, 1.0f}; break;
            case ConsoleEntry::Kind::Query:   color = {0.8f, 0.5f, 1.0f, 1.0f}; break;
            case ConsoleEntry::Kind::Publish: color = {1.0f, 0.85f, 0.4f, 1.0f}; break;
            case ConsoleEntry::Kind::EventIn: color = {0.4f, 1.0f, 0.5f, 1.0f}; break;
            case ConsoleEntry::Kind::Error:   color = {1.0f, 0.35f, 0.35f, 1.0f}; break;
        }
        ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::TextUnformatted(entry.text.c_str());
        ImGui::PopStyleColor();

        if (!entry.detail.empty() && ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 40.0f);
            ImGui::TextUnformatted(entry.detail.c_str());
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }

    if (autoScroll)
        ImGui::SetScrollHereY(1.0f);

    ImGui::EndChild();
}

// ============================================================
// HELPERS
// ============================================================

void DebugMonitorTool::PushEntry(ConsoleEntry::Kind kind,
                                  const std::string& text,
                                  const std::string& detail) {
    console.push_back({ kind, text, detail });
    if ((int)console.size() > MAX_CONSOLE_ENTRIES)
        console.erase(console.begin());
}

double DebugMonitorTool::GetCurrentTime() {
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration<double>(now.time_since_epoch()).count();
}

// ============================================================
// EXPORT DU PLUGIN (C ABI)
// ============================================================

extern "C" {
    PLUGIN_API Core::ToolBase* CreateTool() {
        return new DebugMonitorTool();
    }
    PLUGIN_API void DestroyTool(Core::ToolBase* tool) {
        delete tool;
    }
}
