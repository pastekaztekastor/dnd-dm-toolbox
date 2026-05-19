#include "../../../include/database/repositories/ClassRepository.h"
#include <iostream>
#include <sstream>

namespace Database {

// Parse un TEXT[] PostgreSQL de la forme {val1,val2,...} en vector<string>
static std::vector<std::string> ParsePgTextArray(const std::string& raw) {
    std::vector<std::string> out;
    if (raw.size() < 2 || raw.front() != '{' || raw.back() != '}') return out;
    std::string inner = raw.substr(1, raw.size() - 2);
    std::stringstream ss(inner);
    std::string token;
    while (std::getline(ss, token, ',')) {
        if (!token.empty() && token.front() == '"')
            token = token.substr(1, token.size() - 2);
        out.push_back(token);
    }
    return out;
}

ClassRepository::ClassRepository(PGconn* conn)
    : conn(conn)
{
}

std::vector<ClassData> ClassRepository::LoadAll() {
    if (!conn) return {};

    const char* query = R"(
        SELECT id, COALESCE(classe_parente_id::text, ''), nom,
               COALESCE(alias, ''), COALESCE(description, ''), COALESCE(aide_joueur, ''),
               COALESCE(dee_de_vie, 8),
               COALESCE(jets_de_sauvegarde::text, '{}'),
               COALESCE(caracteristiques_de_sorts, ''),
               COALESCE(image_path, '')
        FROM classes
        WHERE classe_parente_id IS NULL
        ORDER BY nom
    )";

    PGresult* result = PQexec(conn, query);
    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        std::cerr << "Erreur LoadAll (classes): " << PQerrorMessage(conn) << std::endl;
        PQclear(result);
        return {};
    }

    auto classes = Parse(result);
    PQclear(result);

    for (auto& cls : classes) {
        LoadPresentations(cls);
        LoadAptitudes(cls);
    }

    std::cout << classes.size() << " classes chargées depuis la DB" << std::endl;
    return classes;
}

std::vector<ClassData> ClassRepository::LoadSubClasses(const std::string& parentClassId) {
    if (!conn) return {};

    std::string query =
        "SELECT id, COALESCE(classe_parente_id::text, ''), nom, "
        "COALESCE(alias, ''), COALESCE(description, ''), COALESCE(aide_joueur, ''), "
        "COALESCE(dee_de_vie, 8), "
        "COALESCE(jets_de_sauvegarde::text, '{}'), "
        "COALESCE(caracteristiques_de_sorts, ''), "
        "COALESCE(image_path, '') "
        "FROM classes WHERE classe_parente_id = '" + parentClassId + "' ORDER BY nom";

    PGresult* result = PQexec(conn, query.c_str());
    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        std::cerr << "Erreur LoadSubClasses: " << PQerrorMessage(conn) << std::endl;
        PQclear(result);
        return {};
    }

    auto classes = Parse(result);
    PQclear(result);

    for (auto& cls : classes) {
        LoadPresentations(cls);
        LoadAptitudes(cls);
    }

    return classes;
}

void ClassRepository::LoadPresentations(ClassData& cls) {
    std::string query = "SELECT titre, texte FROM classe_presentations "
                        "WHERE classe_id = '" + cls.id + "' ORDER BY ordre";
    PGresult* result = PQexec(conn, query.c_str());
    if (PQresultStatus(result) == PGRES_TUPLES_OK) {
        for (int i = 0; i < PQntuples(result); ++i) {
            ClassPresentation p;
            p.titre = PQgetvalue(result, i, 0);
            p.texte = PQgetvalue(result, i, 1);
            cls.presentations.push_back(p);
        }
    }
    PQclear(result);
}

void ClassRepository::LoadAptitudes(ClassData& cls) {
    std::string query =
        "SELECT nom, COALESCE(description_rapide, ''), niveau_acquisition, "
        "COALESCE(gain_de_sort, false) "
        "FROM classe_aptitudes "
        "WHERE classe_id = '" + cls.id + "' ORDER BY niveau_acquisition, nom";
    PGresult* result = PQexec(conn, query.c_str());
    if (PQresultStatus(result) == PGRES_TUPLES_OK) {
        for (int i = 0; i < PQntuples(result); ++i) {
            ClassAptitude a;
            a.nom                = PQgetvalue(result, i, 0);
            a.description_rapide = PQgetvalue(result, i, 1);
            a.niveau_acquisition = atoi(PQgetvalue(result, i, 2));
            a.gain_de_sort       = std::string(PQgetvalue(result, i, 3)) == "t";
            cls.aptitudes.push_back(a);
        }
    }
    PQclear(result);
}

std::vector<ClassData> ClassRepository::Parse(PGresult* result) {
    std::vector<ClassData> classes;
    int rows = PQntuples(result);
    for (int i = 0; i < rows; ++i) {
        ClassData cls;
        cls.id                        = PQgetvalue(result, i, 0);
        cls.parent_id                 = PQgetvalue(result, i, 1);
        cls.nom                       = PQgetvalue(result, i, 2);
        cls.alias                     = PQgetvalue(result, i, 3);
        cls.description               = PQgetvalue(result, i, 4);
        cls.aide_joueur               = PQgetvalue(result, i, 5);
        cls.dee_de_vie                = atoi(PQgetvalue(result, i, 6));
        cls.jets_de_sauvegarde        = ParsePgTextArray(PQgetvalue(result, i, 7));
        cls.caracteristiques_de_sorts = PQgetvalue(result, i, 8);
        cls.image_path                = PQgetvalue(result, i, 9);
        classes.push_back(cls);
    }
    return classes;
}

} // namespace Database
