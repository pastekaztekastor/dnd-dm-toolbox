#pragma once

#include <string>
#include <vector>
#include <sqlite3.h>
#include <nlohmann/json.hpp>

namespace Core {

    class ToolBase;
    class Logger;

    /**
     * @brief Informations de méta-données d'un fichier .dnd
     */
    struct CampaignMetadata {
        std::string campaignName;       // Nom de la campagne
        double createdAt;               // Date de création (Unix timestamp)
        double lastModified;            // Dernière modification
        std::string description;        // Description (optionnel)
        int version;                    // Version du format de fichier
    };

    /**
     * @brief Gestionnaire de fichiers de sauvegarde .dnd
     *
     * Un fichier .dnd est une base de données SQLite compressée contenant:
     * - Les métadonnées de la campagne
     * - Les instances de tous les tools ouverts (états sérialisés en JSON)
     * - L'historique complet des logs
     *
     * Format interne (SQLite):
     * ```sql
     * CREATE TABLE meta (
     *     campaign_name TEXT,
     *     created_at REAL,
     *     last_modified REAL,
     *     description TEXT,
     *     version INTEGER
     * );
     *
     * CREATE TABLE tool_instances (
     *     id TEXT PRIMARY KEY,        -- UUID de l'instance
     *     tool_type TEXT NOT NULL,    -- Type du tool (ex: "dice_roller")
     *     data TEXT NOT NULL          -- État sérialisé (JSON)
     * );
     *
     * CREATE TABLE action_log (
     *     id INTEGER PRIMARY KEY AUTOINCREMENT,
     *     instance_id TEXT NOT NULL,
     *     tool_type TEXT NOT NULL,
     *     action_type TEXT NOT NULL,
     *     data TEXT NOT NULL,
     *     timestamp REAL NOT NULL
     * );
     * ```
     *
     * Workflow:
     * 1. New() - Créer une nouvelle campagne vide
     * 2. Open() - Ouvrir un fichier .dnd existant
     * 3. Save() - Sauvegarder l'état actuel
     * 4. AutoSave() - Sauvegarde automatique (toutes les 10 min)
     * 5. Import() - Importer des données d'un autre .dnd
     * 6. Close() - Fermer le fichier
     */
    class SaveFileManager {
    public:
        SaveFileManager();
        ~SaveFileManager();

        /**
         * @brief Crée un nouveau fichier de campagne
         *
         * @param filepath Chemin du fichier .dnd à créer
         * @param campaignName Nom de la campagne
         * @param description Description (optionnel)
         * @return true si succès
         */
        bool New(const std::string& filepath,
                const std::string& campaignName,
                const std::string& description = "");

        /**
         * @brief Ouvre un fichier de campagne existant
         *
         * @param filepath Chemin du fichier .dnd
         * @return true si succès
         */
        bool Open(const std::string& filepath);

        /**
         * @brief Sauvegarde l'état actuel dans le fichier ouvert
         *
         * @param tools Liste de tous les tools ouverts
         * @param logger Logger contenant les logs à sauvegarder
         * @return true si succès
         */
        bool Save(const std::vector<ToolBase*>& tools, Logger* logger);

        /**
         * @brief Sauvegarde automatique (appelée toutes les 10 min)
         *
         * Crée un fichier temporaire .autosave à côté du .dnd principal.
         *
         * @param tools Liste de tous les tools ouverts
         * @param logger Logger contenant les logs
         * @return true si succès
         */
        bool AutoSave(const std::vector<ToolBase*>& tools, Logger* logger);

        /**
         * @brief Importe des données depuis un autre fichier .dnd
         *
         * Affiche une UI pour sélectionner quoi importer.
         *
         * @param sourcePath Chemin du fichier .dnd source
         * @return true si succès
         */
        bool Import(const std::string& sourcePath);

        /**
         * @brief Ferme le fichier actuel
         */
        void Close();

        /**
         * @brief Retourne si un fichier est actuellement ouvert
         */
        bool IsOpen() const { return db != nullptr; }

        /**
         * @brief Retourne le chemin du fichier actuellement ouvert
         */
        std::string GetCurrentFilePath() const { return currentFilePath; }

        /**
         * @brief Retourne les métadonnées de la campagne actuelle
         */
        CampaignMetadata GetMetadata() const { return metadata; }

        /**
         * @brief Met à jour les métadonnées de la campagne
         *
         * @param newMetadata Nouvelles métadonnées
         */
        void UpdateMetadata(const CampaignMetadata& newMetadata);

        /**
         * @brief Charge les instances de tools depuis le fichier
         *
         * @return Map instanceID -> (toolType, data JSON)
         */
        std::map<std::string, std::pair<std::string, nlohmann::json>> LoadToolInstances();

        /**
         * @brief Retourne la connexion SQLite (pour accès direct par les tools)
         */
        sqlite3* GetDatabase() { return db; }

        /**
         * @brief Active/désactive l'auto-save
         */
        void SetAutoSaveEnabled(bool enabled) { autoSaveEnabled = enabled; }

        /**
         * @brief Vérifie si un fichier autosave existe et propose de le restaurer
         *
         * @param filepath Chemin du fichier .dnd principal
         * @return true si un autosave existe
         */
        static bool HasAutoSave(const std::string& filepath);

        /**
         * @brief Restaure depuis l'autosave
         *
         * @param filepath Chemin du fichier .dnd principal
         * @return true si succès
         */
        bool RestoreFromAutoSave(const std::string& filepath);

    private:
        sqlite3* db = nullptr;              // Connexion SQLite
        std::string currentFilePath;        // Chemin du fichier actuel
        CampaignMetadata metadata;          // Métadonnées de la campagne
        bool autoSaveEnabled = true;        // Auto-save activé?
        double lastSaveTime = 0;            // Timestamp de la dernière sauvegarde

        /**
         * @brief Crée les tables dans la DB
         */
        bool CreateTables();

        /**
         * @brief Charge les métadonnées depuis la DB
         */
        bool LoadMetadata();

        /**
         * @brief Sauvegarde les métadonnées dans la DB
         */
        bool SaveMetadata();

        /**
         * @brief Compresse un fichier SQLite avec zlib
         *
         * @param inputPath Chemin du fichier SQLite
         * @param outputPath Chemin du fichier compressé
         * @return true si succès
         */
        static bool CompressFile(const std::string& inputPath, const std::string& outputPath);

        /**
         * @brief Décompresse un fichier .dnd
         *
         * @param inputPath Chemin du fichier .dnd compressé
         * @param outputPath Chemin du fichier SQLite décompressé
         * @return true si succès
         */
        static bool DecompressFile(const std::string& inputPath, const std::string& outputPath);
    };

} // namespace Core
