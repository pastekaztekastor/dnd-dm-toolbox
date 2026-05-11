#include "../../include/core/SaveFileManager.h"
#include "../../include/core/ToolBase.h"
#include "../../include/core/Logger.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <filesystem>
#include <zlib.h>

namespace Core {

SaveFileManager::SaveFileManager() {
}

SaveFileManager::~SaveFileManager() {
    Close();
}

bool SaveFileManager::New(const std::string& filepath,
                         const std::string& campaignName,
                         const std::string& description) {
    Close();

    // Ouvrir/créer la DB SQLite
    if (sqlite3_open(filepath.c_str(), &db) != SQLITE_OK) {
        std::cerr << "SaveFileManager: Error creating database: "
                  << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    currentFilePath = filepath;

    // Créer les tables
    if (!CreateTables()) {
        Close();
        return false;
    }

    // Initialiser les métadonnées
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    double timestamp = std::chrono::duration<double>(duration).count();

    metadata.campaignName = campaignName;
    metadata.description = description;
    metadata.createdAt = timestamp;
    metadata.lastModified = timestamp;
    metadata.version = 1; // Version du format

    SaveMetadata();

    std::cout << "SaveFileManager: Created new campaign '" << campaignName
              << "' at " << filepath << std::endl;

    return true;
}

bool SaveFileManager::Open(const std::string& filepath) {
    Close();

    // Vérifier si le fichier existe
    if (!std::filesystem::exists(filepath)) {
        std::cerr << "SaveFileManager: File not found: " << filepath << std::endl;
        return false;
    }

    // Ouvrir la DB SQLite
    if (sqlite3_open(filepath.c_str(), &db) != SQLITE_OK) {
        std::cerr << "SaveFileManager: Error opening database: "
                  << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    currentFilePath = filepath;

    // Charger les métadonnées
    if (!LoadMetadata()) {
        std::cerr << "SaveFileManager: Error loading metadata" << std::endl;
        Close();
        return false;
    }

    std::cout << "SaveFileManager: Opened campaign '" << metadata.campaignName
              << "' from " << filepath << std::endl;

    return true;
}

bool SaveFileManager::Save(const std::vector<ToolBase*>& tools, Logger* logger) {
    if (!db) {
        std::cerr << "SaveFileManager: No database open" << std::endl;
        return false;
    }

    // Mettre à jour le timestamp
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    metadata.lastModified = std::chrono::duration<double>(duration).count();
    lastSaveTime = metadata.lastModified;

    SaveMetadata();

    // Sauvegarder tous les tools
    for (auto* tool : tools) {
        if (tool) {
            tool->OnSave(db);
        }
    }

    // Sauvegarder les logs
    if (logger) {
        logger->SaveToDatabase(db);
    }

    std::cout << "SaveFileManager: Saved " << tools.size()
              << " tools and logs to " << currentFilePath << std::endl;

    return true;
}

bool SaveFileManager::AutoSave(const std::vector<ToolBase*>& tools, Logger* logger) {
    if (!autoSaveEnabled || !db) {
        return false;
    }

    // Créer un chemin d'autosave
    std::string autosavePath = currentFilePath + ".autosave";

    // Sauvegarder dans un fichier temporaire
    std::string tempPath = currentFilePath + ".tmp";

    sqlite3* tempDb = nullptr;
    if (sqlite3_open(tempPath.c_str(), &tempDb) != SQLITE_OK) {
        std::cerr << "SaveFileManager: Error creating autosave database" << std::endl;
        return false;
    }

    // Copier le schéma
    CreateTables();

    // Sauvegarder les tools dans la DB temporaire
    for (auto* tool : tools) {
        if (tool) {
            tool->OnSave(tempDb);
        }
    }

    // Sauvegarder les logs
    if (logger) {
        logger->SaveToDatabase(tempDb);
    }

    sqlite3_close(tempDb);

    // Renommer le fichier temporaire en autosave
    std::filesystem::rename(tempPath, autosavePath);

    std::cout << "SaveFileManager: AutoSaved to " << autosavePath << std::endl;

    return true;
}

bool SaveFileManager::Import(const std::string& sourcePath) {
    if (!db) {
        std::cerr << "SaveFileManager: No database open for import" << std::endl;
        return false;
    }

    // Ouvrir la DB source
    sqlite3* sourceDb = nullptr;
    if (sqlite3_open(sourcePath.c_str(), &sourceDb) != SQLITE_OK) {
        std::cerr << "SaveFileManager: Error opening source database: "
                  << sqlite3_errmsg(sourceDb) << std::endl;
        return false;
    }

    // TODO: Implémenter l'UI de sélection (Phase 2)
    // Pour l'instant, on importe tout

    // Importer les tool_instances
    const char* selectSQL = "SELECT id, tool_type, data FROM tool_instances";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(sourceDb, selectSQL, -1, &stmt, nullptr) == SQLITE_OK) {
        const char* insertSQL = "INSERT OR REPLACE INTO tool_instances (id, tool_type, data) VALUES (?, ?, ?)";
        sqlite3_stmt* insertStmt;

        if (sqlite3_prepare_v2(db, insertSQL, -1, &insertStmt, nullptr) == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                const char* id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                const char* toolType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                const char* data = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

                sqlite3_bind_text(insertStmt, 1, id, -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(insertStmt, 2, toolType, -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(insertStmt, 3, data, -1, SQLITE_TRANSIENT);

                sqlite3_step(insertStmt);
                sqlite3_reset(insertStmt);
            }

            sqlite3_finalize(insertStmt);
        }

        sqlite3_finalize(stmt);
    }

    sqlite3_close(sourceDb);

    std::cout << "SaveFileManager: Imported data from " << sourcePath << std::endl;

    return true;
}

void SaveFileManager::Close() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
        currentFilePath.clear();
    }
}

void SaveFileManager::UpdateMetadata(const CampaignMetadata& newMetadata) {
    metadata = newMetadata;
    SaveMetadata();
}

std::map<std::string, std::pair<std::string, nlohmann::json>> SaveFileManager::LoadToolInstances() {
    std::map<std::string, std::pair<std::string, nlohmann::json>> instances;

    if (!db) {
        return instances;
    }

    const char* selectSQL = "SELECT id, tool_type, data FROM tool_instances";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, selectSQL, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SaveFileManager: Error loading tool instances: "
                  << sqlite3_errmsg(db) << std::endl;
        return instances;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string toolType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* jsonStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

        try {
            nlohmann::json data = nlohmann::json::parse(jsonStr);
            instances[id] = {toolType, data};
        } catch (const std::exception& e) {
            std::cerr << "SaveFileManager: Error parsing tool instance data: "
                      << e.what() << std::endl;
        }
    }

    sqlite3_finalize(stmt);

    std::cout << "SaveFileManager: Loaded " << instances.size() << " tool instances" << std::endl;

    return instances;
}

bool SaveFileManager::HasAutoSave(const std::string& filepath) {
    return std::filesystem::exists(filepath + ".autosave");
}

bool SaveFileManager::RestoreFromAutoSave(const std::string& filepath) {
    std::string autosavePath = filepath + ".autosave";

    if (!std::filesystem::exists(autosavePath)) {
        return false;
    }

    // Copier l'autosave vers le fichier principal
    std::filesystem::copy_file(autosavePath, filepath,
                              std::filesystem::copy_options::overwrite_existing);

    // Supprimer l'autosave
    std::filesystem::remove(autosavePath);

    std::cout << "SaveFileManager: Restored from autosave" << std::endl;

    return true;
}

bool SaveFileManager::CreateTables() {
    if (!db) return false;

    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS meta (
            key TEXT PRIMARY KEY,
            value TEXT
        );

        CREATE TABLE IF NOT EXISTS tool_instances (
            id TEXT PRIMARY KEY,
            tool_type TEXT NOT NULL,
            data TEXT NOT NULL
        );

        CREATE TABLE IF NOT EXISTS action_log (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            instance_id TEXT NOT NULL,
            tool_type TEXT NOT NULL,
            action_type TEXT NOT NULL,
            data TEXT NOT NULL,
            timestamp REAL NOT NULL
        );
    )";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "SaveFileManager: Error creating tables: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

bool SaveFileManager::LoadMetadata() {
    if (!db) return false;

    const char* selectSQL = "SELECT key, value FROM meta";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, selectSQL, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string key = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

        if (key == "campaign_name") metadata.campaignName = value;
        else if (key == "description") metadata.description = value;
        else if (key == "created_at") metadata.createdAt = std::stod(value);
        else if (key == "last_modified") metadata.lastModified = std::stod(value);
        else if (key == "version") metadata.version = std::stoi(value);
    }

    sqlite3_finalize(stmt);
    return true;
}

bool SaveFileManager::SaveMetadata() {
    if (!db) return false;

    const char* insertSQL = "INSERT OR REPLACE INTO meta (key, value) VALUES (?, ?)";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, insertSQL, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    auto bind = [&](const std::string& key, const std::string& value) {
        sqlite3_bind_text(stmt, 1, key.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, value.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        sqlite3_reset(stmt);
    };

    bind("campaign_name", metadata.campaignName);
    bind("description", metadata.description);
    bind("created_at", std::to_string(metadata.createdAt));
    bind("last_modified", std::to_string(metadata.lastModified));
    bind("version", std::to_string(metadata.version));

    sqlite3_finalize(stmt);
    return true;
}

} // namespace Core
