-- ============================================================================
-- SCHÉMA COMPLET DES RACES D&D 5E
-- Fusion de 02_seed_races.sql, 11_new_race_schema.sql, 12_migrate_race_data.sql
-- Utilise la logique du nouveau schéma normalisé (la plus récente)
-- ============================================================================

-- Enable UUID extension
CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

-- ============================================================================
-- SECTION 1: TABLES DE RÉFÉRENCE
-- ============================================================================

-- TABLE: CARACTÉRISTIQUES (Force, Dextérité, etc.)
CREATE TABLE IF NOT EXISTS caracteristiques (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    nom VARCHAR(50) NOT NULL UNIQUE,
    nom_court VARCHAR(3) NOT NULL UNIQUE, -- FOR, DEX, CON, INT, SAG, CHA
    description TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- TABLE: LANGUES
CREATE TABLE IF NOT EXISTS langues (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    nom VARCHAR(100) NOT NULL UNIQUE,
    description TEXT,
    alphabet TEXT, -- Nom de l'alphabet (Commun, Draconique, Elfique, etc.)
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- ============================================================================
-- SECTION 2: TABLE PRINCIPALE DES RACES
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
    description_longue TEXT,
    description_courte TEXT,

    -- Noms de personnages
    exemple_nom_femme TEXT,
    exemple_nom_homme TEXT,
    type_categorie_nom VARCHAR(100), -- Ex: "Nom de clan", "Nom de famille"
    exemple_nom_categorie_speciale TEXT,

    -- Variantes raciales
    nombre_variantes_raciales INTEGER DEFAULT 0,
    race_parent_id UUID REFERENCES races_new(id),

    -- Vitesse (en pieds)
    vitesse_base INTEGER DEFAULT 30,

    -- Méta
    source VARCHAR(100) DEFAULT 'Player Handbook',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- ============================================================================
-- SECTION 3: TABLES DE LIAISON
-- ============================================================================

-- LIAISON: RACE <-> BONUS DE CARACTÉRISTIQUES
CREATE TABLE IF NOT EXISTS race_bonus_caracteristiques (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    race_id UUID NOT NULL REFERENCES races_new(id) ON DELETE CASCADE,
    caracteristique_id UUID NOT NULL REFERENCES caracteristiques(id) ON DELETE CASCADE,
    valeur_bonus INTEGER NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    UNIQUE(race_id, caracteristique_id)
);

-- LIAISON: RACE <-> LANGUES
CREATE TABLE IF NOT EXISTS race_langues (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    race_id UUID NOT NULL REFERENCES races_new(id) ON DELETE CASCADE,
    langue_id UUID NOT NULL REFERENCES langues(id) ON DELETE CASCADE,
    est_langue_bonus BOOLEAN DEFAULT false,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    UNIQUE(race_id, langue_id)
);

-- TABLE: OPINIONS RACIALES ("Ce qu'en disent les autres races")
CREATE TABLE IF NOT EXISTS race_opinions (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    race_source_id UUID NOT NULL REFERENCES races_new(id) ON DELETE CASCADE,
    race_cible_id UUID NOT NULL REFERENCES races_new(id) ON DELETE CASCADE,
    opinion_texte TEXT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    UNIQUE(race_source_id, race_cible_id)
);

-- TABLE: ABILITÉS RACIALES
CREATE TABLE IF NOT EXISTS abilites_raciales (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    nom VARCHAR(255) NOT NULL,
    nom_fr VARCHAR(255) NOT NULL,
    description TEXT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- LIAISON: RACE <-> ABILITÉS RACIALES
CREATE TABLE IF NOT EXISTS race_abilites_raciales (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    race_id UUID NOT NULL REFERENCES races_new(id) ON DELETE CASCADE,
    abilite_raciale_id UUID NOT NULL REFERENCES abilites_raciales(id) ON DELETE CASCADE,
    niveau_acquisition INTEGER DEFAULT 1,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    UNIQUE(race_id, abilite_raciale_id)
);

-- LIAISON: ABILITÉ RACIALE <-> MODIFICATEURS DE CARACTÉRISTIQUES
CREATE TABLE IF NOT EXISTS abilite_raciale_modificateurs_caracteristiques (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    abilite_raciale_id UUID NOT NULL REFERENCES abilites_raciales(id) ON DELETE CASCADE,
    caracteristique_id UUID NOT NULL REFERENCES caracteristiques(id) ON DELETE CASCADE,
    valeur_modification INTEGER NOT NULL,
    type_modification VARCHAR(50),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- LIAISON: ABILITÉ RACIALE <-> SORTS ACQUIS
CREATE TABLE IF NOT EXISTS abilite_raciale_sorts_acquis (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    abilite_raciale_id UUID NOT NULL REFERENCES abilites_raciales(id) ON DELETE CASCADE,
    sort_id UUID NOT NULL REFERENCES game_spells(id) ON DELETE CASCADE,
    niveau_acquisition INTEGER DEFAULT 1,
    nombre_utilisations_par_jour INTEGER,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- LIAISON: ABILITÉ RACIALE <-> JETS DE SAUVEGARDE
CREATE TABLE IF NOT EXISTS abilite_raciale_jets_sauvegarde (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    abilite_raciale_id UUID NOT NULL REFERENCES abilites_raciales(id) ON DELETE CASCADE,
    caracteristique_id UUID NOT NULL REFERENCES caracteristiques(id) ON DELETE CASCADE,
    a_avantage BOOLEAN DEFAULT false,
    a_desavantage BOOLEAN DEFAULT false,
    condition_specifique TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- LIAISON: ABILITÉ RACIALE <-> COMPÉTENCES MAÎTRISÉES
CREATE TABLE IF NOT EXISTS abilite_raciale_competences_maitrisees (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    abilite_raciale_id UUID NOT NULL REFERENCES abilites_raciales(id) ON DELETE CASCADE,
    competence_id UUID NOT NULL REFERENCES skills(id) ON DELETE CASCADE,
    niveau_maitrise VARCHAR(20) DEFAULT 'proficient',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    UNIQUE(abilite_raciale_id, competence_id)
);

-- LIAISON: ABILITÉ RACIALE <-> ARMES SPÉCIALES
CREATE TABLE IF NOT EXISTS abilite_raciale_armes_speciales (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    abilite_raciale_id UUID NOT NULL REFERENCES abilites_raciales(id) ON DELETE CASCADE,
    arme_id UUID REFERENCES weapons(id) ON DELETE CASCADE,
    nom_arme_custom VARCHAR(255),
    description_arme TEXT,
    degats_formule VARCHAR(50),
    type_degats VARCHAR(50),
    portee_normale INTEGER,
    portee_longue INTEGER,
    nombre_utilisations VARCHAR(50),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- ============================================================================
-- SECTION 4: INDEX POUR PERFORMANCE
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
-- SECTION 5: TRIGGERS
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

-- ============================================================================
-- SECTION 6: DONNÉES DE BASE
-- ============================================================================

-- 6.1 CARACTÉRISTIQUES
INSERT INTO caracteristiques (nom, nom_court, description) VALUES
    ('Force', 'FOR', 'Mesure la puissance physique, la capacité athlétique et la force brute'),
    ('Dextérité', 'DEX', 'Mesure l''agilité, les réflexes et l''équilibre'),
    ('Constitution', 'CON', 'Mesure l''endurance, la santé et la force vitale'),
    ('Intelligence', 'INT', 'Mesure l''acuité mentale, la mémoire et le raisonnement'),
    ('Sagesse', 'SAG', 'Mesure la perception, l''intuition et la perspicacité'),
    ('Charisme', 'CHA', 'Mesure la force de personnalité, l''éloquence et le leadership')
ON CONFLICT (nom) DO NOTHING;

-- 6.2 LANGUES
INSERT INTO langues (nom, description, alphabet) VALUES
    ('Commun', 'Langue la plus répandue, parlée par la plupart des humains', 'Commun'),
    ('Nain', 'Langue des nains, gutturale et résonnante', 'Nanique'),
    ('Elfique', 'Langue fluide des elfes, mélodieuse', 'Elfique'),
    ('Géant', 'Langue des géants et des créatures massives', 'Nanique'),
    ('Gnome', 'Langue des gnomes, vive et rapide', 'Nanique'),
    ('Gobelin', 'Langue harsh des gobelins', 'Nanique'),
    ('Halfelin', 'Langue des halfelins, simple et chaleureuse', 'Commun'),
    ('Orc', 'Langue brutale des orcs', 'Nanique'),
    ('Abyssal', 'Langue des démons du plan des Abysses', 'Infernal'),
    ('Céleste', 'Langue des créatures célestes', 'Céleste'),
    ('Draconique', 'Langue ancienne des dragons', 'Draconique'),
    ('Profond', 'Langue étrange des aberrations', 'Profond'),
    ('Infernal', 'Langue des diables et des Enfers', 'Infernal'),
    ('Primordial', 'Langue des élémentaires', 'Nanique'),
    ('Sylvestre', 'Langue des fées et des créatures féeriques', 'Elfique'),
    ('Commun des Profondeurs', 'Langue de l''Outreterre', 'Elfique')
ON CONFLICT (nom) DO NOTHING;

-- ============================================================================
-- SECTION 7: DONNÉES DES RACES
-- ============================================================================

-- 7.1 RACES DE BASE
INSERT INTO races_new (nom, nom_fr, age_min, age_max, taille_min, taille_max, taille_categorie, description_longue, description_courte, vitesse_base, exemple_nom_homme, exemple_nom_femme) VALUES
('Human', 'Humain', 15, 80, 150, 190, 'Medium', 'Les humains sont la race la plus adaptable et ambitieuse. Polyvalents et diversifiés.', 'Race polyvalente et adaptable', 30, 'Jean, Pierre, Marc, Thomas', 'Marie, Sophie, Claire, Anne'),
('Elf', 'Elfe', 100, 750, 150, 180, 'Medium', 'Gracieux et agiles, les elfes vivent en harmonie avec la nature. Vision dans le noir, immunité au sommeil magique.', 'Race gracieuse et mystique', 30, 'Legolas, Thranduil, Celeborn, Elrond', 'Arwen, Galadriel, Tauriel, Luthien'),
('Dwarf', 'Nain', 50, 350, 120, 150, 'Medium', 'Robustes et endurants, les nains sont de fiers artisans et guerriers. Résistance au poison.', 'Race robuste et artisane', 25, 'Thorin, Gimli, Balin, Dwalin', 'Dis, Vilja, Kathra, Riswynn'),
('Halfling', 'Halfelin', 20, 150, 90, 100, 'Small', 'Petits mais courageux, les halfelins sont chanceux et agiles. Capacité Chanceux unique.', 'Race petite et chanceuse', 25, 'Frodo, Bilbo, Merry, Pippin', 'Rose, Lobelia, Primrose, Daisy'),
('Dragonborn', 'Drakéide', 15, 80, 175, 210, 'Medium', 'Descendants de dragons, les drakéides sont fiers et puissants. Souffle draconique et résistance élémentaire.', 'Race draconique et fière', 30, 'Drakus, Kriv, Shamash, Torinn', 'Akra, Biri, Mishann, Thava'),
('Gnome', 'Gnome', 40, 500, 90, 120, 'Small', 'Curieux et inventifs, les gnomes excellent en magie et artifice. Vision dans le noir et ruse gnome.', 'Race inventive et curieuse', 25, 'Alston, Boddynock, Dimble, Fonkin', 'Bimpnottin, Caramip, Duvamil, Ella'),
('Half-Elf', 'Demi-Elfe', 20, 180, 150, 185, 'Medium', 'Mélange d''humain et d''elfe, les demi-elfes sont charismatiques et polyvalents. +2 CHA et +1 à deux autres caractéristiques.', 'Race hybride et charismatique', 30, 'Aelar, Beren, Tanis, Kendel', 'Faral, Immeral, Mara, Syllia'),
('Half-Orc', 'Demi-Orc', 14, 75, 165, 200, 'Medium', 'Force brutale et ténacité. Les demi-orcs ne tombent pas facilement. Ténacité implacable.', 'Race forte et tenace', 30, 'Grog, Throk, Kragg, Mord', 'Baggi, Emen, Ovak, Vola'),
('Tiefling', 'Tieffelin', 15, 100, 155, 190, 'Medium', 'Héritage infernal visible, les tieffelins sont résistants au feu et possèdent une magie innée.', 'Race d''héritage infernal', 30, 'Akmenios, Damakos, Kairon, Leucis', 'Akta, Bryseis, Kallista, Makaria')
ON CONFLICT (nom) DO NOTHING;

-- Ajouter les noms de clan pour certaines races
UPDATE races_new SET
    type_categorie_nom = 'Nom de clan',
    exemple_nom_categorie_speciale = 'Forgefeu, Marteaudefeu, Pierregrise'
WHERE nom = 'Dwarf';

UPDATE races_new SET
    type_categorie_nom = 'Nom de famille',
    exemple_nom_categorie_speciale = 'Tealeaf, Underhill, Goodbarrel'
WHERE nom = 'Halfling';

UPDATE races_new SET
    type_categorie_nom = 'Nom de clan',
    exemple_nom_categorie_speciale = 'Clethtinthiallor, Daardendrian, Delmirev'
WHERE nom = 'Dragonborn';

-- ============================================================================
-- SECTION 8: BONUS DE CARACTÉRISTIQUES PAR RACE
-- ============================================================================

DO $$
DECLARE
    carac_for_id UUID;
    carac_dex_id UUID;
    carac_con_id UUID;
    carac_int_id UUID;
    carac_sag_id UUID;
    carac_cha_id UUID;
    race_id UUID;
BEGIN
    -- Récupérer les IDs des caractéristiques
    SELECT id INTO carac_for_id FROM caracteristiques WHERE nom_court = 'FOR';
    SELECT id INTO carac_dex_id FROM caracteristiques WHERE nom_court = 'DEX';
    SELECT id INTO carac_con_id FROM caracteristiques WHERE nom_court = 'CON';
    SELECT id INTO carac_int_id FROM caracteristiques WHERE nom_court = 'INT';
    SELECT id INTO carac_sag_id FROM caracteristiques WHERE nom_court = 'SAG';
    SELECT id INTO carac_cha_id FROM caracteristiques WHERE nom_court = 'CHA';

    -- HUMAIN (+1 à toutes)
    SELECT id INTO race_id FROM races_new WHERE nom = 'Human';
    INSERT INTO race_bonus_caracteristiques (race_id, caracteristique_id, valeur_bonus) VALUES
        (race_id, carac_for_id, 1), (race_id, carac_dex_id, 1), (race_id, carac_con_id, 1),
        (race_id, carac_int_id, 1), (race_id, carac_sag_id, 1), (race_id, carac_cha_id, 1)
    ON CONFLICT DO NOTHING;

    -- ELFE (+2 DEX)
    SELECT id INTO race_id FROM races_new WHERE nom = 'Elf';
    INSERT INTO race_bonus_caracteristiques (race_id, caracteristique_id, valeur_bonus) VALUES
        (race_id, carac_dex_id, 2)
    ON CONFLICT DO NOTHING;

    -- NAIN (+2 CON)
    SELECT id INTO race_id FROM races_new WHERE nom = 'Dwarf';
    INSERT INTO race_bonus_caracteristiques (race_id, caracteristique_id, valeur_bonus) VALUES
        (race_id, carac_con_id, 2)
    ON CONFLICT DO NOTHING;

    -- HALFELIN (+2 DEX)
    SELECT id INTO race_id FROM races_new WHERE nom = 'Halfling';
    INSERT INTO race_bonus_caracteristiques (race_id, caracteristique_id, valeur_bonus) VALUES
        (race_id, carac_dex_id, 2)
    ON CONFLICT DO NOTHING;

    -- DRAKÉIDE (+2 FOR, +1 CHA)
    SELECT id INTO race_id FROM races_new WHERE nom = 'Dragonborn';
    INSERT INTO race_bonus_caracteristiques (race_id, caracteristique_id, valeur_bonus) VALUES
        (race_id, carac_for_id, 2), (race_id, carac_cha_id, 1)
    ON CONFLICT DO NOTHING;

    -- GNOME (+2 INT)
    SELECT id INTO race_id FROM races_new WHERE nom = 'Gnome';
    INSERT INTO race_bonus_caracteristiques (race_id, caracteristique_id, valeur_bonus) VALUES
        (race_id, carac_int_id, 2)
    ON CONFLICT DO NOTHING;

    -- DEMI-ELFE (+2 CHA, +1 à deux autres au choix - on marque juste le CHA)
    SELECT id INTO race_id FROM races_new WHERE nom = 'Half-Elf';
    INSERT INTO race_bonus_caracteristiques (race_id, caracteristique_id, valeur_bonus) VALUES
        (race_id, carac_cha_id, 2)
    ON CONFLICT DO NOTHING;

    -- DEMI-ORC (+2 FOR, +1 CON)
    SELECT id INTO race_id FROM races_new WHERE nom = 'Half-Orc';
    INSERT INTO race_bonus_caracteristiques (race_id, caracteristique_id, valeur_bonus) VALUES
        (race_id, carac_for_id, 2), (race_id, carac_con_id, 1)
    ON CONFLICT DO NOTHING;

    -- TIEFFELIN (+1 INT, +2 CHA)
    SELECT id INTO race_id FROM races_new WHERE nom = 'Tiefling';
    INSERT INTO race_bonus_caracteristiques (race_id, caracteristique_id, valeur_bonus) VALUES
        (race_id, carac_int_id, 1), (race_id, carac_cha_id, 2)
    ON CONFLICT DO NOTHING;
END $$;

-- ============================================================================
-- SECTION 9: LANGUES PAR RACE
-- ============================================================================

DO $$
DECLARE
    race_id UUID;
    langue_commun_id UUID;
    langue_elfique_id UUID;
    langue_nain_id UUID;
    langue_halfelin_id UUID;
    langue_draconique_id UUID;
    langue_gnome_id UUID;
    langue_orc_id UUID;
    langue_infernal_id UUID;
BEGIN
    -- Récupérer les IDs des langues
    SELECT id INTO langue_commun_id FROM langues WHERE nom = 'Commun';
    SELECT id INTO langue_elfique_id FROM langues WHERE nom = 'Elfique';
    SELECT id INTO langue_nain_id FROM langues WHERE nom = 'Nain';
    SELECT id INTO langue_halfelin_id FROM langues WHERE nom = 'Halfelin';
    SELECT id INTO langue_draconique_id FROM langues WHERE nom = 'Draconique';
    SELECT id INTO langue_gnome_id FROM langues WHERE nom = 'Gnome';
    SELECT id INTO langue_orc_id FROM langues WHERE nom = 'Orc';
    SELECT id INTO langue_infernal_id FROM langues WHERE nom = 'Infernal';

    -- HUMAIN (Commun + 1 au choix)
    SELECT id INTO race_id FROM races_new WHERE nom = 'Human';
    INSERT INTO race_langues (race_id, langue_id, est_langue_bonus) VALUES
        (race_id, langue_commun_id, false)
    ON CONFLICT DO NOTHING;

    -- ELFE (Commun, Elfique)
    SELECT id INTO race_id FROM races_new WHERE nom = 'Elf';
    INSERT INTO race_langues (race_id, langue_id, est_langue_bonus) VALUES
        (race_id, langue_commun_id, false), (race_id, langue_elfique_id, false)
    ON CONFLICT DO NOTHING;

    -- NAIN (Commun, Nain)
    SELECT id INTO race_id FROM races_new WHERE nom = 'Dwarf';
    INSERT INTO race_langues (race_id, langue_id, est_langue_bonus) VALUES
        (race_id, langue_commun_id, false), (race_id, langue_nain_id, false)
    ON CONFLICT DO NOTHING;

    -- HALFELIN (Commun, Halfelin)
    SELECT id INTO race_id FROM races_new WHERE nom = 'Halfling';
    INSERT INTO race_langues (race_id, langue_id, est_langue_bonus) VALUES
        (race_id, langue_commun_id, false), (race_id, langue_halfelin_id, false)
    ON CONFLICT DO NOTHING;

    -- DRAKÉIDE (Commun, Draconique)
    SELECT id INTO race_id FROM races_new WHERE nom = 'Dragonborn';
    INSERT INTO race_langues (race_id, langue_id, est_langue_bonus) VALUES
        (race_id, langue_commun_id, false), (race_id, langue_draconique_id, false)
    ON CONFLICT DO NOTHING;

    -- GNOME (Commun, Gnome)
    SELECT id INTO race_id FROM races_new WHERE nom = 'Gnome';
    INSERT INTO race_langues (race_id, langue_id, est_langue_bonus) VALUES
        (race_id, langue_commun_id, false), (race_id, langue_gnome_id, false)
    ON CONFLICT DO NOTHING;

    -- DEMI-ELFE (Commun, Elfique + 1 au choix)
    SELECT id INTO race_id FROM races_new WHERE nom = 'Half-Elf';
    INSERT INTO race_langues (race_id, langue_id, est_langue_bonus) VALUES
        (race_id, langue_commun_id, false), (race_id, langue_elfique_id, false)
    ON CONFLICT DO NOTHING;

    -- DEMI-ORC (Commun, Orc)
    SELECT id INTO race_id FROM races_new WHERE nom = 'Half-Orc';
    INSERT INTO race_langues (race_id, langue_id, est_langue_bonus) VALUES
        (race_id, langue_commun_id, false), (race_id, langue_orc_id, false)
    ON CONFLICT DO NOTHING;

    -- TIEFFELIN (Commun, Infernal)
    SELECT id INTO race_id FROM races_new WHERE nom = 'Tiefling';
    INSERT INTO race_langues (race_id, langue_id, est_langue_bonus) VALUES
        (race_id, langue_commun_id, false), (race_id, langue_infernal_id, false)
    ON CONFLICT DO NOTHING;
END $$;

-- ============================================================================
-- RÉSUMÉ
-- ============================================================================

DO $$
DECLARE
    nb_races INTEGER;
    nb_bonus INTEGER;
    nb_langues_liees INTEGER;
BEGIN
    SELECT COUNT(*) INTO nb_races FROM races_new;
    SELECT COUNT(*) INTO nb_bonus FROM race_bonus_caracteristiques;
    SELECT COUNT(*) INTO nb_langues_liees FROM race_langues;

    RAISE NOTICE '========================================';
    RAISE NOTICE 'SCHÉMA DES RACES CRÉÉ AVEC SUCCÈS';
    RAISE NOTICE '========================================';
    RAISE NOTICE 'Races créées: %', nb_races;
    RAISE NOTICE 'Bonus de caractéristiques: %', nb_bonus;
    RAISE NOTICE 'Liens race-langue: %', nb_langues_liees;
    RAISE NOTICE '========================================';
END $$;
