-- D&D DM Toolbox Database Schema
-- Version: 1.0.0

-- Enable UUID extension
CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

-- ============================================================================
-- CORE TABLES
-- ============================================================================

-- Campaigns
CREATE TABLE campaigns (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    name VARCHAR(255) NOT NULL,
    description TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    is_active BOOLEAN DEFAULT true
);

-- Characters
CREATE TABLE characters (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    campaign_id UUID REFERENCES campaigns(id) ON DELETE CASCADE,
    name VARCHAR(255) NOT NULL,
    race VARCHAR(100) NOT NULL,
    class VARCHAR(100) NOT NULL,
    level INTEGER DEFAULT 1 CHECK (level >= 1 AND level <= 20),
    background VARCHAR(100),
    alignment VARCHAR(50),
    experience_points INTEGER DEFAULT 0,

    -- Ability Scores
    strength INTEGER DEFAULT 10 CHECK (strength >= 1 AND strength <= 30),
    dexterity INTEGER DEFAULT 10 CHECK (dexterity >= 1 AND dexterity <= 30),
    constitution INTEGER DEFAULT 10 CHECK (constitution >= 1 AND constitution <= 30),
    intelligence INTEGER DEFAULT 10 CHECK (intelligence >= 1 AND intelligence <= 30),
    wisdom INTEGER DEFAULT 10 CHECK (wisdom >= 1 AND wisdom <= 30),
    charisma INTEGER DEFAULT 10 CHECK (charisma >= 1 AND charisma <= 30),

    -- Combat Stats
    armor_class INTEGER DEFAULT 10,
    initiative_bonus INTEGER DEFAULT 0,
    speed INTEGER DEFAULT 30,
    max_hit_points INTEGER NOT NULL,
    current_hit_points INTEGER NOT NULL,
    temporary_hit_points INTEGER DEFAULT 0,

    -- Meta
    is_npc BOOLEAN DEFAULT false,
    notes TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Character Skills (Proficiencies)
CREATE TABLE character_skills (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    character_id UUID REFERENCES characters(id) ON DELETE CASCADE,
    skill_name VARCHAR(50) NOT NULL,
    is_proficient BOOLEAN DEFAULT false,
    is_expert BOOLEAN DEFAULT false,
    UNIQUE(character_id, skill_name)
);

-- Inventory
CREATE TABLE inventory_items (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    character_id UUID REFERENCES characters(id) ON DELETE CASCADE,
    name VARCHAR(255) NOT NULL,
    description TEXT,
    item_type VARCHAR(50), -- weapon, armor, consumable, etc.
    quantity INTEGER DEFAULT 1,
    weight DECIMAL(10, 2),
    value_gold DECIMAL(10, 2),
    is_equipped BOOLEAN DEFAULT false,
    properties JSONB, -- For storing item-specific properties
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Spells
CREATE TABLE spells (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    name VARCHAR(255) NOT NULL UNIQUE,
    level INTEGER CHECK (level >= 0 AND level <= 9),
    school VARCHAR(50), -- evocation, abjuration, etc.
    casting_time VARCHAR(100),
    range VARCHAR(100),
    components VARCHAR(10), -- V, S, M
    material_component TEXT,
    duration VARCHAR(100),
    concentration BOOLEAN DEFAULT false,
    ritual BOOLEAN DEFAULT false,
    description TEXT,
    higher_levels TEXT,
    classes TEXT[], -- Array of class names that can cast this spell
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Character Spells (Known/Prepared)
CREATE TABLE character_spells (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    character_id UUID REFERENCES characters(id) ON DELETE CASCADE,
    spell_id UUID REFERENCES spells(id) ON DELETE CASCADE,
    is_prepared BOOLEAN DEFAULT false,
    is_always_prepared BOOLEAN DEFAULT false, -- For domain spells, etc.
    UNIQUE(character_id, spell_id)
);

-- Spell Slots
CREATE TABLE character_spell_slots (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    character_id UUID REFERENCES characters(id) ON DELETE CASCADE,
    spell_level INTEGER CHECK (spell_level >= 1 AND spell_level <= 9),
    max_slots INTEGER NOT NULL,
    used_slots INTEGER DEFAULT 0,
    UNIQUE(character_id, spell_level)
);

-- ============================================================================
-- COMBAT TABLES
-- ============================================================================

-- Encounters
CREATE TABLE encounters (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    campaign_id UUID REFERENCES campaigns(id) ON DELETE CASCADE,
    name VARCHAR(255) NOT NULL,
    description TEXT,
    difficulty VARCHAR(20), -- easy, medium, hard, deadly
    is_active BOOLEAN DEFAULT false,
    round_number INTEGER DEFAULT 0,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    ended_at TIMESTAMP
);

-- Combat Participants
CREATE TABLE combat_participants (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    encounter_id UUID REFERENCES encounters(id) ON DELETE CASCADE,
    character_id UUID REFERENCES characters(id) ON DELETE CASCADE,
    initiative INTEGER NOT NULL,
    current_hp INTEGER NOT NULL,
    max_hp INTEGER NOT NULL,
    temp_hp INTEGER DEFAULT 0,
    armor_class INTEGER,
    is_surprised BOOLEAN DEFAULT false,
    turn_order INTEGER, -- Calculated from initiative
    has_acted_this_round BOOLEAN DEFAULT false
);

-- Combat Conditions
CREATE TABLE combat_conditions (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    participant_id UUID REFERENCES combat_participants(id) ON DELETE CASCADE,
    condition_name VARCHAR(50) NOT NULL, -- blinded, charmed, deafened, etc.
    duration_rounds INTEGER, -- NULL for indefinite
    description TEXT,
    applied_at_round INTEGER,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- ============================================================================
-- WORLD BUILDING TABLES
-- ============================================================================

-- Locations
CREATE TABLE locations (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    campaign_id UUID REFERENCES campaigns(id) ON DELETE CASCADE,
    name VARCHAR(255) NOT NULL,
    location_type VARCHAR(50), -- city, dungeon, wilderness, etc.
    description TEXT,
    parent_location_id UUID REFERENCES locations(id),
    notes TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Dungeons/Maps
CREATE TABLE maps (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    location_id UUID REFERENCES locations(id) ON DELETE CASCADE,
    name VARCHAR(255) NOT NULL,
    width INTEGER NOT NULL,
    height INTEGER NOT NULL,
    grid_size INTEGER DEFAULT 5, -- feet per grid square
    map_data JSONB, -- Serialized map data
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Quests
CREATE TABLE quests (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    campaign_id UUID REFERENCES campaigns(id) ON DELETE CASCADE,
    title VARCHAR(255) NOT NULL,
    description TEXT,
    quest_giver VARCHAR(255),
    reward TEXT,
    status VARCHAR(20) DEFAULT 'active', -- active, completed, failed
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    completed_at TIMESTAMP
);

-- Session Notes
CREATE TABLE session_notes (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    campaign_id UUID REFERENCES campaigns(id) ON DELETE CASCADE,
    session_number INTEGER,
    session_date DATE,
    title VARCHAR(255),
    summary TEXT,
    dm_notes TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- ============================================================================
-- INDEXES
-- ============================================================================

CREATE INDEX idx_characters_campaign ON characters(campaign_id);
CREATE INDEX idx_characters_name ON characters(name);
CREATE INDEX idx_inventory_character ON inventory_items(character_id);
CREATE INDEX idx_combat_participants_encounter ON combat_participants(encounter_id);
CREATE INDEX idx_combat_participants_initiative ON combat_participants(initiative DESC);
CREATE INDEX idx_encounters_campaign ON encounters(campaign_id);
CREATE INDEX idx_encounters_active ON encounters(is_active);
CREATE INDEX idx_maps_location ON maps(location_id);
CREATE INDEX idx_quests_campaign ON quests(campaign_id);
CREATE INDEX idx_quests_status ON quests(status);

-- ============================================================================
-- TRIGGERS
-- ============================================================================

-- Update updated_at timestamp
CREATE OR REPLACE FUNCTION update_updated_at_column()
RETURNS TRIGGER AS $$
BEGIN
    NEW.updated_at = CURRENT_TIMESTAMP;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER update_campaigns_updated_at BEFORE UPDATE ON campaigns
    FOR EACH ROW EXECUTE FUNCTION update_updated_at_column();

CREATE TRIGGER update_characters_updated_at BEFORE UPDATE ON characters
    FOR EACH ROW EXECUTE FUNCTION update_updated_at_column();

CREATE TRIGGER update_maps_updated_at BEFORE UPDATE ON maps
    FOR EACH ROW EXECUTE FUNCTION update_updated_at_column();

CREATE TRIGGER update_session_notes_updated_at BEFORE UPDATE ON session_notes
    FOR EACH ROW EXECUTE FUNCTION update_updated_at_column();
