-- ============================================================================
-- SCHÉMA SQLite : classe_manager  (v2)
-- ============================================================================
-- ORDRE D'INSERTION OBLIGATOIRE :
--   1. classes
--   2. classe_presentations
--   3. classe_evolutions
--   4. classe_choix_champ
--   5. classe_choix_valeur
--   6. classe_aptitudes       (référence choix_champ_id / choix_valeur_id)
--   7. classe_choix_equipement
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
    dee_de_vie                INTEGER NOT NULL DEFAULT 6,
    points_de_vie             INTEGER NOT NULL DEFAULT 6,
    points_de_vie_par_niveau  INTEGER NOT NULL DEFAULT 4,
    equipement_maitrise       TEXT DEFAULT '[]',
    jets_de_sauvegarde        TEXT DEFAULT '[]',
    competences               TEXT DEFAULT '[]',
    caracteristiques_de_sorts TEXT,
    image_path                TEXT DEFAULT 'placeholder.png',
    source                    TEXT,
    source_ver                TEXT
);

CREATE TABLE IF NOT EXISTS classe_presentations (
    id        TEXT PRIMARY KEY,
    classe_id TEXT NOT NULL REFERENCES classes(id) ON DELETE CASCADE,
    ordre     INTEGER NOT NULL DEFAULT 0,
    titre     TEXT,
    texte     TEXT
);

CREATE TABLE IF NOT EXISTS classe_evolutions (
    id                TEXT PRIMARY KEY,
    classe_id         TEXT NOT NULL REFERENCES classes(id) ON DELETE CASCADE,
    niveau            INTEGER NOT NULL,
    bonus_maitrise    INTEGER NOT NULL DEFAULT 2,
    choix_sous_classe INTEGER NOT NULL DEFAULT 0,
    aptitudes_gagnees TEXT,
    col_1_titre TEXT, col_1_type INTEGER DEFAULT 0, col_1_nombre INTEGER, col_1_texte TEXT,
    col_2_titre TEXT, col_2_type INTEGER DEFAULT 0, col_2_nombre INTEGER, col_2_texte TEXT,
    col_3_titre TEXT, col_3_type INTEGER DEFAULT 0, col_3_nombre INTEGER, col_3_texte TEXT,
    col_4_titre TEXT, col_4_type INTEGER DEFAULT 0, col_4_nombre INTEGER, col_4_texte TEXT,
    col_5_titre TEXT, col_5_type INTEGER DEFAULT 0, col_5_nombre INTEGER, col_5_texte TEXT,
    sort_lvl_1 INTEGER NOT NULL DEFAULT 0,
    sort_lvl_2 INTEGER NOT NULL DEFAULT 0,
    sort_lvl_3 INTEGER NOT NULL DEFAULT 0,
    sort_lvl_4 INTEGER NOT NULL DEFAULT 0,
    sort_lvl_5 INTEGER NOT NULL DEFAULT 0,
    sort_lvl_6 INTEGER NOT NULL DEFAULT 0,
    sort_lvl_7 INTEGER NOT NULL DEFAULT 0,
    sort_lvl_8 INTEGER NOT NULL DEFAULT 0,
    sort_lvl_9 INTEGER NOT NULL DEFAULT 0,
    UNIQUE(classe_id, niveau)
);

-- ============================================================================
-- TABLE: classe_choix_champ
-- ============================================================================
-- Un "champ de choix" = une décision de build que le joueur doit prendre.
-- Appartient toujours à la CLASSE DE BASE (même si déclenché par une sous-classe).
--
-- EXEMPLE barbare :
--   id='barbare_champ_voie'    | 'Voie primitive'    | alias='voie_primitive'   | niveau=3
--   id='barbare_champ_totem'   | 'Totem spirituel'   | alias='totem'            | niveau=3
--   id='barbare_champ_attuning'| 'Attuning totémique'| alias='totem_attuning'   | niveau=14
--
-- Le lien vers l'aptitude qui déclenche le choix est résolu via
--   SELECT id FROM classe_aptitudes WHERE choix_champ_id = ?
-- (inverse lookup, pas de FK pour éviter les dépendances circulaires)
-- ============================================================================
CREATE TABLE IF NOT EXISTS classe_choix_champ (
    id          TEXT PRIMARY KEY,
    classe_id   TEXT NOT NULL REFERENCES classes(id) ON DELETE CASCADE,
    nom         TEXT NOT NULL,
    alias       TEXT NOT NULL,
    niveau      INTEGER NOT NULL DEFAULT 1,
    description TEXT,
    UNIQUE(classe_id, alias)
);

-- ============================================================================
-- TABLE: classe_choix_valeur
-- ============================================================================
-- Options disponibles pour un champ de choix.
-- CASCADE : si on supprime le champ, ses valeurs disparaissent.
-- ============================================================================
CREATE TABLE IF NOT EXISTS classe_choix_valeur (
    id          TEXT PRIMARY KEY,
    champ_id    TEXT NOT NULL REFERENCES classe_choix_champ(id) ON DELETE CASCADE,
    nom         TEXT NOT NULL,
    valeur      TEXT NOT NULL,
    description TEXT,
    UNIQUE(champ_id, valeur)
);

-- ============================================================================
-- TABLE: classe_aptitudes
-- ============================================================================
-- Deux types de liaison avec le système de choix :
--
--   choix_champ_id          → cette aptitude DÉCLENCHE un choix joueur
--                             (ex : barbare_voie_primitive, paladin_style_combat)
--
--   choix_valeur_id         → cette aptitude n'est accessible QUE si le joueur
--                             a fait ce choix précis
--                             (ex : totem_spirituel_aigle n'existe que si totem=aigle)
--
--   aptitude_prerequisite_id→ cette aptitude requiert une autre aptitude au préalable
--                             (ex : critique_brutal_2 requiert critique_brutal_1)
-- ============================================================================
CREATE TABLE IF NOT EXISTS classe_aptitudes (
    id                       TEXT PRIMARY KEY,
    classe_id                TEXT NOT NULL REFERENCES classes(id) ON DELETE CASCADE,
    alias                    TEXT,
    nom                      TEXT NOT NULL,
    description              TEXT NOT NULL DEFAULT '',
    description_rapide       TEXT,
    niveau_acquisition       INTEGER NOT NULL DEFAULT 1,
    choix_champ_id           TEXT,   -- TEXT sans FK pour éviter dépendances circulaires
    choix_valeur_id          TEXT,   -- TEXT sans FK (champ peut être d'une autre classe)
    aptitude_prerequisite_id TEXT    -- auto-référence TEXT sans FK (ordre insert)
);

CREATE TABLE IF NOT EXISTS classe_choix_equipement (
    id          TEXT PRIMARY KEY,
    classe_id   TEXT NOT NULL REFERENCES classes(id) ON DELETE CASCADE,
    choix_num   INTEGER NOT NULL,
    option_num  INTEGER NOT NULL,
    description TEXT
);
