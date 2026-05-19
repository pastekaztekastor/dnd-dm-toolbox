#pragma once
#include <string>
#include <vector>
#include <sqlite3.h>

namespace Database {

struct RacePresentation { std::string titre, texte; };
struct RaceTrait        { std::string titre, texte; };
struct RaceNom          { std::string titre, texte; };

struct RaceData {
    std::string id, race_parent_id, nom, alias, description, aide_joueur;
    int bonus_forces       = 0;
    int bonus_dexterite    = 0;
    int bonus_constitution = 0;
    int bonus_intelligence = 0;
    int bonus_sagesse      = 0;
    int bonus_charisme     = 0;
    int vitesse_base = 30;
    std::string liste_langues, image_path;
    std::vector<RacePresentation> presentations;
    std::vector<RaceTrait>        race_traits;
    std::vector<RaceNom>          noms;
};

class RaceRepository {
public:
    explicit RaceRepository(sqlite3* db);

    std::vector<RaceData> LoadAll();
    std::vector<RaceData> LoadSubRaces(const std::string& parentRaceId);

private:
    sqlite3* db;

    void LoadPresentations(RaceData& race);
    void LoadTraits(RaceData& race);
    void LoadNoms(RaceData& race);
};

} // namespace Database
