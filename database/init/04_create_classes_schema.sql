-- -- ============================================================================
-- Fichier: 04_create_classes_schema.sql
-- Description: Création de la table 
-- - classes                    : Classes et sous classe (genre les ecole de magie ou les choix de terrain)
-- - classe_presentations       : Liste des paragraphe pour présenté la classe, juste parce que c'est dynamique.
-- - classe_evolutions          : Liste de tout les niveau de toutes les classe avec leur aptitude. 
-- - classe_aptitudes           : Les aptitude. c'est pas encore bien défini ce que je veux pour l'instant c'est juste un champs de texte. J'utiliserai les alias en lien MD+
-- - classe_choix_equipement    : C'est pour les différents choix de stuff lors du building de perso 

-- Auteur: MATHURIN CHAMPÉMONT
-- Date: 2026_05-16

-- HACK : Mise en page TURBO dégeux.
-- ============================================================================

-- Enable UUID extension (au cas où)
CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

-- ============================================================================
-- TABLE: CLASS
-- Structure complète pour les classes avec toutes les métadonnées
-- ============================================================================

CREATE TABLE IF NOT EXISTS classes (
    id                      UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    nom                     VARCHAR(100) NOT NULL UNIQUE,  -- Nom de la classe (ex: "Guerrier", "Sorcier", etc.)
    classe_parente_id       UUID REFERENCES classes(id),   -- NULL si classe parente, sinon référence
    alias                   VARCHAR(100),                  -- Nom alternatif pour pseudo markdown

    -- Description générale     
    description             TEXT,                -- Description rapide de la classe 
    aide_joueur             TEXT,                -- Conseils pour les joueurs
    creation_classe         TEXT,                -- Comment créer une classe
    creation_rapide         TEXT,                -- Comment créer une classe rapidement

    -- Aptitudes de classe      
    -- Vie      
    dee_de_vie              INTEGER NOT NULL,    -- D8, D10, etc.
    points_de_vie           INTEGER NOT NULL,    -- Points de vie à 1er niveau
    points_de_vie_par_niveau INTEGER NOT NULL,   -- Points de vie gagnés par

    -- Maitrises        
    equipement_maitrise     UUID[],              -- Liste d'armures (refs vers equipement.id)
    jets_de_sauvegarde      TEXT[],              -- Liste de jets de sauvegarde séparés par virgules
    competences             TEXT[],              -- Liste de compétences séparées par virgules

    -- Sorts  
    caracteristiques_de_sorts VARCHAR(3),        -- Caractéristiques de sorts ("INT", "CHA", etc.)

    -- Image (chemin relatif depuis assets/images/, ex: 'classes/fighter.png')
    -- Source: Wizards of the Coast — usage personnel uniquement (voir assets/images/COPYRIGHT.md)
    image_path              TEXT DEFAULT 'placeholder.png'
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
    choix_sous_classe BOOLEAN DEFAULT FALSE, -- Indique si ce niveau implique le choix de la sous classe
    niveau INTEGER NOT NULL, -- Niveau auquel l'évolution est acquise
    bonus_maîtrise INTEGER, -- Bonus de maîtrise acquis à ce niveau
    aptitudes_gagnees TEXT, -- Liste d'aptitudes gagnées à ce niveau séparées par virgules
    
    -- Différent colonnes pour les différentes aptitudes gagnées à ce niveau (ex: "1 aptitude de classe", "2 compétences", etc.)
    -- Nom de la colonne        type dans la col      nombre dans la col      texte dans la col 
    --                           0 pour Nombre
    --                           1 pour Texte
    col_1_titre VARCHAR(100),   col_1_type BOOLEAN,   col_1_nombre INTEGER,   col_1_texte TEXT,
    col_2_titre VARCHAR(100),   col_2_type BOOLEAN,   col_2_nombre INTEGER,   col_2_texte TEXT,
    col_3_titre VARCHAR(100),   col_3_type BOOLEAN,   col_3_nombre INTEGER,   col_3_texte TEXT,
    col_4_titre VARCHAR(100),   col_4_type BOOLEAN,   col_4_nombre INTEGER,   col_4_texte TEXT,
    col_5_titre VARCHAR(100),   col_5_type BOOLEAN,   col_5_nombre INTEGER,   col_5_texte TEXT,
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
    id                  UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    classe_id           UUID REFERENCES classes(id) ON DELETE CASCADE, -- Référence à la classe
    alias               VARCHAR(100), -- Nom alternatif pour pseudo markdown
    nom                 VARCHAR(100) NOT NULL, -- Nom de l'aptitude
    description         TEXT NOT NULL, -- Description de l'aptitude
    description_rapide  TEXT, -- Description rapide de l'aptitude pour les tableaux de progression
    niveau_acquisition  INTEGER NOT NULL, -- Niveau auquel l'aptitude est acquise
    gain_de_sort        BOOLEAN DEFAULT FALSE, -- Indique si l'aptitude confère un gain de sort (pour les classes de lanceurs de sorts)
    gain_de_compétence  BOOLEAN DEFAULT FALSE, -- Indique si l'aptitude confère un gain de compétence (pour les classes de compétences)
    gain_de_vie         BOOLEAN DEFAULT FALSE, -- Indique si l'aptitude confère un gain de vie (pour les classes de vies)
    gain_de_maitrise    BOOLEAN DEFAULT FALSE -- Indique si l'aptitude confère un gain de maîtrise (pour les classes de maîtrise)
);

-- ============================================================================
-- TABLE: CHOIX_EQUIPMENT
-- Structure pour les choix d'équipement de classe avec référence à la classe
-- Si il u a trois option pour le choix A da la classe B alors il y aura trois entrées dans cette table avec classe_id = B et choix_num = A
-- ============================================================================ 

CREATE TABLE IF NOT EXISTS classe_choix_equipement (
    id                 UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    classe_id          UUID REFERENCES classes(id) ON DELETE CASCADE,  -- Référence à la classe
    choix_num          INTEGER NOT NULL,                               -- Numéro du choix (1, 2, 3 ou 4)
    equipement_id      UUID[],
);