-- ============================================================================
-- NOUVEAU SCHÉMA POUR LES RACES D&D 5E
-- Schéma relationnel normalisé basé sur les spécifications du TODO.md
-- ============================================================================

-- Enable UUID extension (au cas où)
CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

-- ============================================================================
-- TABLE: RACE (nouvelle version)
-- Structure complète pour les races avec toutes les métadonnées
-- ============================================================================
CREATE TABLE IF NOT EXISTS races (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    nom VARCHAR(100) NOT NULL UNIQUE,
    race_parent_id UUID REFERENCES races(id), -- NULL si race parente, sinon référence

    -- Description générale
    description TEXT,
    aide_joueur TEXT, -- Conseils pour les joueurs

    -- Présentation paragraphe
    pres_1_titre VARCHAR(100),
    pres_1_texte TEXT,
    pres_2_titre VARCHAR(100),
    pres_2_texte TEXT,
    pres_3_titre VARCHAR(100),
    pres_3_texte TEXT,
    pres_4_titre VARCHAR(100),
    pres_4_texte TEXT,
    pres_5_titre VARCHAR(100),
    pres_5_texte TEXT,
    pres_6_titre VARCHAR(100),
    pres_6_texte TEXT,
    pres_7_titre VARCHAR(100),
    pres_7_texte TEXT,
    pres_8_titre VARCHAR(100),
    pres_8_texte TEXT,
    pres_9_titre VARCHAR(100),
    pres_9_texte TEXT,
    pres_10_titre VARCHAR(100),
    pres_10_texte TEXT,

    -- Noms de personnages
    pres_nom_titre VARCHAR(100),
    pres_nom_texte TEXT,
    nom_1_titre VARCHAR(100),
    nom_1-texte TEXT, -- Exemples séparés par virgules
    nom_2_titre VARCHAR(100),
    nom_2-texte TEXT,
    nom_3_titre VARCHAR(100),
    nom_3-texte TEXT,
    nom_4_titre VARCHAR(100),
    nom_4-texte TEXT,

    -- Traits raciaux

    bonus_forces INTEGER DEFAULT 0,
    bonus_charisme INTEGER DEFAULT 0,
    bonus_dexterite INTEGER DEFAULT 0,
    bonus_constitution INTEGER DEFAULT 0,
    bonus_intelligence INTEGER DEFAULT 0,
    bonus_sagesse INTEGER DEFAULT 0,
    age_adulte INTEGER,
    age_max INTEGER,
    taille_min INTEGER,
    taille_max INTEGER,
    vitesse_base INTEGER DEFAULT 30,

    trait_1_titre VARCHAR(100),
    trait_1_texte TEXT,
    trait_2_titre VARCHAR(100),
    trait_2_texte TEXT, 
    trait_3_titre VARCHAR(100),
    trait_3_texte TEXT,
    trait_4_titre VARCHAR(100),
    trait_4_texte TEXT,
    trait_5_titre VARCHAR(100),
    trait_5_texte TEXT,
    trait_6_titre VARCHAR(100),
    trait_6_texte TEXT,
    trait_7_titre VARCHAR(100),
    trait_7_texte TEXT,
    trait_8_titre VARCHAR(100),
    trait_8_texte TEXT,
    trait_9_titre VARCHAR(100),
    trait_9_texte TEXT,
    trait_10_titre VARCHAR(100),
    trait_10_texte TEXT,

    -- Sortilèges raciaux
    liste_sortileges REFERENCES sortileges(id), -- Référence à une table de sortilèges

    -- Méta
    source VARCHAR(100) DEFAULT 'Player Handbook',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);