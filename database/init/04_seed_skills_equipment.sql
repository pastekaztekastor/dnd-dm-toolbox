-- ============================================================================
-- COMPÉTENCES ET ÉQUIPEMENT D&D 5E
-- ============================================================================

-- ============================================================================
-- COMPÉTENCES (SKILLS)
-- ============================================================================

INSERT INTO skills (name, name_fr, ability, description) VALUES
('Acrobatics', 'Acrobaties', 'DEX', 'Équilibre, pirouettes, rouler-bouler, sauts périlleux.'),
('Animal Handling', 'Dressage', 'WIS', 'Calmer un animal domestique, empêcher une monture de paniquer.'),
('Arcana', 'Arcanes', 'INT', 'Connaissance de la magie, des sorts, des objets magiques.'),
('Athletics', 'Athlétisme', 'STR', 'Escalade, saut, natation, lutte, bousculade.'),
('Deception', 'Tromperie', 'CHA', 'Mentir convaincante, bluffer, déguisement verbal.'),
('History', 'Histoire', 'INT', 'Connaissances historiques, légendes, événements passés.'),
('Insight', 'Intuition', 'WIS', 'Déterminer les vraies intentions, détecter les mensonges.'),
('Intimidation', 'Intimidation', 'CHA', 'Influencer par menaces, hostilité, violence physique.'),
('Investigation', 'Investigation', 'INT', 'Chercher des indices, déductions, recherches.'),
('Medicine', 'Médecine', 'WIS', 'Stabiliser un mourant, diagnostiquer une maladie.'),
('Nature', 'Nature', 'INT', 'Connaissance du terrain, plantes, animaux, météo.'),
('Perception', 'Perception', 'WIS', 'Repérer, entendre, détecter la présence de quelque chose.'),
('Performance', 'Représentation', 'CHA', 'Divertir par musique, danse, théâtre, conte.'),
('Persuasion', 'Persuasion', 'CHA', 'Influencer par tact, politesse, grâce sociale.'),
('Religion', 'Religion', 'INT', 'Connaissance des divinités, rites, structures religieuses.'),
('Sleight of Hand', 'Escamotage', 'DEX', 'Pickpocket, tours de passe-passe, crochetage.'),
('Stealth', 'Discrétion', 'DEX', 'Se cacher, se déplacer silencieusement.'),
('Survival', 'Survie', 'WIS', 'Pister, chasser, trouver de l''eau, prédire la météo.');

-- ============================================================================
-- ARMES
-- ============================================================================

-- ARMES DE CORPS-À-CORPS COURANTES
INSERT INTO weapons (name, name_fr, weapon_type, damage_dice, damage_type, properties, weight, cost_gp)
VALUES
('Club', 'Gourdin', 'Simple Melee', '1d4', 'bludgeoning', ARRAY['light'], 2, 0.1),
('Dagger', 'Dague', 'Simple Melee', '1d4', 'piercing', ARRAY['finesse', 'light', 'thrown'], 1, 2),
('Greatclub', 'Massue', 'Simple Melee', '1d8', 'bludgeoning', ARRAY['two-handed'], 10, 0.2),
('Handaxe', 'Hachette', 'Simple Melee', '1d6', 'slashing', ARRAY['light', 'thrown'], 2, 5),
('Javelin', 'Javeline', 'Simple Melee', '1d6', 'piercing', ARRAY['thrown'], 2, 0.5),
('Light Hammer', 'Marteau léger', 'Simple Melee', '1d4', 'bludgeoning', ARRAY['light', 'thrown'], 2, 2),
('Mace', 'Masse d''armes', 'Simple Melee', '1d6', 'bludgeoning', ARRAY[], 4, 5),
('Quarterstaff', 'Bâton', 'Simple Melee', '1d6', 'bludgeoning', ARRAY['versatile'], 4, 0.2),
('Sickle', 'Serpe', 'Simple Melee', '1d4', 'slashing', ARRAY['light'], 2, 1),
('Spear', 'Lance', 'Simple Melee', '1d6', 'piercing', ARRAY['thrown', 'versatile'], 3, 1);

-- Armes versatiles - dégâts à 2 mains
UPDATE weapons SET versatile_damage = '1d8' WHERE name IN ('Quarterstaff', 'Spear');

-- ARMES À DISTANCE COURANTES
INSERT INTO weapons (name, name_fr, weapon_type, damage_dice, damage_type, properties, range_normal, range_long, weight, cost_gp)
VALUES
('Crossbow, light', 'Arbalète légère', 'Simple Ranged', '1d8', 'piercing', ARRAY['ammunition', 'loading', 'two-handed'], 80, 320, 5, 25),
('Dart', 'Fléchette', 'Simple Ranged', '1d4', 'piercing', ARRAY['finesse', 'thrown'], 20, 60, 0.25, 0.05),
('Shortbow', 'Arc court', 'Simple Ranged', '1d6', 'piercing', ARRAY['ammunition', 'two-handed'], 80, 320, 2, 25),
('Sling', 'Fronde', 'Simple Ranged', '1d4', 'bludgeoning', ARRAY['ammunition'], 30, 120, 0, 0.1);

-- ARMES DE CORPS-À-CORPS DE GUERRE
INSERT INTO weapons (name, name_fr, weapon_type, damage_dice, damage_type, properties, weight, cost_gp)
VALUES
('Battleaxe', 'Hache d''armes', 'Martial Melee', '1d8', 'slashing', ARRAY['versatile'], 4, 10),
('Flail', 'Fléau d''armes', 'Martial Melee', '1d8', 'bludgeoning', ARRAY[], 2, 10),
('Glaive', 'Coutille', 'Martial Melee', '1d10', 'slashing', ARRAY['heavy', 'reach', 'two-handed'], 6, 20),
('Greataxe', 'Hache à deux mains', 'Martial Melee', '1d12', 'slashing', ARRAY['heavy', 'two-handed'], 7, 30),
('Greatsword', 'Épée à deux mains', 'Martial Melee', '2d6', 'slashing', ARRAY['heavy', 'two-handed'], 6, 50),
('Halberd', 'Hallebarde', 'Martial Melee', '1d10', 'slashing', ARRAY['heavy', 'reach', 'two-handed'], 6, 20),
('Lance', 'Lance d''arçon', 'Martial Melee', '1d12', 'piercing', ARRAY['reach', 'special'], 6, 10),
('Longsword', 'Épée longue', 'Martial Melee', '1d8', 'slashing', ARRAY['versatile'], 3, 15),
('Maul', 'Maillet', 'Martial Melee', '2d6', 'bludgeoning', ARRAY['heavy', 'two-handed'], 10, 10),
('Morningstar', 'Morgenstern', 'Martial Melee', '1d8', 'piercing', ARRAY[], 4, 15),
('Pike', 'Pique', 'Martial Melee', '1d10', 'piercing', ARRAY['heavy', 'reach', 'two-handed'], 18, 5),
('Rapier', 'Rapière', 'Martial Melee', '1d8', 'piercing', ARRAY['finesse'], 2, 25),
('Scimitar', 'Cimeterre', 'Martial Melee', '1d6', 'slashing', ARRAY['finesse', 'light'], 3, 25),
('Shortsword', 'Épée courte', 'Martial Melee', '1d6', 'piercing', ARRAY['finesse', 'light'], 2, 10),
('Trident', 'Trident', 'Martial Melee', '1d6', 'piercing', ARRAY['thrown', 'versatile'], 4, 5),
('War pick', 'Pic de guerre', 'Martial Melee', '1d8', 'piercing', ARRAY[], 2, 5),
('Warhammer', 'Marteau de guerre', 'Martial Melee', '1d8', 'bludgeoning', ARRAY['versatile'], 2, 15),
('Whip', 'Fouet', 'Martial Melee', '1d4', 'slashing', ARRAY['finesse', 'reach'], 3, 2);

-- Armes versatiles - dégâts à 2 mains
UPDATE weapons SET versatile_damage = '1d10' WHERE name IN ('Battleaxe', 'Longsword', 'Trident', 'Warhammer');

-- ARMES À DISTANCE DE GUERRE
INSERT INTO weapons (name, name_fr, weapon_type, damage_dice, damage_type, properties, range_normal, range_long, weight, cost_gp)
VALUES
('Blowgun', 'Sarbacane', 'Martial Ranged', '1', 'piercing', ARRAY['ammunition', 'loading'], 25, 100, 1, 10),
('Crossbow, hand', 'Arbalète de poing', 'Martial Ranged', '1d6', 'piercing', ARRAY['ammunition', 'light', 'loading'], 30, 120, 3, 75),
('Crossbow, heavy', 'Arbalète lourde', 'Martial Ranged', '1d10', 'piercing', ARRAY['ammunition', 'heavy', 'loading', 'two-handed'], 100, 400, 18, 50),
('Longbow', 'Arc long', 'Martial Ranged', '1d8', 'piercing', ARRAY['ammunition', 'heavy', 'two-handed'], 150, 600, 2, 50),
('Net', 'Filet', 'Martial Ranged', '0', 'none', ARRAY['special', 'thrown'], 5, 15, 3, 1);

-- ============================================================================
-- ARMURES
-- ============================================================================

-- ARMURES LÉGÈRES
INSERT INTO armors (name, name_fr, armor_type, base_ac, dex_bonus_max, adds_dex_bonus, stealth_disadvantage, weight, cost_gp)
VALUES
('Padded', 'Matelassée', 'Light', 11, NULL, true, true, 8, 5),
('Leather', 'Cuir', 'Light', 11, NULL, true, false, 10, 10),
('Studded Leather', 'Cuir clouté', 'Light', 12, NULL, true, false, 13, 45);

-- ARMURES INTERMÉDIAIRES
INSERT INTO armors (name, name_fr, armor_type, base_ac, dex_bonus_max, adds_dex_bonus, stealth_disadvantage, weight, cost_gp)
VALUES
('Hide', 'Peau', 'Medium', 12, 2, true, false, 12, 10),
('Chain Shirt', 'Chemise de mailles', 'Medium', 13, 2, true, false, 20, 50),
('Scale Mail', 'Armure d''écailles', 'Medium', 14, 2, true, true, 45, 50),
('Breastplate', 'Cuirasse', 'Medium', 14, 2, true, false, 20, 400),
('Half Plate', 'Demi-plate', 'Medium', 15, 2, true, true, 40, 750);

-- ARMURES LOURDES
INSERT INTO armors (name, name_fr, armor_type, base_ac, dex_bonus_max, adds_dex_bonus, stealth_disadvantage, strength_requirement, weight, cost_gp)
VALUES
('Ring Mail', 'Broigne', 'Heavy', 14, NULL, false, true, NULL, 40, 30),
('Chain Mail', 'Cotte de mailles', 'Heavy', 16, NULL, false, true, 13, 55, 75),
('Splint', 'Clibanion', 'Heavy', 17, NULL, false, true, 15, 60, 200),
('Plate', 'Harnois', 'Heavy', 18, NULL, false, true, 15, 65, 1500);

-- BOUCLIER
INSERT INTO armors (name, name_fr, armor_type, base_ac, dex_bonus_max, adds_dex_bonus, stealth_disadvantage, weight, cost_gp)
VALUES
('Shield', 'Bouclier', 'Shield', 2, NULL, false, false, 6, 10);

-- ============================================================================
-- ÉQUIPEMENT D'AVENTURE
-- ============================================================================

INSERT INTO equipment (name, name_fr, category, subcategory, description, weight, cost_gp) VALUES
('Backpack', 'Sac à dos', 'Adventuring Gear', 'Container', 'Contenance 30 livres/1 pied cube', 5, 2),
('Bedroll', 'Couverture', 'Adventuring Gear', 'Camping', 'Pour dormir', 7, 1),
('Rope, hempen (50 feet)', 'Corde de chanvre (15 m)', 'Adventuring Gear', 'Utility', '2 points de vie, peut être brisée avec un test de Force DD 17', 10, 1),
('Rope, silk (50 feet)', 'Corde de soie (15 m)', 'Adventuring Gear', 'Utility', 'Légère et résistante', 5, 10),
('Torch', 'Torche', 'Adventuring Gear', 'Light', 'Lumière vive 20 pieds, faible 20 pieds. Dure 1 heure.', 1, 0.01),
('Tinderbox', 'Boîte d''allume-feu', 'Adventuring Gear', 'Utility', 'Pour allumer un feu', 1, 0.5),
('Rations (1 day)', 'Rations (1 jour)', 'Adventuring Gear', 'Food', 'Nourriture sèche pour une journée', 2, 0.5),
('Waterskin', 'Outre d''eau', 'Adventuring Gear', 'Food', 'Contient environ 1 litre', 5, 0.2),
('Crowbar', 'Pied-de-biche', 'Adventuring Gear', 'Utility', 'Avantage aux tests de Force pour forcer', 5, 2),
('Hammer', 'Marteau', 'Adventuring Gear', 'Tool', 'Pour planter des piquets', 3, 1),
('Piton', 'Piton', 'Adventuring Gear', 'Climbing', 'À enfoncer dans la roche', 0.25, 0.05),
('Lantern, hooded', 'Lanterne à capuchon', 'Adventuring Gear', 'Light', 'Lumière vive 30 pieds, faible 30 pieds. 6 heures/flasque d''huile.', 2, 5),
('Oil (flask)', 'Huile (flasque)', 'Adventuring Gear', 'Utility', 'Combustible pour lanterne ou arme incendiaire', 1, 0.1),
('Potion of Healing', 'Potion de soins', 'Consumable', 'Potion', 'Restaure 2d4+2 points de vie', 0.5, 50),
('Healer''s Kit', 'Trousse de soins', 'Adventuring Gear', 'Medical', '10 utilisations. Stabilise sans test.', 3, 5),
('Ball bearings (bag of 1,000)', 'Billes (sac de 1000)', 'Adventuring Gear', 'Utility', 'Couvre 10x10 pieds. Jet DEX DD 10 ou tomber.', 2, 1),
('Caltrops (bag of 20)', 'Chausse-trappes (sac de 20)', 'Adventuring Gear', 'Utility', 'Couvre 5x5 pieds. 1 dégât perforant + vitesse réduite.', 2, 1),
('Chalk (1 piece)', 'Craie', 'Adventuring Gear', 'Utility', 'Pour marquer', 0, 0.01),
('Grappling hook', 'Grappin', 'Adventuring Gear', 'Climbing', 'Accrocher à une distance', 4, 2);

-- ============================================================================
-- OUTILS
-- ============================================================================

INSERT INTO equipment (name, name_fr, category, subcategory, weight, cost_gp) VALUES
('Alchemist''s Supplies', 'Matériel d''alchimiste', 'Tools', 'Artisan''s Tools', 8, 50),
('Brewer''s Supplies', 'Matériel de brasseur', 'Tools', 'Artisan''s Tools', 9, 20),
('Calligrapher''s Supplies', 'Matériel de calligraphe', 'Tools', 'Artisan''s Tools', 5, 10),
('Carpenter''s Tools', 'Outils de charpentier', 'Tools', 'Artisan''s Tools', 6, 8),
('Cook''s Utensils', 'Ustensiles de cuisinier', 'Tools', 'Artisan''s Tools', 8, 1),
('Thieves'' Tools', 'Outils de voleur', 'Tools', 'Special', 'Pour crocheter les serrures', 1, 25),
('Musical Instrument (various)', 'Instrument de musique', 'Tools', 'Musical', 'Divers types', 3, 25);

-- ============================================================================
-- PACKS D'ÉQUIPEMENT
-- ============================================================================

-- Pack d'explorateur
INSERT INTO equipment_packs (name, name_fr, description, cost_gp)
VALUES ('Explorer''s Pack', 'Paquetage d''explorateur',
'Comprend un sac à dos, un sac de couchage, une gamelle, une boîte d''allume-feu, 10 torches, 10 jours de rations, une outre d''eau et 50 pieds de corde de chanvre.',
10);

-- Pack de cambrioleur
INSERT INTO equipment_packs (name, name_fr, description, cost_gp)
VALUES ('Burglar''s Pack', 'Paquetage de cambrioleur',
'Comprend un sac à dos, 1000 billes, 10 pieds de ficelle, une cloche, 5 bougies, un pied-de-biche, un marteau, 10 pitons, une lanterne à capuchon, 2 flasques d''huile, 5 jours de rations, une outre d''eau et 50 pieds de corde de chanvre.',
16);

-- Pack de prêtre
INSERT INTO equipment_packs (name, name_fr, description, cost_gp)
VALUES ('Priest''s Pack', 'Paquetage de prêtre',
'Comprend un sac à dos, une couverture, 10 bougies, une boîte d''allume-feu, une boîte pour aumônes, 2 blocs d''encens, un encensoir, des vêtements de cérémonie, 2 jours de rations et une outre d''eau.',
19);

-- Pack d'érudit
INSERT INTO equipment_packs (name, name_fr, description, cost_gp)
VALUES ('Scholar''s Pack', 'Paquetage d''érudit',
'Comprend un sac à dos, un livre, une bouteille d''encre, une plume, 10 feuilles de parchemin, un petit sac de sable et un petit couteau.',
40);
