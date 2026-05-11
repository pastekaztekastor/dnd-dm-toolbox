#include "../../include/database/DatabaseManager.h"
#include <iostream>
#include <sstream>

namespace Database {

DatabaseManager::DatabaseManager()
    : connection(nullptr)
    , connected(false)
    , cacheLoaded(false)
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

    // Charger le cache
    LoadCache();

    return true;
}

void DatabaseManager::Disconnect() {
    if (connection) {
        PQfinish(connection);
        connection = nullptr;
        connected = false;
    }
}

std::string DatabaseManager::GetLastError() const {
    if (connection) {
        return PQerrorMessage(connection);
    }
    return "Pas de connexion active";
}

void DatabaseManager::LoadCache() {
    if (!connected || cacheLoaded) return;

    racesCache = LoadRaces();
    classesCache = LoadClasses();

    // Charger les traits pour chaque race
    for (const auto& race : racesCache) {
        racialTraitsCache[race.id] = LoadRacialTraits(race.id);
    }

    // Charger les features de niveau 1 pour chaque classe
    for (const auto& cls : classesCache) {
        classFeaturesCache[cls.id] = LoadClassFeatures(cls.id, 1);
    }

    cacheLoaded = true;
}

std::vector<RaceData> DatabaseManager::LoadRaces() {
    if (!connected) return {};

    const char* query = R"(
        SELECT id, name, name_fr, description,
               COALESCE(str_bonus, 0), COALESCE(dex_bonus, 0),
               COALESCE(con_bonus, 0), COALESCE(int_bonus, 0),
               COALESCE(wis_bonus, 0), COALESCE(cha_bonus, 0),
               base_speed, size, languages
        FROM races
        ORDER BY name_fr
    )";

    PGresult* result = PQexec(connection, query);

    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        std::cerr << "Erreur lors du chargement des races: "
                  << PQerrorMessage(connection) << std::endl;
        PQclear(result);
        return {};
    }

    auto races = ParseRaces(result);
    PQclear(result);

    std::cout << "" << races.size() << " races chargées depuis la DB" << std::endl;

    return races;
}

std::vector<ClassData> DatabaseManager::LoadClasses() {
    if (!connected) return {};

    const char* query = R"(
        SELECT id, name, name_fr, description, hit_die, primary_ability, skill_choices_count
        FROM classes
        ORDER BY name_fr
    )";

    PGresult* result = PQexec(connection, query);

    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        std::cerr << "Erreur lors du chargement des classes: "
                  << PQerrorMessage(connection) << std::endl;
        PQclear(result);
        return {};
    }

    auto classes = ParseClasses(result);
    PQclear(result);

    // Charger les skills disponibles pour chaque classe
    for (auto& cls : classes) {
        std::stringstream querySkills;
        querySkills << "SELECT skill_name FROM class_skill_choices "
                   << "WHERE class_id = '" << cls.id << "'";

        PGresult* skillsResult = PQexec(connection, querySkills.str().c_str());

        if (PQresultStatus(skillsResult) == PGRES_TUPLES_OK) {
            // Récupérer toutes les skills disponibles
            for (int i = 0; i < PQntuples(skillsResult); ++i) {
                cls.available_skills.push_back(PQgetvalue(skillsResult, i, 0));
            }
        }

        PQclear(skillsResult);
    }

    std::cout << "" << classes.size() << " classes chargées depuis la DB" << std::endl;

    return classes;
}

std::vector<RacialTrait> DatabaseManager::LoadRacialTraits(const std::string& raceId) {
    if (!connected) return {};

    std::stringstream query;
    query << "SELECT name, name_fr, description, trait_type "
          << "FROM racial_traits "
          << "WHERE race_id = '" << raceId << "' "
          << "ORDER BY name_fr";

    PGresult* result = PQexec(connection, query.str().c_str());

    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        std::cerr << "Erreur lors du chargement des traits raciaux: "
                  << PQerrorMessage(connection) << std::endl;
        PQclear(result);
        return {};
    }

    auto traits = ParseRacialTraits(result);
    PQclear(result);

    return traits;
}

std::vector<ClassFeature> DatabaseManager::LoadClassFeatures(const std::string& classId, int level) {
    if (!connected) return {};

    // La table class_features_by_level contient features_gained (array de noms)
    // On charge juste les noms pour l'instant, sans détails
    std::stringstream query;
    query << "SELECT features_gained FROM class_features_by_level "
          << "WHERE class_id = '" << classId << "' AND level = " << level;

    PGresult* result = PQexec(connection, query.str().c_str());

    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        std::cerr << "Erreur lors du chargement des capacités de classe: "
                  << PQerrorMessage(connection) << std::endl;
        PQclear(result);
        return {};
    }

    std::vector<ClassFeature> features;

    if (PQntuples(result) > 0 && !PQgetisnull(result, 0, 0)) {
        // Parser l'array PostgreSQL {feature1,feature2,...}
        std::string featuresStr = PQgetvalue(result, 0, 0);
        if (!featuresStr.empty() && featuresStr[0] == '{') {
            featuresStr = featuresStr.substr(1, featuresStr.length() - 2); // Remove {}
            std::stringstream ss(featuresStr);
            std::string featureName;
            while (std::getline(ss, featureName, ',')) {
                // Trim quotes si présentes
                if (!featureName.empty() && featureName[0] == '"') {
                    featureName = featureName.substr(1, featureName.length() - 2);
                }

                ClassFeature feature;
                feature.name = featureName;
                feature.name_fr = featureName;
                feature.description = ""; // Pas de description pour l'instant
                feature.level_gained = level;
                features.push_back(feature);
            }
        }
    }

    PQclear(result);
    return features;
}

// Parsers
std::vector<RaceData> DatabaseManager::ParseRaces(PGresult* result) {
    std::vector<RaceData> races;

    int rows = PQntuples(result);
    for (int i = 0; i < rows; ++i) {
        RaceData race;
        race.id = PQgetvalue(result, i, 0);
        race.name = PQgetvalue(result, i, 1);
        race.name_fr = PQgetvalue(result, i, 2);
        race.description = PQgetvalue(result, i, 3);
        race.str_bonus = atoi(PQgetvalue(result, i, 4));
        race.dex_bonus = atoi(PQgetvalue(result, i, 5));
        race.con_bonus = atoi(PQgetvalue(result, i, 6));
        race.int_bonus = atoi(PQgetvalue(result, i, 7));
        race.wis_bonus = atoi(PQgetvalue(result, i, 8));
        race.cha_bonus = atoi(PQgetvalue(result, i, 9));
        race.base_speed = atoi(PQgetvalue(result, i, 10));
        race.size = PQgetvalue(result, i, 11);

        // Parser les langues (PostgreSQL array)
        // Format: {Common,Elvish}
        std::string langsStr = PQgetvalue(result, i, 12);
        if (!langsStr.empty() && langsStr[0] == '{') {
            langsStr = langsStr.substr(1, langsStr.length() - 2); // Remove {}
            std::stringstream ss(langsStr);
            std::string lang;
            while (std::getline(ss, lang, ',')) {
                race.languages.push_back(lang);
            }
        }

        races.push_back(race);
    }

    return races;
}

std::vector<ClassData> DatabaseManager::ParseClasses(PGresult* result) {
    std::vector<ClassData> classes;

    int rows = PQntuples(result);
    for (int i = 0; i < rows; ++i) {
        ClassData cls;
        cls.id = PQgetvalue(result, i, 0);
        cls.name = PQgetvalue(result, i, 1);
        cls.name_fr = PQgetvalue(result, i, 2);
        cls.description = PQgetvalue(result, i, 3);
        cls.hit_die = atoi(PQgetvalue(result, i, 4));
        cls.primary_ability = PQgetvalue(result, i, 5);
        cls.skill_choices = atoi(PQgetvalue(result, i, 6));

        classes.push_back(cls);
    }

    return classes;
}

std::vector<RacialTrait> DatabaseManager::ParseRacialTraits(PGresult* result) {
    std::vector<RacialTrait> traits;

    int rows = PQntuples(result);
    for (int i = 0; i < rows; ++i) {
        RacialTrait trait;
        trait.name = PQgetvalue(result, i, 0);
        trait.name_fr = PQgetvalue(result, i, 1);
        trait.description = PQgetvalue(result, i, 2);
        trait.trait_type = PQgetvalue(result, i, 3);

        traits.push_back(trait);
    }

    return traits;
}

std::vector<ClassFeature> DatabaseManager::ParseClassFeatures(PGresult* result) {
    std::vector<ClassFeature> features;

    int rows = PQntuples(result);
    for (int i = 0; i < rows; ++i) {
        ClassFeature feature;
        feature.name = PQgetvalue(result, i, 0);
        feature.name_fr = PQgetvalue(result, i, 1);
        feature.description = PQgetvalue(result, i, 2);
        feature.level_gained = atoi(PQgetvalue(result, i, 3));

        features.push_back(feature);
    }

    return features;
}

} // namespace Database
