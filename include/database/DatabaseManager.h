#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <libpq-fe.h>

namespace Database {

struct RacePresentation {
    std::string titre;
    std::string texte;
};

struct RaceTrait {
    std::string titre;
    std::string texte;
};

struct RaceNom {
    std::string titre;
    std::string texte;
};

// Structure pour représenter une race
struct RaceData {
    std::string id;
    std::string race_parent_id; // vide si race parente
    std::string nom;
    std::string alias;
    std::string description;
    std::string aide_joueur;
    int bonus_forces       = 0;
    int bonus_dexterite    = 0;
    int bonus_constitution = 0;
    int bonus_intelligence = 0;
    int bonus_sagesse      = 0;
    int bonus_charisme     = 0;
    int vitesse_base = 30;
    std::string liste_langues;
    std::string image_path;
    std::vector<RacePresentation> presentations;
    std::vector<RaceTrait>        race_traits;
    std::vector<RaceNom>          noms;
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
    std::vector<ClassFeature> LoadClassFeatures(const std::string& classId, int level = 1);

    // Helpers
    std::string GetLastError() const;

private:
    void LoadCache();
    std::vector<RaceData>    ParseRaces(PGresult* result);
    std::vector<ClassData>   ParseClasses(PGresult* result);
    std::vector<ClassFeature> ParseClassFeatures(PGresult* result);

    void LoadRacePresentations(RaceData& race);
    void LoadRaceTraitsData(RaceData& race);
    void LoadRaceNoms(RaceData& race);
};

} // namespace Database
