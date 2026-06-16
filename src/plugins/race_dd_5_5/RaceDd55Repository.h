#pragma once

#include <string>
#include <vector>
#include <sqlite3.h>

namespace RaceDd55 {

struct PresentationData {
    std::string id, race_id, titre, texte;
    int ordre = 0;
};

struct TraitData {
    std::string id, race_id, titre, texte;
    int ordre = 0;
};

struct NomData {
    std::string id, race_id, titre, texte;
    int ordre = 0;
};

struct RaceData {
    std::string id, race_parent_id, nom, alias;
    std::string description, aide_joueur;
    int bonus_forces = 0, bonus_dexterite = 0, bonus_constitution = 0;
    int bonus_intelligence = 0, bonus_sagesse = 0, bonus_charisme = 0;
    int age_adulte = 0, age_max = 0;
    int taille_min = 0, taille_max = 0;
    float vitesse_base = 0.f;
    std::string liste_langues, source, source_ver;
    std::string image_path;
    std::vector<PresentationData> presentations;
    std::vector<TraitData>        traits;
    std::vector<NomData>          noms;
};

class RaceDd55Repository {
public:
    explicit RaceDd55Repository(sqlite3* db);

    std::vector<RaceData> LoadAllRaces();
    std::vector<RaceData> LoadSubRaces(const std::string& parentId);
    RaceData              LoadRaceById(const std::string& id);
    void LoadPresentations(RaceData& race);
    void LoadTraits(RaceData& race);
    void LoadNoms(RaceData& race);

    std::string GenerateId();
    bool InsertRace(const RaceData& data);
    bool UpdateRace(const RaceData& data);
    void DeleteRaceById(const std::string& id);
    void SavePresentations(const std::string& raceId, const std::vector<PresentationData>& items);
    void SaveTraits(const std::string& raceId, const std::vector<TraitData>& items);
    void SaveNoms(const std::string& raceId, const std::vector<NomData>& items);

    bool ExistsId(const std::string& id);
    bool ExistsNom(const std::string& nom, const std::string& excludeId = "");
    void BeginTransaction();
    void Commit();
    void Rollback();

private:
    sqlite3* db;
};

} // namespace RaceDd55
