-- ===============================================================
-- Fichier: 02_create_sorts.sql
-- Description: Création de la table "sorts" pour stocker les sorts
-- Auteur: MATHURIN CHAMPÉMONT
-- Date: 2026_05-16

-- HACK : Mise en page dégueu.
-- ===============================================================

-- Cette table comprend un champs d'alias. Il faudra donc que le DataManager ai une méthode 
-- TODO :  - IsOver() : qui permet d'afficher une info bulle. Le champs info en question peux être généré en fonction des info de l'équipement . 
-- TODO :  - IsClick() : Qui doit renvoyer une page ImGui de l'equipement. Elle doit avoir son propre render. Le gestionnaire déquipement remplit cette mission.
-- TODO : Pour le moment on met le nom de la classe, mais il faudra peut être faire une table de liaison pour les classes et les sorts, afin de permettre une flexibilité maximale dans la gestion des classes et des sorts. Cependant, pour simplifier la structure initiale, nous allons inclure les classes directement dans la table des sorts.

-- Liste des écoles de magie : Abjuration, Conjuration, Divination, Enchantement, Evocation, Illusion, Nécromancie, Transmutation
-- Liste des composantes : V (verbale), S (somatique), M (matérielle)
-- Liste des classes : Barde, Clerc, Druide, Guerrier, Moine, Paladin, Rôdeur, Roublard, Sorcier, Magicien. En effet les Barbare ne font pas de magie.

-- TABLE: SORTS
CREATE TABLE IF NOT EXISTS sorts (
    id                  UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    nom                 VARCHAR(100) NOT NULL UNIQUE,   -- Nom du sort
    alias               VARCHAR(100) NOT NULL UNIQUE,   -- Nom alternatif pour pseudo markdown
    description         TEXT NOT NULL,                  -- Description du sort
    niveau              INTEGER NOT NULL,               -- Niveau du sort (0 pour les cantrips)
    ecole               VARCHAR(50) NOT NULL,           -- École de magie 
    composantes         TEXT NOT NULL,                  -- Composantes du sort.
    temps_incantation   VARCHAR(50) NOT NULL,           -- Temps d'incantation du sort (ex: "1 action", "1 minute", etc.)
    portee              VARCHAR(50),                    -- Portée du sort (ex: "Toucher", "30 mètres", etc.) la distance est en mètres
    classes             TEXT                            -- Classes pouvant lancer le sort 
    duree               VARCHAR(50) NOT NULL,           -- Durée du sort (ex: "Instantané", "Concentration, jusqu'à 1 minute", etc.)
);