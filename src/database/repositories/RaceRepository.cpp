#include "../../../include/database/repositories/RaceRepository.h"
#include <iostream>

namespace Database {

RaceRepository::RaceRepository(PGconn* conn)
    : conn(conn)
{
}

std::vector<RaceData> RaceRepository::LoadAll() {
    if (!conn) return {};

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

    PGresult* result = PQexec(conn, query);
    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        std::cerr << "Erreur LoadAll (races): " << PQerrorMessage(conn) << std::endl;
        PQclear(result);
        return {};
    }

    auto races = Parse(result);
    PQclear(result);

    for (auto& race : races) {
        LoadPresentations(race);
        LoadTraits(race);
        LoadNoms(race);
    }

    std::cout << races.size() << " races chargées depuis la DB" << std::endl;
    return races;
}

std::vector<RaceData> RaceRepository::LoadSubRaces(const std::string& parentRaceId) {
    if (!conn) return {};

    std::string query =
        "SELECT id, COALESCE(race_parent_id::text, ''), nom, "
        "COALESCE(alias, ''), COALESCE(description, ''), COALESCE(aide_joueur, ''), "
        "COALESCE(bonus_forces, 0), COALESCE(bonus_dexterite, 0), "
        "COALESCE(bonus_constitution, 0), COALESCE(bonus_intelligence, 0), "
        "COALESCE(bonus_sagesse, 0), COALESCE(bonus_charisme, 0), "
        "COALESCE(vitesse_base, 30), COALESCE(liste_langues, ''), COALESCE(image_path, '') "
        "FROM races WHERE race_parent_id = '" + parentRaceId + "' ORDER BY nom";

    PGresult* result = PQexec(conn, query.c_str());
    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        std::cerr << "Erreur LoadSubRaces: " << PQerrorMessage(conn) << std::endl;
        PQclear(result);
        return {};
    }

    auto races = Parse(result);
    PQclear(result);

    for (auto& race : races) {
        LoadPresentations(race);
        LoadTraits(race);
        LoadNoms(race);
    }

    return races;
}

void RaceRepository::LoadPresentations(RaceData& race) {
    std::string query = "SELECT titre, texte FROM race_presentations "
                        "WHERE race_id = '" + race.id + "' ORDER BY ordre";
    PGresult* result = PQexec(conn, query.c_str());
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

void RaceRepository::LoadTraits(RaceData& race) {
    std::string query = "SELECT titre, texte FROM race_traits "
                        "WHERE race_id = '" + race.id + "' ORDER BY ordre";
    PGresult* result = PQexec(conn, query.c_str());
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

void RaceRepository::LoadNoms(RaceData& race) {
    std::string query = "SELECT titre, texte FROM race_noms "
                        "WHERE race_id = '" + race.id + "' ORDER BY ordre";
    PGresult* result = PQexec(conn, query.c_str());
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

std::vector<RaceData> RaceRepository::Parse(PGresult* result) {
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
        race.vitesse_base       = atoi(PQgetvalue(result, i, 12));
        race.liste_langues      = PQgetvalue(result, i, 13);
        race.image_path         = PQgetvalue(result, i, 14);
        races.push_back(race);
    }
    return races;
}

} // namespace Database
