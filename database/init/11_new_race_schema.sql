-- ============================================================================
-- NOUVEAU SCHÉMA POUR LES RACES D&D 5E
-- Schéma relationnel normalisé basé sur les spécifications du TODO.md
-- ============================================================================

-- Enable UUID extension (au cas où)
CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

-- ============================================================================
-- TABLE: CARACTÉRISTIQUE
-- Les 6 caractéristiques principales (Force, Dextérité, etc.)
-- ============================================================================
CREATE TABLE IF NOT EXISTS caracteristiques (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    nom VARCHAR(50) NOT NULL UNIQUE,
    nom_court VARCHAR(3) NOT NULL UNIQUE, -- FOR, DEX, CON, INT, SAG, CHA
    description TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- ============================================================================
-- TABLE: LANGUE
-- Toutes les langues disponibles dans D&D 5e
-- ============================================================================
CREATE TABLE IF NOT EXISTS langues (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    nom VARCHAR(100) NOT NULL UNIQUE,
    description TEXT,
    alphabet TEXT, -- Nom de l'alphabet (Commun, Draconique, Elfique, etc.)
    -- TODO: Ajouter support pour image de l'alphabet plus tard
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- ============================================================================
-- TABLE: RACE (nouvelle version)
-- Structure complète pour les races avec toutes les métadonnées
-- ============================================================================
CREATE TABLE IF NOT EXISTS races_new (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    nom VARCHAR(100) NOT NULL UNIQUE,
    nom_fr VARCHAR(100) NOT NULL,

    -- Âge
    age_min INTEGER,
    age_max INTEGER,

    -- Taille (en centimètres)
    taille_min INTEGER,
    taille_max INTEGER,
    taille_categorie VARCHAR(20), -- Small, Medium, Large

    -- Descriptions
    description_longue TEXT, -- Texte formaté (markdown?)
    description_courte TEXT, -- Pour l'aide contextuelle

    -- Noms de personnages
    exemple_nom_femme TEXT, -- Exemples séparés par virgules
    exemple_nom_homme TEXT,
    type_categorie_nom VARCHAR(100), -- Ex: "Nom de clan", "Nom de famille"
    exemple_nom_categorie_speciale TEXT,

    -- Variantes raciales
    nombre_variantes_raciales INTEGER DEFAULT 0,
    race_parent_id UUID REFERENCES races_new(id), -- NULL si race parente, sinon référence

    -- Vitesse (en pieds)
    vitesse_base INTEGER DEFAULT 30,

    -- Méta
    source VARCHAR(100) DEFAULT 'Player Handbook',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- ============================================================================
-- TABLE DE LIAISON: RACE <-> CARACTÉRISTIQUE (Bonus raciaux)
-- Permet des bonus flexibles (ex: Humain +1 à toutes, Elfe +2 DEX)
-- ============================================================================
CREATE TABLE IF NOT EXISTS race_bonus_caracteristiques (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    race_id UUID NOT NULL REFERENCES races_new(id) ON DELETE CASCADE,
    caracteristique_id UUID NOT NULL REFERENCES caracteristiques(id) ON DELETE CASCADE,
    valeur_bonus INTEGER NOT NULL, -- +1, +2, etc.
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    UNIQUE(race_id, caracteristique_id)
);

-- ============================================================================
-- TABLE DE LIAISON: RACE <-> LANGUE
-- Langues parlées par chaque race
-- ============================================================================
CREATE TABLE IF NOT EXISTS race_langues (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    race_id UUID NOT NULL REFERENCES races_new(id) ON DELETE CASCADE,
    langue_id UUID NOT NULL REFERENCES langues(id) ON DELETE CASCADE,
    est_langue_bonus BOOLEAN DEFAULT false, -- true si c'est un choix de langue bonus
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    UNIQUE(race_id, langue_id)
);

-- ============================================================================
-- TABLE: OPINIONS RACIALES
-- "Ce qu'en disent les autres races"
-- ============================================================================
CREATE TABLE IF NOT EXISTS race_opinions (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    race_source_id UUID NOT NULL REFERENCES races_new(id) ON DELETE CASCADE, -- Qui parle
    race_cible_id UUID NOT NULL REFERENCES races_new(id) ON DELETE CASCADE,  -- De qui on parle
    opinion_texte TEXT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    UNIQUE(race_source_id, race_cible_id)
);

-- ============================================================================
-- TABLE: ABILITÉ RACIALE
-- Capacités spéciales des races (Vision dans le noir, Chanceux, etc.)
-- ============================================================================
CREATE TABLE IF NOT EXISTS abilites_raciales (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    nom VARCHAR(255) NOT NULL,
    nom_fr VARCHAR(255) NOT NULL,
    description TEXT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- ============================================================================
-- TABLE DE LIAISON: RACE <-> ABILITÉ RACIALE
-- Quelles abilités possède chaque race
-- ============================================================================
CREATE TABLE IF NOT EXISTS race_abilites_raciales (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    race_id UUID NOT NULL REFERENCES races_new(id) ON DELETE CASCADE,
    abilite_raciale_id UUID NOT NULL REFERENCES abilites_raciales(id) ON DELETE CASCADE,
    niveau_acquisition INTEGER DEFAULT 1, -- Niveau où on gagne l'abilité
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    UNIQUE(race_id, abilite_raciale_id)
);

-- ============================================================================
-- TABLE DE LIAISON: ABILITÉ RACIALE <-> MODIFICATEUR DE CARACTÉRISTIQUE
-- Ex: "Robustesse naine" donne +1 PV par niveau
-- ============================================================================
CREATE TABLE IF NOT EXISTS abilite_raciale_modificateurs_caracteristiques (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    abilite_raciale_id UUID NOT NULL REFERENCES abilites_raciales(id) ON DELETE CASCADE,
    caracteristique_id UUID NOT NULL REFERENCES caracteristiques(id) ON DELETE CASCADE,
    valeur_modification INTEGER NOT NULL,
    type_modification VARCHAR(50), -- bonus, multiplicateur, etc.
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- ============================================================================
-- TABLE DE LIAISON: ABILITÉ RACIALE <-> SORTS ACQUIS
-- Sorts gratuits donnés par certaines races (ex: Tieffelin)
-- ============================================================================
CREATE TABLE IF NOT EXISTS abilite_raciale_sorts_acquis (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    abilite_raciale_id UUID NOT NULL REFERENCES abilites_raciales(id) ON DELETE CASCADE,
    sort_id UUID NOT NULL REFERENCES game_spells(id) ON DELETE CASCADE,
    niveau_acquisition INTEGER DEFAULT 1, -- Niveau où on apprend le sort
    nombre_utilisations_par_jour INTEGER, -- NULL = illimité
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- ============================================================================
-- TABLE DE LIAISON: ABILITÉ RACIALE <-> JETS DE SAUVEGARDE
-- Avantages/désavantages aux jets de sauvegarde
-- ============================================================================
CREATE TABLE IF NOT EXISTS abilite_raciale_jets_sauvegarde (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    abilite_raciale_id UUID NOT NULL REFERENCES abilites_raciales(id) ON DELETE CASCADE,
    caracteristique_id UUID NOT NULL REFERENCES caracteristiques(id) ON DELETE CASCADE,
    a_avantage BOOLEAN DEFAULT false,
    a_desavantage BOOLEAN DEFAULT false,
    condition_specifique TEXT, -- Ex: "contre le poison", "contre les charmes"
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- ============================================================================
-- TABLE DE LIAISON: ABILITÉ RACIALE <-> COMPÉTENCES MAÎTRISÉES
-- Compétences automatiquement maîtrisées par l'abilité
-- ============================================================================
CREATE TABLE IF NOT EXISTS abilite_raciale_competences_maitrisees (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    abilite_raciale_id UUID NOT NULL REFERENCES abilites_raciales(id) ON DELETE CASCADE,
    competence_id UUID NOT NULL REFERENCES skills(id) ON DELETE CASCADE,
    niveau_maitrise VARCHAR(20) DEFAULT 'proficient', -- proficient, expertise
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    UNIQUE(abilite_raciale_id, competence_id)
);

-- ============================================================================
-- TABLE DE LIAISON: ABILITÉ RACIALE <-> ARMES SPÉCIALES
-- Attaques ou armes spéciales (ex: souffle du Drakéide)
-- ============================================================================
CREATE TABLE IF NOT EXISTS abilite_raciale_armes_speciales (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    abilite_raciale_id UUID NOT NULL REFERENCES abilites_raciales(id) ON DELETE CASCADE,
    arme_id UUID REFERENCES weapons(id) ON DELETE CASCADE, -- NULL si arme custom
    nom_arme_custom VARCHAR(255), -- Si arme non standard
    description_arme TEXT,
    degats_formule VARCHAR(50), -- Ex: "2d6"
    type_degats VARCHAR(50), -- fire, acid, etc.
    portee_normale INTEGER, -- en pieds
    portee_longue INTEGER,
    nombre_utilisations VARCHAR(50), -- Ex: "1/court repos"
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- ============================================================================
-- INDEX POUR PERFORMANCE
-- ============================================================================
CREATE INDEX IF NOT EXISTS idx_races_new_nom ON races_new(nom);
CREATE INDEX IF NOT EXISTS idx_races_new_parent ON races_new(race_parent_id);
CREATE INDEX IF NOT EXISTS idx_race_bonus_carac_race ON race_bonus_caracteristiques(race_id);
CREATE INDEX IF NOT EXISTS idx_race_langues_race ON race_langues(race_id);
CREATE INDEX IF NOT EXISTS idx_race_opinions_source ON race_opinions(race_source_id);
CREATE INDEX IF NOT EXISTS idx_race_opinions_cible ON race_opinions(race_cible_id);
CREATE INDEX IF NOT EXISTS idx_race_abilites_race ON race_abilites_raciales(race_id);
CREATE INDEX IF NOT EXISTS idx_abilite_sorts_abilite ON abilite_raciale_sorts_acquis(abilite_raciale_id);
CREATE INDEX IF NOT EXISTS idx_abilite_saves_abilite ON abilite_raciale_jets_sauvegarde(abilite_raciale_id);
CREATE INDEX IF NOT EXISTS idx_abilite_skills_abilite ON abilite_raciale_competences_maitrisees(abilite_raciale_id);
CREATE INDEX IF NOT EXISTS idx_abilite_armes_abilite ON abilite_raciale_armes_speciales(abilite_raciale_id);

-- ============================================================================
-- TRIGGER: Mise à jour automatique de updated_at
-- ============================================================================
CREATE OR REPLACE FUNCTION update_races_new_updated_at()
RETURNS TRIGGER AS $$
BEGIN
    NEW.updated_at = CURRENT_TIMESTAMP;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trigger_races_new_updated_at
BEFORE UPDATE ON races_new
FOR EACH ROW EXECUTE FUNCTION update_races_new_updated_at();
