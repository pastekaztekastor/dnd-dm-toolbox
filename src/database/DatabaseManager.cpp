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

    // Charger les features de niveau 1 pour chaque classe
    for (const auto& cls : classesCache) {
        classFeaturesCache[cls.id] = LoadClassFeatures(cls.id, 1);
    }

    cacheLoaded = true;
}

std::vector<RaceData> DatabaseManager::LoadRaces() {
    if (!connected) return {};

    const char* query = R"(
        SELECT id, COALESCE(race_parent_id::text, ''), nom,
               COALESCE(alias, ''), COALESCE(description, ''), COALESCE(aide_joueur, ''),
               COALESCE(bonus_forces, 0), COALESCE(bonus_dexterite, 0),
               COALESCE(bonus_constitution, 0), COALESCE(bonus_intelligence, 0),
               COALESCE(bonus_sagesse, 0), COALESCE(bonus_charisme, 0),
               COALESCE(vitesse_base, 30), COALESCE(liste_langues, ''), COALESCE(image_path, '')
        FROM races
        WHERE race_parent_id IS NULL
        ORDER BY nom
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

    for (auto& race : races) {
        LoadRacePresentations(race);
        LoadRaceTraitsData(race);
        LoadRaceNoms(race);
    }

    std::cout << races.size() << " races chargées depuis la DB" << std::endl;

    return races;
}

std::vector<RaceData> DatabaseManager::LoadSubRaces(const std::string& parentRaceId) {
    if (!connected) return {};

    std::string query = "SELECT id, COALESCE(race_parent_id::text, ''), nom, "
                       "COALESCE(alias, ''), COALESCE(description, ''), COALESCE(aide_joueur, ''), "
                       "COALESCE(bonus_forces, 0), COALESCE(bonus_dexterite, 0), "
                       "COALESCE(bonus_constitution, 0), COALESCE(bonus_intelligence, 0), "
                       "COALESCE(bonus_sagesse, 0), COALESCE(bonus_charisme, 0), "
                       "COALESCE(vitesse_base, 30), COALESCE(liste_langues, ''), COALESCE(image_path, '') "
                       "FROM races WHERE race_parent_id = '" + parentRaceId + "' ORDER BY nom";

    PGresult* result = PQexec(connection, query.c_str());

    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        std::cerr << "Erreur lors du chargement des sous-races: "
                  << PQerrorMessage(connection) << std::endl;
        PQclear(result);
        return {};
    }

    auto races = ParseRaces(result);
    PQclear(result);

    for (auto& race : races) {
        LoadRacePresentations(race);
        LoadRaceTraitsData(race);
        LoadRaceNoms(race);
    }

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

void DatabaseManager::LoadRacePresentations(RaceData& race) {
    std::string query = "SELECT titre, texte FROM race_presentations "
                        "WHERE race_id = '" + race.id + "' ORDER BY ordre";

    PGresult* result = PQexec(connection, query.c_str());
    if (PQresultStatus(result) == PGRES_TUPLES_OK) {
        for (int i = 0; i < PQntuples(result); ++i) {
            RacePresentation p;
            p.titre = PQgetvalue(result, i, 0);
            p.texte = PQgetvalue(result, i, 1);
            race.presentations.push_back(p);
        }
    }
    PQclear(result);
}

void DatabaseManager::LoadRaceTraitsData(RaceData& race) {
    std::string query = "SELECT titre, texte FROM race_traits "
                        "WHERE race_id = '" + race.id + "' ORDER BY ordre";

    PGresult* result = PQexec(connection, query.c_str());
    if (PQresultStatus(result) == PGRES_TUPLES_OK) {
        for (int i = 0; i < PQntuples(result); ++i) {
            RaceTrait t;
            t.titre = PQgetvalue(result, i, 0);
            t.texte = PQgetvalue(result, i, 1);
            race.race_traits.push_back(t);
        }
    }
    PQclear(result);
}

void DatabaseManager::LoadRaceNoms(RaceData& race) {
    std::string query = "SELECT titre, texte FROM race_noms "
                        "WHERE race_id = '" + race.id + "' ORDER BY ordre";

    PGresult* result = PQexec(connection, query.c_str());
    if (PQresultStatus(result) == PGRES_TUPLES_OK) {
        for (int i = 0; i < PQntuples(result); ++i) {
            RaceNom n;
            n.titre = PQgetvalue(result, i, 0);
            n.texte = PQgetvalue(result, i, 1);
            race.noms.push_back(n);
        }
    }
    PQclear(result);
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
        race.id             = PQgetvalue(result, i, 0);
        race.race_parent_id = PQgetvalue(result, i, 1);
        race.nom            = PQgetvalue(result, i, 2);
        race.alias          = PQgetvalue(result, i, 3);
        race.description    = PQgetvalue(result, i, 4);
        race.aide_joueur    = PQgetvalue(result, i, 5);
        race.bonus_forces       = atoi(PQgetvalue(result, i, 6));
        race.bonus_dexterite    = atoi(PQgetvalue(result, i, 7));
        race.bonus_constitution = atoi(PQgetvalue(result, i, 8));
        race.bonus_intelligence = atoi(PQgetvalue(result, i, 9));
        race.bonus_sagesse      = atoi(PQgetvalue(result, i, 10));
        race.bonus_charisme     = atoi(PQgetvalue(result, i, 11));
        race.vitesse_base  = atoi(PQgetvalue(result, i, 12));
        race.liste_langues = PQgetvalue(result, i, 13);
        race.image_path    = PQgetvalue(result, i, 14);

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
