-- ============================================================================
-- SEED : RÔDEUR
-- Source : _setaside/resources/books/player handbook/03_CLASSES/RODEUR/classe_rodeur_DD5e.md
-- ============================================================================

-- ============================================================================
-- 1. CLASSE PRINCIPALE
-- ============================================================================
INSERT OR IGNORE INTO classes (
    id, nom, alias,
    description, aide_joueur, creation_classe, creation_rapide,
    dee_de_vie, points_de_vie, points_de_vie_par_niveau,
    equipement_maitrise, jets_de_sauvegarde, competences,
    caracteristiques_de_sorts,
    source, source_ver
) VALUES (
    'rodeur',
    'Rôdeur',
    'rodeur',
    'Combattant des étendues sauvages, le rôdeur traque les monstres qui menacent les abords de la civilisation. Il allie techniques martiales, magie de la nature (SAG) et une connaissance intime d''un terrain et d''un type d''ennemi juré. Archétype au niveau 3 : Chasseur ou Maître des bêtes.',
    'Dès le niveau 1, choisissez un ennemi juré (type de créature) et un terrain de prédilection : vous obtenez des avantages significatifs contre/dans chacun. À partir du niveau 2, vous lancez des sorts grâce à la Sagesse. Choisissez votre style de combat au niveau 2 et votre archétype au niveau 3.',
    'Réfléchissez à votre ennemi juré et à votre terrain de prédilection, ils définissent votre identité. Choisissez l''archétype Chasseur pour optimiser vos options de combat polyvalent, ou Maître des bêtes pour un compagnon animal fidèle.',
    'Attribuez votre valeur de caractéristique la plus haute à la Dextérité, la suivante à la Sagesse. Choisissez le contexte d''explorateur.',
    10, 10, 6,
    '["Armures légères","Armures intermédiaires","Boucliers","Armes courantes","Armes de guerre"]',
    '["Force","Dextérité"]',
    '["Trois au choix parmi : Athlétisme, Discrétion, Dressage, Intuition, Investigation, Nature, Perception, Survie"]',
    'SAG',
    'PHB', '5e'
);

-- ============================================================================
-- 2. SOUS-CLASSES (archétypes)
-- ============================================================================
INSERT OR IGNORE INTO classes (id, nom, alias, classe_parente_id, description, source, source_ver) VALUES
(
    'chasseur',
    'Chasseur',
    'chasseur',
    'rodeur',
    'Archétype offensif polyvalent. À chaque palier (3, 7, 11, 15), choisissez une option parmi plusieurs tactiques : une proie de chasseur, une défense, un mode d''attaques multiples, une défense supérieure.',
    'PHB', '5e'
),
(
    'maitre_betes',
    'Maître des bêtes',
    'maitre_betes',
    'rodeur',
    'Forge un lien puissant avec un compagnon animal (bête ID ≤ 1/4, INT ≤ 4, Moy. ou moins). Le compagnon s''améliore avec le rôdeur et peut attaquer en tandem à partir du niveau 5.',
    'PHB', '5e'
);

-- ============================================================================
-- 3. PRÉSENTATIONS
-- ============================================================================
INSERT OR IGNORE INTO classe_presentations (id, classe_id, ordre, titre, texte) VALUES
('rodeur_pres_0', 'rodeur', 0, NULL,
    'Un humain à l''air féroce et sauvage se glisse seul dans les ombres de la forêt, sur les traces d''orcs qui, il le sait, prépareront d''attaquer une ferme voisine.'),
('rodeur_pres_1', 'rodeur', 1, 'De redoutables chasseurs',
    'Combattants des étendues sauvages, les rôdeurs se spécialisent dans la traque des monstres menaçant les abords de la civilisation. Ils traquent leur proie à la manière des prédateurs, en se déplaçant en silence dans les sous-bois. Ils connaissent si bien la nature qu''ils peuvent lancer des sorts en puisant dedans, à la manière des druides.'),
('rodeur_pres_2', 'rodeur', 2, 'Des aventuriers indépendants',
    'Le rôdeur peut gagner sa vie comme chasseur, guide ou pisteur, mais sa véritable vocation est de défendre les abords de la civilisation contre les hordes d''humanoïdes. Ils s''organisent rarement en ordre formel, mais en confréries secrètes. Leur fierté indépendante en fait de parfaits aventuriers.');

-- ============================================================================
-- 4. ÉVOLUTIONS — Rôdeur (classe de base)
-- col_1 : "Sorts connus" (nombre)
-- sort_lvl_1..5 : emplacements de sorts (NULL pour niv 1 sans incantation)
-- ============================================================================
INSERT OR IGNORE INTO classe_evolutions (
    id, classe_id, niveau, bonus_maitrise, choix_sous_classe, aptitudes_gagnees,
    col_1_titre, col_1_type, col_1_nombre,
    sort_lvl_1, sort_lvl_2, sort_lvl_3, sort_lvl_4, sort_lvl_5
) VALUES
('rodeur_lvl_1',  'rodeur',  1, 2, 0,
    'rodeur_ennemi_jure,rodeur_explorateur_ne',
    'Sorts connus', 0, 0,
    NULL, NULL, NULL, NULL, NULL),
('rodeur_lvl_2',  'rodeur',  2, 2, 0,
    'rodeur_style_combat,rodeur_sorts',
    'Sorts connus', 0, 2,
    2, NULL, NULL, NULL, NULL),
('rodeur_lvl_3',  'rodeur',  3, 2, 1,
    'rodeur_archeType,rodeur_vigilance_primitive',
    'Sorts connus', 0, 3,
    3, NULL, NULL, NULL, NULL),
('rodeur_lvl_4',  'rodeur',  4, 2, 0,
    'rodeur_amelioration_carac',
    'Sorts connus', 0, 3,
    3, NULL, NULL, NULL, NULL),
('rodeur_lvl_5',  'rodeur',  5, 3, 0,
    'rodeur_frappe_supplementaire',
    'Sorts connus', 0, 4,
    4, 2, NULL, NULL, NULL),
('rodeur_lvl_6',  'rodeur',  6, 3, 0,
    'rodeur_ennemi_jure_2,rodeur_explorateur_ne_2',
    'Sorts connus', 0, 4,
    4, 2, NULL, NULL, NULL),
('rodeur_lvl_7',  'rodeur',  7, 3, 0,
    'rodeur_aptitude_archeType',
    'Sorts connus', 0, 5,
    4, 3, NULL, NULL, NULL),
('rodeur_lvl_8',  'rodeur',  8, 3, 0,
    'rodeur_amelioration_carac,rodeur_foulee_tellurique',
    'Sorts connus', 0, 5,
    4, 3, NULL, NULL, NULL),
('rodeur_lvl_9',  'rodeur',  9, 4, 0,
    NULL,
    'Sorts connus', 0, 6,
    4, 3, 2, NULL, NULL),
('rodeur_lvl_10', 'rodeur', 10, 4, 0,
    'rodeur_explorateur_ne_3,rodeur_camouflage_naturel',
    'Sorts connus', 0, 6,
    4, 3, 2, NULL, NULL),
('rodeur_lvl_11', 'rodeur', 11, 4, 0,
    'rodeur_aptitude_archeType_2',
    'Sorts connus', 0, 7,
    4, 3, 3, NULL, NULL),
('rodeur_lvl_12', 'rodeur', 12, 4, 0,
    'rodeur_amelioration_carac',
    'Sorts connus', 0, 7,
    4, 3, 3, NULL, NULL),
('rodeur_lvl_13', 'rodeur', 13, 5, 0,
    NULL,
    'Sorts connus', 0, 8,
    4, 3, 3, 1, NULL),
('rodeur_lvl_14', 'rodeur', 14, 5, 0,
    'rodeur_ennemi_jure_3,rodeur_disparition',
    'Sorts connus', 0, 8,
    4, 3, 3, 1, NULL),
('rodeur_lvl_15', 'rodeur', 15, 5, 0,
    'rodeur_aptitude_archeType_3',
    'Sorts connus', 0, 9,
    4, 3, 3, 2, NULL),
('rodeur_lvl_16', 'rodeur', 16, 5, 0,
    'rodeur_amelioration_carac',
    'Sorts connus', 0, 9,
    4, 3, 3, 2, NULL),
('rodeur_lvl_17', 'rodeur', 17, 6, 0,
    NULL,
    'Sorts connus', 0, 10,
    4, 3, 3, 3, 1),
('rodeur_lvl_18', 'rodeur', 18, 6, 0,
    'rodeur_sens_sauvages',
    'Sorts connus', 0, 10,
    4, 3, 3, 3, 1),
('rodeur_lvl_19', 'rodeur', 19, 6, 0,
    'rodeur_amelioration_carac',
    'Sorts connus', 0, 11,
    4, 3, 3, 3, 2),
('rodeur_lvl_20', 'rodeur', 20, 6, 0,
    'rodeur_tueur_implacable',
    'Sorts connus', 0, 11,
    4, 3, 3, 3, 2);

-- ============================================================================
-- 4b. ÉVOLUTIONS — Sous-classes
-- ============================================================================
INSERT OR IGNORE INTO classe_evolutions (id, classe_id, niveau, bonus_maitrise, choix_sous_classe, aptitudes_gagnees) VALUES
-- Chasseur
('chasseur_lvl_3',  'chasseur',  3, 2, 0, 'chasseur_proie'),
('chasseur_lvl_7',  'chasseur',  7, 3, 0, 'chasseur_tactiques'),
('chasseur_lvl_11', 'chasseur', 11, 4, 0, 'chasseur_multiples'),
('chasseur_lvl_15', 'chasseur', 15, 5, 0, 'chasseur_superieure'),
-- Maître des bêtes
('mb_lvl_3',  'maitre_betes',  3, 2, 0, 'mb_compagnon'),
('mb_lvl_7',  'maitre_betes',  7, 3, 0, 'mb_dressage'),
('mb_lvl_11', 'maitre_betes', 11, 4, 0, 'mb_fureur'),
('mb_lvl_15', 'maitre_betes', 15, 5, 0, 'mb_sorts_partages');

-- ============================================================================
-- 5. CHOIX DE BUILD
-- ============================================================================
-- Style de combat (niv 2, classe de base)
INSERT OR IGNORE INTO classe_choix_champ (id, classe_id, nom, alias, niveau, description) VALUES
('rodeur_champ_style',    'rodeur', 'Style de combat',       'style_combat',    2, NULL),
('rodeur_champ_archetype','rodeur', 'Archétype de rôdeur',   'archetype_rodeur', 3, NULL);

INSERT OR IGNORE INTO classe_choix_valeur (id, champ_id, nom, valeur, description) VALUES
('rodeur_style_archerie',      'rodeur_champ_style', 'Archerie',              'archerie',    '+2 aux jets d''attaque avec les armes à distance'),
('rodeur_style_deux_armes',    'rodeur_champ_style', 'Combat à deux armes',   'deux_armes',  'Ajoutez le mod. de carac. aux dégâts de la 2e attaque'),
('rodeur_style_defense',       'rodeur_champ_style', 'Défense',               'defense',     '+1 à la CA quand vous portez une armure'),
('rodeur_style_duel',          'rodeur_champ_style', 'Duel',                  'duel',        '+2 aux jets de dégâts : arme 1 main, autre main vide'),
('rodeur_arch_chasseur',       'rodeur_champ_archetype', 'Chasseur',          'chasseur',    NULL),
('rodeur_arch_maitre_betes',   'rodeur_champ_archetype', 'Maître des bêtes',  'maitre_betes',NULL);

-- Choix internes du Chasseur
INSERT OR IGNORE INTO classe_choix_champ (id, classe_id, nom, alias, niveau, description) VALUES
('chasseur_champ_proie',      'chasseur', 'Proie du chasseur',           'proie',      3, NULL),
('chasseur_champ_tactiques',  'chasseur', 'Tactiques défensives',        'tactiques',  7, NULL),
('chasseur_champ_multiples',  'chasseur', 'Attaques multiples',          'multiples', 11, NULL),
('chasseur_champ_superieure', 'chasseur', 'Défense de chasseur supérieure', 'superieure', 15, NULL);

INSERT OR IGNORE INTO classe_choix_valeur (id, champ_id, nom, valeur, description) VALUES
-- Proie niv 3
('chasseur_proie_colosses', 'chasseur_champ_proie', 'Bourreau des colosses',  'bourreau_colosses', '1×/tour : +1d8 dégâts si la cible est sous son max de PV'),
('chasseur_proie_geants',   'chasseur_champ_proie', 'Tueur de géants',        'tueur_geants',      'Réaction : attaquez une créature Grande+ qui vous touche/rate'),
('chasseur_proie_hordes',   'chasseur_champ_proie', 'Pourfendeur de hordes',  'pourfendeur_hordes','1×/tour : 2e attaque (même arme) contre une créature adjacente'),
-- Tactiques niv 7
('chasseur_tact_instinct',  'chasseur_champ_tactiques', 'Esquive instinctive',        'esquive_instinctive', 'Attaques d''opportunité contre vous : désavantage'),
('chasseur_tact_defense',   'chasseur_champ_tactiques', 'Défense contre multi-attaques','defense_multiples','Touché : +4 CA contre les attaques suivantes de cette créature ce tour'),
('chasseur_tact_moral',     'chasseur_champ_tactiques', 'Moral d''acier',              'moral_acier',        'Avantage aux JS contre l''état effrayé'),
-- Multiples niv 11
('chasseur_mult_volee',     'chasseur_champ_multiples', 'Volée',                'volee',        'Action : attaques à distance contre toutes créatures dans un rayon de 3 m d''un point'),
('chasseur_mult_tourbillon','chasseur_champ_multiples', 'Attaque tourbillonnante','tourbillonnante','Action : attaques corps à corps contre toutes créatures à 1,5 m'),
-- Supérieure niv 15
('chasseur_sup_totale',     'chasseur_champ_superieure', 'Esquive totale',        'esquive_totale',     'JS DEX ½ dégâts → succès = 0 dégâts ; échec = ½'),
('chasseur_sup_flot',       'chasseur_champ_superieure', 'Résister au flot',     'resister_flot',      'Réaction : une créature qui rate corpo à corpo répète l''attaque sur une autre cible'),
('chasseur_sup_parade',     'chasseur_champ_superieure', 'Parade instinctive',   'parade_instinctive', 'Réaction : réduisez de moitié les dégâts d''une attaque qui vous touche');

-- ============================================================================
-- 6. APTITUDES — Classe de base
-- ============================================================================
INSERT OR IGNORE INTO classe_aptitudes
    (id, classe_id, alias, nom, description, description_rapide, niveau_acquisition,
     choix_champ_id, choix_valeur_id, aptitude_prerequisite_id)
VALUES

('rodeur_ennemi_jure', 'rodeur', 'ennemi_jure', 'Ennemi juré',
'Dès le niveau 1, choisissez un type d''ennemi juré (aberrations, artificiels, bêtes, célestes, dragons, élémentaires, fées, fiélons, géants, monstruosités, morts-vivants, plantes, vases, ou deux races d''humanoïdes).\n\nVous avez l''avantage aux jets de Sagesse (Survie) pour pister vos ennemis jurés et aux tests d''Intelligence pour vous rappeler les informations les concernant. Vous apprenez une langue parlée par vos ennemis jurés.',
'Avantage : pister + connaître vos ennemis jurés ; apprenez leur langue', 1, NULL, NULL, NULL),

('rodeur_explorateur_ne', 'rodeur', 'explorateur_ne', 'Explorateur-né',
'Dès le niveau 1, choisissez un terrain de prédilection (arctique, désert, forêt, littoral, marais, montagne, outreterre ou plaine). Quand vous voyagez 1h+ dans ce terrain :\n• Pas de ralentissement par terrain difficile (groupe)\n• Pas de perte de chemin (sauf effet magique)\n• Vous restez vigilant même occupé\n• Solo : déplacement discret à vitesse normale\n• Nourriture trouvée ×2\n• Vous connaissez nombre, taille et moment de passage des créatures pistées',
'Terrain préféré : pas de terrain difficile, jamais perdu, discret en solo, nourriture ×2, traces détaillées', 1, NULL, NULL, NULL),

('rodeur_style_combat', 'rodeur', 'style_combat', 'Style de combat',
'Au niveau 2, choisissez un style de combat : Archerie (+2 attaques à distance), Combat à deux armes (+mod. carac. 2e attaque), Défense (+1 CA avec armure) ou Duel (+2 dégâts : arme 1 main, autre main vide). Chaque style ne peut être choisi qu''une seule fois.',
'Choisissez : Archerie / Combat deux armes / Défense / Duel', 2, 'rodeur_champ_style', NULL, NULL),

('rodeur_sorts', 'rodeur', 'sorts', 'Sorts',
'À partir du niveau 2, vous lancez des sorts de rôdeur grâce à la Sagesse. Vous récupérez tous vos emplacements après un repos long. Vous pouvez remplacer un sort connu par un autre à chaque gain de niveau.\n\nDD de sauvegarde = 8 + BM + mod.SAG\nModificateur d''attaque = BM + mod.SAG',
'Incantation SAG ; repos long → récup. emplacements ; remplacez 1 sort/niveau', 2, NULL, NULL, NULL),

('rodeur_archeType', 'rodeur', 'archetype', 'Archétype de rôdeur',
'Au niveau 3, choisissez un archétype : Chasseur (options de combat polyvalentes) ou Maître des bêtes (compagnon animal). Votre archétype vous octroie des aptitudes aux niveaux 3, 7, 11 et 15.',
'Choisissez votre archétype (aptitudes aux niveaux 3, 7, 11, 15)', 3, 'rodeur_champ_archetype', NULL, NULL),

('rodeur_vigilance_primitive', 'rodeur', 'vigilance_primitive', 'Vigilance primitive',
'Au niveau 3 : action + dépensez un emplacement de sort → concentrez-vous 1 min par niveau de l''emplacement. Pendant cette durée, vous percevez la présence de créatures de votre type d''ennemi juré dans un rayon de 1,5 km (10 km dans votre terrain de prédilection). Cette aptitude ne révèle ni localisation ni nombre.',
'Action + emplacement : sentez vos ennemis jurés dans 1,5 km (10 km en terrain préféré) pendant 1 min/niv emplacement', 3, NULL, NULL, NULL),

('rodeur_amelioration_carac', 'rodeur', 'amelioration_carac', 'Amélioration de caractéristiques',
'Quand vous atteignez les niveaux 4, 8, 12, 16 et 19, vous pouvez augmenter la valeur d''une caractéristique de votre choix de 2, ou augmenter deux valeurs de caractéristiques de votre choix de 1 chacune. Limite : 20 dans une valeur.',
'+2 à une carac ou +1 à deux caracs (max 20)', 4, NULL, NULL, NULL),

('rodeur_frappe_supplementaire', 'rodeur', 'frappe_supplementaire', 'Attaque supplémentaire',
'À partir du niveau 5, vous pouvez effectuer deux attaques au lieu d''une lorsque vous utilisez l''action Attaquer lors de votre tour.',
'2 attaques par action Attaquer', 5, NULL, NULL, NULL),

('rodeur_ennemi_jure_2', 'rodeur', 'ennemi_jure_2', 'Ennemi juré supplémentaire (niv 6)',
'Au niveau 6, vous choisissez un type d''ennemi juré supplémentaire et apprenez une nouvelle langue associée.',
'+1 ennemi juré et langue au niveau 6', 6, NULL, NULL, 'rodeur_ennemi_jure'),

('rodeur_explorateur_ne_2', 'rodeur', 'explorateur_ne_2', 'Terrain de prédilection supplémentaire (niv 6)',
'Au niveau 6, vous gagnez un terrain de prédilection supplémentaire.',
'+1 terrain de prédilection au niveau 6', 6, NULL, NULL, 'rodeur_explorateur_ne'),

('rodeur_aptitude_archeType', 'rodeur', 'aptitude_archeType', 'Aptitude d''archétype (niv 7)',
'Vous gagnez une aptitude propre à votre archétype au niveau 7.',
'Aptitude d''archétype niveau 7', 7, NULL, NULL, NULL),

('rodeur_foulee_tellurique', 'rodeur', 'foulee_tellurique', 'Foulée tellurique',
'À partir du niveau 8, se déplacer à travers un terrain difficile non magique ne vous coûte pas de mouvement supplémentaire. Vous pouvez traverser des plantes non magiques sans être ralenti ni blessé par elles. Vous avez l''avantage aux JS contre les plantes créées ou manipulées par magie qui entravent vos déplacements.',
'Terrain difficile non magique gratuit ; plantes non magiques sans dégâts ; avantage JS plantes magiques', 8, NULL, NULL, NULL),

('rodeur_explorateur_ne_3', 'rodeur', 'explorateur_ne_3', 'Terrain de prédilection supplémentaire (niv 10)',
'Au niveau 10, vous gagnez un troisième terrain de prédilection.',
'+1 terrain de prédilection au niveau 10', 10, NULL, NULL, 'rodeur_explorateur_ne_2'),

('rodeur_camouflage_naturel', 'rodeur', 'camouflage_naturel', 'Camouflage naturel',
'Au niveau 10, vous pouvez passer 1 minute à créer un camouflage (boue, terre, plantes, suie). Une fois camouflé, plaqué contre une surface solide et immobile, vous gagnez +10 aux jets de Dextérité (Discrétion). Le bénéfice cesse si vous vous déplacez ou attaquez.',
'1 min + matériaux naturels → camouflage : +10 Discrétion en restant immobile contre une surface', 10, NULL, NULL, NULL),

('rodeur_aptitude_archeType_2', 'rodeur', 'aptitude_archeType_2', 'Aptitude d''archétype (niv 11)',
'Vous gagnez une aptitude propre à votre archétype au niveau 11.',
'Aptitude d''archétype niveau 11', 11, NULL, NULL, 'rodeur_aptitude_archeType'),

('rodeur_ennemi_jure_3', 'rodeur', 'ennemi_jure_3', 'Ennemi juré supplémentaire (niv 14)',
'Au niveau 14, vous choisissez un troisième type d''ennemi juré et apprenez une nouvelle langue associée.',
'+1 ennemi juré et langue au niveau 14', 14, NULL, NULL, 'rodeur_ennemi_jure_2'),

('rodeur_disparition', 'rodeur', 'disparition', 'Disparition',
'À partir du niveau 14, vous pouvez utiliser l''action Se Cacher comme action bonus lors de votre tour. De plus, vous ne pouvez pas être pistés par des moyens non magiques, à moins que vous ne choisissiez de laisser des traces.',
'AB : Se Cacher ; impossibilité de vous pister par voie non magique', 14, NULL, NULL, NULL),

('rodeur_aptitude_archeType_3', 'rodeur', 'aptitude_archeType_3', 'Aptitude d''archétype (niv 15)',
'Vous gagnez une aptitude propre à votre archétype au niveau 15.',
'Aptitude d''archétype niveau 15', 15, NULL, NULL, 'rodeur_aptitude_archeType_2'),

('rodeur_sens_sauvages', 'rodeur', 'sens_sauvages', 'Sens sauvages',
'Au niveau 18, vos sens surnaturels vous aident à combattre des ennemis que vous ne pouvez pas voir : aucun désavantage aux jets d''attaque contre les créatures invisibles. Vous connaissez la localisation de toute créature invisible dans un rayon de 9 m autour de vous (si non cachée et si vous n''êtes pas aveuglé/assourdi).',
'Attaquez les invisibles sans désavantage ; localisez les invisibles à 9 m', 18, NULL, NULL, NULL),

('rodeur_tueur_implacable', 'rodeur', 'tueur_implacable', 'Tueur implacable',
'Au niveau 20, une fois par tour, vous pouvez ajouter votre modificateur de Sagesse à un jet d''attaque ou de dégâts contre l''une de vos créatures ennemies jurées. Vous décidez avant ou après le lancer, mais avant les effets.',
'1×/tour : +mod.SAG à un jet d''attaque ou dégâts contre un ennemi juré', 20, NULL, NULL, NULL);

-- ============================================================================
-- 7. APTITUDES — Chasseur
-- ============================================================================
INSERT OR IGNORE INTO classe_aptitudes
    (id, classe_id, alias, nom, description, description_rapide, niveau_acquisition,
     choix_champ_id, choix_valeur_id, aptitude_prerequisite_id)
VALUES
-- Niv 3 : Proie du chasseur (wrapper choix)
('chasseur_proie', 'chasseur', 'proie', 'Proie du chasseur',
'Au niveau 3, choisissez l''une des trois options :\n• Bourreau des colosses : 1×/tour, +1d8 dégâts si la cible est sous son max de PV\n• Tueur de géants : réaction pour attaquer une créature Grande+ qui vous touche ou rate\n• Pourfendeur de hordes : 1×/tour, 2e attaque (même arme) contre une créature adjacente à la cible',
'Choisissez : Bourreau colosses / Tueur géants / Pourfendeur hordes', 3,
'chasseur_champ_proie', NULL, NULL),

('chasseur_bourreau_colosses', 'chasseur', 'bourreau_colosses', 'Bourreau des colosses',
'Une fois lors de chacun de vos tours, quand vous touchez une créature avec une attaque d''arme, vous pouvez lui infliger 1d8 dégâts supplémentaires si ses points de vie sont inférieurs à son maximum.',
'1×/tour : +1d8 dégâts si la cible est sous son max de PV', 3,
NULL, 'chasseur_proie_colosses', 'chasseur_proie'),

('chasseur_tueur_geants', 'chasseur', 'tueur_geants', 'Tueur de géants',
'Quand une créature de taille Grande ou supérieure située dans un rayon de 1,5 mètre de vous vous touche ou vous rate avec une attaque, vous pouvez utiliser votre réaction pour l''attaquer immédiatement après son attaque, à condition que vous puissiez la voir.',
'Réaction : attaquez une créature Grande+ à 1,5 m qui vient de vous toucher ou rater', 3,
NULL, 'chasseur_proie_geants', 'chasseur_proie'),

('chasseur_pourfendeur_hordes', 'chasseur', 'pourfendeur_hordes', 'Pourfendeur de hordes',
'Une fois lors de chacun de vos tours, quand vous effectuez une attaque d''arme, vous pouvez effectuer une deuxième attaque avec la même arme contre une autre créature dans un rayon de 1,5 mètre de la cible originale et à portée de votre arme.',
'1×/tour : 2e attaque (même arme) contre une créature à 1,5 m de la cible', 3,
NULL, 'chasseur_proie_hordes', 'chasseur_proie'),

-- Niv 7 : Tactiques défensives (wrapper choix)
('chasseur_tactiques', 'chasseur', 'tactiques', 'Tactiques défensives',
'Au niveau 7, choisissez l''une des trois options :\n• Esquive instinctive : attaques d''opportunité contre vous avec désavantage\n• Défense contre les attaques multiples : touché → +4 CA contre autres attaques de cette créature ce tour\n• Moral d''acier : avantage aux JS contre l''état effrayé',
'Choisissez : Esquive instinctive / Défense multi-attaque / Moral d''acier', 7,
'chasseur_champ_tactiques', NULL, NULL),

('chasseur_esquive_instinctive', 'chasseur', 'esquive_instinctive', 'Esquive instinctive',
'Les attaques d''opportunité effectuées contre vous sont faites avec un désavantage.',
'Attaques d''opportunité contre vous : désavantage', 7,
NULL, 'chasseur_tact_instinct', 'chasseur_tactiques'),

('chasseur_defense_multiples', 'chasseur', 'defense_multiples', 'Défense contre les attaques multiples',
'Quand une créature vous touche avec une attaque, vous gagnez un bonus de +4 à la CA contre toutes les attaques ultérieures de cette créature pour le reste du tour.',
'Touché : +4 CA contre les attaques suivantes de cette créature ce tour', 7,
NULL, 'chasseur_tact_defense', 'chasseur_tactiques'),

('chasseur_moral_acier', 'chasseur', 'moral_acier', 'Moral d''acier',
'Vous avez l''avantage aux jets de sauvegarde contre l''état effrayé.',
'Avantage aux JS contre l''état effrayé', 7,
NULL, 'chasseur_tact_moral', 'chasseur_tactiques'),

-- Niv 11 : Attaques multiples (wrapper choix)
('chasseur_multiples', 'chasseur', 'multiples', 'Attaques multiples',
'Au niveau 11, choisissez l''une des deux options :\n• Volée : action → attaques à distance contre toutes créatures dans un rayon de 3 m d''un point (jets distincts)\n• Attaque tourbillonnante : action → attaques corps à corps contre toutes créatures à 1,5 m (jets distincts)',
'Choisissez : Volée (distance, rayon 3 m) / Tourbillonnante (corps à corps, 1,5 m)', 11,
'chasseur_champ_multiples', NULL, NULL),

('chasseur_volee', 'chasseur', 'volee', 'Volée',
'Vous pouvez utiliser votre action pour effectuer une attaque à distance contre un nombre quelconque de créatures situées dans un rayon de 3 mètres d''un point que vous pouvez voir, à portée de votre arme à distance. Jet d''attaque distinct pour chaque cible.',
'Action : attaquez à distance toutes créatures dans un rayon de 3 m d''un point (jets distincts)', 11,
NULL, 'chasseur_mult_volee', 'chasseur_multiples'),

('chasseur_tourbillonnante', 'chasseur', 'tourbillonnante', 'Attaque tourbillonnante',
'Vous pouvez utiliser votre action pour effectuer des attaques de corps à corps contre un nombre quelconque de créatures situées dans un rayon de 1,5 mètre de vous, avec des jets d''attaque distincts pour chaque cible.',
'Action : attaquez au corps à corps toutes créatures à 1,5 m (jets distincts)', 11,
NULL, 'chasseur_mult_tourbillon', 'chasseur_multiples'),

-- Niv 15 : Défense supérieure (wrapper choix)
('chasseur_superieure', 'chasseur', 'superieure', 'Défense de chasseur supérieure',
'Au niveau 15, choisissez l''une des trois options :\n• Esquive totale : JS DEX pour ½ dégâts → succès = 0 dégâts, échec = ½\n• Résister au flot : réaction — créature hostile qui rate corpo à corpo répète l''attaque sur une cible de votre choix\n• Parade instinctive : réaction — réduisez de moitié les dégâts d''une attaque qui vous touche',
'Choisissez : Esquive totale / Résister au flot / Parade instinctive', 15,
'chasseur_champ_superieure', NULL, NULL),

('chasseur_esquive_totale', 'chasseur', 'esquive_totale', 'Esquive totale',
'Lorsque vous êtes soumis à un effet permettant un jet de sauvegarde de Dextérité pour ne prendre que la moitié des dégâts, vous prenez 0 dégât en cas de réussite et seulement la moitié en cas d''échec.',
'JS DEX ½ dégâts → succès = 0 dégâts ; échec = ½', 15,
NULL, 'chasseur_sup_totale', 'chasseur_superieure'),

('chasseur_resister_flot', 'chasseur', 'resister_flot', 'Résister au flot',
'Quand une créature hostile rate une attaque de corps à corps contre vous, vous pouvez utiliser votre réaction pour contraindre cette créature à répéter la même attaque contre une autre créature de votre choix.',
'Réaction : la créature qui vous rate rate corpo à corpo répète l''attaque sur une cible de votre choix', 15,
NULL, 'chasseur_sup_flot', 'chasseur_superieure'),

('chasseur_parade_instinctive', 'chasseur', 'parade_instinctive', 'Parade instinctive',
'Quand un attaquant que vous pouvez voir vous touche lors d''une attaque, vous pouvez utiliser votre réaction pour réduire de moitié les dégâts de l''attaque contre vous.',
'Réaction : réduisez de moitié les dégâts d''une attaque qui vous touche', 15,
NULL, 'chasseur_sup_parade', 'chasseur_superieure');

-- ============================================================================
-- 8. APTITUDES — Maître des bêtes
-- ============================================================================
INSERT OR IGNORE INTO classe_aptitudes
    (id, classe_id, alias, nom, description, description_rapide, niveau_acquisition,
     choix_champ_id, choix_valeur_id, aptitude_prerequisite_id)
VALUES
('mb_compagnon', 'maitre_betes', 'compagnon', 'Compagnon du rôdeur',
'Au niveau 3, vous gagnez une bête compagnon (taille Moyenne ou moins, ID ≤ 1/4, INT ≤ 4). Ajoutez votre BM à ses jets d''attaque, dégâts, sauvegarde et à son ID pour ses aptitudes.\n\nLa bête agit à votre initiative. Lors de votre tour, donnez-lui verbalement l''ordre d''Attaquer, Courir, Se Désengager, Esquiver ou Se Cacher (sans action). Sans ordre, elle esquive. À partir du niveau 5, utilisez une de vos attaques (action Attaquer) pour lui ordonner d''attaquer.\n\nSi elle meurt : 8h pour se lier à une nouvelle bête non hostile.',
'Compagnon animal (Moy./moins, ID≤1/4, INT≤4) ; BM ajouté à ses stats ; ordres verbaux ; remplacement en 8h', 3, NULL, NULL, NULL),

('mb_dressage', 'maitre_betes', 'dressage', 'Dressage exceptionnel',
'Au niveau 7, par une action bonus, vous pouvez donner l''ordre à votre bête compagnon d''effectuer l''action Courir, Se Désengager, Esquiver ou Aider lors de son tour, à condition qu''elle n''utilise pas son action pour attaquer.',
'AB : donnez l''ordre Courir / Désengager / Esquiver / Aider à votre compagnon (hors attaque)', 7, NULL, NULL, NULL),

('mb_fureur', 'maitre_betes', 'fureur', 'Fureur du compagnon',
'Au niveau 11, quand vous donnez l''ordre à votre bête compagnon d''effectuer l''action Attaquer, la bête peut effectuer deux attaques, ou utiliser son action Attaque multiple si elle en possède une.',
'Ordre Attaquer → votre compagnon fait 2 attaques (ou Attaque multiple)', 11, NULL, NULL, NULL),

('mb_sorts_partages', 'maitre_betes', 'sorts_partages', 'Sorts partagés',
'À partir du niveau 15, lorsque vous lancez un sort qui vous cible uniquement, vous pouvez également affecter votre bête compagnon avec ce sort si elle se trouve dans un rayon de 9 mètres de vous.',
'Sorts à cible unique sur vous : affectez aussi votre compagnon s''il est à 9 m', 15, NULL, NULL, NULL);

-- ============================================================================
-- 9. CHOIX D'ÉQUIPEMENT
-- ============================================================================
INSERT OR IGNORE INTO classe_choix_equipement (id, classe_id, choix_num, option_num, description) VALUES
('rodeur_equip_1_1', 'rodeur', 1, 1, 'une armure de cuir'),
('rodeur_equip_1_2', 'rodeur', 1, 2, 'une armure d''écailles'),
('rodeur_equip_2_1', 'rodeur', 2, 1, 'deux épées courtes'),
('rodeur_equip_2_2', 'rodeur', 2, 2, 'deux armes courantes de corps à corps'),
('rodeur_equip_3_1', 'rodeur', 3, 1, 'un paquetage d''exploration souterraine'),
('rodeur_equip_3_2', 'rodeur', 3, 2, 'un paquetage d''explorateur'),
('rodeur_equip_4_1', 'rodeur', 4, 1, 'un arc long et un carquois de 20 flèches');
