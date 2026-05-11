-- ============================================================================
-- MIGRATION DES DONNÉES DE L'ANCIEN SCHÉMA VERS LE NOUVEAU
-- Migre les données de 'races' vers 'races_new' + tables de liaison
-- ============================================================================

-- ============================================================================
-- 1. INSERTION DES CARACTÉRISTIQUES DE BASE
-- ============================================================================
INSERT INTO caracteristiques (nom, nom_court, description) VALUES
    ('Force', 'FOR', 'Mesure la puissance physique, la capacité athlétique et la force brute'),
    ('Dextérité', 'DEX', 'Mesure l''agilité, les réflexes et l''équilibre'),
    ('Constitution', 'CON', 'Mesure l''endurance, la santé et la force vitale'),
    ('Intelligence', 'INT', 'Mesure l''acuité mentale, la mémoire et le raisonnement'),
    ('Sagesse', 'SAG', 'Mesure la perception, l''intuition et la perspicacité'),
    ('Charisme', 'CHA', 'Mesure la force de personnalité, l''éloquence et le leadership')
ON CONFLICT (nom) DO NOTHING;

-- ============================================================================
-- 2. INSERTION DES LANGUES COURANTES
-- ============================================================================
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
-- 3. MIGRATION DES RACES
-- Copier toutes les données de 'races' vers 'races_new'
-- ============================================================================
INSERT INTO races_new (
    id, nom, nom_fr,
    age_min, age_max,
    taille_min, taille_max, taille_categorie,
    description_longue, description_courte,
    vitesse_base,
    nombre_variantes_raciales,
    source,
    created_at
)
SELECT
    id,
    name,
    name_fr,
    -- Âges estimés par race (à affiner plus tard)
    CASE name
        WHEN 'Human' THEN 15
        WHEN 'Elf' THEN 100
        WHEN 'Dwarf' THEN 50
        WHEN 'Halfling' THEN 20
        WHEN 'Dragonborn' THEN 15
        WHEN 'Gnome' THEN 40
        WHEN 'Half-Elf' THEN 20
        WHEN 'Half-Orc' THEN 14
        WHEN 'Tiefling' THEN 15
        ELSE 15
    END as age_min,
    CASE name
        WHEN 'Human' THEN 80
        WHEN 'Elf' THEN 750
        WHEN 'Dwarf' THEN 350
        WHEN 'Halfling' THEN 150
        WHEN 'Dragonborn' THEN 80
        WHEN 'Gnome' THEN 500
        WHEN 'Half-Elf' THEN 180
        WHEN 'Half-Orc' THEN 75
        WHEN 'Tiefling' THEN 100
        ELSE 80
    END as age_max,
    -- Tailles estimées en cm
    CASE name
        WHEN 'Human' THEN 150
        WHEN 'Elf' THEN 150
        WHEN 'Dwarf' THEN 120
        WHEN 'Halfling' THEN 90
        WHEN 'Dragonborn' THEN 175
        WHEN 'Gnome' THEN 90
        WHEN 'Half-Elf' THEN 150
        WHEN 'Half-Orc' THEN 165
        WHEN 'Tiefling' THEN 155
        ELSE 150
    END as taille_min,
    CASE name
        WHEN 'Human' THEN 190
        WHEN 'Elf' THEN 180
        WHEN 'Dwarf' THEN 150
        WHEN 'Halfling' THEN 100
        WHEN 'Dragonborn' THEN 210
        WHEN 'Gnome' THEN 120
        WHEN 'Half-Elf' THEN 185
        WHEN 'Half-Orc' THEN 200
        WHEN 'Tiefling' THEN 190
        ELSE 190
    END as taille_max,
    size as taille_categorie,
    description as description_longue,
    description as description_courte, -- Pour l'instant même texte
    base_speed as vitesse_base,
    0 as nombre_variantes_raciales, -- À remplir manuellement plus tard
    source,
    created_at
FROM races
ON CONFLICT (nom) DO NOTHING;

-- ============================================================================
-- 4. CRÉER LES LIENS RACE <-> BONUS DE CARACTÉRISTIQUES
-- Convertir str_bonus, dex_bonus, etc. en entrées de liaison
-- ============================================================================

-- Helper: Récupérer les IDs des caractéristiques
DO $$
DECLARE
    carac_for_id UUID;
    carac_dex_id UUID;
    carac_con_id UUID;
    carac_int_id UUID;
    carac_sag_id UUID;
    carac_cha_id UUID;
BEGIN
    SELECT id INTO carac_for_id FROM caracteristiques WHERE nom_court = 'FOR';
    SELECT id INTO carac_dex_id FROM caracteristiques WHERE nom_court = 'DEX';
    SELECT id INTO carac_con_id FROM caracteristiques WHERE nom_court = 'CON';
    SELECT id INTO carac_int_id FROM caracteristiques WHERE nom_court = 'INT';
    SELECT id INTO carac_sag_id FROM caracteristiques WHERE nom_court = 'SAG';
    SELECT id INTO carac_cha_id FROM caracteristiques WHERE nom_court = 'CHA';

    -- Insérer les bonus de FOR
    INSERT INTO race_bonus_caracteristiques (race_id, caracteristique_id, valeur_bonus)
    SELECT r.id, carac_for_id, old.str_bonus
    FROM races old
    JOIN races_new r ON r.nom = old.name
    WHERE old.str_bonus > 0
    ON CONFLICT DO NOTHING;

    -- Insérer les bonus de DEX
    INSERT INTO race_bonus_caracteristiques (race_id, caracteristique_id, valeur_bonus)
    SELECT r.id, carac_dex_id, old.dex_bonus
    FROM races old
    JOIN races_new r ON r.nom = old.name
    WHERE old.dex_bonus > 0
    ON CONFLICT DO NOTHING;

    -- Insérer les bonus de CON
    INSERT INTO race_bonus_caracteristiques (race_id, caracteristique_id, valeur_bonus)
    SELECT r.id, carac_con_id, old.con_bonus
    FROM races old
    JOIN races_new r ON r.nom = old.name
    WHERE old.con_bonus > 0
    ON CONFLICT DO NOTHING;

    -- Insérer les bonus de INT
    INSERT INTO race_bonus_caracteristiques (race_id, caracteristique_id, valeur_bonus)
    SELECT r.id, carac_int_id, old.int_bonus
    FROM races old
    JOIN races_new r ON r.nom = old.name
    WHERE old.int_bonus > 0
    ON CONFLICT DO NOTHING;

    -- Insérer les bonus de SAG
    INSERT INTO race_bonus_caracteristiques (race_id, caracteristique_id, valeur_bonus)
    SELECT r.id, carac_sag_id, old.wis_bonus
    FROM races old
    JOIN races_new r ON r.nom = old.name
    WHERE old.wis_bonus > 0
    ON CONFLICT DO NOTHING;

    -- Insérer les bonus de CHA
    INSERT INTO race_bonus_caracteristiques (race_id, caracteristique_id, valeur_bonus)
    SELECT r.id, carac_cha_id, old.cha_bonus
    FROM races old
    JOIN races_new r ON r.nom = old.name
    WHERE old.cha_bonus > 0
    ON CONFLICT DO NOTHING;
END $$;

-- ============================================================================
-- 5. CRÉER LES LIENS RACE <-> LANGUES
-- Convertir l'array 'languages' en entrées de liaison
-- ============================================================================

-- Fonction temporaire pour extraire et lier les langues
DO $$
DECLARE
    race_record RECORD;
    langue_record RECORD;
    langue_nom TEXT;
BEGIN
    -- Pour chaque race
    FOR race_record IN SELECT id, name, languages FROM races WHERE languages IS NOT NULL LOOP
        -- Pour chaque langue dans l'array
        FOR langue_nom IN SELECT unnest(race_record.languages) LOOP
            -- Trouver l'ID de la langue (mapping approximatif)
            SELECT id INTO langue_record FROM langues WHERE nom ILIKE langue_nom LIMIT 1;

            IF langue_record.id IS NOT NULL THEN
                -- Créer le lien
                INSERT INTO race_langues (race_id, langue_id, est_langue_bonus)
                SELECT r.id, langue_record.id, false
                FROM races_new r
                WHERE r.nom = race_record.name
                ON CONFLICT DO NOTHING;
            END IF;
        END LOOP;
    END LOOP;
END $$;

-- ============================================================================
-- 6. MIGRER LES ABILITÉS RACIALES EXISTANTES
-- Copier depuis racial_traits vers abilites_raciales
-- ============================================================================

-- Créer les abilités
INSERT INTO abilites_raciales (nom, nom_fr, description)
SELECT DISTINCT name, name_fr, description
FROM racial_traits
ON CONFLICT DO NOTHING;

-- Lier les abilités aux races
INSERT INTO race_abilites_raciales (race_id, abilite_raciale_id, niveau_acquisition)
SELECT
    rn.id as race_id,
    ar.id as abilite_raciale_id,
    1 as niveau_acquisition
FROM racial_traits rt
JOIN races r ON r.id = rt.race_id
JOIN races_new rn ON rn.nom = r.name
JOIN abilites_raciales ar ON ar.nom = rt.name
ON CONFLICT DO NOTHING;

-- ============================================================================
-- 7. AJOUTER DES EXEMPLES DE NOMS (données minimales pour test)
-- ============================================================================
UPDATE races_new SET
    exemple_nom_homme = CASE nom
        WHEN 'Human' THEN 'Jean, Pierre, Marc, Thomas'
        WHEN 'Elf' THEN 'Legolas, Thranduil, Celeborn, Elrond'
        WHEN 'Dwarf' THEN 'Thorin, Gimli, Balin, Dwalin'
        WHEN 'Halfling' THEN 'Frodo, Bilbo, Merry, Pippin'
        WHEN 'Dragonborn' THEN 'Drakus, Kriv, Shamash, Torinn'
        WHEN 'Gnome' THEN 'Alston, Boddynock, Dimble, Fonkin'
        WHEN 'Half-Elf' THEN 'Aelar, Beren, Tanis, Kendel'
        WHEN 'Half-Orc' THEN 'Grog, Throk, Kragg, Mord'
        WHEN 'Tiefling' THEN 'Akmenios, Damakos, Kairon, Leucis'
        ELSE NULL
    END,
    exemple_nom_femme = CASE nom
        WHEN 'Human' THEN 'Marie, Sophie, Claire, Anne'
        WHEN 'Elf' THEN 'Arwen, Galadriel, Tauriel, Luthien'
        WHEN 'Dwarf' THEN 'Dis, Vilja, Kathra, Riswynn'
        WHEN 'Halfling' THEN 'Rose, Lobelia, Primrose, Daisy'
        WHEN 'Dragonborn' THEN 'Akra, Biri, Mishann, Thava'
        WHEN 'Gnome' THEN 'Bimpnottin, Caramip, Duvamil, Ella'
        WHEN 'Half-Elf' THEN 'Faral, Immeral, Mara, Syllia'
        WHEN 'Half-Orc' THEN 'Baggi, Emen, Ovak, Vola'
        WHEN 'Tiefling' THEN 'Akta, Bryseis, Kallista, Makaria'
        ELSE NULL
    END,
    type_categorie_nom = CASE nom
        WHEN 'Dwarf' THEN 'Nom de clan'
        WHEN 'Halfling' THEN 'Nom de famille'
        WHEN 'Dragonborn' THEN 'Nom de clan'
        ELSE NULL
    END,
    exemple_nom_categorie_speciale = CASE nom
        WHEN 'Dwarf' THEN 'Forgefeu, Marteaudefeu, Pierregrise'
        WHEN 'Halfling' THEN 'Tealeaf, Underhill, Goodbarrel'
        WHEN 'Dragonborn' THEN 'Clethtinthiallor, Daardendrian, Delmirev'
        ELSE NULL
    END
WHERE nom IN ('Human', 'Elf', 'Dwarf', 'Halfling', 'Dragonborn', 'Gnome', 'Half-Elf', 'Half-Orc', 'Tiefling');

-- ============================================================================
-- RÉSUMÉ DE LA MIGRATION
-- ============================================================================
DO $$
DECLARE
    nb_races INTEGER;
    nb_bonus INTEGER;
    nb_langues_liees INTEGER;
    nb_abilites INTEGER;
BEGIN
    SELECT COUNT(*) INTO nb_races FROM races_new;
    SELECT COUNT(*) INTO nb_bonus FROM race_bonus_caracteristiques;
    SELECT COUNT(*) INTO nb_langues_liees FROM race_langues;
    SELECT COUNT(*) INTO nb_abilites FROM abilites_raciales;

    RAISE NOTICE '========================================';
    RAISE NOTICE 'MIGRATION TERMINÉE';
    RAISE NOTICE '========================================';
    RAISE NOTICE 'Races migrées: %', nb_races;
    RAISE NOTICE 'Bonus de caractéristiques créés: %', nb_bonus;
    RAISE NOTICE 'Liens race-langue créés: %', nb_langues_liees;
    RAISE NOTICE 'Abilités raciales créées: %', nb_abilites;
    RAISE NOTICE '========================================';
END $$;
