#pragma once

#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>
#include "ToolBase.h"

namespace Core {
    class EventBus;
    class Logger;

    /**
     * @brief Métadonnées d'un plugin chargées depuis manifest.json
     */
    struct PluginManifest {
        std::string id;                     // ID unique (ex: "dice_roller")
        std::string name;                   // Nom affiché (ex: "Dice Roller")
        std::string description;            // Description
        std::string version;                // Version (ex: "1.0.0")
        std::string author;                 // Auteur

        struct Menu {
            std::string category;           // Catégorie (ex: "Création", "Gestion")
            int order = 0;                  // Ordre d'affichage
            std::string icon;               // Icône (emoji ou path)
            std::vector<std::string> shortcuts; // Raccourcis (ex: ["Ctrl+N"])
        } menu;

        struct Requires {
            bool database = false;
            bool event_bus = false;
            bool logger = false;
        } dependencies;

        struct Permissions {
            bool read_database = false;
            bool write_campaign = false;
            bool publish_events = false;
            bool subscribe_events = false;
        } permissions;

        struct Events {
            std::vector<std::string> publishes;   // Événements publiés
            std::vector<std::string> subscribes;  // Événements écoutés
        } events;

        struct UI {
            std::string window_title;       // Titre de la fenêtre
            int default_width = 800;
            int default_height = 600;
            bool resizable = true;
            bool can_dock = true;
        } ui;
    };

    /**
     * @brief Informations sur un plugin chargé
     */
    struct LoadedPlugin {
        PluginManifest manifest;            // Métadonnées du plugin
        void* libraryHandle = nullptr;      // Handle dlopen/LoadLibrary
        std::string libraryPath;            // Chemin de la .so/.dll

        // Pointeurs de fonctions exportées par le plugin
        using CreateToolFunc = ToolBase* (*)();
        using DestroyToolFunc = void (*)(ToolBase*);

        CreateToolFunc createTool = nullptr;
        DestroyToolFunc destroyTool = nullptr;
    };

    /**
     * @brief Registre et chargeur de plugins
     *
     * Le ToolRegistry est responsable de:
     * 1. Scanner le dossier `plugins/` pour trouver les .so/.dll
     * 2. Charger chaque plugin avec dlopen/LoadLibrary
     * 3. Lire le manifest.json de chaque plugin
     * 4. Fournir une factory pour créer des instances de tools
     * 5. Gérer le cycle de vie des plugins (chargement/déchargement)
     *
     * Workflow:
     * 1. LoadAllPlugins("plugins/") - Scan et charge tous les plugins
     * 2. CreateToolInstance("dice_roller") - Crée une instance d'un tool
     * 3. DestroyToolInstance(instance) - Détruit une instance
     *
     * Exemple d'usage:
     * @code
     * ToolRegistry registry;
     * registry.SetEventBus(&eventBus);
     * registry.SetLogger(&logger);
     *
     * // Charger tous les plugins
     * registry.LoadAllPlugins("plugins/");
     *
     * // Créer une instance de DiceRoller
     * auto* diceRoller = registry.CreateToolInstance("dice_roller");
     *
     * // Utiliser...
     * diceRoller->Render();
     *
     * // Détruire
     * registry.DestroyToolInstance(diceRoller);
     * @endcode
     */
    class ToolRegistry {
    public:
        ToolRegistry();
        ~ToolRegistry();

        /**
         * @brief Charge tous les plugins depuis un dossier
         *
         * Scanne le dossier pour les fichiers .so (Linux) ou .dll (Windows).
         * Pour chaque bibliothèque:
         * 1. Charge avec dlopen/LoadLibrary
         * 2. Résout les symboles CreateTool et DestroyTool
         * 3. Lit le manifest.json associé
         * 4. Enregistre le plugin
         *
         * @param pluginsDir Chemin du dossier des plugins (ex: "plugins/")
         * @return Nombre de plugins chargés
         */
        int LoadAllPlugins(const std::string& pluginsDir);

        /**
         * @brief Charge un plugin individuel
         *
         * @param libraryPath Chemin de la .so/.dll
         * @return true si succès
         */
        bool LoadPlugin(const std::string& libraryPath);

        /**
         * @brief Décharge un plugin
         *
         * @param pluginID ID du plugin
         */
        void UnloadPlugin(const std::string& pluginID);

        /**
         * @brief Décharge tous les plugins
         */
        void UnloadAllPlugins();

        /**
         * @brief Crée une instance d'un tool
         *
         * @param toolType Type du tool (doit correspondre à l'ID dans le manifest)
         * @return Nouvelle instance (à détruire avec DestroyToolInstance)
         */
        ToolBase* CreateToolInstance(const std::string& toolType);

        /**
         * @brief Détruit une instance d'un tool
         *
         * @param instance Instance à détruire
         */
        void DestroyToolInstance(ToolBase* instance);

        /**
         * @brief Retourne tous les plugins chargés
         */
        const std::map<std::string, LoadedPlugin>& GetLoadedPlugins() const { return loadedPlugins; }

        /**
         * @brief Retourne le manifest d'un plugin
         *
         * @param pluginID ID du plugin
         * @return Manifest (ou nullptr si non trouvé)
         */
        const PluginManifest* GetManifest(const std::string& pluginID) const;

        /**
         * @brief Retourne tous les plugins groupés par catégorie de menu
         *
         * @return Map catégorie -> liste de plugins
         */
        std::map<std::string, std::vector<const PluginManifest*>> GetPluginsByCategory() const;

        // Setters pour les ressources partagées
        void SetEventBus(EventBus* bus) { eventBus = bus; }
        void SetLogger(Logger* log) { logger = log; }

    private:
        std::map<std::string, LoadedPlugin> loadedPlugins;  // pluginID -> plugin
        std::map<ToolBase*, std::string> instanceToPlugin;  // instance -> pluginID

        // Ressources partagées (injectées dans chaque instance)
        EventBus* eventBus = nullptr;
        Logger* logger = nullptr;

        /**
         * @brief Lit un fichier manifest.json
         *
         * @param manifestPath Chemin du manifest.json
         * @param outManifest Manifest parsé (output)
         * @return true si succès
         */
        bool ReadManifest(const std::string& manifestPath, PluginManifest& outManifest);

        /**
         * @brief Génère un UUID unique pour une instance de tool
         */
        std::string GenerateInstanceID();

        /**
         * @brief Compteur pour générer des IDs uniques
         */
        static int nextInstanceID;
    };

} // namespace Core
