-- ============================================================================
-- DONNÉES DES RACES D&D 5E
-- ============================================================================

-- ============================================================================
-- RACES DE BASE
-- ============================================================================

-- Humain
INSERT INTO races (name, name_fr, description, str_bonus, dex_bonus, con_bonus, int_bonus, wis_bonus, cha_bonus, base_speed, size, languages, language_choice_count)
VALUES (
    'Human',
    'Humain',
    'Les humains sont la race la plus adaptable et ambitieuse. Polyvalents et diversifiés.',
    1, 1, 1, 1, 1, 1,
    30,
    'Medium',
    ARRAY['Common'],
    1
);

-- Elfe
INSERT INTO races (name, name_fr, description, dex_bonus, base_speed, size, languages)
VALUES (
    'Elf',
    'Elfe',
    'Gracieux et agiles, les elfes vivent en harmonie avec la nature. Vision dans le noir, immunité au sommeil magique.',
    2,
    30,
    'Medium',
    ARRAY['Common', 'Elvish']
);

-- Nain
INSERT INTO races (name, name_fr, description, con_bonus, base_speed, size, languages)
VALUES (
    'Dwarf',
    'Nain',
    'Robustes et endurants, les nains sont de fiers artisans et guerriers. Résistance au poison.',
    2,
    25,
    'Medium',
    ARRAY['Common', 'Dwarvish']
);

-- Halfelin
INSERT INTO races (name, name_fr, description, dex_bonus, base_speed, size, languages)
VALUES (
    'Halfling',
    'Halfelin',
    'Petits mais courageux, les halfelins sont chanceux et agiles. Capacité Chanceux unique.',
    2,
    25,
    'Small',
    ARRAY['Common', 'Halfling']
);

-- Drakéide
INSERT INTO races (name, name_fr, description, str_bonus, cha_bonus, base_speed, size, languages)
VALUES (
    'Dragonborn',
    'Drakéide',
    'Descendants de dragons, les drakéides sont fiers et puissants. Souffle draconique et résistance élémentaire.',
    2,
    1,
    30,
    'Medium',
    ARRAY['Common', 'Draconic']
);

-- Gnome
INSERT INTO races (name, name_fr, description, int_bonus, base_speed, size, languages)
VALUES (
    'Gnome',
    'Gnome',
    'Curieux et inventifs, les gnomes excellent en magie et artifice. Vision dans le noir et ruse gnome.',
    2,
    25,
    'Small',
    ARRAY['Common', 'Gnomish']
);

-- Demi-Elfe
INSERT INTO races (name, name_fr, description, cha_bonus, ability_choice_count, ability_choice_value, base_speed, size, languages, language_choice_count)
VALUES (
    'Half-Elf',
    'Demi-Elfe',
    'Mélange d''humain et d''elfe, les demi-elfes sont charismatiques et polyvalents. +2 CHA et +1 à deux autres caractéristiques.',
    2,
    2,
    1,
    30,
    'Medium',
    ARRAY['Common', 'Elvish'],
    1
);

-- Demi-Orc
INSERT INTO races (name, name_fr, description, str_bonus, con_bonus, base_speed, size, languages)
VALUES (
    'Half-Orc',
    'Demi-Orc',
    'Force brutale et ténacité. Les demi-orcs ne tombent pas facilement. Ténacité implacable.',
    2,
    1,
    30,
    'Medium',
    ARRAY['Common', 'Orc']
);

-- Tieffelin
INSERT INTO races (name, name_fr, description, int_bonus, cha_bonus, base_speed, size, languages)
VALUES (
    'Tiefling',
    'Tieffelin',
    'Héritage infernal visible, les tieffelins sont résistants au feu et possèdent une magie innée.',
    1,
    2,
    30,
    'Medium',
    ARRAY['Common', 'Infernal']
);

-- ============================================================================
-- TRAITS RACIAUX
-- ============================================================================

-- HUMAIN
INSERT INTO racial_traits (race_id, name, name_fr, description, trait_type)
SELECT id, 'Versatile', 'Polyvalent', '+1 à toutes les caractéristiques.', 'passive'
FROM races WHERE name = 'Human';

-- ELFE
INSERT INTO racial_traits (race_id, name, name_fr, description, trait_type)
SELECT id, 'Darkvision', 'Vision dans le noir', 'Vision dans le noir jusqu''à 60 pieds.', 'passive'
FROM races WHERE name = 'Elf';

INSERT INTO racial_traits (race_id, name, name_fr, description, trait_type)
SELECT id, 'Fey Ancestry', 'Ascendance féerique', 'Avantage contre le charme, immunité au sommeil magique.', 'passive'
FROM races WHERE name = 'Elf';

INSERT INTO racial_traits (race_id, name, name_fr, description, trait_type)
SELECT id, 'Trance', 'Transe', 'Méditation de 4 heures au lieu de 8 heures de sommeil.', 'passive'
FROM races WHERE name = 'Elf';

INSERT INTO racial_traits (race_id, name, name_fr, description, trait_type)
SELECT id, 'Keen Senses', 'Sens aiguisés', 'Maîtrise de la compétence Perception.', 'passive'
FROM races WHERE name = 'Elf';

-- NAIN
INSERT INTO racial_traits (race_id, name, name_fr, description, trait_type)
SELECT id, 'Darkvision', 'Vision dans le noir', 'Vision dans le noir jusqu''à 60 pieds.', 'passive'
FROM races WHERE name = 'Dwarf';

INSERT INTO racial_traits (race_id, name, name_fr, description, trait_type)
SELECT id, 'Dwarven Resilience', 'Résistance naine', 'Avantage aux JdS contre le poison, résistance aux dégâts de poison.', 'resistance'
FROM races WHERE name = 'Dwarf';

INSERT INTO racial_traits (race_id, name, name_fr, description, trait_type)
SELECT id, 'Stonecunning', 'Connaissance de la pierre', 'Double bonus de maîtrise pour Histoire liée à la pierre.', 'passive'
FROM races WHERE name = 'Dwarf';

INSERT INTO racial_traits (race_id, name, name_fr, description, trait_type)
SELECT id, 'Dwarven Combat Training', 'Entraînement aux armes naines', 'Maîtrise des haches de guerre, hachettes, marteaux légers et marteaux de guerre.', 'passive'
FROM races WHERE name = 'Dwarf';

-- HALFELIN
INSERT INTO racial_traits (race_id, name, name_fr, description, trait_type)
SELECT id, 'Lucky', 'Chanceux', 'Relancez un 1 sur un d20 pour attaque, carac, ou JdS.', 'passive'
FROM races WHERE name = 'Halfling';

INSERT INTO racial_traits (race_id, name, name_fr, description, trait_type)
SELECT id, 'Brave', 'Brave', 'Avantage aux JdS contre la peur.', 'passive'
FROM races WHERE name = 'Halfling';

INSERT INTO racial_traits (race_id, name, name_fr, description, trait_type)
SELECT id, 'Halfling Nimbleness', 'Agilité halfeline', 'Vous pouvez vous déplacer dans l''espace de créatures plus grandes.', 'passive'
FROM races WHERE name = 'Halfling';

-- DRAKÉIDE
INSERT INTO racial_traits (race_id, name, name_fr, description, trait_type)
SELECT id, 'Draconic Ancestry', 'Ascendance draconique', 'Choisissez un type de dragon pour déterminer votre souffle et résistance.', 'passive'
FROM races WHERE name = 'Dragonborn';

INSERT INTO racial_traits (race_id, name, name_fr, description, trait_type)
SELECT id, 'Breath Weapon', 'Souffle draconique', 'Exhalez énergie destructrice (utilisable 1/repos court). JdS DEX ou CON DD 8+CON+maîtrise. 2d6 dégâts (augmente avec niveaux).', 'active'
FROM races WHERE name = 'Dragonborn';

INSERT INTO racial_traits (race_id, name, name_fr, description, trait_type)
SELECT id, 'Damage Resistance', 'Résistance aux dégâts', 'Résistance au type de dégâts de votre ascendance draconique.', 'resistance'
FROM races WHERE name = 'Dragonborn';

-- GNOME
INSERT INTO racial_traits (race_id, name, name_fr, description, trait_type)
SELECT id, 'Darkvision', 'Vision dans le noir', 'Vision dans le noir jusqu''à 60 pieds.', 'passive'
FROM races WHERE name = 'Gnome';

INSERT INTO racial_traits (race_id, name, name_fr, description, trait_type)
SELECT id, 'Gnome Cunning', 'Ruse gnome', 'Avantage aux JdS d''INT, SAG et CHA contre la magie.', 'passive'
FROM races WHERE name = 'Gnome';

-- DEMI-ELFE
INSERT INTO racial_traits (race_id, name, name_fr, description, trait_type)
SELECT id, 'Darkvision', 'Vision dans le noir', 'Vision dans le noir jusqu''à 60 pieds.', 'passive'
FROM races WHERE name = 'Half-Elf';

INSERT INTO racial_traits (race_id, name, name_fr, description, trait_type)
SELECT id, 'Fey Ancestry', 'Ascendance féerique', 'Avantage contre le charme, immunité au sommeil magique.', 'passive'
FROM races WHERE name = 'Half-Elf';

INSERT INTO racial_traits (race_id, name, name_fr, description, trait_type)
SELECT id, 'Skill Versatility', 'Polyvalence', 'Maîtrise de deux compétences au choix.', 'passive'
FROM races WHERE name = 'Half-Elf';

-- DEMI-ORC
INSERT INTO racial_traits (race_id, name, name_fr, description, trait_type)
SELECT id, 'Darkvision', 'Vision dans le noir', 'Vision dans le noir jusqu''à 60 pieds.', 'passive'
FROM races WHERE name = 'Half-Orc';

INSERT INTO racial_traits (race_id, name, name_fr, description, trait_type)
SELECT id, 'Relentless Endurance', 'Ténacité implacable', 'Quand réduit à 0 PV mais pas tué, descendez à 1 PV (1/repos long).', 'active'
FROM races WHERE name = 'Half-Orc';

INSERT INTO racial_traits (race_id, name, name_fr, description, trait_type)
SELECT id, 'Savage Attacks', 'Attaques sauvages', 'Lors d''un coup critique au corps-à-corps, lancez un dé de dégâts de l''arme supplémentaire.', 'passive'
FROM races WHERE name = 'Half-Orc';

-- TIEFFELIN
INSERT INTO racial_traits (race_id, name, name_fr, description, trait_type)
SELECT id, 'Darkvision', 'Vision dans le noir', 'Vision dans le noir jusqu''à 60 pieds.', 'passive'
FROM races WHERE name = 'Tiefling';

INSERT INTO racial_traits (race_id, name, name_fr, description, trait_type)
SELECT id, 'Hellish Resistance', 'Résistance infernale', 'Résistance aux dégâts de feu.', 'resistance'
FROM races WHERE name = 'Tiefling';

INSERT INTO racial_traits (race_id, name, name_fr, description, trait_type)
SELECT id, 'Infernal Legacy', 'Héritage infernal', 'Sorts innés : Thaumaturgie (cantrip), Représailles infernales (niv 3), Ténèbres (niv 5). Utilise CHA.', 'spell'
FROM races WHERE name = 'Tiefling';

-- ============================================================================
-- SOUS-RACES
-- ============================================================================

-- ELFE - Haut-Elfe
INSERT INTO subraces (parent_race_id, name, name_fr, description, int_bonus)
SELECT id, 'High Elf', 'Haut-Elfe', 'Maîtrise des armes elfiques, cantrip de magicien, langue supplémentaire.', 1
FROM races WHERE name = 'Elf';

-- ELFE - Elfe des bois
INSERT INTO subraces (parent_race_id, name, name_fr, description, wis_bonus)
SELECT id, 'Wood Elf', 'Elfe des bois', 'Vitesse augmentée à 35 pieds, se cacher en terrain naturel, maîtrise des armes elfiques.', 1
FROM races WHERE name = 'Elf';

-- ELFE - Elfe noir (Drow)
INSERT INTO subraces (parent_race_id, name, name_fr, description, cha_bonus)
SELECT id, 'Dark Elf (Drow)', 'Elfe noir (Drow)', 'Vision dans le noir supérieure (120 pieds), magie drow (lumières dansantes, lueurs féeriques, ténèbres), sensibilité au soleil.', 1
FROM races WHERE name = 'Elf';

-- NAIN - Nain des collines
INSERT INTO subraces (parent_race_id, name, name_fr, description, wis_bonus)
SELECT id, 'Hill Dwarf', 'Nain des collines', 'Ténacité naine : +1 PV par niveau.', 1
FROM races WHERE name = 'Dwarf';

-- NAIN - Nain des montagnes
INSERT INTO subraces (parent_race_id, name, name_fr, description, str_bonus)
SELECT id, 'Mountain Dwarf', 'Nain des montagnes', 'Maîtrise des armures légères et intermédiaires.', 2
FROM races WHERE name = 'Dwarf';

-- HALFELIN - Halfelin pied-léger
INSERT INTO subraces (parent_race_id, name, name_fr, description, cha_bonus)
SELECT id, 'Lightfoot Halfling', 'Halfelin pied-léger', 'Discrétion naturelle : peut se cacher derrière une créature plus grande.', 1
FROM races WHERE name = 'Halfling';

-- HALFELIN - Halfelin robuste
INSERT INTO subraces (parent_race_id, name, name_fr, description, con_bonus)
SELECT id, 'Stout Halfling', 'Halfelin robuste', 'Résistance au poison : avantage aux JdS contre le poison et résistance aux dégâts de poison.', 1
FROM races WHERE name = 'Halfling';

-- GNOME - Gnome des forêts
INSERT INTO subraces (parent_race_id, name, name_fr, description, dex_bonus)
SELECT id, 'Forest Gnome', 'Gnome des forêts', 'Illusionniste né : cantrip illusion mineure. Parler aux petites bêtes.', 1
FROM races WHERE name = 'Gnome';

-- GNOME - Gnome des roches
INSERT INTO subraces (parent_race_id, name, name_fr, description, con_bonus)
SELECT id, 'Rock Gnome', 'Gnome des roches', 'Connaissance en artifice : double bonus de maîtrise pour Histoire sur objets magiques/alchimiques/technologiques. Peut créer des gadgets mécaniques.', 1
FROM races WHERE name = 'Gnome';
