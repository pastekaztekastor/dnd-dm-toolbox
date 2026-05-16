#include "../../include/database/DatabaseManager.h"
#include <iostream>
#include <sstream>

namespace Database {

DatabaseManager::DatabaseManager()
    : connection(nullptr)
    , connected(false)
{
}

DatabaseManager::~DatabaseManager() {
    Disconnect();
}

bool DatabaseManager::Connect(const std::string& host,
                              const std::string& port,
                              const std::string& dbname,
                              const std::string& user,
                              const std::string& password)
{
    std::stringstream ss;
    ss << "host=" << host
       << " port=" << port
       << " dbname=" << dbname
       << " user=" << user
       << " password=" << password;

    connectionString = ss.str();
    connection = PQconnectdb(connectionString.c_str());

    if (PQstatus(connection) != CONNECTION_OK) {
        std::cerr << "Erreur de connexion à la base de données: "
                  << PQerrorMessage(connection) << std::endl;
        PQfinish(connection);
        connection = nullptr;
        connected = false;
        return false;
    }

    connected = true;
    std::cout << "Connexion à PostgreSQL réussie!" << std::endl;

    raceRepo  = std::make_unique<RaceRepository>(connection);
    classRepo = std::make_unique<ClassRepository>(connection);

    return true;
}

void DatabaseManager::Disconnect() {
    raceRepo.reset();
    classRepo.reset();

    if (connection) {
        PQfinish(connection);
        connection = nullptr;
        connected = false;
    }
}

std::string DatabaseManager::GetLastError() const {
    if (connection)
        return PQerrorMessage(connection);
    return "Pas de connexion active";
}

} // namespace Database
