-- ============================================================================
-- DONNÉES DES CLASSES D&D 5E
-- ============================================================================

-- ============================================================================
-- CLASSES DE BASE
-- ============================================================================

-- BARBARE
INSERT INTO classes (name, name_fr, description, hit_die, primary_ability, saving_throw_proficiencies, skill_choices_count)
VALUES (
    'Barbarian',
    'Barbare',
    'Guerrier féroce d''origine primitive qui entre en rage au combat.',
    12,
    'STR',
    ARRAY['STR', 'CON'],
    2
);

-- BARDE
INSERT INTO classes (name, name_fr, description, hit_die, primary_ability, saving_throw_proficiencies, skill_choices_count, spellcasting_ability, is_full_caster)
VALUES (
    'Bard',
    'Barde',
    'Lanceur de sorts inspirant qui utilise la musique et la magie.',
    8,
    'CHA',
    ARRAY['DEX', 'CHA'],
    3,
    'CHA',
    true
);

-- CLERC
INSERT INTO classes (name, name_fr, description, hit_die, primary_ability, saving_throw_proficiencies, skill_choices_count, spellcasting_ability, is_full_caster)
VALUES (
    'Cleric',
    'Clerc',
    'Prêtre champion d''une divinité, maniant magie divine et puissance spirituelle.',
    8,
    'WIS',
    ARRAY['WIS', 'CHA'],
    2,
    'WIS',
    true
);

-- DRUIDE
INSERT INTO classes (name, name_fr, description, hit_die, primary_ability, saving_throw_proficiencies, skill_choices_count, spellcasting_ability, is_full_caster)
VALUES (
    'Druid',
    'Druide',
    'Prêtre de la nature capable de se transformer en bêtes et lancer des sorts naturels.',
    8,
    'WIS',
    ARRAY['INT', 'WIS'],
    2,
    'WIS',
    true
);

-- GUERRIER
INSERT INTO classes (name, name_fr, description, hit_die, primary_ability, saving_throw_proficiencies, skill_choices_count)
VALUES (
    'Fighter',
    'Guerrier',
    'Maître du combat martial, expert dans diverses techniques d''armes et armures.',
    10,
    'STR',
    ARRAY['STR', 'CON'],
    2
);

-- MOINE
INSERT INTO classes (name, name_fr, description, hit_die, primary_ability, saving_throw_proficiencies, skill_choices_count)
VALUES (
    'Monk',
    'Moine',
    'Maître des arts martiaux qui canalise le ki pour accomplir des exploits surhumains.',
    8,
    'DEX',
    ARRAY['STR', 'DEX'],
    2
);

-- PALADIN
INSERT INTO classes (name, name_fr, description, hit_die, primary_ability, saving_throw_proficiencies, skill_choices_count, spellcasting_ability, is_half_caster)
VALUES (
    'Paladin',
    'Paladin',
    'Guerrier saint qui a prêté un serment sacré et manie pouvoir divin.',
    10,
    'STR',
    ARRAY['WIS', 'CHA'],
    2,
    'CHA',
    true
);

-- RÔDEUR
INSERT INTO classes (name, name_fr, description, hit_die, primary_ability, saving_throw_proficiencies, skill_choices_count, spellcasting_ability, is_half_caster)
VALUES (
    'Ranger',
    'Rôdeur',
    'Guerrier des terres sauvages qui traque ses ennemis jurés.',
    10,
    'DEX',
    ARRAY['STR', 'DEX'],
    3,
    'WIS',
    true
);

-- ROUBLARD
INSERT INTO classes (name, name_fr, description, hit_die, primary_ability, saving_throw_proficiencies, skill_choices_count)
VALUES (
    'Rogue',
    'Roublard',
    'Spécialiste de la discrétion et des attaques sournoises.',
    8,
    'DEX',
    ARRAY['DEX', 'INT'],
    4
);

-- ENSORCELEUR
INSERT INTO classes (name, name_fr, description, hit_die, primary_ability, saving_throw_proficiencies, skill_choices_count, spellcasting_ability, is_full_caster)
VALUES (
    'Sorcerer',
    'Ensorceleur',
    'Lanceur de sorts inné qui tire sa magie de son héritage.',
    6,
    'CHA',
    ARRAY['CON', 'CHA'],
    2,
    'CHA',
    true
);

-- SORCIER
INSERT INTO classes (name, name_fr, description, hit_die, primary_ability, saving_throw_proficiencies, skill_choices_count, spellcasting_ability, is_full_caster)
VALUES (
    'Warlock',
    'Sorcier',
    'Chercheur de savoir occulte lié à un puissant patron extraplanaire.',
    8,
    'CHA',
    ARRAY['WIS', 'CHA'],
    2,
    'CHA',
    true
);

-- MAGICIEN
INSERT INTO classes (name, name_fr, description, hit_die, primary_ability, saving_throw_proficiencies, skill_choices_count, spellcasting_ability, is_full_caster)
VALUES (
    'Wizard',
    'Magicien',
    'Érudit de la magie arcanique capable de manipuler les forces de l''univers.',
    6,
    'INT',
    ARRAY['INT', 'WIS'],
    2,
    'INT',
    true
);

-- ============================================================================
-- COMPÉTENCES DISPONIBLES PAR CLASSE
-- ============================================================================

-- BARBARE
INSERT INTO class_skill_choices (class_id, skill_name)
SELECT id, skill_name FROM classes, UNNEST(ARRAY['Animal Handling', 'Athletics', 'Intimidation', 'Nature', 'Perception', 'Survival']) AS skill_name
WHERE name = 'Barbarian';

-- BARDE (toutes les compétences)
INSERT INTO class_skill_choices (class_id, skill_name)
SELECT id, skill_name FROM classes,
UNNEST(ARRAY['Acrobatics', 'Animal Handling', 'Arcana', 'Athletics', 'Deception', 'History',
              'Insight', 'Intimidation', 'Investigation', 'Medicine', 'Nature', 'Perception',
              'Performance', 'Persuasion', 'Religion', 'Sleight of Hand', 'Stealth', 'Survival']) AS skill_name
WHERE name = 'Bard';

-- CLERC
INSERT INTO class_skill_choices (class_id, skill_name)
SELECT id, skill_name FROM classes, UNNEST(ARRAY['History', 'Insight', 'Medicine', 'Persuasion', 'Religion']) AS skill_name
WHERE name = 'Cleric';

-- DRUIDE
INSERT INTO class_skill_choices (class_id, skill_name)
SELECT id, skill_name FROM classes, UNNEST(ARRAY['Arcana', 'Animal Handling', 'Insight', 'Medicine', 'Nature', 'Perception', 'Religion', 'Survival']) AS skill_name
WHERE name = 'Druid';

-- GUERRIER
INSERT INTO class_skill_choices (class_id, skill_name)
SELECT id, skill_name FROM classes, UNNEST(ARRAY['Acrobatics', 'Animal Handling', 'Athletics', 'History', 'Insight', 'Intimidation', 'Perception', 'Survival']) AS skill_name
WHERE name = 'Fighter';

-- MOINE
INSERT INTO class_skill_choices (class_id, skill_name)
SELECT id, skill_name FROM classes, UNNEST(ARRAY['Acrobatics', 'Athletics', 'History', 'Insight', 'Religion', 'Stealth']) AS skill_name
WHERE name = 'Monk';

-- PALADIN
INSERT INTO class_skill_choices (class_id, skill_name)
SELECT id, skill_name FROM classes, UNNEST(ARRAY['Athletics', 'Insight', 'Intimidation', 'Medicine', 'Persuasion', 'Religion']) AS skill_name
WHERE name = 'Paladin';

-- RÔDEUR
INSERT INTO class_skill_choices (class_id, skill_name)
SELECT id, skill_name FROM classes, UNNEST(ARRAY['Animal Handling', 'Athletics', 'Insight', 'Investigation', 'Nature', 'Perception', 'Stealth', 'Survival']) AS skill_name
WHERE name = 'Ranger';

-- ROUBLARD
INSERT INTO class_skill_choices (class_id, skill_name)
SELECT id, skill_name FROM classes, UNNEST(ARRAY['Acrobatics', 'Athletics', 'Deception', 'Insight', 'Intimidation', 'Investigation', 'Perception', 'Performance', 'Persuasion', 'Sleight of Hand', 'Stealth']) AS skill_name
WHERE name = 'Rogue';

-- ENSORCELEUR
INSERT INTO class_skill_choices (class_id, skill_name)
SELECT id, skill_name FROM classes, UNNEST(ARRAY['Arcana', 'Deception', 'Insight', 'Intimidation', 'Persuasion', 'Religion']) AS skill_name
WHERE name = 'Sorcerer';

-- SORCIER
INSERT INTO class_skill_choices (class_id, skill_name)
SELECT id, skill_name FROM classes, UNNEST(ARRAY['Arcana', 'Deception', 'History', 'Intimidation', 'Investigation', 'Nature', 'Religion']) AS skill_name
WHERE name = 'Warlock';

-- MAGICIEN
INSERT INTO class_skill_choices (class_id, skill_name)
SELECT id, skill_name FROM classes, UNNEST(ARRAY['Arcana', 'History', 'Insight', 'Investigation', 'Medicine', 'Religion']) AS skill_name
WHERE name = 'Wizard';

-- ============================================================================
-- PROGRESSION PAR NIVEAU - GUERRIER (exemple complet)
-- ============================================================================

-- Niveau 1
INSERT INTO class_features_by_level (class_id, level, proficiency_bonus, features_gained)
SELECT id, 1, 2, ARRAY['Fighting Style', 'Second Wind']
FROM classes WHERE name = 'Fighter';

-- Niveau 2
INSERT INTO class_features_by_level (class_id, level, proficiency_bonus, features_gained)
SELECT id, 2, 2, ARRAY['Action Surge']
FROM classes WHERE name = 'Fighter';

-- Niveau 3
INSERT INTO class_features_by_level (class_id, level, proficiency_bonus, features_gained)
SELECT id, 3, 2, ARRAY['Martial Archetype']
FROM classes WHERE name = 'Fighter';

-- Niveaux 4-20 pour Guerrier
INSERT INTO class_features_by_level (class_id, level, proficiency_bonus, features_gained)
SELECT id, 4, 2, ARRAY['Ability Score Improvement'] FROM classes WHERE name = 'Fighter'
UNION ALL
SELECT id, 5, 3, ARRAY['Extra Attack'] FROM classes WHERE name = 'Fighter'
UNION ALL
SELECT id, 6, 3, ARRAY['Ability Score Improvement'] FROM classes WHERE name = 'Fighter'
UNION ALL
SELECT id, 7, 3, ARRAY['Martial Archetype Feature'] FROM classes WHERE name = 'Fighter'
UNION ALL
SELECT id, 8, 3, ARRAY['Ability Score Improvement'] FROM classes WHERE name = 'Fighter'
UNION ALL
SELECT id, 9, 4, ARRAY['Indomitable'] FROM classes WHERE name = 'Fighter'
UNION ALL
SELECT id, 10, 4, ARRAY['Martial Archetype Feature'] FROM classes WHERE name = 'Fighter'
UNION ALL
SELECT id, 11, 4, ARRAY['Extra Attack (2)'] FROM classes WHERE name = 'Fighter'
UNION ALL
SELECT id, 12, 4, ARRAY['Ability Score Improvement'] FROM classes WHERE name = 'Fighter'
UNION ALL
SELECT id, 13, 5, ARRAY['Indomitable (2 uses)'] FROM classes WHERE name = 'Fighter'
UNION ALL
SELECT id, 14, 5, ARRAY['Ability Score Improvement'] FROM classes WHERE name = 'Fighter'
UNION ALL
SELECT id, 15, 5, ARRAY['Martial Archetype Feature'] FROM classes WHERE name = 'Fighter'
UNION ALL
SELECT id, 16, 5, ARRAY['Ability Score Improvement'] FROM classes WHERE name = 'Fighter'
UNION ALL
SELECT id, 17, 6, ARRAY['Action Surge (2 uses)', 'Indomitable (3 uses)'] FROM classes WHERE name = 'Fighter'
UNION ALL
SELECT id, 18, 6, ARRAY['Martial Archetype Feature'] FROM classes WHERE name = 'Fighter'
UNION ALL
SELECT id, 19, 6, ARRAY['Ability Score Improvement'] FROM classes WHERE name = 'Fighter'
UNION ALL
SELECT id, 20, 6, ARRAY['Extra Attack (3)'] FROM classes WHERE name = 'Fighter';

-- ============================================================================
-- PROGRESSION MAGICIEN (lanceur complet)
-- ============================================================================

INSERT INTO class_features_by_level (class_id, level, proficiency_bonus, features_gained,
                                     spell_slots_1, spell_slots_2, spell_slots_3, spell_slots_4, spell_slots_5,
                                     spell_slots_6, spell_slots_7, spell_slots_8, spell_slots_9,
                                     cantrips_known)
SELECT id, 1, 2, ARRAY['Spellcasting', 'Arcane Recovery'], 2, 0, 0, 0, 0, 0, 0, 0, 0, 3 FROM classes WHERE name = 'Wizard'
UNION ALL
SELECT id, 2, 2, ARRAY['Arcane Tradition'], 3, 0, 0, 0, 0, 0, 0, 0, 0, 3 FROM classes WHERE name = 'Wizard'
UNION ALL
SELECT id, 3, 2, ARRAY[], 4, 2, 0, 0, 0, 0, 0, 0, 0, 3 FROM classes WHERE name = 'Wizard'
UNION ALL
SELECT id, 4, 2, ARRAY['Ability Score Improvement'], 4, 3, 0, 0, 0, 0, 0, 0, 0, 4 FROM classes WHERE name = 'Wizard'
UNION ALL
SELECT id, 5, 3, ARRAY[], 4, 3, 2, 0, 0, 0, 0, 0, 0, 4 FROM classes WHERE name = 'Wizard'
UNION ALL
SELECT id, 6, 3, ARRAY['Arcane Tradition Feature'], 4, 3, 3, 0, 0, 0, 0, 0, 0, 4 FROM classes WHERE name = 'Wizard'
UNION ALL
SELECT id, 7, 3, ARRAY[], 4, 3, 3, 1, 0, 0, 0, 0, 0, 4 FROM classes WHERE name = 'Wizard'
UNION ALL
SELECT id, 8, 3, ARRAY['Ability Score Improvement'], 4, 3, 3, 2, 0, 0, 0, 0, 0, 4 FROM classes WHERE name = 'Wizard'
UNION ALL
SELECT id, 9, 4, ARRAY[], 4, 3, 3, 3, 1, 0, 0, 0, 0, 4 FROM classes WHERE name = 'Wizard'
UNION ALL
SELECT id, 10, 4, ARRAY['Arcane Tradition Feature'], 4, 3, 3, 3, 2, 0, 0, 0, 0, 5 FROM classes WHERE name = 'Wizard'
UNION ALL
SELECT id, 11, 4, ARRAY[], 4, 3, 3, 3, 2, 1, 0, 0, 0, 5 FROM classes WHERE name = 'Wizard'
UNION ALL
SELECT id, 12, 4, ARRAY['Ability Score Improvement'], 4, 3, 3, 3, 2, 1, 0, 0, 0, 5 FROM classes WHERE name = 'Wizard'
UNION ALL
SELECT id, 13, 5, ARRAY[], 4, 3, 3, 3, 2, 1, 1, 0, 0, 5 FROM classes WHERE name = 'Wizard'
UNION ALL
SELECT id, 14, 5, ARRAY['Arcane Tradition Feature'], 4, 3, 3, 3, 2, 1, 1, 0, 0, 5 FROM classes WHERE name = 'Wizard'
UNION ALL
SELECT id, 15, 5, ARRAY[], 4, 3, 3, 3, 2, 1, 1, 1, 0, 5 FROM classes WHERE name = 'Wizard'
UNION ALL
SELECT id, 16, 5, ARRAY['Ability Score Improvement'], 4, 3, 3, 3, 2, 1, 1, 1, 0, 5 FROM classes WHERE name = 'Wizard'
UNION ALL
SELECT id, 17, 6, ARRAY[], 4, 3, 3, 3, 2, 1, 1, 1, 1, 5 FROM classes WHERE name = 'Wizard'
UNION ALL
SELECT id, 18, 6, ARRAY['Spell Mastery'], 4, 3, 3, 3, 3, 1, 1, 1, 1, 5 FROM classes WHERE name = 'Wizard'
UNION ALL
SELECT id, 19, 6, ARRAY['Ability Score Improvement'], 4, 3, 3, 3, 3, 2, 1, 1, 1, 5 FROM classes WHERE name = 'Wizard'
UNION ALL
SELECT id, 20, 6, ARRAY['Signature Spells'], 4, 3, 3, 3, 3, 2, 2, 1, 1, 5 FROM classes WHERE name = 'Wizard';

-- ============================================================================
-- CAPACITÉS DE CLASSE - Exemples
-- ============================================================================

-- GUERRIER - Second Wind
INSERT INTO class_features (class_id, name, name_fr, level_required, description, feature_type, uses_per_rest, uses_count)
SELECT id, 'Second Wind', 'Second souffle', 1,
'Vous pouvez utiliser une action bonus pour regagner des points de vie égaux à 1d10 + votre niveau de guerrier.',
'active', 'short', 1
FROM classes WHERE name = 'Fighter';

-- GUERRIER - Action Surge
INSERT INTO class_features (class_id, name, name_fr, level_required, description, feature_type, uses_per_rest, uses_count)
SELECT id, 'Action Surge', 'Fougue', 2,
'Vous pouvez effectuer une action supplémentaire lors de votre tour (en plus de votre action normale et éventuelle action bonus).',
'active', 'short', 1
FROM classes WHERE name = 'Fighter';

-- ROUBLARD - Sneak Attack
INSERT INTO class_features (class_id, name, name_fr, level_required, description, feature_type, uses_per_rest)
SELECT id, 'Sneak Attack', 'Attaque sournoise', 1,
'Une fois par tour, vous infligez 1d6 dégâts supplémentaires à une créature touchée si vous avez l''avantage ou si un allié est à 5 pieds d''elle. Augmente de 1d6 tous les 2 niveaux.',
'passive', 'unlimited'
FROM classes WHERE name = 'Rogue';

-- ROUBLARD - Expertise
INSERT INTO class_features (class_id, name, name_fr, level_required, description, feature_type, uses_per_rest)
SELECT id, 'Expertise', 'Expertise', 1,
'Choisissez deux maîtrises de compétences ou d''outils. Votre bonus de maîtrise est doublé pour ces compétences.',
'passive', 'unlimited'
FROM classes WHERE name = 'Rogue';

-- BARBARE - Rage
INSERT INTO class_features (class_id, name, name_fr, level_required, description, feature_type, uses_per_rest, uses_count)
SELECT id, 'Rage', 'Rage', 1,
'En action bonus, entrez en rage pour 1 minute. Avantage aux jets de FOR, +2 dégâts au corps-à-corps, résistance contondants/perforants/tranchants.',
'active', 'long', 2
FROM classes WHERE name = 'Barbarian';

-- BARBARE - Défense sans armure
INSERT INTO class_features (class_id, name, name_fr, level_required, description, feature_type, uses_per_rest)
SELECT id, 'Unarmored Defense', 'Défense sans armure', 1,
'Quand vous ne portez pas d''armure, votre CA = 10 + modificateur DEX + modificateur CON.',
'passive', 'unlimited'
FROM classes WHERE name = 'Barbarian';
