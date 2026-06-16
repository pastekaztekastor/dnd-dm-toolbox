#pragma once

#include "../../include/core/ToolBase.h"
#include <string>
#include <vector>

class DebugMonitorTool : public Core::ToolBase {
public:
    DebugMonitorTool();
    ~DebugMonitorTool() override;

    std::string GetID()          const override { return "debug_monitor"; }
    std::string GetName()        const override { return "Moniteur / Console"; }
    std::string GetCategory()    const override { return "Debug"; }
    std::string GetDescription() const override { return "Console de debug : appels ServiceBus et écoute EventBus"; }

    void OnCreate() override;
    void OnDestroy() override;
    void OnSave(sqlite3* db) override;
    void OnLoad(sqlite3* db) override;
    void Render() override;
    void OnEvent(const Core::Event& event) override;

private:
    // === Catalogue des services (depuis registry.getManifests) ===
    struct ServiceInfo {
        std::string name;
        std::string description;
        std::string paramsTemplate; // JSON formaté pour pré-remplir le champ params
    };
    struct PluginServiceEntry {
        std::string id;
        std::string name;
        std::vector<ServiceInfo> services;
    };
    std::vector<PluginServiceEntry> catalogPlugins;
    int selectedPluginIdx  = 0;
    int selectedServiceIdx = 0;
    bool catalogLoaded     = false;

    void LoadCatalog();

    // === Entrées ServiceBus ===
    char svcParams[2048] = "{}";

    // === Entrées EventBus ===
    char evtType[256]   = "";
    char evtData[2048]  = "{}";
    char subType[256]   = "";

    struct Subscription {
        std::string eventType;
        int         subID;
    };
    std::vector<Subscription> subscriptions;

    // === Console ===
    struct ConsoleEntry {
        enum class Kind { Call, Query, Publish, EventIn, Error };
        Kind        kind;
        std::string text;    // ligne résumé (affichée)
        std::string detail;  // JSON complet (tooltip au survol)
    };
    std::vector<ConsoleEntry> console;
    static constexpr int MAX_CONSOLE_ENTRIES = 500;
    bool autoScroll = true;

    void PushEntry(ConsoleEntry::Kind kind,
                   const std::string& text,
                   const std::string& detail = "");
    void RenderServiceBus();
    void RenderEventBus();
    void RenderConsole();
    double GetCurrentTime();
};
