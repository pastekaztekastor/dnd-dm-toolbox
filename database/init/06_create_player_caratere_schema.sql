-- -- ============================================================================
-- NOUVEAU SCHÉMA POUR LES CLASSES D&D 5E
-- ============================================================================

-- Enable UUID extension (au cas où)
CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

-- ============================================================================
-- TABLE: PERSONNAGE_JOUEUR
-- Structure complète pour les personnages-joueurs avec toutes les métadonnées
-- ============================================================================

CREATE TABLE IF NOT EXISTS personnage_joueur (
    id                      UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    nom                     VARCHAR(100) NOT NULL UNIQUE,           -- Nom du personnage
    alias                   VARCHAR(100),                           -- Nom alternatif pour pseudo markdown

    -- RACE         
    race_id                 UUID REFERENCES races(id),              -- Référence à la race du personnage ou sous race
    trait_racial_id         UUID REFERENCES race_traits(id),        -- Référence à un trait racial spécifique

    -- CLASSE       
    classe_id               UUID REFERENCES classes(id),            -- Référence à la classe du personnage ou sous classe
    sous_classe_id          UUID REFERENCES classes(id),            -- Référence à la sous-classe du personnage
    classe_aptitudes_id     UUID REFERENCES classe_aptitudes(id),   -- Référence à un trait de classe spécifique

    -- HISTORIQUE
    historique_id           UUID REFERENCES historiques(id),        -- Référence à l'historique du personnage
    traits_de_personnalite  TEXT,                                   -- Traits de personnalité
    ideaux                  TEXT,                                   -- Idéaux
    liens                   TEXT,                                   -- Liens
    defauts                 TEXT,                                   -- Défauts
    autres_traits           TEXT,                                   -- Autres traits de caractère
    histoire                TEXT,                                   -- Histoire du personnage

    -- POSSEDE  
    stuff_id                UUID[] REFERENCES equipements(id)
    stuff                   TEXT,                                   -- Description du stuff du personnage c'est du md ammélioré qui permet de remplir toutes les liste simplement. 

    -- SORTS
    sorts_id                UUID[] REFERENCES sorts(id),                                 -- Liste de sorts (refs vers sorts.id)
    sorts                   TEXT,                                   -- Description des sorts du personnage c'est du md ammélioré qui permet de remplir toutes les liste simplement.

    -- MAITRISES        
    -- Liste d'outils maîtrisés (refs vers outils.id)
    maitrise                TEXT[],                                 -- Liste de maîtrises séparées par virgules

    -- CARACTÉRISTIQUES DE JEU      
    carac_force                 INTEGER NOT NULL DEFAULT 10,        
    carac_dexterite             INTEGER NOT NULL DEFAULT 10,        
    carac_constitution          INTEGER NOT NULL DEFAULT 10,        
    carac_intelligence          INTEGER NOT NULL DEFAULT 10,        
    carac_sagesse               INTEGER NOT NULL DEFAULT 10,        
    carac_charisme              INTEGER NOT NULL DEFAULT 10,   
    sauvegarde_force            BOOLEAN DEFAULT FALSE, -- Indique si le personnage est compétent en sauvegarde de Force
    sauvegarde_dexterite        BOOLEAN DEFAULT FALSE, -- Indique si le personnage est compétent en sauvegarde de Dextérité
    sauvegarde_constitution     BOOLEAN DEFAULT FALSE, -- Indique si le personnage est compétent en sauvegarde de Constitution
    sauvegarde_intelligence     BOOLEAN DEFAULT FALSE, -- Indique si le personnage est compétent en sauvegarde d'Intelligence
    sauvegarde_sagesse          BOOLEAN DEFAULT FALSE, -- Indique si le personnage est compétent en sauvegarde de Sagesse
    sauvegarde_charisme         BOOLEAN DEFAULT FALSE, -- Indique si le personnage est compétent en sauvegarde de Charisme  
    
    -- COMPÉTENCES DD5E
    competence_athletisme       BOOLEAN DEFAULT FALSE, -- Indique si le personnage est compétent en Athlétisme
    competence_acrobaties       BOOLEAN DEFAULT FALSE, -- Indique si le personnage est compétent en Acrobaties
    competence_discretion       BOOLEAN DEFAULT FALSE, -- Indique si le personnage est compétent en Discrétion
    competence_escamotage       BOOLEAN DEFAULT FALSE, -- Indique si le personnage est compétent en Escamotage
    competence_arcane           BOOLEAN DEFAULT FALSE, -- Indique si le personnage est compétent en Arcane
    competence_histoire         BOOLEAN DEFAULT FALSE, -- Indique si le personnage est compétent en Histoire
    competence_investigation    BOOLEAN DEFAULT FALSE, -- Indique si le personnage est compétent en Investigation
    competence_nature           BOOLEAN DEFAULT FALSE, -- Indique si le personnage est compétent en Nature
    competence_religion         BOOLEAN DEFAULT FALSE, -- Indique si le personnage est compétent en Religion
    competence_dressage         BOOLEAN DEFAULT FALSE, -- Indique si le personnage est compétent en Dressage
    competence_intuition        BOOLEAN DEFAULT FALSE, -- Indique si le personnage est compétent en Intuition
    competence_medicine         BOOLEAN DEFAULT FALSE, -- Indique si le personnage est compétent en Médecine
    competence_perception       BOOLEAN DEFAULT FALSE, -- Indique si le personnage est compétent en Perception
    competence_survie           BOOLEAN DEFAULT FALSE, -- Indique si le personnage est compétent en Survie
    competence_intimidation     BOOLEAN DEFAULT FALSE, -- Indique si le personnage est compétent en Intimidation
    competence_persuasion       BOOLEAN DEFAULT FALSE, -- Indique si le personnage est compétent en Persuasion
    competence_representation   BOOLEAN DEFAULT FALSE, -- Indique si le personnage est compétent en Représentation
    competence_tromperie        BOOLEAN DEFAULT FALSE, -- Indique si le personnage est compétent en Tromperie

    -- CARACTÉRISTIQUES ROLEPLAY        
    niveau                  INTEGER NOT NULL DEFAULT 1,             -- Niveau
    experience              INTEGER NOT NULL DEFAULT 0,             -- Points d'expérience

    -- PERSONNALITÉ     
    alignement              VARCHAR(2),                             -- Alignement (ex: "LG", "NE", etc.)
    personnalite            TEXT,                                   -- Description de la personnalité du personnage

    -- DESCRIPTIONS PHYSIQUES       
    sexe                    VARCHAR(20),                            -- Sexe du personnage
    vitesse                 FLOAT NOT NULL DEFAULT 7.5,             -- Vitesse de déplacement en m
    age                     INTEGER,                                -- Âge du personnage
    taille                  INTEGER,                                -- Taille en cm
    poids                   INTEGER,                                -- Poids en kg
    descriptions            TEXT,                                   -- Description physique et personnalité du personnage

    -- Image (chemin relatif depuis resources/images/, ex: 'personnages/hero.png')
    image_path              TEXT DEFAULT 'placeholder.png'
);
