-- ============================================================================
-- Fichier: 01_create_equipement_schema.sql
-- Description: Création de la table 
--  - equipements                   : pour stocker tous ce qui peut être transporté par le joueur. 
--  - equipement_proprietes         : Définit toute les propriété possible d'un equipement.
--  - equipement_propriete_liens    : Table de lien entre equipements et equipement_proprietes

-- Auteur: MATHURIN CHAMPÉMONT
-- Date: 2026_05-16

-- HACK : Mise en page dégueux
-- ============================================================================

-- TABLE: EQUIPEMENT

-- Cette table comprend un champs d'alias. Il faudra donc que le DataManager ai une méthode 
-- TODO :  - IsOver() : qui permet d'afficher une info bulle. Le champs info en question peux être généré en fonction des info de l'équipement . 
-- TODO :  - IsClick() : Qui doit renvoyer une page ImGui de l'equipement. Elle doit avoir son propre render. Le gestionnaire déquipement remplit cette mission. 
 
-- TODO : Il faudra peut être ajouter une table de liaison pour les catégories d'équipements, afin de permettre une flexibilité maximale dans la gestion des catégories. Cependant, pour simplifier la structure initiale, nous allons inclure la catégorie directement dans la table des équipements.

CREATE TABLE IF NOT EXISTS equipements (
    id              UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    nom             VARCHAR(100) NOT NULL UNIQUE,   -- Nom de l'équipement
    alias           VARCHAR(100) NOT NULL UNIQUE,   -- Nom alternatif pour pseudo markdown
    categorie       VARCHAR(50),                    -- Catégorisation de l'équipement ex "arme", "armure", "outil", "objet", "babioles"
    description     TEXT,                           -- Description de l'équipement
    poid            INTEGER,                        -- Poids de l'équipement en grammes
    prix            INTEGER,                        -- Prix en pièces de cuivre

    -- En fonction de la catégorie il y a des champs qui peuvent être demandeé ou non.
    -- LISTE DES CATEGORIES: 
    --   - arme     : pour les armes, les champs commencant par "arme_" sont obligatoires
    --                arme_degat_dee
    --                arme_degat_type
    --                type (ex: "Mêlée", "Distance", etc.)
    --                propriete (ex: "Léger", "Finesse", etc.)  
    --   - armure   : pour les armures, les champs commencant par "armure_" sont obligatoires
    --                armure_classe
    --                type (ex: "Légère", "Moyenne", "Lourde", etc.)
    --                propriete (ex: "Résistance au feu", "enchantement", etc.)
    --   - outil    : pour les outils, aucun champ spécifique n'est requis
    --   - objet    : pour les objets, aucun champ spécifique n'est requis
    --   - babioles : pour les babioles, aucun champ spécifique n'est requis
    -- ATTENTION : Les type et propriete sont des champs communs pour les armes et armures, mais leur contenu dépend de la catégorie de l'équipement.

    -- Pour les armes uniquement    (NULL pour les autres types d'équipement)
    arme_degat_dee  VARCHAR(20) ,                   -- Dégâts de l'arme (ex: "1d8", "2d6", etc.)
    arme_degat_type VARCHAR(50) ,                   -- Type de dégâts (ex: "Tranchant", "Perforant", etc.)
    arme_distance_normal FLOAT DEFAULT(1.5),        -- Distance en mètre ou tout se passe bien. Pour toute les armes
    arme_distance_max    FLOAT ,
    
    -- Pour les armures uniquement  (NULL pour les autres types d'équipement)
    armure_classe   INTEGER ,                       -- Classe d'armure fournie par l'armure
    
    -- Pour les armes et armures    (NULL pour les autres types d'équipement)
    type            VARCHAR(50) ,                   -- Type d'arme (ex: "Mêlée", "Distance", etc.)
                                                    -- Type d'armure (ex: "Légère", "Moyenne", "Lourde", etc.)
    propriete       TEXT                            -- Propriétés de l'arme (ex: "Léger", "Finesse", etc.)
                                                    -- Propriétés de l'armure (ex: "Résistance au feu", "enchantement", etc.)
);

-- ============================================================================

-- TABLE: EQUIPEMENT_PROPRIETES

-- Doit contenir toutes les propriété des armes et des armue (pour le moment)
CREATE TABLE IF NOT EXISTS equipement_proprietes(
    id              UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    nom             VARCHAR(100) NOT NULL UNIQUE,   -- Nom de la propriété
    description     TEXT,                           -- Description de la propriété
    categorie       VARCHAR(50)                     -- Cette propriété s'appuie sur quel catégorie ? ex "arme", "armure", "outil", "objet", "babioles"
);

-- ============================================================================

-- TABLE: EQUIPEMENT_PROPRIETE_LIENS

CREATE TABLE IF NOT EXISTS equipement_propriete_liens(
    id                      UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    equipement_id           UUID NOT NULL,
    equipement_proprietes   UUID NOT NULL
);