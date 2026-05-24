-- ============================================================================
-- Base de données SQLite — DnD DM Toolbox
-- Converti depuis les schémas PostgreSQL
-- UUID remplacé par TEXT, BOOLEAN par INTEGER, TEXT[]/UUID[] par TEXT (CSV)
-- ============================================================================

PRAGMA foreign_keys = ON;

-- ============================================================================
-- TABLE: EQUIPEMENTS
-- ============================================================================
CREATE TABLE IF NOT EXISTS equipements (
    id                   TEXT PRIMARY KEY,
    nom                  TEXT NOT NULL UNIQUE,
    alias                TEXT NOT NULL UNIQUE,
    categorie            TEXT,
    description          TEXT,
    poid                 INTEGER,
    prix                 INTEGER,
    arme_degat_dee       TEXT,
    arme_degat_type      TEXT,
    arme_distance_normal REAL DEFAULT 1.5,
    arme_distance_max    REAL,
    armure_classe        INTEGER,
    type                 TEXT,
    propriete            TEXT
);

CREATE TABLE IF NOT EXISTS equipement_proprietes (
    id          TEXT PRIMARY KEY,
    nom         TEXT NOT NULL UNIQUE,
    description TEXT,
    categorie   TEXT
);

CREATE TABLE IF NOT EXISTS equipement_propriete_liens (
    id                    TEXT PRIMARY KEY,
    equipement_id         TEXT NOT NULL REFERENCES equipements(id) ON DELETE CASCADE,
    equipement_proprietes TEXT NOT NULL REFERENCES equipement_proprietes(id) ON DELETE CASCADE
);

-- ============================================================================
-- TABLE: SORTS
-- ============================================================================
CREATE TABLE IF NOT EXISTS sorts (
    id                TEXT PRIMARY KEY,
    nom               TEXT NOT NULL UNIQUE,
    alias             TEXT NOT NULL UNIQUE,
    description       TEXT NOT NULL,
    niveau            INTEGER NOT NULL,
    ecole             TEXT NOT NULL,
    composantes       TEXT NOT NULL,
    temps_incantation TEXT NOT NULL,
    portee            TEXT,
    classes           TEXT,
    duree             TEXT NOT NULL
);

-- ============================================================================
-- TABLE: RACES
-- ============================================================================
CREATE TABLE IF NOT EXISTS races (
    id                 TEXT PRIMARY KEY,
    nom                TEXT NOT NULL UNIQUE,
    race_parent_id     TEXT REFERENCES races(id) DEFAULT NULL,
    alias              TEXT NOT NULL UNIQUE,
    description        TEXT,
    bonus_forces       INTEGER NOT NULL DEFAULT 0,
    bonus_charisme     INTEGER NOT NULL DEFAULT 0,
    bonus_dexterite    INTEGER NOT NULL DEFAULT 0,
    bonus_constitution INTEGER NOT NULL DEFAULT 0,
    bonus_intelligence INTEGER NOT NULL DEFAULT 0,
    bonus_sagesse      INTEGER NOT NULL DEFAULT 0,
    age_adulte         INTEGER NOT NULL DEFAULT 0,
    age_max            INTEGER NOT NULL DEFAULT 0,
    dee_de_mod         TEXT NOT NULL DEFAULT '1D4',
    taille_min         INTEGER NOT NULL DEFAULT 0,
    taille_max         INTEGER NOT NULL DEFAULT 0,
    taille_base        INTEGER NOT NULL DEFAULT 0,
    taille_facteur     INTEGER NOT NULL DEFAULT 0,
    poids_base         INTEGER NOT NULL DEFAULT 0,
    poids_facteur      INTEGER NOT NULL DEFAULT 0,
    vitesse_base       REAL NOT NULL DEFAULT 0,
    liste_langues      TEXT,
    image_path         TEXT DEFAULT 'placeholder.png',
    source             TEXT DEFAULT 'Player Handbook',
    source_ver         TEXT DEFAULT '5E',
    aide_joueur        TEXT,
    created_at         TEXT DEFAULT CURRENT_TIMESTAMP,
    updated_at         TEXT DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS race_presentations (
    id      TEXT PRIMARY KEY,
    race_id TEXT NOT NULL REFERENCES races(id) ON DELETE CASCADE,
    ordre   INTEGER NOT NULL DEFAULT -1,
    titre   TEXT DEFAULT '',
    texte   TEXT DEFAULT ''
);

CREATE TABLE IF NOT EXISTS race_traits (
    id      TEXT PRIMARY KEY,
    race_id TEXT NOT NULL REFERENCES races(id) ON DELETE CASCADE,
    ordre   INTEGER NOT NULL DEFAULT -1,
    titre   TEXT DEFAULT '',
    texte   TEXT DEFAULT ''
);

CREATE TABLE IF NOT EXISTS race_noms (
    id      TEXT PRIMARY KEY,
    race_id TEXT NOT NULL REFERENCES races(id) ON DELETE CASCADE,
    ordre   INTEGER NOT NULL DEFAULT -1,
    titre   TEXT DEFAULT '',
    texte   TEXT DEFAULT ''
);

CREATE TABLE IF NOT EXISTS race_sorts (
    id      TEXT PRIMARY KEY,
    race_id TEXT NOT NULL REFERENCES races(id) ON DELETE CASCADE,
    sort_id TEXT NOT NULL REFERENCES sorts(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS race_equipements (
    id            TEXT PRIMARY KEY,
    race_id       TEXT NOT NULL REFERENCES races(id) ON DELETE CASCADE,
    equipement_id TEXT NOT NULL REFERENCES equipements(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS race_langues (
    id                TEXT PRIMARY KEY,
    race_id           TEXT NOT NULL REFERENCES races(id) ON DELETE CASCADE,
    nom_langue        TEXT NOT NULL,
    typo_langue       TEXT NOT NULL,
    description_langue TEXT
);

-- ============================================================================
-- TABLE: CLASSES
-- ============================================================================
CREATE TABLE IF NOT EXISTS classes (
    id                        TEXT PRIMARY KEY,
    nom                       TEXT NOT NULL UNIQUE,
    classe_parente_id         TEXT REFERENCES classes(id),
    alias                     TEXT,
    description               TEXT,
    aide_joueur               TEXT,
    creation_classe           TEXT,
    creation_rapide           TEXT,
    dee_de_vie                INTEGER NOT NULL DEFAULT 8,
    points_de_vie             INTEGER NOT NULL DEFAULT 8,
    points_de_vie_par_niveau  INTEGER NOT NULL DEFAULT 4,
    equipement_maitrise       TEXT,   -- CSV de TEXT (refs vers equipements.id)
    jets_de_sauvegarde        TEXT,   -- CSV (ex: "FOR,CON")
    competences               TEXT,   -- CSV
    caracteristiques_de_sorts TEXT,
    image_path                TEXT DEFAULT 'placeholder.png'
);

CREATE TABLE IF NOT EXISTS classe_presentations (
    id        TEXT PRIMARY KEY,
    classe_id TEXT NOT NULL REFERENCES classes(id) ON DELETE CASCADE,
    ordre     INTEGER NOT NULL,
    titre     TEXT,
    texte     TEXT
);

CREATE TABLE IF NOT EXISTS classe_evolutions (
    id                TEXT PRIMARY KEY,
    classe_id         TEXT REFERENCES classes(id) ON DELETE CASCADE,
    choix_sous_classe INTEGER DEFAULT 0,
    niveau            INTEGER NOT NULL,
    bonus_maitrise    INTEGER,
    aptitudes_gagnees TEXT,
    col_1_titre TEXT, col_1_type INTEGER, col_1_nombre INTEGER, col_1_texte TEXT,
    col_2_titre TEXT, col_2_type INTEGER, col_2_nombre INTEGER, col_2_texte TEXT,
    col_3_titre TEXT, col_3_type INTEGER, col_3_nombre INTEGER, col_3_texte TEXT,
    col_4_titre TEXT, col_4_type INTEGER, col_4_nombre INTEGER, col_4_texte TEXT,
    col_5_titre TEXT, col_5_type INTEGER, col_5_nombre INTEGER, col_5_texte TEXT,
    sort_lvl_1 INTEGER DEFAULT 0,
    sort_lvl_2 INTEGER DEFAULT 0,
    sort_lvl_3 INTEGER DEFAULT 0,
    sort_lvl_4 INTEGER DEFAULT 0,
    sort_lvl_5 INTEGER DEFAULT 0,
    sort_lvl_6 INTEGER DEFAULT 0,
    sort_lvl_7 INTEGER DEFAULT 0,
    sort_lvl_8 INTEGER DEFAULT 0,
    sort_lvl_9 INTEGER DEFAULT 0
);

CREATE TABLE IF NOT EXISTS classe_aptitudes (
    id                  TEXT PRIMARY KEY,
    classe_id           TEXT REFERENCES classes(id) ON DELETE CASCADE,
    alias               TEXT,
    nom                 TEXT NOT NULL,
    description         TEXT NOT NULL,
    description_rapide  TEXT,
    niveau_acquisition  INTEGER NOT NULL,
    gain_de_sort        INTEGER DEFAULT 0,
    gain_de_competence  INTEGER DEFAULT 0,
    gain_de_vie         INTEGER DEFAULT 0,
    gain_de_maitrise    INTEGER DEFAULT 0
);

CREATE TABLE IF NOT EXISTS classe_choix_equipement (
    id            TEXT PRIMARY KEY,
    classe_id     TEXT REFERENCES classes(id) ON DELETE CASCADE,
    choix_num     INTEGER NOT NULL,
    equipement_id TEXT    -- CSV de refs vers equipements.id
);

-- ============================================================================
-- TABLE: HISTORIQUES
-- ============================================================================
CREATE TABLE IF NOT EXISTS historiques (
    id          TEXT PRIMARY KEY,
    nom         TEXT NOT NULL UNIQUE,
    alias       TEXT NOT NULL UNIQUE,
    description TEXT,
    competences TEXT,       -- CSV
    equipement_id TEXT      -- CSV de refs vers equipements.id
);

CREATE TABLE IF NOT EXISTS historique_traits (
    id            TEXT PRIMARY KEY,
    historique_id TEXT REFERENCES historiques(id) ON DELETE CASCADE,
    nom           TEXT NOT NULL,
    description   TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS historique_ideaux (
    id            TEXT PRIMARY KEY,
    historique_id TEXT REFERENCES historiques(id) ON DELETE CASCADE,
    nom           TEXT NOT NULL,
    description   TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS historique_liens (
    id            TEXT PRIMARY KEY,
    historique_id TEXT REFERENCES historiques(id) ON DELETE CASCADE,
    nom           TEXT NOT NULL,
    description   TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS historique_defauts (
    id            TEXT PRIMARY KEY,
    historique_id TEXT REFERENCES historiques(id) ON DELETE CASCADE,
    nom           TEXT NOT NULL,
    description   TEXT NOT NULL
);

-- ============================================================================
-- TABLE: PERSONNAGE_JOUEUR
-- ============================================================================
CREATE TABLE IF NOT EXISTS personnage_joueur (
    id                          TEXT PRIMARY KEY,
    nom                         TEXT NOT NULL UNIQUE,
    alias                       TEXT,
    race_id                     TEXT REFERENCES races(id),
    trait_racial_id             TEXT REFERENCES race_traits(id),
    classe_id                   TEXT REFERENCES classes(id),
    sous_classe_id              TEXT REFERENCES classes(id),
    classe_aptitudes_id         TEXT REFERENCES classe_aptitudes(id),
    historique_id               TEXT REFERENCES historiques(id),
    traits_de_personnalite      TEXT,
    ideaux                      TEXT,
    liens                       TEXT,
    defauts                     TEXT,
    autres_traits               TEXT,
    histoire                    TEXT,
    stuff_id                    TEXT,   -- CSV de refs vers equipements.id
    stuff                       TEXT,
    sorts_id                    TEXT,   -- CSV de refs vers sorts.id
    sorts                       TEXT,
    maitrise                    TEXT,   -- CSV
    carac_force                 INTEGER NOT NULL DEFAULT 10,
    carac_dexterite             INTEGER NOT NULL DEFAULT 10,
    carac_constitution          INTEGER NOT NULL DEFAULT 10,
    carac_intelligence          INTEGER NOT NULL DEFAULT 10,
    carac_sagesse               INTEGER NOT NULL DEFAULT 10,
    carac_charisme              INTEGER NOT NULL DEFAULT 10,
    sauvegarde_force            INTEGER DEFAULT 0,
    sauvegarde_dexterite        INTEGER DEFAULT 0,
    sauvegarde_constitution     INTEGER DEFAULT 0,
    sauvegarde_intelligence     INTEGER DEFAULT 0,
    sauvegarde_sagesse          INTEGER DEFAULT 0,
    sauvegarde_charisme         INTEGER DEFAULT 0,
    competence_athletisme       INTEGER DEFAULT 0,
    competence_acrobaties       INTEGER DEFAULT 0,
    competence_discretion       INTEGER DEFAULT 0,
    competence_escamotage       INTEGER DEFAULT 0,
    competence_arcane           INTEGER DEFAULT 0,
    competence_histoire         INTEGER DEFAULT 0,
    competence_investigation    INTEGER DEFAULT 0,
    competence_nature           INTEGER DEFAULT 0,
    competence_religion         INTEGER DEFAULT 0,
    competence_dressage         INTEGER DEFAULT 0,
    competence_intuition        INTEGER DEFAULT 0,
    competence_medicine         INTEGER DEFAULT 0,
    competence_perception       INTEGER DEFAULT 0,
    competence_survie           INTEGER DEFAULT 0,
    competence_intimidation     INTEGER DEFAULT 0,
    competence_persuasion       INTEGER DEFAULT 0,
    competence_representation   INTEGER DEFAULT 0,
    competence_tromperie        INTEGER DEFAULT 0,
    niveau                      INTEGER NOT NULL DEFAULT 1,
    experience                  INTEGER NOT NULL DEFAULT 0,
    alignement                  TEXT,
    personnalite                TEXT,
    sexe                        TEXT,
    vitesse                     REAL NOT NULL DEFAULT 7.5,
    age                         INTEGER,
    taille                      INTEGER,
    poids                       INTEGER,
    descriptions                TEXT,
    image_path                  TEXT DEFAULT 'placeholder.png'
);
