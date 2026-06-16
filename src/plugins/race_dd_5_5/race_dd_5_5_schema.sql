-- Schéma SQLite du plugin race_dd_5_5
-- Races D&D 5e — Player's Handbook (VF)

CREATE TABLE IF NOT EXISTS races (
    id                 TEXT PRIMARY KEY,
    race_parent_id     TEXT REFERENCES races(id),
    nom                TEXT NOT NULL UNIQUE,
    alias              TEXT NOT NULL UNIQUE,
    description        TEXT DEFAULT '',
    aide_joueur        TEXT DEFAULT '',
    bonus_forces       INTEGER DEFAULT 0,
    bonus_dexterite    INTEGER DEFAULT 0,
    bonus_constitution INTEGER DEFAULT 0,
    bonus_intelligence INTEGER DEFAULT 0,
    bonus_sagesse      INTEGER DEFAULT 0,
    bonus_charisme     INTEGER DEFAULT 0,
    age_adulte         INTEGER DEFAULT 0,
    age_max            INTEGER DEFAULT 0,
    dee_de_mod         TEXT DEFAULT '',
    taille_min         INTEGER DEFAULT 0,
    taille_max         INTEGER DEFAULT 0,
    taille_base        INTEGER DEFAULT 0,
    taille_facteur     INTEGER DEFAULT 0,
    poids_base         INTEGER DEFAULT 0,
    poids_facteur      INTEGER DEFAULT 0,
    vitesse_base       REAL DEFAULT 0,
    liste_langues      TEXT DEFAULT '',
    image_path         TEXT DEFAULT 'placeholder.png',
    source             TEXT DEFAULT 'Player Handbook',
    source_ver         TEXT DEFAULT '5E'
);

CREATE TABLE IF NOT EXISTS race_presentations (
    id      TEXT PRIMARY KEY,
    race_id TEXT NOT NULL REFERENCES races(id) ON DELETE CASCADE,
    ordre   INTEGER DEFAULT -1,
    titre   TEXT DEFAULT '',
    texte   TEXT DEFAULT ''
);

CREATE TABLE IF NOT EXISTS race_traits (
    id      TEXT PRIMARY KEY,
    race_id TEXT NOT NULL REFERENCES races(id) ON DELETE CASCADE,
    ordre   INTEGER DEFAULT -1,
    titre   TEXT DEFAULT '',
    texte   TEXT DEFAULT ''
);

CREATE TABLE IF NOT EXISTS race_noms (
    id      TEXT PRIMARY KEY,
    race_id TEXT NOT NULL REFERENCES races(id) ON DELETE CASCADE,
    ordre   INTEGER DEFAULT -1,
    titre   TEXT DEFAULT '',
    texte   TEXT DEFAULT ''
);
