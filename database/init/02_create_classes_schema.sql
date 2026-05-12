-- -- ============================================================================
-- NOUVEAU SCHÉMA POUR LES CLASSES D&D 5E
-- Schéma relationnel normalisé basé sur les spécifications du TODO.md
-- ============================================================================

-- Enable UUID extension (au cas où)
CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

-- ============================================================================
-- TABLE: CLASS
-- Structure complète pour les classes avec toutes les métadonnées
-- ============================================================================

CREATE TABLE IF NOT EXISTS classes (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    nom VARCHAR(100) NOT NULL UNIQUE,
    classe_parente_id UUID REFERENCES classes(id), -- NULL si classe parente, sinon référence
    alias VARCHAR(100), -- Nom alternatif pour pseudo markdown

    -- Description générale
    description TEXT,
    aide_joueur TEXT, -- Conseils pour les joueurs

    creation_classe TEXT, -- Comment créer une classe
    creation_rapide TEXT, -- Comment créer une classe rapidement

    -- Aptitudes de classe
    -- Vie
    dee_de_vie INTEGER NOT NULL, -- D8, D10, etc.
    points_de_vie INTEGER NOT NULL, -- Points de vie à 1er niveau
    points_de_vie_par_niveau INTEGER NOT NULL, -- Points de vie gagnés par
    -- Maitrises
    armures             UUID[],  -- Liste d'armures séparées par virgules
    arme                UUID[],  -- Liste d'armes séparées par virgules
    outils              UUID[],  -- Liste d'outils séparées par virgules
    jets_de_sauvegarde  UUID[],  -- Liste de jets de sauvegarde séparés par virgules
    competences         UUID[],  -- Liste de compétences séparées par virgules
    -- Equipement
    equipement_de_base_1 UUID[], -- Liste d'équipement de base séparés par virgules
    equipement_de_base_2 UUID[], -- Liste d'équipement de base séparés par virgules
    equipement_de_base_3 UUID[], -- Liste d'équipement de base séparés par virgules
    equipement_de_base_4 UUID[], -- Liste d'équipement de base séparés par virgules
    -- Sorts
    caracteristiques_de_sorts TEXT, -- Caractéristiques de sorts (ex: "Intelligence", "Sagesse", etc.)

    -- Image
    image_path TEXT
);

-- ============================================================================
-- TABLE: CLASSE_PRESENTATIONS
-- Paragraphes de présentation d'une classe (remplace pres_1..10)
-- ============================================================================
CREATE TABLE IF NOT EXISTS classe_presentations (
    id       UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    classe_id UUID NOT NULL REFERENCES classes(id) ON DELETE CASCADE,
    ordre    INTEGER NOT NULL, -- Ordre d'affichage
    titre    VARCHAR(100),
    texte    TEXT
);

-- ===========================================================================
-- TABLE: EVOLUTION
-- Structure pour les évolutions de classe (sous-classes) avec référence à la classe
-- ===========================================================================

CREATE TABLE IF NOT EXISTS classe_evolutions (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    classe_id UUID REFERENCES classes(id) ON DELETE CASCADE, -- Référence à la classe d'évolution
    
    niveau INTEGER NOT NULL, -- Niveau auquel l'évolution est acquise
    bonus_maîtrise INTEGER, -- Bonus de maîtrise acquis à ce niveau
    aptitudes_gagnees TEXT, -- Liste d'aptitudes gagnées à ce niveau séparées par virgules
    
    -- Différent colonnes pour les différentes aptitudes gagnées à ce niveau (ex: "1 aptitude de classe", "2 compétences", etc.)
    -- Nom de la colonne        type dans la col      nombre dans la col      texte dans la col 
    --                           0 pour Nombre
    --                           1 pour Texte
    col_1_titre VARCHAR(100),   col_1_type BOOLEAN,   col_1_nombre INTEGER,   col_1_texte TEXT,
    col_2_titre VARCHAR(100),   col_1_type BOOLEAN,   col_2_nombre INTEGER,   col_2_texte TEXT,
    col_3_titre VARCHAR(100),   col_1_type BOOLEAN,   col_3_nombre INTEGER,   col_3_texte TEXT,
    col_4_titre VARCHAR(100),   col_1_type BOOLEAN,   col_4_nombre INTEGER,   col_4_texte TEXT,
    col_5_titre VARCHAR(100),   col_1_type BOOLEAN,   col_5_nombre INTEGER,   col_5_texte TEXT,
    -- Sorts acquis à ce niveau (nombre de sorts de chaque niveau acquis à ce niveau)

    sort_lvl_1 INTEGER DEFAULT 0, -- Nombre de sorts de niveau 1 acquis à ce niveau
    sort_lvl_2 INTEGER DEFAULT 0, -- Nombre de sorts de niveau 2 acquis à ce niveau
    sort_lvl_3 INTEGER DEFAULT 0, -- Nombre de sorts de niveau 3 acquis à ce niveau
    sort_lvl_4 INTEGER DEFAULT 0, -- Nombre de sorts de niveau 4 acquis à ce niveau
    sort_lvl_5 INTEGER DEFAULT 0, -- Nombre de sorts de niveau 5 acquis à ce niveau
    sort_lvl_6 INTEGER DEFAULT 0, -- Nombre de sorts de niveau 6 acquis à ce niveau
    sort_lvl_7 INTEGER DEFAULT 0, -- Nombre de sorts de niveau 7 acquis à ce niveau
    sort_lvl_8 INTEGER DEFAULT 0, -- Nombre de sorts de niveau 8 acquis à ce niveau
    sort_lvl_9 INTEGER DEFAULT 0  -- Nombre de sorts de niveau 9 acquis à ce niveau
);

-- ===========================================================================
-- TABLE: APTITUDE
-- Structure pour les aptitudes de classe avec référence à la classe
-- ===========================================================================

CREATE TABLE IF NOT EXISTS classe_aptitudes (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    classe_id UUID REFERENCES classes(id) ON DELETE CASCADE, -- Référence à la classe
    nom VARCHAR(100) NOT NULL, -- Nom de l'aptitude
    description TEXT NOT NULL, -- Description de l'aptitude
    description_rapide TEXT, -- Description rapide de l'aptitude pour les tableaux de progression
    niveau_acquisition INTEGER NOT NULL -- Niveau auquel l'aptitude est acquise
);
