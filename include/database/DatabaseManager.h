#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <libpq-fe.h>

namespace Database {

// Structure pour représenter une race
struct RaceData {
    std::string id;
    std::string name;
    std::string name_fr;
    std::string description;
    int str_bonus = 0;
    int dex_bonus = 0;
    int con_bonus = 0;
    int int_bonus = 0;
    int wis_bonus = 0;
    int cha_bonus = 0;
    int base_speed = 30;
    std::string size;
    std::vector<std::string> languages;
    std::vector<std::string> traits; // Noms des traits raciaux
};

// Structure pour représenter un trait racial
struct RacialTrait {
    std::string name;
    std::string name_fr;
    std::string description;
    std::string trait_type; // passive, active
};

// Structure pour représenter une classe
struct ClassData {
    std::string id;
    std::string name;
    std::string name_fr;
    std::string description;
    int hit_die = 8;
    std::string primary_ability;
    std::vector<std::string> saving_throw_proficiencies;
    int skill_choices = 2;
    std::vector<std::string> available_skills;
    std::vector<std::string> level1_features; // Capacités de niveau 1
};

// Structure pour représenter une capacité de classe
struct ClassFeature {
    std::string name;
    std::string name_fr;
    std::string description;
    int level_gained = 1;
};

class DatabaseManager {
private:
    PGconn* connection;
    std::string connectionString;
    bool connected;

    // Cache
    std::vector<RaceData> racesCache;
    std::vector<ClassData> classesCache;
    std::map<std::string, std::vector<RacialTrait>> racialTraitsCache;
    std::map<std::string, std::vector<ClassFeature>> classFeaturesCache;
    bool cacheLoaded;

public:
    DatabaseManager();
    ~DatabaseManager();

    // Connexion
    bool Connect(const std::string& host = "localhost",
                 const std::string& port = "5432",
                 const std::string& dbname = "dnd_toolbox",
                 const std::string& user = "dnd_user",
                 const std::string& password = "dnd_password");
    void Disconnect();
    bool IsConnected() const { return connected; }

    // Chargement des données
    std::vector<RaceData> LoadRaces();
    std::vector<ClassData> LoadClasses();
    std::vector<RacialTrait> LoadRacialTraits(const std::string& raceId);
    std::vector<ClassFeature> LoadClassFeatures(const std::string& classId, int level = 1);

    // Helpers
    std::string GetLastError() const;

private:
    void LoadCache();
    std::vector<RaceData> ParseRaces(PGresult* result);
    std::vector<ClassData> ParseClasses(PGresult* result);
    std::vector<RacialTrait> ParseRacialTraits(PGresult* result);
    std::vector<ClassFeature> ParseClassFeatures(PGresult* result);
};

} // namespace Database
