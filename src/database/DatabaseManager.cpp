#include "../../include/database/DatabaseManager.h"
#include <iostream>

namespace Database {

DatabaseManager::DatabaseManager()
    : db(nullptr)
    , connected(false)
{}

DatabaseManager::~DatabaseManager() {
    Disconnect();
}

bool DatabaseManager::Connect(const std::string& dbPath) {
    int rc = sqlite3_open(dbPath.c_str(), &db);
    if (rc != SQLITE_OK) {
        lastError = sqlite3_errmsg(db);
        std::cerr << "Erreur d'ouverture SQLite: " << lastError << std::endl;
        sqlite3_close(db);
        db = nullptr;
        connected = false;
        return false;
    }

    connected = true;
    std::cout << "Base de données SQLite ouverte: " << dbPath << std::endl;

    raceRepo  = std::make_unique<RaceRepository>(db);
    classRepo = std::make_unique<ClassRepository>(db);

    return true;
}

void DatabaseManager::Disconnect() {
    raceRepo.reset();
    classRepo.reset();

    if (db) {
        sqlite3_close(db);
        db = nullptr;
        connected = false;
    }
}

std::string DatabaseManager::GetLastError() const {
    if (db)
        return sqlite3_errmsg(db);
    return lastError;
}

} // namespace Database
