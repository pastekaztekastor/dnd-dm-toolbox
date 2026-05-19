#include "../../../include/database/repositories/ClassRepository.h"
#include <iostream>
#include <sstream>

namespace Database {

static std::string col(sqlite3_stmt* stmt, int i) {
    const char* val = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
    return val ? val : "";
}

// Parse une liste CSV "val1,val2,..." en vector<string>
static std::vector<std::string> ParseCsvList(const std::string& raw) {
    std::vector<std::string> out;
    if (raw.empty()) return out;
    std::stringstream ss(raw);
    std::string token;
    while (std::getline(ss, token, ',')) {
        if (!token.empty())
            out.push_back(token);
    }
    return out;
}

ClassRepository::ClassRepository(sqlite3* db) : db(db) {}

std::vector<ClassData> ClassRepository::LoadAll() {
    if (!db) return {};

    const char* query =
        "SELECT id, COALESCE(classe_parente_id, ''), nom, "
        "COALESCE(alias, ''), COALESCE(description, ''), COALESCE(aide_joueur, ''), "
        "COALESCE(dee_de_vie, 8), "
        "COALESCE(jets_de_sauvegarde, ''), "
        "COALESCE(caracteristiques_de_sorts, ''), "
        "COALESCE(image_path, '') "
        "FROM classes WHERE classe_parente_id IS NULL ORDER BY nom";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Erreur LoadAll (classes): " << sqlite3_errmsg(db) << std::endl;
        return {};
    }

    std::vector<ClassData> classes;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        ClassData cls;
        cls.id                        = col(stmt, 0);
        cls.parent_id                 = col(stmt, 1);
        cls.nom                       = col(stmt, 2);
        cls.alias                     = col(stmt, 3);
        cls.description               = col(stmt, 4);
        cls.aide_joueur               = col(stmt, 5);
        cls.dee_de_vie                = sqlite3_column_int(stmt, 6);
        cls.jets_de_sauvegarde        = ParseCsvList(col(stmt, 7));
        cls.caracteristiques_de_sorts = col(stmt, 8);
        cls.image_path                = col(stmt, 9);
        classes.push_back(cls);
    }
    sqlite3_finalize(stmt);

    for (auto& cls : classes) {
        LoadPresentations(cls);
        LoadAptitudes(cls);
    }

    std::cout << classes.size() << " classes chargées depuis la DB" << std::endl;
    return classes;
}

std::vector<ClassData> ClassRepository::LoadSubClasses(const std::string& parentClassId) {
    if (!db) return {};

    const char* query =
        "SELECT id, COALESCE(classe_parente_id, ''), nom, "
        "COALESCE(alias, ''), COALESCE(description, ''), COALESCE(aide_joueur, ''), "
        "COALESCE(dee_de_vie, 8), "
        "COALESCE(jets_de_sauvegarde, ''), "
        "COALESCE(caracteristiques_de_sorts, ''), "
        "COALESCE(image_path, '') "
        "FROM classes WHERE classe_parente_id = ? ORDER BY nom";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Erreur LoadSubClasses: " << sqlite3_errmsg(db) << std::endl;
        return {};
    }
    sqlite3_bind_text(stmt, 1, parentClassId.c_str(), -1, SQLITE_STATIC);

    std::vector<ClassData> classes;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        ClassData cls;
        cls.id                        = col(stmt, 0);
        cls.parent_id                 = col(stmt, 1);
        cls.nom                       = col(stmt, 2);
        cls.alias                     = col(stmt, 3);
        cls.description               = col(stmt, 4);
        cls.aide_joueur               = col(stmt, 5);
        cls.dee_de_vie                = sqlite3_column_int(stmt, 6);
        cls.jets_de_sauvegarde        = ParseCsvList(col(stmt, 7));
        cls.caracteristiques_de_sorts = col(stmt, 8);
        cls.image_path                = col(stmt, 9);
        classes.push_back(cls);
    }
    sqlite3_finalize(stmt);

    for (auto& cls : classes) {
        LoadPresentations(cls);
        LoadAptitudes(cls);
    }

    return classes;
}

void ClassRepository::LoadPresentations(ClassData& cls) {
    const char* query = "SELECT titre, texte FROM classe_presentations WHERE classe_id = ? ORDER BY ordre";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) return;
    sqlite3_bind_text(stmt, 1, cls.id.c_str(), -1, SQLITE_STATIC);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        ClassPresentation p;
        p.titre = col(stmt, 0);
        p.texte = col(stmt, 1);
        cls.presentations.push_back(p);
    }
    sqlite3_finalize(stmt);
}

void ClassRepository::LoadAptitudes(ClassData& cls) {
    const char* query =
        "SELECT nom, COALESCE(description_rapide, ''), niveau_acquisition, "
        "COALESCE(gain_de_sort, 0) "
        "FROM classe_aptitudes WHERE classe_id = ? ORDER BY niveau_acquisition, nom";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) return;
    sqlite3_bind_text(stmt, 1, cls.id.c_str(), -1, SQLITE_STATIC);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        ClassAptitude a;
        a.nom                = col(stmt, 0);
        a.description_rapide = col(stmt, 1);
        a.niveau_acquisition = sqlite3_column_int(stmt, 2);
        a.gain_de_sort       = sqlite3_column_int(stmt, 3) != 0;
        cls.aptitudes.push_back(a);
    }
    sqlite3_finalize(stmt);
}

} // namespace Database
