#pragma once

#include <string>
#include <vector>
#include <array>
#include <sqlite3.h>

// Couche d'accès aux données — miroir 1:1 de classe_manager_schema.sql (v2).
// TODO : corps des méthodes à écrire ensemble.

namespace ClasseManager {

struct PresentationData {
    std::string id, classe_id, titre, texte;
    int ordre = 0;
};

// classe_evolution_colonnes
struct EvolutionColonneData {
    std::string id, classe_id, titre, titre_court;
    int ordre = 0;
    int type   = 0; // 0=texte, 1=nombre
};

// classe_evolution_valeurs — une valeur pour (evolution_id, colonne_id)
struct EvolutionValeurData {
    std::string id, evolution_id, colonne_id;
    int         valeur_nombre = 0;
    std::string valeur_texte;
};

struct EvolutionData {
    std::string id, classe_id;
    int niveau = 1, bonus_maitrise = 2, choix_sous_classe = 0;
    std::array<int, 9> sort_slots = {}; // sort_lvl_1..9
    std::vector<EvolutionValeurData> valeurs; // colonnes dynamiques, jointes à l'affichage
};

struct ChoixValeurData {
    std::string id, champ_id, nom, valeur, description;
};

struct ChoixChampData {
    std::string id, classe_id, nom, alias, description;
    int niveau = 1;
    std::vector<ChoixValeurData> valeurs;
};

struct AptitudeData {
    std::string id, classe_id, alias, nom;
    std::string description, description_rapide;
    int niveau_acquisition = 1;
    std::string choix_champ_id;           // déclenche un choix
    std::string choix_valeur_id;          // dépend d'une valeur de choix
    std::string aptitude_prerequisite_id; // requiert une autre aptitude (version précédente)
    int version_aptitude = 1;
};

struct ChoixEquipementData {
    std::string id, classe_id, description;
    int choix_num = 1, option_num = 1;
};

struct ClasseData {
    std::string id, nom, alias, classe_parente_id;
    std::string description, aide_joueur, creation_classe, creation_rapide;
    int dee_de_vie = 6, points_de_vie_lvl_1 = 6, points_de_vie_par_niveau = 4;
    std::string equipement_maitrise, jets_de_sauvegarde, competences;
    std::string caracteristiques_de_sorts, image_path, source, source_ver;
    bool utilise_magie = true;

    std::vector<PresentationData>     presentations;
    std::vector<EvolutionColonneData> evolution_colonnes;
    std::vector<EvolutionData>        evolutions;
    std::vector<ChoixChampData>       choix_champs;   // choix de build (classe de base uniquement)
    std::vector<AptitudeData>         aptitudes;
    std::vector<ChoixEquipementData>  choix_equipement;
    std::vector<ClasseData>           sous_classes;
};

class ClasseManagerRepository {
public:
    explicit ClasseManagerRepository(sqlite3* db);

    // Lecture
    std::vector<ClasseData> LoadBaseClasses();
    std::vector<ClasseData> LoadSubClasses(const std::string& parent_id);
    void LoadChildData(ClasseData& c);

    // Classe
    bool InsertClasse(const ClasseData& c);
    bool UpdateClasse(const ClasseData& c);
    bool DeleteClasse(const std::string& id);

    // Colonnes d'évolution dynamiques
    bool InsertEvolutionColonne(const EvolutionColonneData& col);
    bool UpdateEvolutionColonne(const EvolutionColonneData& col);
    bool DeleteEvolutionColonne(const std::string& id);

    // Évolution
    bool InsertEvolution(const EvolutionData& e);
    bool UpdateEvolution(const EvolutionData& e);
    bool DeleteEvolution(const std::string& id);

    // Valeurs de colonnes dynamiques (par évolution)
    bool UpsertEvolutionValeur(const EvolutionValeurData& v);
    bool DeleteEvolutionValeur(const std::string& id);

    // Choix de build — champ
    bool InsertChoixChamp(const ChoixChampData& c);
    bool UpdateChoixChamp(const ChoixChampData& c);
    bool DeleteChoixChamp(const std::string& id);

    // Choix de build — valeur
    bool InsertChoixValeur(const ChoixValeurData& v);
    bool UpdateChoixValeur(const ChoixValeurData& v);
    bool DeleteChoixValeur(const std::string& id);

    // Aptitude
    bool InsertAptitude(const AptitudeData& a);
    bool UpdateAptitude(const AptitudeData& a);
    bool DeleteAptitude(const std::string& id);

    // Présentation (textes de lore)
    bool InsertPresentation(const PresentationData& p);
    bool UpdatePresentation(const PresentationData& p);
    bool DeletePresentation(const std::string& id);

    // Équipement
    bool InsertChoixEquipement(const ChoixEquipementData& e);
    bool UpdateChoixEquipement(const ChoixEquipementData& e);
    bool DeleteChoixEquipement(const std::string& id);

    // Transactions
    void BeginTransaction();
    void Commit();
    void Rollback();

    // Utilitaires
    bool        ExistsNom(const std::string& nom, const std::string& excludeId = "");
    std::string GenerateId();

private:
    sqlite3* db;
    static ClasseData            row_to_classe(sqlite3_stmt* s);
    static EvolutionColonneData  row_to_evolution_colonne(sqlite3_stmt* s);
    static EvolutionData         row_to_evolution(sqlite3_stmt* s);
    static AptitudeData          row_to_aptitude(sqlite3_stmt* s);
};

} // namespace ClasseManager
