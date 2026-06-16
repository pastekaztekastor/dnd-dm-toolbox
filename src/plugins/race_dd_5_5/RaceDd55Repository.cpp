#include "RaceDd55Repository.h"
#include <functional>

namespace RaceDd55 {

RaceDd55Repository::RaceDd55Repository(sqlite3* db) : db(db) {}

static RaceData row_to_race(sqlite3_stmt* s) {
    RaceData r;
    auto text = [&](int col) -> std::string {
        const unsigned char* v = sqlite3_column_text(s, col);
        return v ? reinterpret_cast<const char*>(v) : "";
    };
    r.id               = text(0);
    r.race_parent_id   = text(1);
    r.nom              = text(2);
    r.alias            = text(3);
    r.description      = text(4);
    r.aide_joueur      = text(5);
    r.bonus_forces         = sqlite3_column_int(s, 6);
    r.bonus_dexterite      = sqlite3_column_int(s, 7);
    r.bonus_constitution   = sqlite3_column_int(s, 8);
    r.bonus_intelligence   = sqlite3_column_int(s, 9);
    r.bonus_sagesse        = sqlite3_column_int(s, 10);
    r.bonus_charisme       = sqlite3_column_int(s, 11);
    r.age_adulte       = sqlite3_column_int(s, 12);
    r.age_max          = sqlite3_column_int(s, 13);
    r.taille_min       = sqlite3_column_int(s, 14);
    r.taille_max       = sqlite3_column_int(s, 15);
    r.vitesse_base     = static_cast<float>(sqlite3_column_double(s, 16));
    r.liste_langues    = text(17);
    r.source           = text(18);
    r.source_ver       = text(19);
    r.image_path       = text(20);
    return r;
}

static const char* RACE_SELECT =
    "SELECT id, race_parent_id, nom, alias, description, aide_joueur,"
    " bonus_forces, bonus_dexterite, bonus_constitution,"
    " bonus_intelligence, bonus_sagesse, bonus_charisme,"
    " age_adulte, age_max, taille_min, taille_max, vitesse_base,"
    " liste_langues, source, source_ver, image_path"
    " FROM races";

std::vector<RaceData> RaceDd55Repository::LoadAllRaces() {
    std::vector<RaceData> out;
    std::string sql = std::string(RACE_SELECT)
        + " WHERE race_parent_id IS NULL OR race_parent_id = '' ORDER BY nom";
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &s, nullptr) != SQLITE_OK) return out;
    while (sqlite3_step(s) == SQLITE_ROW) out.push_back(row_to_race(s));
    sqlite3_finalize(s);
    return out;
}

std::vector<RaceData> RaceDd55Repository::LoadSubRaces(const std::string& parentId) {
    std::vector<RaceData> out;
    std::string sql = std::string(RACE_SELECT) + " WHERE race_parent_id = ? ORDER BY nom";
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &s, nullptr) != SQLITE_OK) return out;
    sqlite3_bind_text(s, 1, parentId.c_str(), -1, SQLITE_TRANSIENT);
    while (sqlite3_step(s) == SQLITE_ROW) out.push_back(row_to_race(s));
    sqlite3_finalize(s);
    return out;
}

RaceData RaceDd55Repository::LoadRaceById(const std::string& id) {
    RaceData r;
    std::string sql = std::string(RACE_SELECT) + " WHERE id = ?";
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &s, nullptr) != SQLITE_OK) return r;
    sqlite3_bind_text(s, 1, id.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlite3_step(s) == SQLITE_ROW) r = row_to_race(s);
    sqlite3_finalize(s);
    return r;
}

void RaceDd55Repository::LoadPresentations(RaceData& race) {
    race.presentations.clear();
    const char* sql =
        "SELECT id, race_id, titre, texte, ordre FROM race_presentations"
        " WHERE race_id = ? ORDER BY ordre";
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db, sql, -1, &s, nullptr) != SQLITE_OK) return;
    sqlite3_bind_text(s, 1, race.id.c_str(), -1, SQLITE_TRANSIENT);
    while (sqlite3_step(s) == SQLITE_ROW) {
        PresentationData p;
        auto text = [&](int col) -> std::string {
            const unsigned char* v = sqlite3_column_text(s, col);
            return v ? reinterpret_cast<const char*>(v) : "";
        };
        p.id      = text(0);
        p.race_id = text(1);
        p.titre   = text(2);
        p.texte   = text(3);
        p.ordre   = sqlite3_column_int(s, 4);
        race.presentations.push_back(p);
    }
    sqlite3_finalize(s);
}

void RaceDd55Repository::LoadTraits(RaceData& race) {
    race.traits.clear();
    const char* sql =
        "SELECT id, race_id, titre, texte, ordre FROM race_traits"
        " WHERE race_id = ? ORDER BY ordre";
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db, sql, -1, &s, nullptr) != SQLITE_OK) return;
    sqlite3_bind_text(s, 1, race.id.c_str(), -1, SQLITE_TRANSIENT);
    while (sqlite3_step(s) == SQLITE_ROW) {
        TraitData t;
        auto text = [&](int col) -> std::string {
            const unsigned char* v = sqlite3_column_text(s, col);
            return v ? reinterpret_cast<const char*>(v) : "";
        };
        t.id      = text(0);
        t.race_id = text(1);
        t.titre   = text(2);
        t.texte   = text(3);
        t.ordre   = sqlite3_column_int(s, 4);
        race.traits.push_back(t);
    }
    sqlite3_finalize(s);
}

void RaceDd55Repository::LoadNoms(RaceData& race) {
    race.noms.clear();
    const char* sql =
        "SELECT id, race_id, titre, texte, ordre FROM race_noms"
        " WHERE race_id = ? ORDER BY ordre";
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db, sql, -1, &s, nullptr) != SQLITE_OK) return;
    sqlite3_bind_text(s, 1, race.id.c_str(), -1, SQLITE_TRANSIENT);
    while (sqlite3_step(s) == SQLITE_ROW) {
        NomData n;
        auto text = [&](int col) -> std::string {
            const unsigned char* v = sqlite3_column_text(s, col);
            return v ? reinterpret_cast<const char*>(v) : "";
        };
        n.id      = text(0);
        n.race_id = text(1);
        n.titre   = text(2);
        n.texte   = text(3);
        n.ordre   = sqlite3_column_int(s, 4);
        race.noms.push_back(n);
    }
    sqlite3_finalize(s);
}

std::string RaceDd55Repository::GenerateId() {
    std::string id;
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db, "SELECT lower(hex(randomblob(4)))", -1, &s, nullptr) == SQLITE_OK) {
        if (sqlite3_step(s) == SQLITE_ROW) {
            const unsigned char* v = sqlite3_column_text(s, 0);
            if (v) id = reinterpret_cast<const char*>(v);
        }
        sqlite3_finalize(s);
    }
    return id;
}

bool RaceDd55Repository::InsertRace(const RaceData& d) {
    const char* sql =
        "INSERT INTO races (id, race_parent_id, nom, alias, description, aide_joueur,"
        " bonus_forces, bonus_dexterite, bonus_constitution,"
        " bonus_intelligence, bonus_sagesse, bonus_charisme,"
        " age_adulte, age_max, taille_min, taille_max, vitesse_base, liste_langues, source, source_ver)"
        " VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db, sql, -1, &s, nullptr) != SQLITE_OK) return false;
    auto bindStr = [&](int i, const std::string& v) {
        v.empty() ? sqlite3_bind_null(s, i)
                  : sqlite3_bind_text(s, i, v.c_str(), -1, SQLITE_TRANSIENT);
    };
    bindStr(1, d.id);
    bindStr(2, d.race_parent_id);
    bindStr(3, d.nom);
    bindStr(4, d.alias);
    bindStr(5, d.description);
    bindStr(6, d.aide_joueur);
    sqlite3_bind_int(s, 7,  d.bonus_forces);
    sqlite3_bind_int(s, 8,  d.bonus_dexterite);
    sqlite3_bind_int(s, 9,  d.bonus_constitution);
    sqlite3_bind_int(s, 10, d.bonus_intelligence);
    sqlite3_bind_int(s, 11, d.bonus_sagesse);
    sqlite3_bind_int(s, 12, d.bonus_charisme);
    sqlite3_bind_int(s, 13, d.age_adulte);
    sqlite3_bind_int(s, 14, d.age_max);
    sqlite3_bind_int(s, 15, d.taille_min);
    sqlite3_bind_int(s, 16, d.taille_max);
    sqlite3_bind_double(s, 17, d.vitesse_base);
    bindStr(18, d.liste_langues);
    bindStr(19, d.source.empty() ? std::string("Player's Handbook") : d.source);
    bindStr(20, d.source_ver.empty() ? std::string("5E") : d.source_ver);
    int rc = sqlite3_step(s);
    sqlite3_finalize(s);
    return rc == SQLITE_DONE;
}

bool RaceDd55Repository::UpdateRace(const RaceData& d) {
    const char* sql =
        "UPDATE races SET race_parent_id=?, nom=?, description=?, aide_joueur=?,"
        " bonus_forces=?, bonus_dexterite=?, bonus_constitution=?,"
        " bonus_intelligence=?, bonus_sagesse=?, bonus_charisme=?,"
        " age_adulte=?, age_max=?, taille_min=?, taille_max=?, vitesse_base=?, liste_langues=?"
        " WHERE id=?";
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db, sql, -1, &s, nullptr) != SQLITE_OK) return false;
    auto bindStr = [&](int i, const std::string& v) {
        v.empty() ? sqlite3_bind_null(s, i)
                  : sqlite3_bind_text(s, i, v.c_str(), -1, SQLITE_TRANSIENT);
    };
    bindStr(1, d.race_parent_id);
    bindStr(2, d.nom);
    bindStr(3, d.description);
    bindStr(4, d.aide_joueur);
    sqlite3_bind_int(s, 5,  d.bonus_forces);
    sqlite3_bind_int(s, 6,  d.bonus_dexterite);
    sqlite3_bind_int(s, 7,  d.bonus_constitution);
    sqlite3_bind_int(s, 8,  d.bonus_intelligence);
    sqlite3_bind_int(s, 9,  d.bonus_sagesse);
    sqlite3_bind_int(s, 10, d.bonus_charisme);
    sqlite3_bind_int(s, 11, d.age_adulte);
    sqlite3_bind_int(s, 12, d.age_max);
    sqlite3_bind_int(s, 13, d.taille_min);
    sqlite3_bind_int(s, 14, d.taille_max);
    sqlite3_bind_double(s, 15, d.vitesse_base);
    bindStr(16, d.liste_langues);
    bindStr(17, d.id);
    int rc = sqlite3_step(s);
    sqlite3_finalize(s);
    return rc == SQLITE_DONE;
}

bool RaceDd55Repository::ExistsId(const std::string& id) {
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db, "SELECT 1 FROM races WHERE id=?", -1, &s, nullptr) != SQLITE_OK) return false;
    sqlite3_bind_text(s, 1, id.c_str(), -1, SQLITE_TRANSIENT);
    bool found = sqlite3_step(s) == SQLITE_ROW;
    sqlite3_finalize(s);
    return found;
}

bool RaceDd55Repository::ExistsNom(const std::string& nom, const std::string& excludeId) {
    sqlite3_stmt* s;
    const char* sql = excludeId.empty()
        ? "SELECT 1 FROM races WHERE nom=?"
        : "SELECT 1 FROM races WHERE nom=? AND id!=?";
    if (sqlite3_prepare_v2(db, sql, -1, &s, nullptr) != SQLITE_OK) return false;
    sqlite3_bind_text(s, 1, nom.c_str(), -1, SQLITE_TRANSIENT);
    if (!excludeId.empty())
        sqlite3_bind_text(s, 2, excludeId.c_str(), -1, SQLITE_TRANSIENT);
    bool found = sqlite3_step(s) == SQLITE_ROW;
    sqlite3_finalize(s);
    return found;
}

void RaceDd55Repository::BeginTransaction() {
    sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, nullptr);
}

void RaceDd55Repository::Commit() {
    sqlite3_exec(db, "COMMIT", nullptr, nullptr, nullptr);
}

void RaceDd55Repository::Rollback() {
    sqlite3_exec(db, "ROLLBACK", nullptr, nullptr, nullptr);
}

void RaceDd55Repository::DeleteRaceById(const std::string& id) {
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db, "DELETE FROM races WHERE id=?", -1, &s, nullptr) != SQLITE_OK) return;
    sqlite3_bind_text(s, 1, id.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_step(s);
    sqlite3_finalize(s);
}

static void save_secondary(sqlite3* db, const char* table,
    const std::string& raceId, const std::vector<std::tuple<std::string,std::string,std::string>>& items,
    const std::function<std::string()>& genId)
{
    std::string del = std::string("DELETE FROM ") + table + " WHERE race_id=?";
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db, del.c_str(), -1, &s, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(s, 1, raceId.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_step(s);
        sqlite3_finalize(s);
    }
    std::string ins = std::string("INSERT INTO ") + table + " (id,race_id,ordre,titre,texte) VALUES (?,?,?,?,?)";
    if (sqlite3_prepare_v2(db, ins.c_str(), -1, &s, nullptr) != SQLITE_OK) return;
    for (int i = 0; i < (int)items.size(); ++i) {
        const auto& [id, titre, texte] = items[i];
        std::string rowId = id.empty() ? genId() : id;
        sqlite3_bind_text(s, 1, rowId.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(s, 2, raceId.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int (s, 3, i + 1);
        sqlite3_bind_text(s, 4, titre.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(s, 5, texte.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_step(s);
        sqlite3_reset(s);
    }
    sqlite3_finalize(s);
}

void RaceDd55Repository::SavePresentations(const std::string& raceId, const std::vector<PresentationData>& items) {
    std::vector<std::tuple<std::string,std::string,std::string>> rows;
    for (auto& p : items) rows.emplace_back(p.id, p.titre, p.texte);
    save_secondary(db, "race_presentations", raceId, rows, [this]{ return GenerateId(); });
}

void RaceDd55Repository::SaveTraits(const std::string& raceId, const std::vector<TraitData>& items) {
    std::vector<std::tuple<std::string,std::string,std::string>> rows;
    for (auto& t : items) rows.emplace_back(t.id, t.titre, t.texte);
    save_secondary(db, "race_traits", raceId, rows, [this]{ return GenerateId(); });
}

void RaceDd55Repository::SaveNoms(const std::string& raceId, const std::vector<NomData>& items) {
    std::vector<std::tuple<std::string,std::string,std::string>> rows;
    for (auto& n : items) rows.emplace_back(n.id, n.titre, n.texte);
    save_secondary(db, "race_noms", raceId, rows, [this]{ return GenerateId(); });
}

} // namespace RaceDd55
