-- ============================================================================
-- NOUVEAU SCHÉMA POUR LES RACES D&D 5E
-- Schéma relationnel normalisé basé sur les spécifications du TODO.md
-- ============================================================================

-- Enable UUID extension (au cas où)
CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

-- ============================================================================
-- TABLE: RACES
-- ============================================================================
CREATE TABLE IF NOT EXISTS races (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(), 
    nom VARCHAR(100) NOT NULL UNIQUE,
    race_parent_id UUID REFERENCES races(id) DEFAULT NULL, -- NULL si race parente, sinon référence
    alias VARCHAR(100) DEFAULT '', -- Nom alternatif pour pseudo markdown CamelCase du nom (ex: "HalfElf" pour "Half-Elf")
    -- Description générale
    description TEXT,
    aide_joueur TEXT, -- Conseils pour les joueurs

    -- Traits raciaux — bonus de caractéristiques
    bonus_forces       INTEGER DEFAULT 0,
    bonus_charisme     INTEGER DEFAULT 0,
    bonus_dexterite    INTEGER DEFAULT 0,
    bonus_constitution INTEGER DEFAULT 0,
    bonus_intelligence INTEGER DEFAULT 0,
    bonus_sagesse      INTEGER DEFAULT 0,

    -- Âge
    age_adulte         INTEGER DEFAULT 0,
    age_max            INTEGER DEFAULT 0,

    -- Taille
    taille_min         INTEGER DEFAULT 0,
    taille_max         INTEGER DEFAULT 0,
    taille_base        INTEGER DEFAULT 0,
    taille_facteur     INTEGER DEFAULT 0,  -- Facteur pour calculer la taille à partir de taille_base + dee_de_mod
    dee_de_mod         VARCHAR(20) DEFAULT '1D2', -- Ex: "2D10"

    -- Poids
    poids_base         INTEGER DEFAULT 0,
    poids_facteur      INTEGER DEFAULT 0, -- Facteur pour calculer le poids à partir de poids_base

    -- Vitesse
    vitesse_base       INTEGER DEFAULT 30,

    -- Sortilèges & équipement raciaux
    liste_langues    TEXT                          , -- Noms des langues connues (ex: "Commun, Elfe, Nain")
    liste_sortileges UUID[] DEFAULT ARRAY[]::UUID[], -- IDs vers la table sortileges (TODO: migrer vers table de liaison)
    liste_equipement UUID[] DEFAULT ARRAY[]::UUID[], -- IDs vers la table equipement  (TODO: migrer vers table de liaison)

    -- Image (chemin relatif depuis assets/images/, ex: 'races/human.png')
    -- Source: Wizards of the Coast — usage personnel uniquement (voir assets/images/COPYRIGHT.md)
    image_path TEXT DEFAULT 'placeholder.png',

    -- Méta
    source     VARCHAR(100) DEFAULT 'Player Handbook',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- ============================================================================
-- TABLE: RACE_PRESENTATIONS
-- Paragraphes de présentation d'une race (remplace pres_1..10)
-- ============================================================================
CREATE TABLE IF NOT EXISTS race_presentations (
    id      UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    race_id UUID NOT NULL REFERENCES races(id) ON DELETE CASCADE,
    ordre   INTEGER NOT NULL DEFAULT -1, -- Ordre d'affichage
    titre   VARCHAR(100)     DEFAULT '', -- Ex: "Apparence", "Société", "Relations avec
    texte   TEXT             DEFAULT ''  -- Texte de présentation
);

-- ============================================================================
-- TABLE: RACE_TRAITS
-- Traits raciaux d'une race (remplace trait_1..10)
-- ============================================================================
CREATE TABLE IF NOT EXISTS race_traits (
    id      UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    race_id UUID NOT NULL REFERENCES races(id) ON DELETE CASCADE,
    ordre   INTEGER NOT NULL DEFAULT -1, -- Ordre d'affichage
    titre   VARCHAR(100)     DEFAULT '', -- Ex: "Apparence", "Société", "Relations avec"
    texte   TEXT             DEFAULT ''  -- Texte de présentation
);

-- ============================================================================
-- TABLE: RACE_NOMS
-- Sections de noms de personnages d'une race (remplace pres_nom + nom_1..4)
-- ============================================================================
CREATE TABLE IF NOT EXISTS race_noms (
    id      UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    race_id UUID NOT NULL REFERENCES races(id) ON DELETE CASCADE,
    ordre   INTEGER NOT NULL DEFAULT -1, -- Ordre d'affichage
    titre   VARCHAR(100)     DEFAULT '', -- Ex: "Prénoms masculins", "Noms de famille"
    texte   TEXT             DEFAULT ''  -- Exemples séparés par virgules
);