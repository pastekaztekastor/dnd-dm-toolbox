#include "../../include/core/Logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace Core {

Logger::Logger() {
    // Enregistrer des formatteurs par défaut
    RegisterFormatter("default", [](const LogEntry& entry) {
        return entry.actionType + ": " + entry.data.dump();
    });
}

Logger::~Logger() {
}

void Logger::Log(const std::string& instanceID,
                const std::string& toolType,
                const std::string& actionType,
                const nlohmann::json& data) {
    LogEntry entry;
    entry.instanceID = instanceID;
    entry.toolType = toolType;
    entry.actionType = actionType;
    entry.data = data;

    // Timestamp Unix en secondes
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    entry.timestamp = std::chrono::duration<double>(duration).count();

    // Générer le texte d'affichage
    entry.displayText = GenerateDisplayText(entry);

    logs.push_back(entry);

    // Debug output
    std::cout << "[LOG] " << FormatTimestamp(entry.timestamp)
              << " " << entry.displayText << std::endl;
}

void Logger::SaveToDatabase(sqlite3* db) {
    if (!db) return;

    // Créer la table si elle n'existe pas
    const char* createTableSQL = R"(
        CREATE TABLE IF NOT EXISTS action_log (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            instance_id TEXT NOT NULL,
            tool_type TEXT NOT NULL,
            action_type TEXT NOT NULL,
            data TEXT NOT NULL,
            timestamp REAL NOT NULL
        )
    )";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Logger: Error creating action_log table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return;
    }

    // Préparer la requête d'insertion
    const char* insertSQL = R"(
        INSERT INTO action_log (instance_id, tool_type, action_type, data, timestamp)
        VALUES (?, ?, ?, ?, ?)
    )";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, insertSQL, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Logger: Error preparing insert statement: "
                  << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Insérer tous les logs
    for (const auto& entry : logs) {
        sqlite3_bind_text(stmt, 1, entry.instanceID.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, entry.toolType.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, entry.actionType.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, entry.data.dump().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_double(stmt, 5, entry.timestamp);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Logger: Error inserting log: " << sqlite3_errmsg(db) << std::endl;
        }

        sqlite3_reset(stmt);
    }

    sqlite3_finalize(stmt);

    std::cout << "Logger: Saved " << logs.size() << " logs to database" << std::endl;
}

void Logger::LoadFromDatabase(sqlite3* db) {
    if (!db) return;

    logs.clear();

    const char* selectSQL = R"(
        SELECT id, instance_id, tool_type, action_type, data, timestamp
        FROM action_log
        ORDER BY timestamp ASC
    )";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, selectSQL, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Logger: Error preparing select statement: "
                  << sqlite3_errmsg(db) << std::endl;
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        LogEntry entry;
        entry.id = sqlite3_column_int(stmt, 0);
        entry.instanceID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        entry.toolType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        entry.actionType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

        const char* jsonStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        try {
            entry.data = nlohmann::json::parse(jsonStr);
        } catch (const std::exception& e) {
            std::cerr << "Logger: Error parsing JSON: " << e.what() << std::endl;
            entry.data = nlohmann::json::object();
        }

        entry.timestamp = sqlite3_column_double(stmt, 5);
        entry.displayText = GenerateDisplayText(entry);

        logs.push_back(entry);
    }

    sqlite3_finalize(stmt);

    std::cout << "Logger: Loaded " << logs.size() << " logs from database" << std::endl;
}

std::vector<LogEntry> Logger::GetLogsByTool(const std::string& toolType) const {
    std::vector<LogEntry> result;
    for (const auto& entry : logs) {
        if (entry.toolType == toolType) {
            result.push_back(entry);
        }
    }
    return result;
}

std::vector<LogEntry> Logger::GetLogsByInstance(const std::string& instanceID) const {
    std::vector<LogEntry> result;
    for (const auto& entry : logs) {
        if (entry.instanceID == instanceID) {
            result.push_back(entry);
        }
    }
    return result;
}

std::vector<LogEntry> Logger::GetRecentLogs(size_t count) const {
    // Si aucun log ou si on demande tous les logs
    if (logs.empty() || logs.size() <= count) {
        return logs;
    }

    // Retourner les N derniers logs
    return std::vector<LogEntry>(logs.end() - count, logs.end());
}

void Logger::RegisterFormatter(const std::string& actionType, LogFormatter formatter) {
    formatters[actionType] = formatter;
}

void Logger::RegisterToolDisplayName(const std::string& toolType, const std::string& displayName) {
    toolDisplayNames[toolType] = displayName;
}

std::string Logger::GenerateDisplayText(const LogEntry& entry) {
    std::string timestamp = FormatTimestamp(entry.timestamp);
    std::string toolName = entry.toolType;

    // Utiliser le nom affiché si disponible
    auto it = toolDisplayNames.find(entry.toolType);
    if (it != toolDisplayNames.end()) {
        toolName = it->second;
    }

    // Utiliser le formatteur personnalisé si disponible
    auto formatterIt = formatters.find(entry.actionType);
    std::string content;

    if (formatterIt != formatters.end()) {
        content = formatterIt->second(entry);
    } else {
        // Formatteur par défaut
        auto defaultIt = formatters.find("default");
        if (defaultIt != formatters.end()) {
            content = defaultIt->second(entry);
        } else {
            content = entry.actionType + ": " + entry.data.dump();
        }
    }

    return "[" + timestamp + "] " + toolName + ": " + content;
}

std::string Logger::FormatTimestamp(double timestamp) {
    auto timeT = static_cast<time_t>(timestamp);
    auto tm = *std::localtime(&timeT);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%H:%M:%S");
    return oss.str();
}

} // namespace Core
