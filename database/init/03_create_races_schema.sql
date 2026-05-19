-- ============================================================================
-- Fichier: 03_create_races_schema.sql
-- Description: Création de la table 
--  - races                 : pour les différentes races et sous races
--  - race_presentations    : chaque paragraphe de présentation de la race, 
--                            surtout là pour plus de souplesse sur les descritpion de race
--  - race_traits           : pour pouvoir définir tout les trait raciaux. Les plus commun 
--                            seront strandartisé comme les bonus de caractéristiques ou la 
--                            vitesse. On laisse quand même le race trait de ces traits raciaux.
--  - race_noms             : Comme les nom c'est le bordel on fait une table de nom c'est 
--                            plus simple et plus modulaire.
--  - race_equipements      : Si jamais une race à des aptitude d'équipement 
--  - race_sorts            : Si jamais une race à des sort inné.

-- Auteur: MATHURIN CHAMPÉMONT
-- Date: 2026_05-16

-- HACK : Mise en page TURBO dégeux.
-- ============================================================================

-- Enable UUID extension (au cas où)
CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

-- ============================================================================
-- TABLE: RACES

-- Attention la table possède le champs alias. Vu la taille de de table en faire un champs IsOver semble pas con car je pense qu'il sera dur dans faire un résumé simple. 
-- TODO : IsOver() : Voir pour faire un champs dédié au problème de IsOver et à la complexité de la tache
-- TODO : IsClick() : Elle doit avoire son propre Render. Je me demande si en faite c'est pertinent de lui mettre un alias. On verra.  
-- HACK : Les langue sont en dure mais les langue auront des informations propre il leur faudra une table. Pour le moment ça passe.

-- ============================================================================
CREATE TABLE IF NOT EXISTS races (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(), 
    nom VARCHAR(100) NOT NULL UNIQUE,
    race_parent_id UUID REFERENCES races(id) DEFAULT NULL, -- NULL si race parente, sinon référence
    alias VARCHAR(100) NOT NULL UNIQUE, -- Nom alternatif pour pseudo markdown CamelCase du nom (ex: "HalfElf" pour "Half-Elf")
    -- Description générale
    description TEXT,

    -- Traits raciaux — bonus de caractéristiques
    bonus_forces       INTEGER DEFAULT 0 NOT NULL,
    bonus_charisme     INTEGER DEFAULT 0 NOT NULL,
    bonus_dexterite    INTEGER DEFAULT 0 NOT NULL,
    bonus_constitution INTEGER DEFAULT 0 NOT NULL,
    bonus_intelligence INTEGER DEFAULT 0 NOT NULL,
    bonus_sagesse      INTEGER DEFAULT 0 NOT NULL,

    -- Âge
    age_adulte         INTEGER DEFAULT 0 NOT NULL,
    age_max            INTEGER DEFAULT 0 NOT NULL,

    -- Personnalisaiton
    dee_de_mod         VARCHAR(20) NOT NULL, -- Ex: "2D10"

    -- Taille
    taille_min         INTEGER DEFAULT 0 NOT NULL,
    taille_max         INTEGER DEFAULT 0 NOT NULL,
    taille_base        INTEGER DEFAULT 0 NOT NULL,
    taille_facteur     INTEGER DEFAULT 0 NOT NULL,  -- Facteur pour calculer la taille à partir de taille_base + dee_de_mod * taille_facteur

    -- Poids
    poids_base         INTEGER DEFAULT 0 NOT NULL,
    poids_facteur      INTEGER DEFAULT 0 NOT NULL, -- Facteur pour calculer le poids à partir de poids_base + dee_de_mod * poids_facteur

    -- Vitesse
    vitesse_base       FLOAT   DEFAULT 0 NOT NULL, -- Vitesse en mètre.

    -- Langue
    liste_langues      TEXT  , -- Noms des langues connues (ex: "Commun, Elfe, Nain")

    -- Image (chemin relatif depuis assets/images/, ex: 'races/human.png')
    -- Source: Wizards of the Coast — usage personnel uniquement (voir assets/images/COPYRIGHT.md)
    image_path TEXT DEFAULT 'placeholder.png',

    -- Méta
    source     VARCHAR(100) DEFAULT 'Player Handbook',
    source_ver VARCHAR(5)   DEFAULT '5E' 
    created_at TIMESTAMP    DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP    DEFAULT CURRENT_TIMESTAMP
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

-- ============================================================================
-- TABLE: RACE_SORTS
-- Sorts raciaux d'une race connu
-- ============================================================================
CREATE TABLE IF NOT EXISTS race_sorts (
    id              UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    race_id         UUID NOT NULL REFERENCES races(id) ON DELETE CASCADE,
    sort_id         UUID NOT NULL REFERENCES sorts(id) ON DELETE CASCADE
);

-- ============================================================================
-- TABLE: RACE_EQUIPEMENTS
-- Equipement raciaux maitrisé 
-- ============================================================================
CREATE TABLE IF NOT EXISTS race_equipements (
    id              UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    race_id         UUID NOT NULL REFERENCES races(id) ON DELETE CASCADE,
    equipement_id   UUID NOT NULL REFERENCES equipements(id) ON DELETE CASCADE
);

-- ===========================================================================
-- TABLE: RACE_LANGUES
-- Langues raciales maitrisé 
-- ============================================================================
CREATE TABLE IF NOT EXISTS race_langues (
    id              UUID PRIMARY KEY DEFAULT uuid_generate_v4(),    
    race_id         UUID NOT NULL REFERENCES races(id) ON DELETE CASCADE,
    nom_langue      VARCHAR(100) NOT NULL -- Ex: "Commun", "Elfe", "Nain"
    typo_langue     VARCHAR(50) NOT NULL -- Ex: "Standard", "Exotique", etc.
    description_langue TEXT -- Description de la langue (ex: "Langue commune utilisée par la plupart des habitants du monde", "Langue mystérieuse parlée par les elfes, avec une grammaire complexe et un vocabulaire riche en poésie", etc.)*
);
