#pragma once

#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>
#include <sqlite3.h>

namespace Core {

    /**
     * @brief Entrée de log dans le système
     */
    struct LogEntry {
        int id = 0;                         // ID auto-incrémenté (dans la DB)
        std::string instanceID;             // ID de l'instance du tool qui a loggé
        std::string toolType;               // Type du tool (ex: "dice_roller")
        std::string actionType;             // Type d'action (ex: "roll_dice", "create_character")
        nlohmann::json data;                // Données associées (JSON)
        double timestamp;                   // Timestamp Unix
        std::string displayText;            // Texte formaté pour affichage (généré automatiquement)
    };

    /**
     * @brief Système de log global de l'application
     *
     * Le Logger enregistre toutes les actions significatives des tools.
     * Les logs sont:
     * - Affichés dans une fenêtre ImGui filtrable
     * - Sauvegardés dans le fichier .dnd
     * - Utilisés pour reconstruire l'historique de la campagne
     *
     * Philosophie:
     * "En lisant les logs, on doit pouvoir reconstruire l'histoire racontée"
     *
     * Exemple d'usage:
     * @code
     * logger->Log("dice_roller_instance_123", "dice_roller", "roll_dice", {
     *     {"formula", "2d6+3"},
     *     {"result", 11}
     * });
     *
     * // Génère:
     * // [10:23:45] Dice Roller: Lance 2d6+3 → 11
     * @endcode
     */
    class Logger {
    public:
        Logger();
        ~Logger();

        /**
         * @brief Log une action
         *
         * @param instanceID ID de l'instance du tool
         * @param toolType Type du tool (ex: "dice_roller")
         * @param actionType Type d'action (ex: "roll_dice")
         * @param data Données JSON associées
         */
        void Log(const std::string& instanceID,
                const std::string& toolType,
                const std::string& actionType,
                const nlohmann::json& data);

        /**
         * @brief Sauvegarde tous les logs dans la DB SQLite
         *
         * @param db Connexion SQLite vers le fichier .dnd
         */
        void SaveToDatabase(sqlite3* db);

        /**
         * @brief Charge les logs depuis la DB SQLite
         *
         * @param db Connexion SQLite vers le fichier .dnd
         */
        void LoadFromDatabase(sqlite3* db);

        /**
         * @brief Retourne tous les logs
         */
        const std::vector<LogEntry>& GetAllLogs() const { return logs; }

        /**
         * @brief Retourne les logs filtrés par type de tool
         *
         * @param toolType Type de tool (ex: "dice_roller")
         */
        std::vector<LogEntry> GetLogsByTool(const std::string& toolType) const;

        /**
         * @brief Retourne les logs filtrés par instance
         *
         * @param instanceID ID de l'instance
         */
        std::vector<LogEntry> GetLogsByInstance(const std::string& instanceID) const;

        /**
         * @brief Retourne les N derniers logs
         *
         * @param count Nombre de logs à retourner
         */
        std::vector<LogEntry> GetRecentLogs(size_t count) const;

        /**
         * @brief Vide tous les logs (utile pour les tests)
         */
        void Clear() { logs.clear(); }

        /**
         * @brief Retourne le nombre total de logs
         */
        size_t GetLogCount() const { return logs.size(); }

        /**
         * @brief Enregistre un formatteur personnalisé pour un type d'action
         *
         * Les formatteurs permettent de personnaliser l'affichage des logs.
         *
         * @param actionType Type d'action (ex: "roll_dice")
         * @param formatter Fonction qui génère le texte d'affichage
         *
         * Exemple:
         * @code
         * logger->RegisterFormatter("roll_dice", [](const LogEntry& entry) {
         *     std::string formula = entry.data["formula"];
         *     int result = entry.data["result"];
         *     return "Lance " + formula + " → " + std::to_string(result);
         * });
         * @endcode
         */
        using LogFormatter = std::function<std::string(const LogEntry&)>;
        void RegisterFormatter(const std::string& actionType, LogFormatter formatter);

        /**
         * @brief Enregistre un nom lisible pour un type de tool
         *
         * @param toolType Type de tool (ex: "dice_roller")
         * @param displayName Nom affiché (ex: "Dice Roller")
         */
        void RegisterToolDisplayName(const std::string& toolType, const std::string& displayName);

    private:
        std::vector<LogEntry> logs;                                  // Tous les logs
        std::map<std::string, LogFormatter> formatters;              // Formatteurs personnalisés
        std::map<std::string, std::string> toolDisplayNames;         // Noms affichés des tools

        /**
         * @brief Génère le texte d'affichage pour un log
         *
         * Utilise les formatteurs personnalisés si disponibles, sinon génère un texte par défaut.
         */
        std::string GenerateDisplayText(const LogEntry& entry);

        /**
         * @brief Formate un timestamp Unix en chaîne lisible
         *
         * @param timestamp Timestamp Unix
         * @return Chaîne formatée (ex: "10:23:45")
         */
        std::string FormatTimestamp(double timestamp);
    };

} // namespace Core
