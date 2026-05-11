-- ============================================================================
-- D&D 5E GAME DATA SCHEMA
-- Schéma pour stocker toutes les données du jeu D&D 5e
-- (Races, Classes, Sorts, Équipement, etc.)
-- ============================================================================

-- Enable UUID extension
CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

-- ============================================================================
-- RACES ET TRAITS RACIAUX
-- ============================================================================

CREATE TABLE races (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    name VARCHAR(100) NOT NULL UNIQUE,
    name_fr VARCHAR(100) NOT NULL,
    description TEXT,

    -- Bonus de caractéristiques
    str_bonus INTEGER DEFAULT 0,
    dex_bonus INTEGER DEFAULT 0,
    con_bonus INTEGER DEFAULT 0,
    int_bonus INTEGER DEFAULT 0,
    wis_bonus INTEGER DEFAULT 0,
    cha_bonus INTEGER DEFAULT 0,

    -- Caractéristiques au choix (ex: Humain variant)
    ability_choice_count INTEGER DEFAULT 0,
    ability_choice_value INTEGER DEFAULT 0,

    -- Vitesse et taille
    base_speed INTEGER DEFAULT 30,
    size VARCHAR(20) DEFAULT 'Medium', -- Small, Medium, Large

    -- Langues
    languages TEXT[], -- Array de langues connues
    language_choice_count INTEGER DEFAULT 0,

    -- Méta
    source VARCHAR(100) DEFAULT 'Player Handbook',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Traits raciaux (Vision dans le noir, Chanceux, etc.)
CREATE TABLE racial_traits (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    race_id UUID REFERENCES races(id) ON DELETE CASCADE,
    name VARCHAR(255) NOT NULL,
    name_fr VARCHAR(255) NOT NULL,
    description TEXT NOT NULL,
    trait_type VARCHAR(50), -- passive, active, spell, resistance
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Sous-races (High Elf, Wood Elf, etc.)
CREATE TABLE subraces (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    parent_race_id UUID REFERENCES races(id) ON DELETE CASCADE,
    name VARCHAR(100) NOT NULL,
    name_fr VARCHAR(100) NOT NULL,
    description TEXT,

    -- Bonus additionnels
    str_bonus INTEGER DEFAULT 0,
    dex_bonus INTEGER DEFAULT 0,
    con_bonus INTEGER DEFAULT 0,
    int_bonus INTEGER DEFAULT 0,
    wis_bonus INTEGER DEFAULT 0,
    cha_bonus INTEGER DEFAULT 0,

    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    UNIQUE(parent_race_id, name)
);

-- Traits de sous-race
CREATE TABLE subrace_traits (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    subrace_id UUID REFERENCES subraces(id) ON DELETE CASCADE,
    name VARCHAR(255) NOT NULL,
    name_fr VARCHAR(255) NOT NULL,
    description TEXT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- ============================================================================
-- CLASSES ET PROGRESSION
-- ============================================================================

CREATE TABLE classes (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    name VARCHAR(100) NOT NULL UNIQUE,
    name_fr VARCHAR(100) NOT NULL,
    description TEXT,

    -- Dé de vie
    hit_die INTEGER NOT NULL CHECK (hit_die IN (6, 8, 10, 12)),

    -- Caractéristique principale
    primary_ability VARCHAR(20), -- STR, DEX, CON, INT, WIS, CHA

    -- Maîtrises de jets de sauvegarde
    saving_throw_proficiencies VARCHAR(20)[], -- Array ['STR', 'CON']

    -- Nombre de compétences à choisir
    skill_choices_count INTEGER NOT NULL,

    -- Type de lanceur de sorts
    spellcasting_ability VARCHAR(20), -- NULL, INT, WIS, CHA
    is_full_caster BOOLEAN DEFAULT false,
    is_half_caster BOOLEAN DEFAULT false,
    is_third_caster BOOLEAN DEFAULT false,

    -- Méta
    source VARCHAR(100) DEFAULT 'Player Handbook',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Compétences disponibles par classe
CREATE TABLE class_skill_choices (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    class_id UUID REFERENCES classes(id) ON DELETE CASCADE,
    skill_name VARCHAR(50) NOT NULL,
    UNIQUE(class_id, skill_name)
);

-- Progression de classe par niveau
CREATE TABLE class_features_by_level (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    class_id UUID REFERENCES classes(id) ON DELETE CASCADE,
    level INTEGER NOT NULL CHECK (level >= 1 AND level <= 20),

    -- Bonus de maîtrise
    proficiency_bonus INTEGER NOT NULL,

    -- Capacités gagnées à ce niveau
    features_gained TEXT[], -- Array de noms de capacités

    -- Emplacements de sorts par niveau (pour lanceurs)
    spell_slots_1 INTEGER DEFAULT 0,
    spell_slots_2 INTEGER DEFAULT 0,
    spell_slots_3 INTEGER DEFAULT 0,
    spell_slots_4 INTEGER DEFAULT 0,
    spell_slots_5 INTEGER DEFAULT 0,
    spell_slots_6 INTEGER DEFAULT 0,
    spell_slots_7 INTEGER DEFAULT 0,
    spell_slots_8 INTEGER DEFAULT 0,
    spell_slots_9 INTEGER DEFAULT 0,

    -- Sorts connus (pour certaines classes)
    spells_known INTEGER DEFAULT 0,
    cantrips_known INTEGER DEFAULT 0,

    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    UNIQUE(class_id, level)
);

-- Capacités de classe
CREATE TABLE class_features (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    class_id UUID REFERENCES classes(id) ON DELETE CASCADE,
    name VARCHAR(255) NOT NULL,
    name_fr VARCHAR(255) NOT NULL,
    level_required INTEGER NOT NULL CHECK (level_required >= 1 AND level_required <= 20),
    description TEXT NOT NULL,
    feature_type VARCHAR(50), -- passive, active, spell, upgrade
    uses_per_rest VARCHAR(20), -- NULL, 'short', 'long', 'unlimited'
    uses_count INTEGER, -- Nombre d'utilisations si limité
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Archétypes/Sous-classes
CREATE TABLE archetypes (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    class_id UUID REFERENCES classes(id) ON DELETE CASCADE,
    name VARCHAR(255) NOT NULL,
    name_fr VARCHAR(255) NOT NULL,
    description TEXT,
    level_available INTEGER DEFAULT 3, -- Niveau où on choisit l'archétype
    source VARCHAR(100) DEFAULT 'Player Handbook',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Capacités d'archétype
CREATE TABLE archetype_features (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    archetype_id UUID REFERENCES archetypes(id) ON DELETE CASCADE,
    name VARCHAR(255) NOT NULL,
    name_fr VARCHAR(255) NOT NULL,
    level_required INTEGER NOT NULL,
    description TEXT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- ============================================================================
-- COMPÉTENCES (SKILLS)
-- ============================================================================

CREATE TABLE skills (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    name VARCHAR(100) NOT NULL UNIQUE,
    name_fr VARCHAR(100) NOT NULL,
    ability VARCHAR(20) NOT NULL, -- STR, DEX, CON, INT, WIS, CHA
    description TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- ============================================================================
-- ARMES
-- ============================================================================

CREATE TABLE weapons (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    name VARCHAR(255) NOT NULL,
    name_fr VARCHAR(255) NOT NULL,
    weapon_type VARCHAR(50) NOT NULL, -- Simple Melee, Martial Melee, Simple Ranged, Martial Ranged

    -- Dégâts
    damage_dice VARCHAR(20), -- 1d6, 2d4, etc.
    damage_type VARCHAR(20), -- slashing, piercing, bludgeoning

    -- Propriétés
    properties TEXT[], -- finesse, light, heavy, two-handed, versatile, etc.
    versatile_damage VARCHAR(20), -- Pour armes versatiles (1d10 si à 2 mains)

    -- Portée (pour armes à distance)
    range_normal INTEGER, -- en pieds
    range_long INTEGER,

    -- Poids et coût
    weight DECIMAL(5, 2), -- en livres
    cost_gp DECIMAL(10, 2),

    -- Méta
    description TEXT,
    source VARCHAR(100) DEFAULT 'Player Handbook',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- ============================================================================
-- ARMURES
-- ============================================================================

CREATE TABLE armors (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    name VARCHAR(255) NOT NULL,
    name_fr VARCHAR(255) NOT NULL,
    armor_type VARCHAR(50) NOT NULL, -- Light, Medium, Heavy, Shield

    -- Classe d'armure
    base_ac INTEGER NOT NULL,
    dex_bonus_max INTEGER, -- NULL = illimité, 2 = max +2
    adds_dex_bonus BOOLEAN DEFAULT false,

    -- Propriétés
    stealth_disadvantage BOOLEAN DEFAULT false,
    strength_requirement INTEGER, -- STR minimum requise

    -- Poids et coût
    weight DECIMAL(5, 2),
    cost_gp DECIMAL(10, 2),

    -- Méta
    description TEXT,
    source VARCHAR(100) DEFAULT 'Player Handbook',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- ============================================================================
-- ÉQUIPEMENT ET OBJETS
-- ============================================================================

CREATE TABLE equipment (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    name VARCHAR(255) NOT NULL,
    name_fr VARCHAR(255) NOT NULL,
    category VARCHAR(100), -- Tools, Adventuring Gear, Mounts, etc.
    subcategory VARCHAR(100),

    description TEXT,
    weight DECIMAL(5, 2),
    cost_gp DECIMAL(10, 2),

    -- Propriétés spéciales
    properties JSONB,

    source VARCHAR(100) DEFAULT 'Player Handbook',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Packs d'équipement (Explorer's Pack, Dungeoneer's Pack, etc.)
CREATE TABLE equipment_packs (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    name VARCHAR(255) NOT NULL,
    name_fr VARCHAR(255) NOT NULL,
    description TEXT,
    cost_gp DECIMAL(10, 2),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Contenu des packs
CREATE TABLE pack_contents (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    pack_id UUID REFERENCES equipment_packs(id) ON DELETE CASCADE,
    equipment_id UUID REFERENCES equipment(id) ON DELETE CASCADE,
    quantity INTEGER DEFAULT 1
);

-- ============================================================================
-- SORTS (SPELLS)
-- ============================================================================

CREATE TABLE game_spells (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    name VARCHAR(255) NOT NULL UNIQUE,
    name_fr VARCHAR(255) NOT NULL,
    level INTEGER NOT NULL CHECK (level >= 0 AND level <= 9),
    school VARCHAR(50) NOT NULL, -- Abjuration, Conjuration, Divination, etc.
    school_fr VARCHAR(50) NOT NULL,

    -- Lancement
    casting_time VARCHAR(100) NOT NULL, -- 1 action, 1 bonus action, etc.
    casting_time_fr VARCHAR(100) NOT NULL,
    range VARCHAR(100) NOT NULL, -- Self, Touch, 30 feet, etc.
    range_fr VARCHAR(100) NOT NULL,

    -- Composantes
    has_verbal BOOLEAN DEFAULT false,
    has_somatic BOOLEAN DEFAULT false,
    has_material BOOLEAN DEFAULT false,
    material_component TEXT,
    material_cost_gp DECIMAL(10, 2), -- Coût si matériau coûteux
    material_consumed BOOLEAN DEFAULT false,

    -- Durée
    duration VARCHAR(100) NOT NULL,
    duration_fr VARCHAR(100) NOT NULL,
    concentration BOOLEAN DEFAULT false,
    ritual BOOLEAN DEFAULT false,

    -- Effets
    description TEXT NOT NULL,
    description_fr TEXT NOT NULL,
    at_higher_levels TEXT,
    at_higher_levels_fr TEXT,

    -- Classes qui peuvent lancer
    available_to_classes VARCHAR(50)[], -- ['Wizard', 'Sorcerer', ...]

    -- Méta
    source VARCHAR(100) DEFAULT 'Player Handbook',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- ============================================================================
-- MONSTRES ET CRÉATURES
-- ============================================================================

CREATE TABLE monsters (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    name VARCHAR(255) NOT NULL,
    name_fr VARCHAR(255) NOT NULL,
    size VARCHAR(20) NOT NULL, -- Tiny, Small, Medium, Large, Huge, Gargantuan
    type VARCHAR(50) NOT NULL, -- Beast, Humanoid, Dragon, etc.
    subtype VARCHAR(50),
    alignment VARCHAR(50),

    -- Classe d'armure et PV
    armor_class INTEGER NOT NULL,
    hit_points_average INTEGER NOT NULL,
    hit_dice VARCHAR(20), -- 8d10, etc.

    -- Vitesse
    speed_walk INTEGER DEFAULT 30,
    speed_fly INTEGER,
    speed_swim INTEGER,
    speed_climb INTEGER,
    speed_burrow INTEGER,

    -- Caractéristiques
    strength INTEGER NOT NULL,
    dexterity INTEGER NOT NULL,
    constitution INTEGER NOT NULL,
    intelligence INTEGER NOT NULL,
    wisdom INTEGER NOT NULL,
    charisma INTEGER NOT NULL,

    -- Jets de sauvegarde maîtrisés
    saving_throws JSONB, -- {"dex": 5, "wis": 3}

    -- Compétences
    skills JSONB, -- {"perception": 5, "stealth": 7}

    -- Résistances et immunités
    damage_vulnerabilities TEXT[],
    damage_resistances TEXT[],
    damage_immunities TEXT[],
    condition_immunities TEXT[],

    -- Sens
    darkvision INTEGER, -- en pieds
    blindsight INTEGER,
    tremorsense INTEGER,
    truesight INTEGER,
    passive_perception INTEGER NOT NULL,

    -- Langues
    languages TEXT[],

    -- Challenge Rating
    challenge_rating VARCHAR(10) NOT NULL, -- 0, 1/8, 1/4, 1/2, 1, 2, etc.
    experience_points INTEGER NOT NULL,

    -- Description
    description TEXT,

    source VARCHAR(100) DEFAULT 'Monster Manual',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Traits spéciaux des monstres
CREATE TABLE monster_traits (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    monster_id UUID REFERENCES monsters(id) ON DELETE CASCADE,
    name VARCHAR(255) NOT NULL,
    name_fr VARCHAR(255) NOT NULL,
    description TEXT NOT NULL,
    trait_type VARCHAR(50) DEFAULT 'special' -- special, legendary, lair
);

-- Actions des monstres
CREATE TABLE monster_actions (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    monster_id UUID REFERENCES monsters(id) ON DELETE CASCADE,
    name VARCHAR(255) NOT NULL,
    name_fr VARCHAR(255) NOT NULL,
    description TEXT NOT NULL,
    action_type VARCHAR(50) DEFAULT 'action', -- action, bonus_action, reaction, legendary
    attack_bonus INTEGER,
    damage_dice VARCHAR(50),
    damage_type VARCHAR(50)
);

-- ============================================================================
-- OBJETS MAGIQUES
-- ============================================================================

CREATE TABLE magic_items (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    name VARCHAR(255) NOT NULL,
    name_fr VARCHAR(255) NOT NULL,
    item_type VARCHAR(100), -- Weapon, Armor, Wondrous Item, Potion, etc.
    rarity VARCHAR(50) NOT NULL, -- Common, Uncommon, Rare, Very Rare, Legendary, Artifact

    requires_attunement BOOLEAN DEFAULT false,
    attunement_requirements TEXT,

    description TEXT NOT NULL,
    description_fr TEXT NOT NULL,

    -- Propriétés
    properties JSONB,

    source VARCHAR(100) DEFAULT 'Dungeon Master Guide',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- ============================================================================
-- HISTORIQUES (BACKGROUNDS)
-- ============================================================================

CREATE TABLE backgrounds (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    name VARCHAR(255) NOT NULL UNIQUE,
    name_fr VARCHAR(255) NOT NULL,
    description TEXT,

    -- Maîtrises de compétences
    skill_proficiencies VARCHAR(50)[],

    -- Maîtrises d'outils
    tool_proficiencies TEXT[],

    -- Langues
    languages_count INTEGER DEFAULT 0,

    -- Équipement de départ
    starting_equipment TEXT,
    starting_gold DECIMAL(10, 2),

    -- Capacité spéciale
    feature_name VARCHAR(255),
    feature_name_fr VARCHAR(255),
    feature_description TEXT,

    -- Personnalité suggérée
    suggested_characteristics TEXT,

    source VARCHAR(100) DEFAULT 'Player Handbook',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- ============================================================================
-- DONS (FEATS)
-- ============================================================================

CREATE TABLE feats (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    name VARCHAR(255) NOT NULL UNIQUE,
    name_fr VARCHAR(255) NOT NULL,
    description TEXT NOT NULL,
    description_fr TEXT NOT NULL,

    -- Prérequis
    prerequisite TEXT,
    prerequisite_fr TEXT,

    -- Bonus de caractéristiques (si le don en donne)
    ability_increase_count INTEGER DEFAULT 0,

    source VARCHAR(100) DEFAULT 'Player Handbook',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- ============================================================================
-- CONDITIONS
-- ============================================================================

CREATE TABLE conditions (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    name VARCHAR(50) NOT NULL UNIQUE,
    name_fr VARCHAR(50) NOT NULL,
    description TEXT NOT NULL,
    description_fr TEXT NOT NULL,
    effects TEXT[] NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- ============================================================================
-- INDEXES POUR PERFORMANCE
-- ============================================================================

-- Races
CREATE INDEX idx_races_name ON races(name);
CREATE INDEX idx_racial_traits_race ON racial_traits(race_id);
CREATE INDEX idx_subraces_parent ON subraces(parent_race_id);

-- Classes
CREATE INDEX idx_classes_name ON classes(name);
CREATE INDEX idx_class_features_class_level ON class_features_by_level(class_id, level);
CREATE INDEX idx_class_features_class ON class_features(class_id);
CREATE INDEX idx_archetypes_class ON archetypes(class_id);

-- Combat
CREATE INDEX idx_weapons_type ON weapons(weapon_type);
CREATE INDEX idx_armors_type ON armors(armor_type);

-- Sorts
CREATE INDEX idx_game_spells_level ON game_spells(level);
CREATE INDEX idx_game_spells_school ON game_spells(school);
CREATE INDEX idx_game_spells_classes ON game_spells USING GIN(available_to_classes);

-- Monstres
CREATE INDEX idx_monsters_type ON monsters(type);
CREATE INDEX idx_monsters_cr ON monsters(challenge_rating);
CREATE INDEX idx_monster_traits_monster ON monster_traits(monster_id);
CREATE INDEX idx_monster_actions_monster ON monster_actions(monster_id);

-- Objets magiques
CREATE INDEX idx_magic_items_rarity ON magic_items(rarity);
CREATE INDEX idx_magic_items_type ON magic_items(item_type);
