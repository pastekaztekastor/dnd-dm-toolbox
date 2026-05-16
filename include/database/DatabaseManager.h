#pragma once
#include <string>
#include <memory>
#include <libpq-fe.h>
#include "repositories/RaceRepository.h"
#include "repositories/ClassRepository.h"

namespace Database {

class DatabaseManager {
public:
    DatabaseManager();
    ~DatabaseManager();

    bool Connect(const std::string& host     = "localhost",
                 const std::string& port     = "5432",
                 const std::string& dbname   = "dnd_toolbox",
                 const std::string& user     = "dnd_user",
                 const std::string& password = "dnd_password");
    void Disconnect();
    bool IsConnected() const { return connected; }
    std::string GetLastError() const;

    RaceRepository*  Races()   { return raceRepo.get(); }
    ClassRepository* Classes() { return classRepo.get(); }

private:
    PGconn*     connection = nullptr;
    std::string connectionString;
    bool        connected  = false;

    std::unique_ptr<RaceRepository>  raceRepo;
    std::unique_ptr<ClassRepository> classRepo;
};

} // namespace Database
