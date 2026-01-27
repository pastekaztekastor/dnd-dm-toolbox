# Base de Données D&D 5e - DM Toolbox

## 📚 Vue d'ensemble

Cette base de données PostgreSQL contient toutes les informations nécessaires pour gérer des campagnes D&D 5e, incluant :

- **Données de jeu** : Races, classes, sorts, équipement, monstres
- **Données de campagne** : Personnages, inventaires, quêtes, notes de session
- **Système de combat** : Rencontres, initiative, conditions
- **Monde** : Cartes, donjons, lieux

## 🗂️ Structure des fichiers

```
database/
├── init/
│   ├── 00_game_data_schema.sql    # Schéma pour données du jeu D&D 5e
│   ├── 01_create_schema.sql       # Schéma pour données de campagne
│   ├── 02_seed_races.sql          # Données des 9 races + sous-races
│   ├── 03_seed_classes.sql        # Données des 12 classes + progression
│   ├── 04_seed_skills_equipment.sql # Compétences, armes, armures, équipement
│   └── 05_seed_spells_sample.sql  # Échantillon de sorts (cantrips à niveau 3)
└── README.md                       # Ce fichier
```

## 📊 Schéma de la base de données

### DONNÉES DE JEU (00_game_data_schema.sql)

#### Races et Traits Raciaux

| Table | Description |
|-------|-------------|
| `races` | 9 races de base (Humain, Elfe, Nain, etc.) |
| `racial_traits` | Traits raciaux (Vision dans le noir, Chanceux, etc.) |
| `subraces` | Sous-races (Haut-Elfe, Elfe des bois, etc.) |
| `subrace_traits` | Traits spécifiques aux sous-races |

**Exemple de données** :
- Humain : +1 à toutes les caractéristiques
- Elfe : +2 DEX, vision dans le noir, ascendance féerique
- Nain : +2 CON, résistance au poison, vitesse 25 ft

#### Classes et Progression

| Table | Description |
|-------|-------------|
| `classes` | 12 classes de base |
| `class_skill_choices` | Compétences disponibles par classe |
| `class_features_by_level` | Progression niveau par niveau (1-20) |
| `class_features` | Capacités de classe détaillées |
| `archetypes` | Sous-classes (Archétypes) |
| `archetype_features` | Capacités d'archétype |

**Exemple de données** :
- **Guerrier** : d10 HP, 2 compétences, JdS FOR/CON
- **Magicien** : d6 HP, 2 compétences, JdS INT/SAG, lanceur complet
- Progression avec emplacements de sorts pour lanceurs
- Capacités par niveau (Second souffle, Fougue, etc.)

#### Compétences

| Table | Description |
|-------|-------------|
| `skills` | 18 compétences D&D 5e |

**Données** : Acrobaties (DEX), Athlétisme (FOR), Perception (SAG), etc.

#### Armes et Armures

| Table | Description |
|-------|-------------|
| `weapons` | Armes courantes et de guerre |
| `armors` | Armures légères, intermédiaires, lourdes, boucliers |

**Exemples** :
- **Épée longue** : 1d8 tranchant, polyvalente (1d10), 15 po
- **Harnois** : CA 18, FOR 15, désavantage Discrétion, 1500 po

#### Équipement

| Table | Description |
|-------|-------------|
| `equipment` | Équipement d'aventure, outils |
| `equipment_packs` | Packs pré-configurés |
| `pack_contents` | Contenu des packs |

#### Sorts

| Table | Description |
|-------|-------------|
| `game_spells` | Tous les sorts D&D 5e |

**Structure** :
- Niveau 0-9
- École de magie
- Composantes (V, S, M)
- Durée, portée, concentration
- Description FR et EN
- Classes disponibles

**Exemples fournis** :
- Cantrips : Trait de feu, Décharge occulte, Main du mage
- Niveau 1 : Projectile magique, Bouclier, Soin des blessures
- Niveau 2 : Pas brumeux, Rayon ardent
- Niveau 3 : Boule de feu, Contresort, Rappel à la vie

#### Monstres et Créatures

| Table | Description |
|-------|-------------|
| `monsters` | Créatures avec stats complètes |
| `monster_traits` | Traits spéciaux |
| `monster_actions` | Actions et attaques |

#### Objets Magiques

| Table | Description |
|-------|-------------|
| `magic_items` | Objets magiques par rareté |

#### Autres

| Table | Description |
|-------|-------------|
| `backgrounds` | Historiques de personnage |
| `feats` | Dons optionnels |
| `conditions` | Conditions (aveuglé, charmé, etc.) |

### DONNÉES DE CAMPAGNE (01_create_schema.sql)

#### Campagnes et Personnages

| Table | Description |
|-------|-------------|
| `campaigns` | Campagnes du MJ |
| `characters` | Personnages joueurs et PNJ |
| `character_skills` | Maîtrises de compétences |
| `character_spells` | Sorts connus/préparés |
| `character_spell_slots` | Emplacements de sorts |

#### Inventaire

| Table | Description |
|-------|-------------|
| `inventory_items` | Équipement des personnages |

#### Combat

| Table | Description |
|-------|-------------|
| `encounters` | Rencontres de combat |
| `combat_participants` | Initiative et stats en combat |
| `combat_conditions` | Conditions appliquées en combat |

#### Monde

| Table | Description |
|-------|-------------|
| `locations` | Lieux de la campagne |
| `maps` | Cartes et donjons |
| `quests` | Quêtes actives/complétées |
| `session_notes` | Notes de session |

## 🚀 Utilisation avec Docker

### Démarrer la base de données

```bash
# Démarrer PostgreSQL et PgAdmin
docker-compose up -d

# Vérifier que c'est lancé
docker-compose ps
```

### Accès

- **PostgreSQL** : `localhost:5432`
  - User: `dnd_user`
  - Password: `dnd_password`
  - Database: `dnd_toolbox`

- **PgAdmin** : `http://localhost:5050`
  - Email: `admin@dndtoolbox.local`
  - Password: `admin`

### Commandes utiles

```bash
# Voir les logs
docker-compose logs postgres

# Arrêter
docker-compose down

# Arrêter et supprimer les données
docker-compose down -v

# Se connecter à PostgreSQL
docker-compose exec postgres psql -U dnd_user -d dnd_toolbox
```

## 📝 Requêtes SQL utiles

### Lister toutes les races avec leurs bonus

```sql
SELECT name_fr, str_bonus, dex_bonus, con_bonus, int_bonus, wis_bonus, cha_bonus, base_speed
FROM races
ORDER BY name_fr;
```

### Obtenir toutes les capacités d'un personnage Guerrier niveau 5

```sql
SELECT cfl.level, cfl.proficiency_bonus, unnest(cfl.features_gained) as feature
FROM classes c
JOIN class_features_by_level cfl ON c.id = cfl.class_id
WHERE c.name = 'Fighter' AND cfl.level <= 5
ORDER BY cfl.level;
```

### Trouver tous les sorts de Magicien de niveau 1

```sql
SELECT name_fr, school_fr, casting_time_fr, description_fr
FROM game_spells
WHERE level = 1 AND 'Wizard' = ANY(available_to_classes)
ORDER BY name_fr;
```

### Compétences disponibles pour un Roublard

```sql
SELECT s.name_fr, s.ability, s.description
FROM classes c
JOIN class_skill_choices csc ON c.id = csc.class_id
JOIN skills s ON s.name = csc.skill_name
WHERE c.name = 'Rogue'
ORDER BY s.name_fr;
```

### Armes à 2 mains avec plus de 10 dégâts moyens

```sql
SELECT name_fr, damage_dice, damage_type, cost_gp, weight
FROM weapons
WHERE 'two-handed' = ANY(properties)
ORDER BY cost_gp DESC;
```

## 🔧 Ajouter des données

### Nouvelle race

```sql
INSERT INTO races (name, name_fr, description, dex_bonus, base_speed, size, languages)
VALUES ('Tabaxi', 'Tabaxi', 'Humanoïdes félins agiles et curieux.', 2, 30, 'Medium', ARRAY['Common']);

-- Ajouter un trait
INSERT INTO racial_traits (race_id, name, name_fr, description, trait_type)
SELECT id, 'Feline Agility', 'Agilité féline',
'Vous pouvez doubler votre vitesse jusqu''à la fin de votre tour (1/mouvement).', 'active'
FROM races WHERE name = 'Tabaxi';
```

### Nouveau sort

```sql
INSERT INTO game_spells (
    name, name_fr, level, school, school_fr,
    casting_time, casting_time_fr, range, range_fr,
    has_verbal, has_somatic, has_material,
    duration, duration_fr, concentration,
    description, description_fr,
    available_to_classes
) VALUES (
    'Thunderwave', 'Vague tonnante', 1, 'Evocation', 'Évocation',
    '1 action', '1 action', 'Self (15-foot cube)', 'Personnelle (cube de 4,5 m)',
    true, true, false,
    'Instantaneous', 'Instantanée', false,
    'A wave of thunderous force sweeps out. Each creature must make a Constitution save or take 2d8 thunder damage and be pushed 10 feet away.',
    'Une vague de force tonitruante déferle. Chaque créature doit réussir un JdS de Constitution ou subir 2d8 dégâts de tonnerre et être repoussée de 3 mètres.',
    ARRAY['Wizard', 'Sorcerer', 'Bard', 'Druid']
);
```

## 📈 Évolutions futures

- [ ] Compléter tous les sorts du Player's Handbook
- [ ] Ajouter les monstres du Monster Manual
- [ ] Ajouter les objets magiques du DMG
- [ ] Races supplémentaires (Volo's Guide, etc.)
- [ ] Classes supplémentaires (Artificier, etc.)
- [ ] Archétypes complets avec toutes les capacités
- [ ] Historiques détaillés
- [ ] Dons complets

## 🛠️ Maintenance

### Backup de la base

```bash
docker-compose exec postgres pg_dump -U dnd_user dnd_toolbox > backup.sql
```

### Restaurer un backup

```bash
cat backup.sql | docker-compose exec -T postgres psql -U dnd_user -d dnd_toolbox
```

### Réinitialiser la base

```bash
docker-compose down -v
docker-compose up -d
# Les scripts init/*.sql seront automatiquement exécutés
```

## 📚 Ressources

- [PostgreSQL Documentation](https://www.postgresql.org/docs/)
- [D&D 5e SRD](https://dnd.wizards.com/resources/systems-reference-document)
- [Docker Compose Documentation](https://docs.docker.com/compose/)

## 📄 Licence

Les données D&D 5e sont basées sur le System Reference Document (SRD) sous licence OGL 1.0a.
