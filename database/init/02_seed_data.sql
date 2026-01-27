-- D&D 5e Reference Data
-- This file contains seed data for core D&D 5e rules

-- ============================================================================
-- D&D 5e SKILLS
-- ============================================================================
-- Skills are stored in application code, but here's the reference list:
-- Acrobatics (DEX), Animal Handling (WIS), Arcana (INT), Athletics (STR),
-- Deception (CHA), History (INT), Insight (WIS), Intimidation (CHA),
-- Investigation (INT), Medicine (WIS), Nature (INT), Perception (WIS),
-- Performance (CHA), Persuasion (CHA), Religion (INT), Sleight of Hand (DEX),
-- Stealth (DEX), Survival (WIS)

-- ============================================================================
-- COMMON SPELLS (Sample Data)
-- ============================================================================

INSERT INTO spells (name, level, school, casting_time, range, components, material_component, duration, concentration, ritual, description, higher_levels, classes) VALUES
('Fire Bolt', 0, 'Evocation', '1 action', '120 feet', 'V,S', NULL, 'Instantaneous', false, false,
 'You hurl a mote of fire at a creature or object within range. Make a ranged spell attack against the target. On a hit, the target takes 1d10 fire damage. A flammable object hit by this spell ignites if it isn''t being worn or carried.',
 'This spell''s damage increases by 1d10 when you reach 5th level (2d10), 11th level (3d10), and 17th level (4d10).',
 ARRAY['Sorcerer', 'Wizard']),

('Magic Missile', 1, 'Evocation', '1 action', '120 feet', 'V,S', NULL, 'Instantaneous', false, false,
 'You create three glowing darts of magical force. Each dart hits a creature of your choice that you can see within range. A dart deals 1d4 + 1 force damage to its target. The darts all strike simultaneously, and you can direct them to hit one creature or several.',
 'When you cast this spell using a spell slot of 2nd level or higher, the spell creates one more dart for each slot level above 1st.',
 ARRAY['Sorcerer', 'Wizard']),

('Cure Wounds', 1, 'Evocation', '1 action', 'Touch', 'V,S', NULL, 'Instantaneous', false, false,
 'A creature you touch regains a number of hit points equal to 1d8 + your spellcasting ability modifier. This spell has no effect on undead or constructs.',
 'When you cast this spell using a spell slot of 2nd level or higher, the healing increases by 1d8 for each slot level above 1st.',
 ARRAY['Bard', 'Cleric', 'Druid', 'Paladin', 'Ranger']),

('Shield', 1, 'Abjuration', '1 reaction', 'Self', 'V,S', NULL, '1 round', false, false,
 'An invisible barrier of magical force appears and protects you. Until the start of your next turn, you have a +5 bonus to AC, including against the triggering attack, and you take no damage from magic missile.',
 NULL,
 ARRAY['Sorcerer', 'Wizard']),

('Fireball', 3, 'Evocation', '1 action', '150 feet', 'V,S,M', 'a tiny ball of bat guano and sulfur', 'Instantaneous', false, false,
 'A bright streak flashes from your pointing finger to a point you choose within range and then blossoms with a low roar into an explosion of flame. Each creature in a 20-foot-radius sphere centered on that point must make a Dexterity saving throw. A target takes 8d6 fire damage on a failed save, or half as much damage on a successful one.',
 'When you cast this spell using a spell slot of 4th level or higher, the damage increases by 1d6 for each slot level above 3rd.',
 ARRAY['Sorcerer', 'Wizard']),

('Healing Word', 1, 'Evocation', '1 bonus action', '60 feet', 'V', NULL, 'Instantaneous', false, false,
 'A creature of your choice that you can see within range regains hit points equal to 1d4 + your spellcasting ability modifier. This spell has no effect on undead or constructs.',
 'When you cast this spell using a spell slot of 2nd level or higher, the healing increases by 1d4 for each slot level above 1st.',
 ARRAY['Bard', 'Cleric', 'Druid']),

('Counterspell', 3, 'Abjuration', '1 reaction', '60 feet', 'S', NULL, 'Instantaneous', false, false,
 'You attempt to interrupt a creature in the process of casting a spell. If the creature is casting a spell of 3rd level or lower, its spell fails and has no effect. If it is casting a spell of 4th level or higher, make an ability check using your spellcasting ability. The DC equals 10 + the spell''s level. On a success, the creature''s spell fails and has no effect.',
 'When you cast this spell using a spell slot of 4th level or higher, the interrupted spell has no effect if its level is less than or equal to the level of the spell slot you used.',
 ARRAY['Sorcerer', 'Warlock', 'Wizard']),

('Detect Magic', 1, 'Divination', '1 action', 'Self', 'V,S', NULL, 'Concentration, up to 10 minutes', true, true,
 'For the duration, you sense the presence of magic within 30 feet of you. If you sense magic in this way, you can use your action to see a faint aura around any visible creature or object in the area that bears magic, and you learn its school of magic, if any.',
 NULL,
 ARRAY['Bard', 'Cleric', 'Druid', 'Paladin', 'Ranger', 'Sorcerer', 'Wizard']);

-- ============================================================================
-- DEMO CAMPAIGN & CHARACTERS
-- ============================================================================

-- Insert a demo campaign
INSERT INTO campaigns (id, name, description, is_active) VALUES
('00000000-0000-0000-0000-000000000001', 'Lost Mines of Phandelver', 'A classic D&D adventure for new adventurers', true);

-- Insert demo characters
INSERT INTO characters (id, campaign_id, name, race, class, level, background, alignment,
    strength, dexterity, constitution, intelligence, wisdom, charisma,
    armor_class, speed, max_hit_points, current_hit_points, is_npc) VALUES

('00000000-0000-0000-0000-000000000101', '00000000-0000-0000-0000-000000000001',
 'Thorin Stonehammer', 'Dwarf', 'Fighter', 3, 'Soldier', 'Lawful Good',
 16, 12, 16, 10, 13, 8,
 18, 25, 28, 28, false),

('00000000-0000-0000-0000-000000000102', '00000000-0000-0000-0000-000000000001',
 'Elara Moonwhisper', 'Elf', 'Wizard', 3, 'Sage', 'Neutral Good',
 8, 14, 12, 16, 13, 10,
 12, 30, 15, 15, false),

('00000000-0000-0000-0000-000000000103', '00000000-0000-0000-0000-000000000001',
 'Bran the Bold', 'Human', 'Rogue', 3, 'Criminal', 'Chaotic Neutral',
 10, 16, 13, 12, 14, 15,
 14, 30, 21, 21, false),

('00000000-0000-0000-0000-000000000104', '00000000-0000-0000-0000-000000000001',
 'Sister Miriam', 'Human', 'Cleric', 3, 'Acolyte', 'Lawful Good',
 14, 10, 14, 11, 16, 13,
 16, 30, 24, 24, false);

-- Insert character skills for demo characters
INSERT INTO character_skills (character_id, skill_name, is_proficient, is_expert) VALUES
-- Thorin (Fighter)
('00000000-0000-0000-0000-000000000101', 'Athletics', true, false),
('00000000-0000-0000-0000-000000000101', 'Intimidation', true, false),
('00000000-0000-0000-0000-000000000101', 'Survival', true, false),

-- Elara (Wizard)
('00000000-0000-0000-0000-000000000102', 'Arcana', true, false),
('00000000-0000-0000-0000-000000000102', 'History', true, false),
('00000000-0000-0000-0000-000000000102', 'Investigation', true, false),

-- Bran (Rogue)
('00000000-0000-0000-0000-000000000103', 'Stealth', true, true),
('00000000-0000-0000-0000-000000000103', 'Sleight of Hand', true, true),
('00000000-0000-0000-0000-000000000103', 'Deception', true, false),
('00000000-0000-0000-0000-000000000103', 'Perception', true, false),

-- Miriam (Cleric)
('00000000-0000-0000-0000-000000000104', 'Religion', true, false),
('00000000-0000-0000-0000-000000000104', 'Medicine', true, false),
('00000000-0000-0000-0000-000000000104', 'Insight', true, false);

-- Add some starting equipment
INSERT INTO inventory_items (character_id, name, description, item_type, quantity, weight, value_gold, is_equipped) VALUES
('00000000-0000-0000-0000-000000000101', 'Longsword', 'A well-balanced longsword', 'weapon', 1, 3.0, 15.0, true),
('00000000-0000-0000-0000-000000000101', 'Shield', 'A sturdy wooden shield', 'armor', 1, 6.0, 10.0, true),
('00000000-0000-0000-0000-000000000101', 'Chain Mail', 'Heavy armor', 'armor', 1, 55.0, 75.0, true),
('00000000-0000-0000-0000-000000000102', 'Spellbook', 'Contains known spells', 'equipment', 1, 3.0, 50.0, false),
('00000000-0000-0000-0000-000000000102', 'Quarterstaff', 'Simple weapon', 'weapon', 1, 4.0, 0.2, true),
('00000000-0000-0000-0000-000000000103', 'Shortsword', 'Quick and deadly', 'weapon', 1, 2.0, 10.0, true),
('00000000-0000-0000-0000-000000000103', 'Shortbow', 'Ranged weapon', 'weapon', 1, 2.0, 25.0, false),
('00000000-0000-0000-0000-000000000103', 'Thieves'' Tools', 'For picking locks', 'equipment', 1, 1.0, 25.0, false),
('00000000-0000-0000-0000-000000000104', 'Mace', 'Holy symbol attached', 'weapon', 1, 4.0, 5.0, true),
('00000000-0000-0000-0000-000000000104', 'Scale Mail', 'Medium armor', 'armor', 1, 45.0, 50.0, true),
('00000000-0000-0000-0000-000000000104', 'Healer''s Kit', '10 uses remaining', 'equipment', 1, 3.0, 5.0, false);

-- Grant Elara some known spells
INSERT INTO character_spells (character_id, spell_id, is_prepared)
SELECT '00000000-0000-0000-0000-000000000102', id, true
FROM spells WHERE name IN ('Fire Bolt', 'Magic Missile', 'Shield', 'Detect Magic');

-- Grant Miriam some known spells
INSERT INTO character_spells (character_id, spell_id, is_prepared, is_always_prepared)
SELECT '00000000-0000-0000-0000-000000000104', id, true, true
FROM spells WHERE name IN ('Cure Wounds', 'Healing Word');

-- Set up spell slots for level 3 casters
INSERT INTO character_spell_slots (character_id, spell_level, max_slots, used_slots) VALUES
-- Elara (Wizard level 3)
('00000000-0000-0000-0000-000000000102', 1, 4, 0),
('00000000-0000-0000-0000-000000000102', 2, 2, 0),
-- Miriam (Cleric level 3)
('00000000-0000-0000-0000-000000000104', 1, 4, 0),
('00000000-0000-0000-0000-000000000104', 2, 2, 0);

-- Add a demo quest
INSERT INTO quests (campaign_id, title, description, quest_giver, reward, status) VALUES
('00000000-0000-0000-0000-000000000001',
 'Find Gundren Rockseeker',
 'Gundren and his escort have gone missing on the Triboar Trail. Find them and ensure their safety.',
 'Sildar Hallwinter',
 '100 gold pieces and a favor from the Lord''s Alliance',
 'active');

COMMIT;
