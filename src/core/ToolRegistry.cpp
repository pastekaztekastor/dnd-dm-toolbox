#include "../../include/core/ToolRegistry.h"
#include "../../include/core/EventBus.h"
#include "../../include/core/Logger.h"
#include <iostream>
#include <filesystem>
#include <fstream>

#ifdef _WIN32
    #include <windows.h>
    #define LOAD_LIBRARY(path) LoadLibraryA(path)
    #define GET_SYMBOL(handle, name) GetProcAddress((HMODULE)handle, name)
    #define CLOSE_LIBRARY(handle) FreeLibrary((HMODULE)handle)
    #define LIBRARY_EXTENSION ".dll"
#else
    #include <dlfcn.h>
    #define LOAD_LIBRARY(path) dlopen(path, RTLD_LAZY)
    #define GET_SYMBOL(handle, name) dlsym(handle, name)
    #define CLOSE_LIBRARY(handle) dlclose(handle)
    #define LIBRARY_EXTENSION ".so"
#endif

namespace Core {

int ToolRegistry::nextInstanceID = 0;

ToolRegistry::ToolRegistry() {
}

ToolRegistry::~ToolRegistry() {
    UnloadAllPlugins();
}

int ToolRegistry::LoadAllPlugins(const std::string& pluginsDir) {
    if (!std::filesystem::exists(pluginsDir)) {
        std::cerr << "ToolRegistry: Plugins directory not found: " << pluginsDir << std::endl;
        return 0;
    }

    int loadedCount = 0;

    // Scanner le dossier pour les fichiers .so ou .dll
    for (const auto& entry : std::filesystem::directory_iterator(pluginsDir)) {
        if (entry.is_regular_file()) {
            std::string path = entry.path().string();
            std::string ext = entry.path().extension().string();

            if (ext == LIBRARY_EXTENSION) {
                if (LoadPlugin(path)) {
                    loadedCount++;
                }
            }
        }
    }

    std::cout << "ToolRegistry: Loaded " << loadedCount << " plugins" << std::endl;

    return loadedCount;
}

bool ToolRegistry::LoadPlugin(const std::string& libraryPath) {
    std::cout << "ToolRegistry: Loading plugin from " << libraryPath << std::endl;

    // Charger la bibliothèque
    void* handle = LOAD_LIBRARY(libraryPath.c_str());
    if (!handle) {
#ifdef _WIN32
        std::cerr << "ToolRegistry: Error loading library: " << GetLastError() << std::endl;
#else
        std::cerr << "ToolRegistry: Error loading library: " << dlerror() << std::endl;
#endif
        return false;
    }

    // Résoudre les symboles
    auto createTool = (LoadedPlugin::CreateToolFunc)GET_SYMBOL(handle, "CreateTool");
    auto destroyTool = (LoadedPlugin::DestroyToolFunc)GET_SYMBOL(handle, "DestroyTool");

    if (!createTool || !destroyTool) {
        std::cerr << "ToolRegistry: Plugin missing required symbols (CreateTool/DestroyTool)" << std::endl;
        CLOSE_LIBRARY(handle);
        return false;
    }

    // Chercher le manifest.json dans le même dossier
    std::filesystem::path libPath(libraryPath);
    std::string baseName = libPath.stem().string();

    // Sur Linux, les .so ont souvent "lib" comme préfixe (ex: libdice_roller.so)
    // On l'enlève pour trouver le manifest (dice_roller.manifest.json)
    if (baseName.substr(0, 3) == "lib") {
        baseName = baseName.substr(3);
    }

    std::filesystem::path manifestPath = libPath.parent_path() / (baseName + ".manifest.json");

    // Si pas trouvé, essayer manifest.json tout court
    if (!std::filesystem::exists(manifestPath)) {
        manifestPath = libPath.parent_path() / "manifest.json";
    }

    PluginManifest manifest;
    if (!ReadManifest(manifestPath.string(), manifest)) {
        std::cerr << "ToolRegistry: Error reading manifest for " << libraryPath << std::endl;
        CLOSE_LIBRARY(handle);
        return false;
    }

    // Enregistrer le plugin
    LoadedPlugin plugin;
    plugin.manifest = manifest;
    plugin.libraryHandle = handle;
    plugin.libraryPath = libraryPath;
    plugin.createTool = createTool;
    plugin.destroyTool = destroyTool;

    loadedPlugins[manifest.id] = plugin;

    std::cout << "ToolRegistry: Loaded plugin '" << manifest.name
              << "' (ID: " << manifest.id << ")" << std::endl;

    return true;
}

void ToolRegistry::UnloadPlugin(const std::string& pluginID) {
    auto it = loadedPlugins.find(pluginID);
    if (it == loadedPlugins.end()) {
        return;
    }

    if (it->second.libraryHandle) {
        CLOSE_LIBRARY(it->second.libraryHandle);
    }

    loadedPlugins.erase(it);

    std::cout << "ToolRegistry: Unloaded plugin " << pluginID << std::endl;
}

void ToolRegistry::UnloadAllPlugins() {
    for (auto& [id, plugin] : loadedPlugins) {
        if (plugin.libraryHandle) {
            CLOSE_LIBRARY(plugin.libraryHandle);
        }
    }

    loadedPlugins.clear();

    std::cout << "ToolRegistry: Unloaded all plugins" << std::endl;
}

ToolBase* ToolRegistry::CreateToolInstance(const std::string& toolType) {
    auto it = loadedPlugins.find(toolType);
    if (it == loadedPlugins.end()) {
        std::cerr << "ToolRegistry: Plugin not found: " << toolType << std::endl;
        return nullptr;
    }

    // Créer l'instance
    ToolBase* instance = it->second.createTool();
    if (!instance) {
        std::cerr << "ToolRegistry: Failed to create instance of " << toolType << std::endl;
        return nullptr;
    }

    // Injecter les dépendances
    instance->SetInstanceID(GenerateInstanceID());
    instance->SetEventBus(eventBus);
    instance->SetLogger(logger);

    // Enregistrer l'instance
    instanceToPlugin[instance] = toolType;

    // Appeler OnCreate
    instance->OnCreate();

    std::cout << "ToolRegistry: Created instance of " << toolType
              << " (ID: " << instance->GetInstanceID() << ")" << std::endl;

    return instance;
}

void ToolRegistry::DestroyToolInstance(ToolBase* instance) {
    if (!instance) return;

    auto it = instanceToPlugin.find(instance);
    if (it == instanceToPlugin.end()) {
        std::cerr << "ToolRegistry: Instance not registered" << std::endl;
        delete instance; // Fallback
        return;
    }

    std::string pluginID = it->second;
    auto pluginIt = loadedPlugins.find(pluginID);

    if (pluginIt == loadedPlugins.end()) {
        std::cerr << "ToolRegistry: Plugin not found for instance" << std::endl;
        delete instance; // Fallback
        return;
    }

    // Appeler OnDestroy
    instance->OnDestroy();

    // Détruire l'instance via le plugin
    if (pluginIt->second.destroyTool) {
        pluginIt->second.destroyTool(instance);
    } else {
        delete instance; // Fallback
    }

    instanceToPlugin.erase(it);

    std::cout << "ToolRegistry: Destroyed instance of " << pluginID << std::endl;
}

const PluginManifest* ToolRegistry::GetManifest(const std::string& pluginID) const {
    auto it = loadedPlugins.find(pluginID);
    if (it != loadedPlugins.end()) {
        return &it->second.manifest;
    }
    return nullptr;
}

std::map<std::string, std::vector<const PluginManifest*>> ToolRegistry::GetPluginsByCategory() const {
    std::map<std::string, std::vector<const PluginManifest*>> result;

    for (const auto& [id, plugin] : loadedPlugins) {
        std::string category = plugin.manifest.menu.category;
        if (category.empty()) {
            category = "Outils";
        }
        result[category].push_back(&plugin.manifest);
    }

    // Trier chaque catégorie par order
    for (auto& [category, manifests] : result) {
        std::sort(manifests.begin(), manifests.end(),
                 [](const PluginManifest* a, const PluginManifest* b) {
                     return a->menu.order < b->menu.order;
                 });
    }

    return result;
}

bool ToolRegistry::ReadManifest(const std::string& manifestPath, PluginManifest& outManifest) {
    if (!std::filesystem::exists(manifestPath)) {
        std::cerr << "ToolRegistry: Manifest not found: " << manifestPath << std::endl;
        return false;
    }

    std::ifstream file(manifestPath);
    if (!file.is_open()) {
        std::cerr << "ToolRegistry: Error opening manifest: " << manifestPath << std::endl;
        return false;
    }

    try {
        nlohmann::json json;
        file >> json;

        // Champs obligatoires
        outManifest.id = json["id"];
        outManifest.name = json["name"];

        // Champs optionnels
        if (json.contains("description")) outManifest.description = json["description"];
        if (json.contains("version")) outManifest.version = json["version"];
        if (json.contains("author")) outManifest.author = json["author"];

        // Menu
        if (json.contains("menu")) {
            auto& menu = json["menu"];
            if (menu.contains("category")) outManifest.menu.category = menu["category"];
            if (menu.contains("order")) outManifest.menu.order = menu["order"];
            if (menu.contains("icon")) outManifest.menu.icon = menu["icon"];
            if (menu.contains("shortcuts")) {
                for (const auto& shortcut : menu["shortcuts"]) {
                    outManifest.menu.shortcuts.push_back(shortcut);
                }
            }
        }

        // Dependencies (anciennement "requires" mais C++20 réserve ce mot-clé)
        if (json.contains("requires")) {
            auto& req = json["requires"];
            if (req.contains("database")) outManifest.dependencies.database = req["database"];
            if (req.contains("event_bus")) outManifest.dependencies.event_bus = req["event_bus"];
            if (req.contains("logger")) outManifest.dependencies.logger = req["logger"];
        }

        // Permissions
        if (json.contains("permissions")) {
            auto& perm = json["permissions"];
            if (perm.contains("read_database")) outManifest.permissions.read_database = perm["read_database"];
            if (perm.contains("write_campaign")) outManifest.permissions.write_campaign = perm["write_campaign"];
            if (perm.contains("publish_events")) outManifest.permissions.publish_events = perm["publish_events"];
            if (perm.contains("subscribe_events")) outManifest.permissions.subscribe_events = perm["subscribe_events"];
        }

        // Events
        if (json.contains("events")) {
            auto& events = json["events"];
            if (events.contains("publishes")) {
                for (const auto& event : events["publishes"]) {
                    outManifest.events.publishes.push_back(event);
                }
            }
            if (events.contains("subscribes")) {
                for (const auto& event : events["subscribes"]) {
                    outManifest.events.subscribes.push_back(event);
                }
            }
        }

        // UI
        if (json.contains("ui")) {
            auto& ui = json["ui"];
            if (ui.contains("window_title")) outManifest.ui.window_title = ui["window_title"];
            if (ui.contains("default_width")) outManifest.ui.default_width = ui["default_width"];
            if (ui.contains("default_height")) outManifest.ui.default_height = ui["default_height"];
            if (ui.contains("resizable")) outManifest.ui.resizable = ui["resizable"];
            if (ui.contains("can_dock")) outManifest.ui.can_dock = ui["can_dock"];
        }

        return true;

    } catch (const std::exception& e) {
        std::cerr << "ToolRegistry: Error parsing manifest: " << e.what() << std::endl;
        return false;
    }
}

std::string ToolRegistry::GenerateInstanceID() {
    return "instance_" + std::to_string(nextInstanceID++);
}

} // namespace Core
