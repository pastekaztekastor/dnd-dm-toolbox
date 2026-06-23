-- ============================================================================
-- SCHÉMA SQLite : classe_manager  (v2)
-- ============================================================================
-- ORDRE D'INSERTION OBLIGATOIRE :
--   1. classes
--   2. classe_evolution_colonnes
--   3. classe_presentations
--   4. classe_evolutions
--   5. classe_evolution_valeurs (référence classe_evolutions + classe_evolution_colonnes)
--   6. classe_choix_champ
--   7. classe_choix_valeur
--   8. classe_aptitudes       (référence choix_champ_id / choix_valeur_id)
--   9. classe_choix_equipement
-- ============================================================================

CREATE TABLE IF NOT EXISTS classes (
    id                        TEXT PRIMARY KEY,                -- Identification de la classe (ex : barbare, paladin, voleur)
    nom                       TEXT NOT NULL UNIQUE,            -- Nom de la classe (ex : Barbare, Paladin, Voleur)
    classe_parente_id         TEXT REFERENCES classes(id),     -- Classe parente si c'est une classe qui dépende d'une autre (ex : barbare_voie_primitive dépend de barbare)
    alias                     TEXT,                            -- C'est ce qui sert pour le linker MD, généralement le nom de la classe en camelCase (ex : barbare, paladin, voleur)
    description               TEXT,                            -- Description complette au format MD
    aide_joueur               TEXT,                            -- Description courte au format MD pour le over mouse
    creation_classe           TEXT,                            -- Pargraphe pour la création de classe
    creation_rapide           TEXT,                            -- Paragraphe pour le création rapide de classe
    dee_de_vie                INTEGER NOT NULL DEFAULT 1,      -- Valeur du dé de vie (1d12 donne 12)
    points_de_vie_lvl_1       INTEGER NOT NULL DEFAULT 1,      -- Point de vie au niveau 1
    points_de_vie_par_niveau  INTEGER NOT NULL DEFAULT 1,      -- Point de vie par niveau supplémentaire
    equipement_maitrise       TEXT DEFAULT '[]',               -- Format JSON : liste des types d'équipement que la classe maîtrise (ex : ["armes_lourdes", "armures_lourdes"])
    jets_de_sauvegarde        TEXT DEFAULT '[]',               -- Format JSON : liste des jets de sauvegarde que la classe maîtrise (ex : ["force", "constitution"])
    competences               TEXT DEFAULT '[]',               -- Format JSON : liste des compétences que la classe maîtrise (ex : ["athletisme", "perception"])
    caracteristiques_de_sorts TEXT,                            -- La caractéristique pour lancer un sort
    image_path                TEXT DEFAULT 'placeholder.png',  -- Le chemin de l'image de la classe
    source                    TEXT,                            -- L'origine de la classe (ex : PHB, TCE, etc.)
    source_ver                TEXT,                            -- La version de l'origine (ex : 5.5.1 pour le PHB 5.5 première version)
    utilise_magie             BOOLEAN NOT NULL DEFAULT 1       -- Si la classe est une lanceuse de sort ou non pour ne pas affiché les colonne liée au sort dans le tableau d'évolution de classe.
);

-- ============================================================================
-- TABLE: classe_evolution_colonnes
-- ============================================================================
-- Définition des colonnes du tableau d'évolution d'une classe (nombre illimité,
-- remplace l'ancien plafond à 5 colonnes figées dans `classes`).
-- ============================================================================
CREATE TABLE IF NOT EXISTS classe_evolution_colonnes (
    id          TEXT PRIMARY KEY,
    classe_id   TEXT NOT NULL REFERENCES classes(id) ON DELETE CASCADE,
    ordre       INTEGER NOT NULL,
    titre       TEXT NOT NULL,
    titre_court TEXT,
    type        INTEGER NOT NULL DEFAULT 0,  -- 0=texte, 1=nombre
    UNIQUE(classe_id, ordre)
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
-- TABLE: classe_evolution_valeurs
-- ============================================================================
-- Valeur d'une ligne d'évolution pour une colonne dynamique donnée
-- (cf. classe_evolution_colonnes). Une seule des deux colonnes valeur_*
-- est renseignée selon le `type` de la colonne (0=texte, 1=nombre).
-- ============================================================================
CREATE TABLE IF NOT EXISTS classe_evolution_valeurs (
    id            TEXT PRIMARY KEY,
    evolution_id  TEXT NOT NULL REFERENCES classe_evolutions(id) ON DELETE CASCADE,
    colonne_id    TEXT NOT NULL REFERENCES classe_evolution_colonnes(id) ON DELETE CASCADE,
    valeur_nombre INTEGER,
    valeur_texte  TEXT,
    UNIQUE(evolution_id, colonne_id)
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
    aptitude_prerequisite_id TEXT,   -- auto-référence TEXT sans FK (ordre insert)
    version_aptitude         INTEGER NOT NULL DEFAULT 1 -- Si une aptitude évolue on a juste une nouvel version. Exemple : critique brutal 1, critique brutal 2, critique brutal 3. On garde le même id mais on change la version.
);

CREATE TABLE IF NOT EXISTS classe_choix_equipement (
    id          TEXT PRIMARY KEY,
    classe_id   TEXT NOT NULL REFERENCES classes(id) ON DELETE CASCADE,
    choix_num   INTEGER NOT NULL,
    option_num  INTEGER NOT NULL,
    description TEXT NOT NULL, -- Description de l'option (ex : "Cotte de mailles, Bouclier") Pour affichage. Comme l'alias c'est le nom de l'item en Camel case on peux aisément en faire une liste de liens clicable en MD. 
);
