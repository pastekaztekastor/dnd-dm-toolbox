#pragma once
#include <string>
#include <vector>
#include <sqlite3.h>

namespace Database {

struct ClassPresentation { std::string titre, texte; };
struct ClassAptitude {
    std::string nom, description_rapide;
    int  niveau_acquisition = 1;
    bool gain_de_sort       = false;
};

struct ClassData {
    std::string id, parent_id, nom, alias, description, aide_joueur;
    int dee_de_vie = 8;
    std::vector<std::string> jets_de_sauvegarde;
    std::string caracteristiques_de_sorts, image_path;
    std::vector<ClassPresentation> presentations;
    std::vector<ClassAptitude>     aptitudes;
};

class ClassRepository {
public:
    explicit ClassRepository(sqlite3* db);

    std::vector<ClassData> LoadAll();
    std::vector<ClassData> LoadSubClasses(const std::string& parentClassId);

private:
    sqlite3* db;

    void LoadPresentations(ClassData& cls);
    void LoadAptitudes(ClassData& cls);
};

} // namespace Database
