#pragma once
#include <string>
#include <memory>
#include <sqlite3.h>
#include "repositories/RaceRepository.h"
#include "repositories/ClassRepository.h"

namespace Database {

class DatabaseManager {
public:
    DatabaseManager();
    ~DatabaseManager();

    bool Connect(const std::string& dbPath = "dnd_toolbox.db");
    void Disconnect();
    bool IsConnected() const { return connected; }
    std::string GetLastError() const;

    RaceRepository*  Races()   { return raceRepo.get(); }
    ClassRepository* Classes() { return classRepo.get(); }

private:
    sqlite3*    db        = nullptr;
    bool        connected = false;
    std::string lastError;

    std::unique_ptr<RaceRepository>  raceRepo;
    std::unique_ptr<ClassRepository> classRepo;
};

} // namespace Database
