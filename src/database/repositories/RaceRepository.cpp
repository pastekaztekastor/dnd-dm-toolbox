#include "../../../include/database/repositories/RaceRepository.h"
#include <iostream>

namespace Database {

static std::string col(sqlite3_stmt* stmt, int i) {
    const char* val = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
    return val ? val : "";
}

RaceRepository::RaceRepository(sqlite3* db) : db(db) {}

std::vector<RaceData> RaceRepository::LoadAll() {
    if (!db) return {};

    const char* query =
        "SELECT id, COALESCE(race_parent_id, ''), nom, "
        "COALESCE(alias, ''), COALESCE(description, ''), COALESCE(aide_joueur, ''), "
        "COALESCE(bonus_forces, 0), COALESCE(bonus_dexterite, 0), "
        "COALESCE(bonus_constitution, 0), COALESCE(bonus_intelligence, 0), "
        "COALESCE(bonus_sagesse, 0), COALESCE(bonus_charisme, 0), "
        "COALESCE(vitesse_base, 30), COALESCE(liste_langues, ''), COALESCE(image_path, '') "
        "FROM races WHERE race_parent_id IS NULL ORDER BY nom";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Erreur LoadAll (races): " << sqlite3_errmsg(db) << std::endl;
        return {};
    }

    std::vector<RaceData> races;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        RaceData race;
        race.id                 = col(stmt, 0);
        race.race_parent_id     = col(stmt, 1);
        race.nom                = col(stmt, 2);
        race.alias              = col(stmt, 3);
        race.description        = col(stmt, 4);
        race.aide_joueur        = col(stmt, 5);
        race.bonus_forces       = sqlite3_column_int(stmt, 6);
        race.bonus_dexterite    = sqlite3_column_int(stmt, 7);
        race.bonus_constitution = sqlite3_column_int(stmt, 8);
        race.bonus_intelligence = sqlite3_column_int(stmt, 9);
        race.bonus_sagesse      = sqlite3_column_int(stmt, 10);
        race.bonus_charisme     = sqlite3_column_int(stmt, 11);
        race.vitesse_base       = sqlite3_column_int(stmt, 12);
        race.liste_langues      = col(stmt, 13);
        race.image_path         = col(stmt, 14);
        races.push_back(race);
    }
    sqlite3_finalize(stmt);

    for (auto& race : races) {
        LoadPresentations(race);
        LoadTraits(race);
        LoadNoms(race);
    }

    std::cout << races.size() << " races chargées depuis la DB" << std::endl;
    return races;
}

std::vector<RaceData> RaceRepository::LoadSubRaces(const std::string& parentRaceId) {
    if (!db) return {};

    const char* query =
        "SELECT id, COALESCE(race_parent_id, ''), nom, "
        "COALESCE(alias, ''), COALESCE(description, ''), COALESCE(aide_joueur, ''), "
        "COALESCE(bonus_forces, 0), COALESCE(bonus_dexterite, 0), "
        "COALESCE(bonus_constitution, 0), COALESCE(bonus_intelligence, 0), "
        "COALESCE(bonus_sagesse, 0), COALESCE(bonus_charisme, 0), "
        "COALESCE(vitesse_base, 30), COALESCE(liste_langues, ''), COALESCE(image_path, '') "
        "FROM races WHERE race_parent_id = ? ORDER BY nom";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Erreur LoadSubRaces: " << sqlite3_errmsg(db) << std::endl;
        return {};
    }
    sqlite3_bind_text(stmt, 1, parentRaceId.c_str(), -1, SQLITE_STATIC);

    std::vector<RaceData> races;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        RaceData race;
        race.id                 = col(stmt, 0);
        race.race_parent_id     = col(stmt, 1);
        race.nom                = col(stmt, 2);
        race.alias              = col(stmt, 3);
        race.description        = col(stmt, 4);
        race.aide_joueur        = col(stmt, 5);
        race.bonus_forces       = sqlite3_column_int(stmt, 6);
        race.bonus_dexterite    = sqlite3_column_int(stmt, 7);
        race.bonus_constitution = sqlite3_column_int(stmt, 8);
        race.bonus_intelligence = sqlite3_column_int(stmt, 9);
        race.bonus_sagesse      = sqlite3_column_int(stmt, 10);
        race.bonus_charisme     = sqlite3_column_int(stmt, 11);
        race.vitesse_base       = sqlite3_column_int(stmt, 12);
        race.liste_langues      = col(stmt, 13);
        race.image_path         = col(stmt, 14);
        races.push_back(race);
    }
    sqlite3_finalize(stmt);

    for (auto& race : races) {
        LoadPresentations(race);
        LoadTraits(race);
        LoadNoms(race);
    }

    return races;
}

void RaceRepository::LoadPresentations(RaceData& race) {
    const char* query = "SELECT titre, texte FROM race_presentations WHERE race_id = ? ORDER BY ordre";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) return;
    sqlite3_bind_text(stmt, 1, race.id.c_str(), -1, SQLITE_STATIC);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        RacePresentation p;
        p.titre = col(stmt, 0);
        p.texte = col(stmt, 1);
        race.presentations.push_back(p);
    }
    sqlite3_finalize(stmt);
}

void RaceRepository::LoadTraits(RaceData& race) {
    const char* query = "SELECT titre, texte FROM race_traits WHERE race_id = ? ORDER BY ordre";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) return;
    sqlite3_bind_text(stmt, 1, race.id.c_str(), -1, SQLITE_STATIC);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        RaceTrait t;
        t.titre = col(stmt, 0);
        t.texte = col(stmt, 1);
        race.race_traits.push_back(t);
    }
    sqlite3_finalize(stmt);
}

void RaceRepository::LoadNoms(RaceData& race) {
    const char* query = "SELECT titre, texte FROM race_noms WHERE race_id = ? ORDER BY ordre";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) return;
    sqlite3_bind_text(stmt, 1, race.id.c_str(), -1, SQLITE_STATIC);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        RaceNom n;
        n.titre = col(stmt, 0);
        n.texte = col(stmt, 1);
        race.noms.push_back(n);
    }
    sqlite3_finalize(stmt);
}

} // namespace Database
