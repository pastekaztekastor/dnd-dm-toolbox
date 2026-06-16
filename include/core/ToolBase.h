#pragma once

#include <string>
#include <functional>
#include <optional>
#include <vector>
#include <nlohmann/json.hpp>
#include <sqlite3.h>
#include "ServiceBus.h"

namespace Core {
    class EventBus;
    class Logger;

    /**
     * @brief Structure représentant un événement dans le système
     */
    struct Event {
        std::string type;           // Type d'événement (ex: "dice.rolled")
        nlohmann::json data;        // Données associées
        std::string sourceInstanceID; // ID de l'instance qui a émis l'événement
        double timestamp;           // Timestamp Unix
    };

    /**
     * @brief Classe de base abstraite pour tous les plugins/tools
     *
     * Chaque tool doit hériter de cette classe et implémenter les méthodes virtuelles pures.
     *
     * Architecture:
     * - Un tool est compilé en shared library (.so ou .dll)
     * - Le ToolRegistry charge les plugins au runtime via dlopen/LoadLibrary
     * - Chaque plugin expose une fonction "CreateTool" qui retourne une instance de ToolBase*
     *
     * Exemple d'implémentation:
     * @code
     * class DiceRollerTool : public ToolBase {
     * public:
     *     std::string GetID() const override { return "dice_roller"; }
     *     std::string GetName() const override { return "Dice Roller"; }
     *     void Render() override {
     *         ImGui::Begin("Dice Roller");
     *         // ... UI code ...
     *         ImGui::End();
     *     }
     *     void OnSave(sqlite3* db) override { ... }
     *     void OnLoad(sqlite3* db) override { ... }
     * };
     * @endcode
     */
    class ToolBase {
    public:
        virtual ~ToolBase() = default;

        // ============================================================
        // MÉTADONNÉES (obligatoires)
        // ============================================================

        /**
         * @brief Retourne l'ID unique du tool (doit correspondre au manifest.json)
         * @return ID unique (ex: "dice_roller", "character_sheet")
         */
        virtual std::string GetID() const = 0;

        /**
         * @brief Retourne le nom affiché du tool
         * @return Nom lisible (ex: "Dice Roller", "Fiche Personnage")
         */
        virtual std::string GetName() const = 0;

        /**
         * @brief Retourne la catégorie du tool pour le menu
         * @return Catégorie (ex: "Création", "Gestion", "Jeu")
         */
        virtual std::string GetCategory() const { return "Outils"; }

        /**
         * @brief Retourne la description du tool
         * @return Description courte
         */
        virtual std::string GetDescription() const { return ""; }

        // ============================================================
        // LIFECYCLE (obligatoires)
        // ============================================================

        /**
         * @brief Appelé à la création de l'instance du tool
         *
         * Utilisez cette méthode pour:
         * - Initialiser les variables
         * - S'abonner aux événements via eventBus->Subscribe()
         * - Charger des données depuis la DB PostgreSQL
         */
        virtual void OnCreate() {}

        /**
         * @brief Appelé à la destruction de l'instance
         *
         * Utilisez cette méthode pour:
         * - Libérer les ressources
         * - Se désabonner des événements
         */
        virtual void OnDestroy() {}

        /**
         * @brief Sauvegarde l'état de l'instance dans le fichier .dnd
         *
         * @param db Connexion SQLite vers le fichier .dnd
         *
         * Exemple:
         * @code
         * void MyTool::OnSave(sqlite3* db) {
         *     nlohmann::json state;
         *     state["my_data"] = myVariable;
         *
         *     std::string sql = "INSERT OR REPLACE INTO tool_instances (id, tool_type, data) VALUES (?, ?, ?)";
         *     sqlite3_stmt* stmt;
         *     sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
         *     sqlite3_bind_text(stmt, 1, instanceID.c_str(), -1, SQLITE_TRANSIENT);
         *     sqlite3_bind_text(stmt, 2, GetID().c_str(), -1, SQLITE_TRANSIENT);
         *     sqlite3_bind_text(stmt, 3, state.dump().c_str(), -1, SQLITE_TRANSIENT);
         *     sqlite3_step(stmt);
         *     sqlite3_finalize(stmt);
         * }
         * @endcode
         */
        virtual void OnSave(sqlite3* db) = 0;

        /**
         * @brief Charge l'état de l'instance depuis le fichier .dnd
         *
         * @param db Connexion SQLite vers le fichier .dnd
         *
         * Exemple:
         * @code
         * void MyTool::OnLoad(sqlite3* db) {
         *     std::string sql = "SELECT data FROM tool_instances WHERE id = ?";
         *     sqlite3_stmt* stmt;
         *     sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
         *     sqlite3_bind_text(stmt, 1, instanceID.c_str(), -1, SQLITE_TRANSIENT);
         *     if (sqlite3_step(stmt) == SQLITE_ROW) {
         *         const char* jsonStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
         *         auto state = nlohmann::json::parse(jsonStr);
         *         myVariable = state["my_data"];
         *     }
         *     sqlite3_finalize(stmt);
         * }
         * @endcode
         */
        virtual void OnLoad(sqlite3* db) = 0;

        // ============================================================
        // RENDERING (obligatoire)
        // ============================================================

        /**
         * @brief Rendu ImGui de l'interface du tool
         *
         * Appelé chaque frame par l'Application.
         *
         * Exemple:
         * @code
         * void MyTool::Render() {
         *     if (!isOpen) return;
         *
         *     ImGui::Begin(GetWindowTitle().c_str(), &isOpen);
         *     ImGui::Text("Mon interface");
         *     if (ImGui::Button("Action")) {
         *         DoSomething();
         *     }
         *     ImGui::End();
         * }
         * @endcode
         */
        virtual void Render() = 0;

        // ============================================================
        // EVENT HANDLING (optionnel)
        // ============================================================

        /**
         * @brief Callback appelé quand un événement auquel le tool est abonné est émis
         *
         * @param event L'événement reçu
         *
         * Exemple:
         * @code
         * void MyTool::OnEvent(const Event& event) {
         *     if (event.type == "dice.rolled") {
         *         int result = event.data["result"];
         *         std::cout << "Dé lancé: " << result << std::endl;
         *     }
         * }
         * @endcode
         */
        virtual void OnEvent(const Event& event) {}

        // ============================================================
        // HELPERS FOURNIS PAR LA CLASSE DE BASE
        // ============================================================

        /**
         * @brief Publie un événement sur l'Event Bus
         *
         * @param eventType Type de l'événement (ex: "character.created")
         * @param data Données JSON associées
         */
        void PublishEvent(const std::string& eventType, const nlohmann::json& data);

        /**
         * @brief Enregistre un service sur le ServiceBus
         *
         * @param name    Nom du service (ex: "race.getAll")
         * @param handler Fonction (json params) → json result
         * @return ServiceID à conserver pour UnregisterService dans OnDestroy
         */
        ServiceID RegisterService(const std::string& name, ServiceHandler handler);

        /**
         * @brief Retire un service précédemment enregistré
         *
         * @param id ID retourné par RegisterService
         */
        void UnregisterService(ServiceID id);

        /**
         * @brief Appelle un service d'un autre plugin (request/response synchrone)
         *
         * @param name   Nom du service
         * @param params Paramètres JSON
         * @return Résultat JSON, ou nullopt si le service n'existe pas
         */
        std::optional<nlohmann::json> CallService(const std::string& name,
                                                  const nlohmann::json& params);

        /**
         * @brief Interroge tous les providers d'un service (broadcast query)
         *
         * @param name   Nom du service
         * @param params Paramètres JSON
         * @return Vecteur de résultats (un par provider)
         */
        std::vector<nlohmann::json> QueryServices(const std::string& name,
                                                  const nlohmann::json& params);

        /**
         * @brief Log une action dans le log global
         *
         * @param actionType Type d'action (ex: "create_character", "roll_dice")
         * @param data Données associées (JSON)
         *
         * Les logs sont sauvegardés dans le .dnd et affichés dans la fenêtre de log global.
         *
         * Exemple:
         * @code
         * Log("roll_dice", {
         *     {"formula", "2d6+3"},
         *     {"result", 11}
         * });
         * @endcode
         */
        void Log(const std::string& actionType, const nlohmann::json& data);

        /**
         * @brief Retourne l'ID unique de cette instance du tool
         *
         * Chaque fois qu'un tool est ouvert, une nouvelle instance avec un UUID unique est créée.
         * Cet ID est utilisé pour:
         * - Identifier les logs
         * - Sauvegarder/charger l'état dans le .dnd
         * - Identifier la source des événements
         */
        std::string GetInstanceID() const { return instanceID; }

        /**
         * @brief Définit si la fenêtre du tool est ouverte ou fermée
         */
        void SetOpen(bool open) { isOpen = open; }

        /**
         * @brief Retourne si la fenêtre du tool est ouverte
         */
        bool IsOpen() const { return isOpen; }

        /**
         * @brief Retourne le titre de la fenêtre ImGui
         *
         * Par défaut: "NomDuTool##instanceID"
         * Peut être override pour personnaliser (ex: "Fiche de Gimli##abc123")
         */
        virtual std::string GetWindowTitle() const {
            return GetName() + "##" + instanceID;
        }

        // ============================================================
        // SETTERS (appelés par ToolRegistry au chargement)
        // ============================================================

        void SetInstanceID(const std::string& id) { instanceID = id; }
        void SetEventBus(EventBus* bus) { eventBus = bus; }
        void SetLogger(Logger* log) { logger = log; }
        void SetServiceBus(ServiceBus* bus) { serviceBus = bus; }

    protected:
        // Ressources partagées (injectées par ToolRegistry)
        std::string instanceID;                     // UUID unique de cette instance
        EventBus*   eventBus   = nullptr;             // Bus d'événements (pub/sub async)
        Logger*     logger     = nullptr;             // Système de log
        ServiceBus* serviceBus = nullptr;             // Bus de services (request/response sync)

        // État de la fenêtre
        bool isOpen = true;                         // La fenêtre est-elle ouverte?
    };

} // namespace Core

/**
 * @brief Point d'entrée du plugin
 *
 * Chaque plugin doit exporter cette fonction en C (pour éviter le name mangling).
 *
 * Exemple d'implémentation dans le .cpp du plugin:
 * @code
 * extern "C" {
 *     PLUGIN_API Core::ToolBase* CreateTool() {
 *         return new DiceRollerTool();
 *     }
 *
 *     PLUGIN_API void DestroyTool(Core::ToolBase* tool) {
 *         delete tool;
 *     }
 *
 *     PLUGIN_API const char* GetManifestPath() {
 *         return "manifest.json";
 *     }
 * }
 * @endcode
 */
#ifdef _WIN32
    #define PLUGIN_API __declspec(dllexport)
#else
    #define PLUGIN_API __attribute__((visibility("default")))
#endif
