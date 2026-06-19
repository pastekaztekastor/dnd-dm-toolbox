#include "ClasseManagerRepository.h"

namespace ClasseManager {

ClasseManagerRepository::ClasseManagerRepository(sqlite3* db) : db(db) {}

static std::string col_text(sqlite3_stmt* s, int col) {
    const unsigned char* v = sqlite3_column_text(s, col);
    return v ? reinterpret_cast<const char*>(v) : "";
}

static const char* CLASSE_SELECT =
    "SELECT id, nom, classe_parente_id, alias,"
    " description, aide_joueur, creation_classe, creation_rapide,"
    " dee_de_vie, points_de_vie, points_de_vie_par_niveau,"
    " equipement_maitrise, jets_de_sauvegarde, competences,"
    " caracteristiques_de_sorts, image_path, source, source_ver"
    " FROM classes";

ClasseData ClasseManagerRepository::row_to_classe(sqlite3_stmt* s) {
    ClasseData c;
    c.id                       = col_text(s, 0);
    c.nom                      = col_text(s, 1);
    c.classe_parente_id        = col_text(s, 2);
    c.alias                    = col_text(s, 3);
    c.description              = col_text(s, 4);
    c.aide_joueur              = col_text(s, 5);
    c.creation_classe          = col_text(s, 6);
    c.creation_rapide          = col_text(s, 7);
    c.dee_de_vie               = sqlite3_column_int(s, 8);
    c.points_de_vie            = sqlite3_column_int(s, 9);
    c.points_de_vie_par_niveau = sqlite3_column_int(s, 10);
    c.equipement_maitrise      = col_text(s, 11);
    c.jets_de_sauvegarde       = col_text(s, 12);
    c.competences              = col_text(s, 13);
    c.caracteristiques_de_sorts= col_text(s, 14);
    c.image_path               = col_text(s, 15);
    c.source                   = col_text(s, 16);
    c.source_ver               = col_text(s, 17);
    return c;
}

EvolutionData ClasseManagerRepository::row_to_evolution(sqlite3_stmt* s) {
    EvolutionData e;
    e.id                = col_text(s, 0);
    e.classe_id         = col_text(s, 1);
    e.niveau            = sqlite3_column_int(s, 2);
    e.bonus_maitrise    = sqlite3_column_int(s, 3);
    e.choix_sous_classe = sqlite3_column_int(s, 4);
    e.aptitudes_gagnees = col_text(s, 5);
    for (int i = 0; i < 5; ++i) {
        int b = 6 + i * 4;
        e.cols[i].titre  = col_text(s, b);
        e.cols[i].type   = sqlite3_column_int(s, b + 1);
        e.cols[i].nombre = sqlite3_column_int(s, b + 2);
        e.cols[i].texte  = col_text(s, b + 3);
    }
    for (int i = 0; i < 9; ++i)
        e.sort_slots[i] = sqlite3_column_int(s, 26 + i);
    return e;
}

AptitudeData ClasseManagerRepository::row_to_aptitude(sqlite3_stmt* s) {
    AptitudeData a;
    a.id                       = col_text(s, 0);
    a.classe_id                = col_text(s, 1);
    a.alias                    = col_text(s, 2);
    a.nom                      = col_text(s, 3);
    a.description              = col_text(s, 4);
    a.description_rapide       = col_text(s, 5);
    a.niveau_acquisition       = sqlite3_column_int(s, 6);
    a.choix_champ_id           = col_text(s, 7);
    a.choix_valeur_id          = col_text(s, 8);
    a.aptitude_prerequisite_id = col_text(s, 9);
    return a;
}

// ── Load ─────────────────────────────────────────────────────────────────────

std::vector<ClasseData> ClasseManagerRepository::LoadBaseClasses() {
    std::vector<ClasseData> out;
    std::string sql = std::string(CLASSE_SELECT)
        + " WHERE classe_parente_id IS NULL ORDER BY nom";
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &s, nullptr) != SQLITE_OK) return out;
    while (sqlite3_step(s) == SQLITE_ROW) out.push_back(row_to_classe(s));
    sqlite3_finalize(s);
    return out;
}

std::vector<ClasseData> ClasseManagerRepository::LoadSubClasses(const std::string& parent_id) {
    std::vector<ClasseData> out;
    std::string sql = std::string(CLASSE_SELECT)
        + " WHERE classe_parente_id = ? ORDER BY nom";
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &s, nullptr) != SQLITE_OK) return out;
    sqlite3_bind_text(s, 1, parent_id.c_str(), -1, SQLITE_TRANSIENT);
    while (sqlite3_step(s) == SQLITE_ROW) out.push_back(row_to_classe(s));
    sqlite3_finalize(s);
    return out;
}

void ClasseManagerRepository::LoadChildData(ClasseData& c) {
    // presentations
    {
        c.presentations.clear();
        sqlite3_stmt* s;
        if (sqlite3_prepare_v2(db,
                "SELECT id,classe_id,ordre,titre,texte FROM classe_presentations"
                " WHERE classe_id=? ORDER BY ordre",
                -1, &s, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(s, 1, c.id.c_str(), -1, SQLITE_TRANSIENT);
            while (sqlite3_step(s) == SQLITE_ROW) {
                PresentationData p;
                p.id        = col_text(s, 0);
                p.classe_id = col_text(s, 1);
                p.ordre     = sqlite3_column_int(s, 2);
                p.titre     = col_text(s, 3);
                p.texte     = col_text(s, 4);
                c.presentations.push_back(p);
            }
            sqlite3_finalize(s);
        }
    }

    // evolutions
    {
        c.evolutions.clear();
        sqlite3_stmt* s;
        if (sqlite3_prepare_v2(db,
                "SELECT id,classe_id,niveau,bonus_maitrise,choix_sous_classe,"
                " aptitudes_gagnees,"
                " col_1_titre,col_1_type,col_1_nombre,col_1_texte,"
                " col_2_titre,col_2_type,col_2_nombre,col_2_texte,"
                " col_3_titre,col_3_type,col_3_nombre,col_3_texte,"
                " col_4_titre,col_4_type,col_4_nombre,col_4_texte,"
                " col_5_titre,col_5_type,col_5_nombre,col_5_texte,"
                " sort_lvl_1,sort_lvl_2,sort_lvl_3,sort_lvl_4,sort_lvl_5,"
                " sort_lvl_6,sort_lvl_7,sort_lvl_8,sort_lvl_9"
                " FROM classe_evolutions WHERE classe_id=? ORDER BY niveau",
                -1, &s, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(s, 1, c.id.c_str(), -1, SQLITE_TRANSIENT);
            while (sqlite3_step(s) == SQLITE_ROW) c.evolutions.push_back(row_to_evolution(s));
            sqlite3_finalize(s);
        }
    }

    // choix_champs (base class only — skip for sub-classes)
    if (c.classe_parente_id.empty()) {
        c.choix_champs.clear();
        sqlite3_stmt* s;
        if (sqlite3_prepare_v2(db,
                "SELECT id,classe_id,nom,alias,niveau,description"
                " FROM classe_choix_champ WHERE classe_id=? ORDER BY niveau,nom",
                -1, &s, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(s, 1, c.id.c_str(), -1, SQLITE_TRANSIENT);
            while (sqlite3_step(s) == SQLITE_ROW) {
                ChoixChampData cc;
                cc.id          = col_text(s, 0);
                cc.classe_id   = col_text(s, 1);
                cc.nom         = col_text(s, 2);
                cc.alias       = col_text(s, 3);
                cc.niveau      = sqlite3_column_int(s, 4);
                cc.description = col_text(s, 5);
                c.choix_champs.push_back(cc);
            }
            sqlite3_finalize(s);
        }
        // Load valeurs for each champ
        for (auto& cc : c.choix_champs) {
            sqlite3_stmt* sv;
            if (sqlite3_prepare_v2(db,
                    "SELECT id,champ_id,nom,valeur,description"
                    " FROM classe_choix_valeur WHERE champ_id=? ORDER BY rowid",
                    -1, &sv, nullptr) == SQLITE_OK) {
                sqlite3_bind_text(sv, 1, cc.id.c_str(), -1, SQLITE_TRANSIENT);
                while (sqlite3_step(sv) == SQLITE_ROW) {
                    ChoixValeurData v;
                    v.id          = col_text(sv, 0);
                    v.champ_id    = col_text(sv, 1);
                    v.nom         = col_text(sv, 2);
                    v.valeur      = col_text(sv, 3);
                    v.description = col_text(sv, 4);
                    cc.valeurs.push_back(v);
                }
                sqlite3_finalize(sv);
            }
        }
    }

    // aptitudes
    {
        c.aptitudes.clear();
        sqlite3_stmt* s;
        if (sqlite3_prepare_v2(db,
                "SELECT id,classe_id,alias,nom,description,description_rapide,"
                " niveau_acquisition,choix_champ_id,choix_valeur_id,aptitude_prerequisite_id"
                " FROM classe_aptitudes WHERE classe_id=? ORDER BY niveau_acquisition,nom",
                -1, &s, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(s, 1, c.id.c_str(), -1, SQLITE_TRANSIENT);
            while (sqlite3_step(s) == SQLITE_ROW) c.aptitudes.push_back(row_to_aptitude(s));
            sqlite3_finalize(s);
        }
    }

    // choix_equipement
    {
        c.choix_equipement.clear();
        sqlite3_stmt* s;
        if (sqlite3_prepare_v2(db,
                "SELECT id,classe_id,description,choix_num,option_num"
                " FROM classe_choix_equipement WHERE classe_id=? ORDER BY choix_num,option_num",
                -1, &s, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(s, 1, c.id.c_str(), -1, SQLITE_TRANSIENT);
            while (sqlite3_step(s) == SQLITE_ROW) {
                ChoixEquipementData e;
                e.id          = col_text(s, 0);
                e.classe_id   = col_text(s, 1);
                e.description = col_text(s, 2);
                e.choix_num   = sqlite3_column_int(s, 3);
                e.option_num  = sqlite3_column_int(s, 4);
                c.choix_equipement.push_back(e);
            }
            sqlite3_finalize(s);
        }
    }

    // sous-classes (recursive, no further choix_champs)
    c.sous_classes = LoadSubClasses(c.id);
    for (auto& sc : c.sous_classes)
        LoadChildData(sc);
}

// ── Classe ────────────────────────────────────────────────────────────────────

bool ClasseManagerRepository::InsertClasse(const ClasseData& d) {
    const char* sql =
        "INSERT INTO classes (id,nom,classe_parente_id,alias,"
        " description,aide_joueur,creation_classe,creation_rapide,"
        " dee_de_vie,points_de_vie,points_de_vie_par_niveau,"
        " equipement_maitrise,jets_de_sauvegarde,competences,"
        " caracteristiques_de_sorts,image_path,source,source_ver)"
        " VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db, sql, -1, &s, nullptr) != SQLITE_OK) return false;
    auto bs = [&](int i, const std::string& v) {
        v.empty() ? sqlite3_bind_null(s, i)
                  : sqlite3_bind_text(s, i, v.c_str(), -1, SQLITE_TRANSIENT);
    };
    bs(1,d.id); bs(2,d.nom); bs(3,d.classe_parente_id); bs(4,d.alias);
    bs(5,d.description); bs(6,d.aide_joueur); bs(7,d.creation_classe); bs(8,d.creation_rapide);
    sqlite3_bind_int(s,9,d.dee_de_vie);
    sqlite3_bind_int(s,10,d.points_de_vie);
    sqlite3_bind_int(s,11,d.points_de_vie_par_niveau);
    bs(12, d.equipement_maitrise.empty() ? std::string("[]") : d.equipement_maitrise);
    bs(13, d.jets_de_sauvegarde.empty()  ? std::string("[]") : d.jets_de_sauvegarde);
    bs(14, d.competences.empty()         ? std::string("[]") : d.competences);
    bs(15,d.caracteristiques_de_sorts);
    bs(16, d.image_path.empty() ? std::string("placeholder.png") : d.image_path);
    bs(17,d.source); bs(18,d.source_ver);
    int rc = sqlite3_step(s); sqlite3_finalize(s);
    return rc == SQLITE_DONE;
}

bool ClasseManagerRepository::UpdateClasse(const ClasseData& d) {
    const char* sql =
        "UPDATE classes SET nom=?,classe_parente_id=?,alias=?,"
        " description=?,aide_joueur=?,creation_classe=?,creation_rapide=?,"
        " dee_de_vie=?,points_de_vie=?,points_de_vie_par_niveau=?,"
        " equipement_maitrise=?,jets_de_sauvegarde=?,competences=?,"
        " caracteristiques_de_sorts=?,image_path=?,source=?,source_ver=?"
        " WHERE id=?";
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db, sql, -1, &s, nullptr) != SQLITE_OK) return false;
    auto bs = [&](int i, const std::string& v) {
        v.empty() ? sqlite3_bind_null(s, i)
                  : sqlite3_bind_text(s, i, v.c_str(), -1, SQLITE_TRANSIENT);
    };
    bs(1,d.nom); bs(2,d.classe_parente_id); bs(3,d.alias);
    bs(4,d.description); bs(5,d.aide_joueur); bs(6,d.creation_classe); bs(7,d.creation_rapide);
    sqlite3_bind_int(s,8,d.dee_de_vie);
    sqlite3_bind_int(s,9,d.points_de_vie);
    sqlite3_bind_int(s,10,d.points_de_vie_par_niveau);
    bs(11,d.equipement_maitrise); bs(12,d.jets_de_sauvegarde); bs(13,d.competences);
    bs(14,d.caracteristiques_de_sorts); bs(15,d.image_path); bs(16,d.source); bs(17,d.source_ver);
    bs(18,d.id);
    int rc = sqlite3_step(s); sqlite3_finalize(s);
    return rc == SQLITE_DONE;
}

bool ClasseManagerRepository::DeleteClasse(const std::string& id) {
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db,"DELETE FROM classes WHERE id=?",-1,&s,nullptr)!=SQLITE_OK) return false;
    sqlite3_bind_text(s,1,id.c_str(),-1,SQLITE_TRANSIENT);
    int rc=sqlite3_step(s); sqlite3_finalize(s); return rc==SQLITE_DONE;
}

// ── Évolution ─────────────────────────────────────────────────────────────────

bool ClasseManagerRepository::InsertEvolution(const EvolutionData& e) {
    const char* sql =
        "INSERT INTO classe_evolutions"
        " (id,classe_id,niveau,bonus_maitrise,choix_sous_classe,aptitudes_gagnees,"
        "  col_1_titre,col_1_type,col_1_nombre,col_1_texte,"
        "  col_2_titre,col_2_type,col_2_nombre,col_2_texte,"
        "  col_3_titre,col_3_type,col_3_nombre,col_3_texte,"
        "  col_4_titre,col_4_type,col_4_nombre,col_4_texte,"
        "  col_5_titre,col_5_type,col_5_nombre,col_5_texte,"
        "  sort_lvl_1,sort_lvl_2,sort_lvl_3,sort_lvl_4,sort_lvl_5,"
        "  sort_lvl_6,sort_lvl_7,sort_lvl_8,sort_lvl_9)"
        " VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db,sql,-1,&s,nullptr)!=SQLITE_OK) return false;
    auto bs=[&](int i,const std::string& v){v.empty()?sqlite3_bind_null(s,i):sqlite3_bind_text(s,i,v.c_str(),-1,SQLITE_TRANSIENT);};
    bs(1,e.id); bs(2,e.classe_id);
    sqlite3_bind_int(s,3,e.niveau); sqlite3_bind_int(s,4,e.bonus_maitrise); sqlite3_bind_int(s,5,e.choix_sous_classe);
    bs(6,e.aptitudes_gagnees);
    for (int i=0;i<5;++i){int b=7+i*4; bs(b,e.cols[i].titre); sqlite3_bind_int(s,b+1,e.cols[i].type); sqlite3_bind_int(s,b+2,e.cols[i].nombre); bs(b+3,e.cols[i].texte);}
    for (int i=0;i<9;++i) sqlite3_bind_int(s,27+i,e.sort_slots[i]);
    int rc=sqlite3_step(s); sqlite3_finalize(s); return rc==SQLITE_DONE;
}

bool ClasseManagerRepository::UpdateEvolution(const EvolutionData& e) {
    const char* sql =
        "UPDATE classe_evolutions SET"
        " niveau=?,bonus_maitrise=?,choix_sous_classe=?,aptitudes_gagnees=?,"
        " col_1_titre=?,col_1_type=?,col_1_nombre=?,col_1_texte=?,"
        " col_2_titre=?,col_2_type=?,col_2_nombre=?,col_2_texte=?,"
        " col_3_titre=?,col_3_type=?,col_3_nombre=?,col_3_texte=?,"
        " col_4_titre=?,col_4_type=?,col_4_nombre=?,col_4_texte=?,"
        " col_5_titre=?,col_5_type=?,col_5_nombre=?,col_5_texte=?,"
        " sort_lvl_1=?,sort_lvl_2=?,sort_lvl_3=?,sort_lvl_4=?,sort_lvl_5=?,"
        " sort_lvl_6=?,sort_lvl_7=?,sort_lvl_8=?,sort_lvl_9=?"
        " WHERE id=?";
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db,sql,-1,&s,nullptr)!=SQLITE_OK) return false;
    auto bs=[&](int i,const std::string& v){v.empty()?sqlite3_bind_null(s,i):sqlite3_bind_text(s,i,v.c_str(),-1,SQLITE_TRANSIENT);};
    sqlite3_bind_int(s,1,e.niveau); sqlite3_bind_int(s,2,e.bonus_maitrise); sqlite3_bind_int(s,3,e.choix_sous_classe);
    bs(4,e.aptitudes_gagnees);
    for (int i=0;i<5;++i){int b=5+i*4; bs(b,e.cols[i].titre); sqlite3_bind_int(s,b+1,e.cols[i].type); sqlite3_bind_int(s,b+2,e.cols[i].nombre); bs(b+3,e.cols[i].texte);}
    for (int i=0;i<9;++i) sqlite3_bind_int(s,25+i,e.sort_slots[i]);
    bs(34,e.id);
    int rc=sqlite3_step(s); sqlite3_finalize(s); return rc==SQLITE_DONE;
}

bool ClasseManagerRepository::DeleteEvolution(const std::string& id) {
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db,"DELETE FROM classe_evolutions WHERE id=?",-1,&s,nullptr)!=SQLITE_OK) return false;
    sqlite3_bind_text(s,1,id.c_str(),-1,SQLITE_TRANSIENT);
    int rc=sqlite3_step(s); sqlite3_finalize(s); return rc==SQLITE_DONE;
}

// ── Choix champ ───────────────────────────────────────────────────────────────

bool ClasseManagerRepository::InsertChoixChamp(const ChoixChampData& c) {
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db,
            "INSERT INTO classe_choix_champ (id,classe_id,nom,alias,niveau,description)"
            " VALUES (?,?,?,?,?,?)",
            -1,&s,nullptr)!=SQLITE_OK) return false;
    auto bs=[&](int i,const std::string& v){v.empty()?sqlite3_bind_null(s,i):sqlite3_bind_text(s,i,v.c_str(),-1,SQLITE_TRANSIENT);};
    bs(1,c.id); bs(2,c.classe_id); bs(3,c.nom); bs(4,c.alias);
    sqlite3_bind_int(s,5,c.niveau); bs(6,c.description);
    int rc=sqlite3_step(s); sqlite3_finalize(s); return rc==SQLITE_DONE;
}

bool ClasseManagerRepository::UpdateChoixChamp(const ChoixChampData& c) {
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db,
            "UPDATE classe_choix_champ SET nom=?,alias=?,niveau=?,description=? WHERE id=?",
            -1,&s,nullptr)!=SQLITE_OK) return false;
    auto bs=[&](int i,const std::string& v){v.empty()?sqlite3_bind_null(s,i):sqlite3_bind_text(s,i,v.c_str(),-1,SQLITE_TRANSIENT);};
    bs(1,c.nom); bs(2,c.alias); sqlite3_bind_int(s,3,c.niveau); bs(4,c.description); bs(5,c.id);
    int rc=sqlite3_step(s); sqlite3_finalize(s); return rc==SQLITE_DONE;
}

bool ClasseManagerRepository::DeleteChoixChamp(const std::string& id) {
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db,"DELETE FROM classe_choix_champ WHERE id=?",-1,&s,nullptr)!=SQLITE_OK) return false;
    sqlite3_bind_text(s,1,id.c_str(),-1,SQLITE_TRANSIENT);
    int rc=sqlite3_step(s); sqlite3_finalize(s); return rc==SQLITE_DONE;
}

// ── Choix valeur ──────────────────────────────────────────────────────────────

bool ClasseManagerRepository::InsertChoixValeur(const ChoixValeurData& v) {
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db,
            "INSERT INTO classe_choix_valeur (id,champ_id,nom,valeur,description)"
            " VALUES (?,?,?,?,?)",
            -1,&s,nullptr)!=SQLITE_OK) return false;
    auto bs=[&](int i,const std::string& str){str.empty()?sqlite3_bind_null(s,i):sqlite3_bind_text(s,i,str.c_str(),-1,SQLITE_TRANSIENT);};
    bs(1,v.id); bs(2,v.champ_id); bs(3,v.nom); bs(4,v.valeur); bs(5,v.description);
    int rc=sqlite3_step(s); sqlite3_finalize(s); return rc==SQLITE_DONE;
}

bool ClasseManagerRepository::UpdateChoixValeur(const ChoixValeurData& v) {
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db,
            "UPDATE classe_choix_valeur SET nom=?,valeur=?,description=? WHERE id=?",
            -1,&s,nullptr)!=SQLITE_OK) return false;
    auto bs=[&](int i,const std::string& str){str.empty()?sqlite3_bind_null(s,i):sqlite3_bind_text(s,i,str.c_str(),-1,SQLITE_TRANSIENT);};
    bs(1,v.nom); bs(2,v.valeur); bs(3,v.description); bs(4,v.id);
    int rc=sqlite3_step(s); sqlite3_finalize(s); return rc==SQLITE_DONE;
}

bool ClasseManagerRepository::DeleteChoixValeur(const std::string& id) {
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db,"DELETE FROM classe_choix_valeur WHERE id=?",-1,&s,nullptr)!=SQLITE_OK) return false;
    sqlite3_bind_text(s,1,id.c_str(),-1,SQLITE_TRANSIENT);
    int rc=sqlite3_step(s); sqlite3_finalize(s); return rc==SQLITE_DONE;
}

// ── Aptitude ──────────────────────────────────────────────────────────────────

bool ClasseManagerRepository::InsertAptitude(const AptitudeData& a) {
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db,
            "INSERT INTO classe_aptitudes"
            " (id,classe_id,alias,nom,description,description_rapide,niveau_acquisition,"
            "  choix_champ_id,choix_valeur_id,aptitude_prerequisite_id)"
            " VALUES (?,?,?,?,?,?,?,?,?,?)",
            -1,&s,nullptr)!=SQLITE_OK) return false;
    auto bs=[&](int i,const std::string& v){v.empty()?sqlite3_bind_null(s,i):sqlite3_bind_text(s,i,v.c_str(),-1,SQLITE_TRANSIENT);};
    bs(1,a.id); bs(2,a.classe_id); bs(3,a.alias); bs(4,a.nom);
    bs(5,a.description); bs(6,a.description_rapide);
    sqlite3_bind_int(s,7,a.niveau_acquisition);
    bs(8,a.choix_champ_id); bs(9,a.choix_valeur_id); bs(10,a.aptitude_prerequisite_id);
    int rc=sqlite3_step(s); sqlite3_finalize(s); return rc==SQLITE_DONE;
}

bool ClasseManagerRepository::UpdateAptitude(const AptitudeData& a) {
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db,
            "UPDATE classe_aptitudes SET"
            " alias=?,nom=?,description=?,description_rapide=?,niveau_acquisition=?,"
            " choix_champ_id=?,choix_valeur_id=?,aptitude_prerequisite_id=?"
            " WHERE id=?",
            -1,&s,nullptr)!=SQLITE_OK) return false;
    auto bs=[&](int i,const std::string& v){v.empty()?sqlite3_bind_null(s,i):sqlite3_bind_text(s,i,v.c_str(),-1,SQLITE_TRANSIENT);};
    bs(1,a.alias); bs(2,a.nom); bs(3,a.description); bs(4,a.description_rapide);
    sqlite3_bind_int(s,5,a.niveau_acquisition);
    bs(6,a.choix_champ_id); bs(7,a.choix_valeur_id); bs(8,a.aptitude_prerequisite_id);
    bs(9,a.id);
    int rc=sqlite3_step(s); sqlite3_finalize(s); return rc==SQLITE_DONE;
}

bool ClasseManagerRepository::DeleteAptitude(const std::string& id) {
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db,"DELETE FROM classe_aptitudes WHERE id=?",-1,&s,nullptr)!=SQLITE_OK) return false;
    sqlite3_bind_text(s,1,id.c_str(),-1,SQLITE_TRANSIENT);
    int rc=sqlite3_step(s); sqlite3_finalize(s); return rc==SQLITE_DONE;
}

// ── Choix équipement ──────────────────────────────────────────────────────────

bool ClasseManagerRepository::InsertChoixEquipement(const ChoixEquipementData& e) {
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db,
            "INSERT INTO classe_choix_equipement (id,classe_id,choix_num,option_num,description)"
            " VALUES (?,?,?,?,?)",
            -1,&s,nullptr)!=SQLITE_OK) return false;
    auto bs=[&](int i,const std::string& v){v.empty()?sqlite3_bind_null(s,i):sqlite3_bind_text(s,i,v.c_str(),-1,SQLITE_TRANSIENT);};
    bs(1,e.id); bs(2,e.classe_id);
    sqlite3_bind_int(s,3,e.choix_num); sqlite3_bind_int(s,4,e.option_num);
    bs(5,e.description);
    int rc=sqlite3_step(s); sqlite3_finalize(s); return rc==SQLITE_DONE;
}

bool ClasseManagerRepository::UpdateChoixEquipement(const ChoixEquipementData& e) {
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db,
            "UPDATE classe_choix_equipement SET choix_num=?,option_num=?,description=? WHERE id=?",
            -1,&s,nullptr)!=SQLITE_OK) return false;
    sqlite3_bind_int(s,1,e.choix_num); sqlite3_bind_int(s,2,e.option_num);
    e.description.empty()?sqlite3_bind_null(s,3):sqlite3_bind_text(s,3,e.description.c_str(),-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(s,4,e.id.c_str(),-1,SQLITE_TRANSIENT);
    int rc=sqlite3_step(s); sqlite3_finalize(s); return rc==SQLITE_DONE;
}

bool ClasseManagerRepository::DeleteChoixEquipement(const std::string& id) {
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db,"DELETE FROM classe_choix_equipement WHERE id=?",-1,&s,nullptr)!=SQLITE_OK) return false;
    sqlite3_bind_text(s,1,id.c_str(),-1,SQLITE_TRANSIENT);
    int rc=sqlite3_step(s); sqlite3_finalize(s); return rc==SQLITE_DONE;
}

// ── Transactions ──────────────────────────────────────────────────────────────

void ClasseManagerRepository::BeginTransaction() { sqlite3_exec(db,"BEGIN TRANSACTION",nullptr,nullptr,nullptr); }
void ClasseManagerRepository::Commit()           { sqlite3_exec(db,"COMMIT",nullptr,nullptr,nullptr); }
void ClasseManagerRepository::Rollback()         { sqlite3_exec(db,"ROLLBACK",nullptr,nullptr,nullptr); }

// ── Utilitaires ───────────────────────────────────────────────────────────────

bool ClasseManagerRepository::ExistsNom(const std::string& nom, const std::string& excludeId) {
    const char* sql = excludeId.empty()
        ? "SELECT 1 FROM classes WHERE nom=?"
        : "SELECT 1 FROM classes WHERE nom=? AND id!=?";
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db,sql,-1,&s,nullptr)!=SQLITE_OK) return false;
    sqlite3_bind_text(s,1,nom.c_str(),-1,SQLITE_TRANSIENT);
    if (!excludeId.empty()) sqlite3_bind_text(s,2,excludeId.c_str(),-1,SQLITE_TRANSIENT);
    bool found = sqlite3_step(s)==SQLITE_ROW;
    sqlite3_finalize(s); return found;
}

std::string ClasseManagerRepository::GenerateId() {
    std::string id;
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(db,"SELECT lower(hex(randomblob(4)))",-1,&s,nullptr)==SQLITE_OK) {
        if (sqlite3_step(s)==SQLITE_ROW) id=col_text(s,0);
        sqlite3_finalize(s);
    }
    return id;
}

} // namespace ClasseManager
