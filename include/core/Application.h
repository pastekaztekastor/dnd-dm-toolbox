#pragma once

#include <string>
#include <vector>
#include <memory>
#include <GLFW/glfw3.h>
#include "ToolBase.h"
#include "ToolRegistry.h"
#include "EventBus.h"
#include "ServiceBus.h"
#include "Logger.h"
#include "SaveFileManager.h"

namespace Core {

    /**
     * @brief Application principale - Orchestrateur de l'ensemble du système
     *
     * L'Application est le point central qui:
     * - Initialise tous les systèmes (ImGui, GLFW, PostgreSQL)
     * - Charge tous les plugins depuis le dossier plugins/
     * - Gère la boucle principale de rendu
     * - Affiche les menus dynamiques générés depuis les manifests
     * - Gère les fichiers de campagne (.dnd)
     * - Orchestre l'auto-save
     * - Affiche la fenêtre de log global
     *
     * Workflow:
     * 1. Init() - Initialisation de tous les systèmes
     * 2. Run() - Boucle principale
     * 3. Shutdown() - Nettoyage
     */
    class Application {
    public:
        Application();
        ~Application();

        /**
         * @brief Initialise l'application
         *
         * - Initialise GLFW + OpenGL + ImGui
         * - Connecte à la base PostgreSQL
         * - Charge tous les plugins depuis plugins/
         * - Initialise l'écran d'accueil
         *
         * @return true si succès
         */
        bool Init();

        /**
         * @brief Lance la boucle principale
         *
         * Boucle jusqu'à ce que l'utilisateur ferme l'application.
         */
        void Run();

        /**
         * @brief Arrête et nettoie l'application
         */
        void Shutdown();

        /**
         * @brief Retourne si l'application doit se fermer
         */
        bool ShouldClose() const;

    private:
        // ============================================================
        // ÉTAT DE L'APPLICATION
        // ============================================================

        GLFWwindow* window = nullptr;               // Fenêtre GLFW
        bool initialized = false;                   // Application initialisée?
        bool showWelcomeScreen = true;              // Afficher l'écran d'accueil?
        bool showDemoWindow = false;                // Afficher ImGui demo?

        // ============================================================
        // SYSTÈMES CORE
        // ============================================================

        std::unique_ptr<ToolRegistry>   toolRegistry;             // Registre de plugins
        std::unique_ptr<EventBus>       eventBus;                 // Bus d'événements (pub/sub async)
        std::unique_ptr<ServiceBus>     serviceBus;               // Bus de services (request/response sync)
        std::unique_ptr<Logger>         logger;                   // Log global
        std::unique_ptr<SaveFileManager> saveFileManager;         // Gestion .dnd

        // ============================================================
        // INSTANCES DE TOOLS
        // ============================================================

        std::vector<ToolBase*> toolInstances;       // Tools avec fenêtre ouverte
        std::vector<ToolBase*> backgroundInstances; // Plugins autoload (services actifs, fenêtre fermée)

        // ============================================================
        // FICHIERS RÉCENTS
        // ============================================================

        std::vector<std::string> recentFiles;       // Liste des fichiers récents
        static constexpr int MAX_RECENT_FILES = 10;

        // ============================================================
        // AUTO-SAVE
        // ============================================================

        double lastAutoSaveTime = 0;                // Timestamp du dernier auto-save
        static constexpr double AUTO_SAVE_INTERVAL = 600.0; // 10 minutes en secondes

        // ============================================================
        // MÉTHODES PRIVÉES - INITIALISATION
        // ============================================================

        /**
         * @brief Initialise GLFW et crée la fenêtre
         */
        bool InitWindow();

        /**
         * @brief Initialise ImGui
         */
        bool InitImGui();

        /**
         * @brief Charge tous les plugins
         */
        bool LoadPlugins();

        /**
         * @brief Charge les fichiers récents depuis un fichier de config
         */
        void LoadRecentFiles();

        /**
         * @brief Sauvegarde les fichiers récents
         */
        void SaveRecentFiles();

        // ============================================================
        // MÉTHODES PRIVÉES - BOUCLE PRINCIPALE
        // ============================================================

        /**
         * @brief Traite les événements GLFW
         */
        void ProcessEvents();

        /**
         * @brief Met à jour la logique (appelé chaque frame)
         */
        void Update(double deltaTime);

        /**
         * @brief Rendu de l'interface (appelé chaque frame)
         */
        void Render();

        /**
         * @brief Gère l'auto-save
         */
        void CheckAutoSave();

        // ============================================================
        // MÉTHODES PRIVÉES - UI
        // ============================================================

        /**
         * @brief Affiche la barre de menu principale
         */
        void RenderMenuBar();

        /**
         * @brief Affiche l'écran d'accueil
         */
        void RenderWelcomeScreen();

        /**
         * @brief Affiche tous les tools ouverts
         */
        void RenderTools();

        // ============================================================
        // MÉTHODES PRIVÉES - ACTIONS
        // ============================================================

        /**
         * @brief Crée une nouvelle campagne
         */
        void NewCampaign();

        /**
         * @brief Ouvre une campagne existante
         */
        void OpenCampaign();

        /**
         * @brief Ouvre un fichier de campagne spécifique
         *
         * @param filepath Chemin du fichier .dnd
         */
        void OpenCampaignFile(const std::string& filepath);

        /**
         * @brief Importe des données depuis un autre .dnd
         */
        void ImportCampaign();

        /**
         * @brief Sauvegarde la campagne actuelle
         */
        void SaveCampaign();

        /**
         * @brief Ferme la campagne actuelle
         */
        void CloseCampaign();

        /**
         * @brief Ouvre un tool
         *
         * @param toolType Type du tool (ID du plugin)
         */
        void OpenTool(const std::string& toolType);

        /**
         * @brief Ferme un tool
         *
         * @param tool Instance du tool à fermer
         */
        void CloseTool(ToolBase* tool);

        /**
         * @brief Ajoute un fichier à la liste des fichiers récents
         *
         * @param filepath Chemin du fichier
         */
        void AddToRecentFiles(const std::string& filepath);

        // ============================================================
        // MÉTHODES PRIVÉES - HELPERS
        // ============================================================

        /**
         * @brief Retourne le temps actuel en secondes (pour auto-save)
         */
        double GetCurrentTime();
    };

} // namespace Core
