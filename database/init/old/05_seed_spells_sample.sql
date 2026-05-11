-- ============================================================================
-- SORTS D&D 5E - ÉCHANTILLON
-- ============================================================================

-- ============================================================================
-- CANTRIPS (Niveau 0)
-- ============================================================================

INSERT INTO game_spells (
    name, name_fr, level, school, school_fr,
    casting_time, casting_time_fr, range, range_fr,
    has_verbal, has_somatic, has_material, material_component,
    duration, duration_fr, concentration,
    description, description_fr,
    available_to_classes
) VALUES
(
    'Fire Bolt',
    'Trait de feu',
    0,
    'Evocation',
    'Évocation',
    '1 action',
    '1 action',
    '120 feet',
    '36 mètres',
    true, true, false, NULL,
    'Instantaneous',
    'Instantanée',
    false,
    'You hurl a mote of fire at a creature or object within range. Make a ranged spell attack. On a hit, the target takes 1d10 fire damage. This spell''s damage increases by 1d10 when you reach 5th level (2d10), 11th level (3d10), and 17th level (4d10).',
    'Vous lancez un projectile de feu sur une créature ou un objet à portée. Effectuez une attaque de sort à distance. En cas de toucher, la cible subit 1d10 dégâts de feu. Les dégâts augmentent de 1d10 aux niveaux 5 (2d10), 11 (3d10) et 17 (4d10).',
    ARRAY['Wizard', 'Sorcerer']
),
(
    'Eldritch Blast',
    'Décharge occulte',
    0,
    'Evocation',
    'Évocation',
    '1 action',
    '1 action',
    '120 feet',
    '36 mètres',
    true, true, false, NULL,
    'Instantaneous',
    'Instantanée',
    false,
    'A beam of crackling energy streaks toward a creature within range. Make a ranged spell attack. On a hit, the target takes 1d10 force damage. Creates multiple beams at higher levels (2 at 5th, 3 at 11th, 4 at 17th).',
    'Un rayon d''énergie crépitante fonce vers une créature à portée. Effectuez une attaque de sort à distance. En cas de toucher, la cible subit 1d10 dégâts de force. Crée plusieurs rayons aux niveaux supérieurs (2 au 5e, 3 au 11e, 4 au 17e).',
    ARRAY['Warlock']
),
(
    'Mage Hand',
    'Main du mage',
    0,
    'Conjuration',
    'Invocation',
    '1 action',
    '1 action',
    '30 feet',
    '9 mètres',
    true, true, false, NULL,
    '1 minute',
    '1 minute',
    false,
    'A spectral, floating hand appears at a point you choose within range. You can use your action to control the hand. You can use the hand to manipulate an object, open an unlocked door or container, stow or retrieve an item, or pour contents out of a vial.',
    'Une main spectrale flottante apparaît à un point que vous choisissez à portée. Vous pouvez utiliser votre action pour contrôler la main. Elle peut manipuler un objet, ouvrir une porte ou un conteneur non verrouillé, ranger ou récupérer un objet.',
    ARRAY['Wizard', 'Sorcerer', 'Warlock', 'Bard']
),
(
    'Prestidigitation',
    'Prestidigitation',
    0,
    'Transmutation',
    'Transmutation',
    '1 action',
    '1 action',
    '10 feet',
    '3 mètres',
    true, true, false, NULL,
    '1 hour',
    '1 heure',
    false,
    'This spell is a minor magical trick. You create a harmless sensory effect, light/extinguish candles, clean/soil items, chill/warm/flavor food, make a mark appear, or create a trinket/illusion.',
    'Ce sort est un tour de magie mineur. Vous créez un effet sensoriel inoffensif, allumez/éteignez des bougies, nettoyez/salissez des objets, refroidissez/réchauffez/aromatisez de la nourriture, faites apparaître une marque, ou créez un bibelot/illusion.',
    ARRAY['Wizard', 'Sorcerer', 'Warlock', 'Bard']
),
(
    'Sacred Flame',
    'Flamme sacrée',
    0,
    'Evocation',
    'Évocation',
    '1 action',
    '1 action',
    '60 feet',
    '18 mètres',
    true, true, false, NULL,
    'Instantaneous',
    'Instantanée',
    false,
    'Flame-like radiance descends on a creature. The target must succeed on a Dexterity saving throw or take 1d8 radiant damage. Damage increases at higher levels.',
    'Une radiance semblable à une flamme descend sur une créature. La cible doit réussir un jet de sauvegarde de Dextérité ou subir 1d8 dégâts radiants. Les dégâts augmentent aux niveaux supérieurs.',
    ARRAY['Cleric']
);

-- ============================================================================
-- SORTS NIVEAU 1
-- ============================================================================

INSERT INTO game_spells (
    name, name_fr, level, school, school_fr,
    casting_time, casting_time_fr, range, range_fr,
    has_verbal, has_somatic, has_material, material_component,
    duration, duration_fr, concentration,
    description, description_fr,
    at_higher_levels, at_higher_levels_fr,
    available_to_classes
) VALUES
(
    'Magic Missile',
    'Projectile magique',
    1,
    'Evocation',
    'Évocation',
    '1 action',
    '1 action',
    '120 feet',
    '36 mètres',
    true, true, false, NULL,
    'Instantaneous',
    'Instantanée',
    false,
    'You create three glowing darts of magical force. Each dart hits a creature of your choice within range. A dart deals 1d4+1 force damage. The darts all strike simultaneously.',
    'Vous créez trois fléchettes luisantes de force magique. Chaque fléchette touche une créature de votre choix à portée. Une fléchette inflige 1d4+1 dégâts de force. Les fléchettes frappent simultanément.',
    'When you cast this spell using a spell slot of 2nd level or higher, the spell creates one more dart for each slot level above 1st.',
    'Lorsque vous lancez ce sort en utilisant un emplacement de niveau 2 ou supérieur, le sort crée une fléchette supplémentaire par niveau au-dessus du 1er.',
    ARRAY['Wizard', 'Sorcerer']
),
(
    'Shield',
    'Bouclier',
    1,
    'Abjuration',
    'Abjuration',
    '1 reaction',
    '1 réaction',
    'Self',
    'Personnelle',
    true, true, false, NULL,
    '1 round',
    '1 round',
    false,
    'An invisible barrier of magical force appears and protects you. Until the start of your next turn, you have a +5 bonus to AC, including against the triggering attack, and you take no damage from magic missile.',
    'Une barrière invisible de force magique apparaît et vous protège. Jusqu''au début de votre prochain tour, vous avez un bonus de +5 à la CA, y compris contre l''attaque déclencheuse, et vous ne subissez aucun dégât de projectile magique.',
    NULL,
    NULL,
    ARRAY['Wizard', 'Sorcerer']
),
(
    'Cure Wounds',
    'Soin des blessures',
    1,
    'Evocation',
    'Évocation',
    '1 action',
    '1 action',
    'Touch',
    'Contact',
    true, true, false, NULL,
    'Instantaneous',
    'Instantanée',
    false,
    'A creature you touch regains a number of hit points equal to 1d8 + your spellcasting ability modifier.',
    'Une créature que vous touchez regagne un nombre de points de vie égal à 1d8 + votre modificateur de caractéristique d''incantation.',
    'When you cast this spell using a spell slot of 2nd level or higher, the healing increases by 1d8 for each slot level above 1st.',
    'Lorsque vous lancez ce sort en utilisant un emplacement de niveau 2 ou supérieur, les soins augmentent de 1d8 par niveau au-dessus du 1er.',
    ARRAY['Cleric', 'Bard', 'Druid', 'Paladin', 'Ranger']
),
(
    'Detect Magic',
    'Détection de la magie',
    1,
    'Divination',
    'Divination',
    '1 action',
    '1 action',
    'Self',
    'Personnelle',
    true, true, false, NULL,
    '10 minutes',
    '10 minutes',
    true,
    'You sense the presence of magic within 30 feet. You learn its school of magic. The spell can penetrate barriers but is blocked by 1 foot of stone, metal, or wood.',
    'Vous détectez la présence de magie dans un rayon de 9 mètres. Vous apprenez son école de magie. Le sort peut pénétrer les barrières mais est bloqué par 30 cm de pierre, métal ou bois.',
    NULL,
    NULL,
    ARRAY['Wizard', 'Sorcerer', 'Bard', 'Cleric', 'Druid', 'Paladin', 'Ranger']
),
(
    'Burning Hands',
    'Mains brûlantes',
    1,
    'Evocation',
    'Évocation',
    '1 action',
    '1 action',
    'Self (15-foot cone)',
    'Personnelle (cône de 4,5 m)',
    true, true, false, NULL,
    'Instantaneous',
    'Instantanée',
    false,
    'A thin sheet of flames shoots forth from your outstretched fingertips. Each creature in a 15-foot cone must make a Dexterity saving throw. A creature takes 3d6 fire damage on a failed save, or half on success.',
    'Un mince rideau de flammes jaillit de vos doigts tendus. Chaque créature dans un cône de 4,5 m doit faire un jet de sauvegarde de Dextérité. Une créature subit 3d6 dégâts de feu en cas d''échec, ou la moitié en cas de réussite.',
    'When you cast this spell using a spell slot of 2nd level or higher, the damage increases by 1d6 for each slot level above 1st.',
    'Lorsque vous lancez ce sort avec un emplacement de niveau 2 ou supérieur, les dégâts augmentent de 1d6 par niveau au-dessus du 1er.',
    ARRAY['Wizard', 'Sorcerer']
);

-- ============================================================================
-- SORTS NIVEAU 2
-- ============================================================================

INSERT INTO game_spells (
    name, name_fr, level, school, school_fr,
    casting_time, casting_time_fr, range, range_fr,
    has_verbal, has_somatic, has_material, material_component,
    duration, duration_fr, concentration,
    description, description_fr,
    at_higher_levels, at_higher_levels_fr,
    available_to_classes
) VALUES
(
    'Misty Step',
    'Pas brumeux',
    2,
    'Conjuration',
    'Invocation',
    '1 bonus action',
    '1 action bonus',
    'Self',
    'Personnelle',
    true, false, false, NULL,
    'Instantaneous',
    'Instantanée',
    false,
    'Briefly surrounded by silvery mist, you teleport up to 30 feet to an unoccupied space that you can see.',
    'Brièvement entouré de brume argentée, vous vous téléportez jusqu''à 9 mètres vers un espace inoccupé que vous pouvez voir.',
    NULL,
    NULL,
    ARRAY['Wizard', 'Sorcerer', 'Warlock']
),
(
    'Scorching Ray',
    'Rayon ardent',
    2,
    'Evocation',
    'Évocation',
    '1 action',
    '1 action',
    '120 feet',
    '36 mètres',
    true, true, false, NULL,
    'Instantaneous',
    'Instantanée',
    false,
    'You create three rays of fire and hurl them at targets within range. Make a ranged spell attack for each ray. On a hit, the target takes 2d6 fire damage.',
    'Vous créez trois rayons de feu et les lancez sur des cibles à portée. Effectuez une attaque de sort à distance pour chaque rayon. En cas de toucher, la cible subit 2d6 dégâts de feu.',
    'When you cast this spell using a spell slot of 3rd level or higher, you create one additional ray for each slot level above 2nd.',
    'Lorsque vous lancez ce sort avec un emplacement de niveau 3 ou supérieur, vous créez un rayon supplémentaire par niveau au-dessus du 2e.',
    ARRAY['Wizard', 'Sorcerer']
),
(
    'Hold Person',
    'Immobiliser un humanoïde',
    2,
    'Enchantment',
    'Enchantement',
    '1 action',
    '1 action',
    '60 feet',
    '18 mètres',
    true, true, true, 'a small, straight piece of iron',
    '1 minute',
    '1 minute',
    true,
    'Choose a humanoid within range. The target must succeed on a Wisdom saving throw or be paralyzed for the duration. At the end of each of its turns, the target can make another save.',
    'Choisissez un humanoïde à portée. La cible doit réussir un jet de sauvegarde de Sagesse ou être paralysée pendant la durée. À la fin de chacun de ses tours, la cible peut refaire un jet de sauvegarde.',
    'When you cast this spell using a spell slot of 3rd level or higher, you can target one additional humanoid for each slot level above 2nd.',
    'Lorsque vous lancez ce sort avec un emplacement de niveau 3 ou supérieur, vous pouvez cibler un humanoïde supplémentaire par niveau au-dessus du 2e.',
    ARRAY['Wizard', 'Sorcerer', 'Bard', 'Cleric', 'Druid', 'Warlock']
);

-- ============================================================================
-- SORTS NIVEAU 3
-- ============================================================================

INSERT INTO game_spells (
    name, name_fr, level, school, school_fr,
    casting_time, casting_time_fr, range, range_fr,
    has_verbal, has_somatic, has_material, material_component,
    duration, duration_fr, concentration,
    description, description_fr,
    at_higher_levels, at_higher_levels_fr,
    available_to_classes
) VALUES
(
    'Fireball',
    'Boule de feu',
    3,
    'Evocation',
    'Évocation',
    '1 action',
    '1 action',
    '150 feet',
    '45 mètres',
    true, true, true, 'a tiny ball of bat guano and sulfur',
    'Instantaneous',
    'Instantanée',
    false,
    'A bright streak flashes to a point within range and then blossoms into a roaring explosion. Each creature in a 20-foot-radius sphere must make a Dexterity saving throw. A target takes 8d6 fire damage on a failed save, or half on success.',
    'Un éclair brillant file vers un point à portée puis éclate en une explosion rugissante. Chaque créature dans une sphère de 6 m de rayon doit faire un jet de sauvegarde de Dextérité. Une cible subit 8d6 dégâts de feu en cas d''échec, ou la moitié en cas de réussite.',
    'When you cast this spell using a spell slot of 4th level or higher, the damage increases by 1d6 for each slot level above 3rd.',
    'Lorsque vous lancez ce sort avec un emplacement de niveau 4 ou supérieur, les dégâts augmentent de 1d6 par niveau au-dessus du 3e.',
    ARRAY['Wizard', 'Sorcerer']
),
(
    'Counterspell',
    'Contresort',
    3,
    'Abjuration',
    'Abjuration',
    '1 reaction',
    '1 réaction',
    '60 feet',
    '18 mètres',
    true, false, false, NULL,
    'Instantaneous',
    'Instantanée',
    false,
    'You attempt to interrupt a creature casting a spell. If the spell is 3rd level or lower, it fails. If higher, make an ability check (DC 10 + spell level) to succeed.',
    'Vous tentez d''interrompre une créature qui lance un sort. Si le sort est de niveau 3 ou moins, il échoue. Si supérieur, faites un test de caractéristique (DD 10 + niveau du sort) pour réussir.',
    'When you cast this spell using a spell slot of 4th level or higher, the interrupted spell has no effect if its level is less than or equal to your spell slot.',
    'Lorsque vous lancez ce sort avec un emplacement de niveau 4 ou supérieur, le sort interrompu n''a aucun effet si son niveau est inférieur ou égal à votre emplacement.',
    ARRAY['Wizard', 'Sorcerer', 'Warlock']
),
(
    'Revivify',
    'Rappel à la vie',
    3,
    'Necromancy',
    'Nécromancie',
    '1 action',
    '1 action',
    'Touch',
    'Contact',
    true, true, true, 'diamonds worth 300 gp, consumed',
    'Instantaneous',
    'Instantanée',
    false,
    'You touch a creature that has died within the last minute. That creature returns to life with 1 hit point. This spell can''t return to life a creature that has died of old age.',
    'Vous touchez une créature morte depuis moins d''une minute. Cette créature revient à la vie avec 1 point de vie. Ce sort ne peut pas ramener à la vie une créature morte de vieillesse.',
    NULL,
    NULL,
    ARRAY['Cleric', 'Paladin']
);

-- ============================================================================
-- SORTS RITUELS
-- ============================================================================

UPDATE game_spells SET ritual = true WHERE name IN ('Detect Magic');

INSERT INTO game_spells (
    name, name_fr, level, school, school_fr,
    casting_time, casting_time_fr, range, range_fr,
    has_verbal, has_somatic, has_material, material_component,
    duration, duration_fr, concentration, ritual,
    description, description_fr,
    available_to_classes
) VALUES
(
    'Identify',
    'Identification',
    1,
    'Divination',
    'Divination',
    '1 minute',
    '1 minute',
    'Touch',
    'Contact',
    true, true, true, 'a pearl worth at least 100 gp and an owl feather',
    'Instantaneous',
    'Instantanée',
    false,
    true,
    'You learn the properties of one magical object, including how to use it, whether it requires attunement, and how many charges it has.',
    'Vous apprenez les propriétés d''un objet magique, y compris comment l''utiliser, s''il nécessite harmonisation, et combien de charges il possède.',
    ARRAY['Wizard', 'Bard']
);
