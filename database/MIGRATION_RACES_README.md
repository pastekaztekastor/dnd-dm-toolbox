# ✅ Migration du Schéma des RACES - TERMINÉE

## 📊 Résumé de la Migration

**Date** : 2026-05-03
**Statut** : ✅ Succès complet

### Nouvelles Tables Créées

#### Tables Principales
1. **`caracteristiques`** - Les 6 caractéristiques (FOR, DEX, CON, INT, SAG, CHA)
   - 6 entrées créées

2. **`langues`** - Toutes les langues D&D 5e
   - 16 langues créées (Commun, Elfique, Nain, Draconique, etc.)

3. **`races_new`** - Nouvelle table races avec structure enrichie
   - 9 races migrées depuis `races`
   - Champs ajoutés :
     - `age_min`, `age_max`
     - `taille_min`, `taille_max` (en cm)
     - `description_longue`, `description_courte`
     - `exemple_nom_homme`, `exemple_nom_femme`
     - `type_categorie_nom`, `exemple_nom_categorie_speciale`
     - `nombre_variantes_raciales`
     - `race_parent_id` (pour sous-races)

4. **`abilites_raciales`** - Capacités raciales
   - 20 abilités migrées depuis `racial_traits`

#### Tables de Liaison

5. **`race_bonus_caracteristiques`** - Race → Caractéristique avec valeur
   - 17 liens créés (Humain +1 à toutes = 6 liens, Elfe +2 DEX = 1 lien, etc.)

6. **`race_langues`** - Race → Langues
   - 2 liens créés (à compléter manuellement)

7. **`race_opinions`** - Opinions entre races
   - Table créée, vide (à remplir manuellement)

8. **`race_abilites_raciales`** - Race → Abilités
   - 26 liens créés

9. **`abilite_raciale_modificateurs_caracteristiques`** - Abilité → Mod de carac
   - Table créée, vide

10. **`abilite_raciale_sorts_acquis`** - Abilité → Sorts gratuits
    - Table créée, vide (ex: Tieffelin gagne des sorts)

11. **`abilite_raciale_jets_sauvegarde`** - Abilité → Avantage/Désavantage aux saves
    - Table créée, vide

12. **`abilite_raciale_competences_maitrisees`** - Abilité → Compétences maîtrisées
    - Table créée, vide

13. **`abilite_raciale_armes_speciales`** - Abilité → Armes spéciales
    - Table créée, vide (ex: Souffle du Drakéide)

## 📝 Exemple de Données Migrées

### ELFE
```
RACE: Elfe
  Âge: 100-750 ans
  Taille: 150-180 cm
  Vitesse: 30 pieds
  Bonus: DEX +2
  Abilités:
    - Ascendance féerique
    - Sens aiguisés
    - Transe
    - Vision dans le noir
  Exemples noms (homme): Legolas, Thranduil, Celeborn, Elrond
  Exemples noms (femme): Arwen, Galadriel, Tauriel, Luthien
```

### HUMAIN
```
RACE: Humain
  Âge: 15-80 ans
  Taille: 150-190 cm
  Vitesse: 30 pieds
  Bonus: +1 à TOUTES les caractéristiques (6 liens créés)
  Exemples noms (homme): Jean, Pierre, Marc, Thomas
  Exemples noms (femme): Marie, Sophie, Claire, Anne
```

### NAIN
```
RACE: Nain
  Âge: 50-350 ans
  Taille: 120-150 cm (Small/Medium)
  Vitesse: 25 pieds
  Bonus: CON +2
  Catégorie de nom: Nom de clan
  Exemples clan: Forgefeu, Marteaudefeu, Pierregrise
```

## 🔗 Nouvelles Relations Disponibles

### 1. Bonus Flexibles
Au lieu de :
```sql
-- Ancien
races.str_bonus = 2
races.dex_bonus = 0
```

Maintenant :
```sql
-- Nouveau - Multiple liens
race_bonus_caracteristiques
  race_id → caracteristique_id → valeur_bonus
```

**Avantage** : Support de races avec choix de bonus (ex: Humain variant)

### 2. Langues Normalisées
```sql
-- Ancienne: races.languages = ARRAY['Common', 'Elvish']
-- Nouvelle: race_langues avec référence vers table langues
SELECT l.nom, l.alphabet
FROM race_langues rl
JOIN langues l ON l.id = rl.langue_id
WHERE rl.race_id = 'id_elfe';
```

### 3. Abilités Raciales Détaillées
Chaque abilité peut avoir :
- Modificateurs de caractéristiques
- Sorts acquis automatiquement
- Avantages aux jets de sauvegarde
- Compétences maîtrisées
- Armes/attaques spéciales

## ⚠️ Tables à Compléter Manuellement

### 1. **race_langues**
Actuellement seulement 2 liens créés. Il faut ajouter :
```sql
-- Ex: Elfe parle Commun + Elfique
INSERT INTO race_langues (race_id, langue_id)
SELECT r.id, l.id
FROM races_new r, langues l
WHERE r.nom = 'Elf' AND l.nom IN ('Commun', 'Elfique');
```

### 2. **race_opinions**
Ajouter les opinions entre races :
```sql
INSERT INTO race_opinions (race_source_id, race_cible_id, opinion_texte)
VALUES (
  (SELECT id FROM races_new WHERE nom = 'Elf'),
  (SELECT id FROM races_new WHERE nom = 'Dwarf'),
  'Les elfes apprécient le travail des nains mais trouvent leurs manières un peu rudes.'
);
```

### 3. **abilite_raciale_sorts_acquis**
Pour le Tieffelin qui gagne des sorts :
```sql
-- Exemple: Tieffelin gagne Thaumaturgy au niveau 1
INSERT INTO abilite_raciale_sorts_acquis (
  abilite_raciale_id,
  sort_id,
  niveau_acquisition
)
SELECT
  (SELECT id FROM abilites_raciales WHERE nom = 'Infernal Legacy'),
  (SELECT id FROM game_spells WHERE name ILIKE 'Thaumaturgy'),
  1;
```

### 4. **abilite_raciale_armes_speciales**
Pour le souffle du Drakéide :
```sql
INSERT INTO abilite_raciale_armes_speciales (
  abilite_raciale_id,
  nom_arme_custom,
  description_arme,
  degats_formule,
  type_degats,
  portee_normale,
  nombre_utilisations
)
VALUES (
  (SELECT id FROM abilites_raciales WHERE nom = 'Souffle draconique'),
  'Souffle de feu',
  'Cône de 15 pieds',
  '2d6',
  'fire',
  15,
  '1/repos court'
);
```

## 🎯 Prochaines Étapes

### Immédiat
- [ ] Compléter les liens `race_langues`
- [ ] Remplir `race_opinions` avec les textes du PHB
- [ ] Ajouter les sorts des Tieffelins dans `abilite_raciale_sorts_acquis`
- [ ] Ajouter le souffle des Drakéides dans `abilite_raciale_armes_speciales`

### Moyen Terme
- [ ] Créer les **sous-races** (Haut-Elfe, Elfe des bois, etc.)
  - Utiliser `race_parent_id` pour pointer vers la race parente
  - Ajouter leurs bonus/abilités spécifiques

- [ ] Enrichir les descriptions longues (markdown)

- [ ] Ajouter plus d'exemples de noms par race

### Long Terme
- [ ] Créer un générateur de noms utilisant ces données
- [ ] Implémenter la sélection de sous-race dans l'UI
- [ ] Afficher les opinions raciales dans l'interface

## 🔧 Requêtes Utiles

### Voir tous les bonus d'une race
```sql
SELECT
    r.nom_fr,
    c.nom_court,
    rbc.valeur_bonus
FROM races_new r
JOIN race_bonus_caracteristiques rbc ON r.id = rbc.race_id
JOIN caracteristiques c ON c.id = rbc.caracteristique_id
WHERE r.nom = 'Human';
```

### Voir toutes les abilités d'une race
```sql
SELECT
    r.nom_fr as race,
    ar.nom_fr as abilite,
    ar.description
FROM races_new r
JOIN race_abilites_raciales rar ON r.id = rar.race_id
JOIN abilites_raciales ar ON ar.id = rar.abilite_raciale_id
WHERE r.nom = 'Dragonborn';
```

### Compter les races par catégorie de taille
```sql
SELECT taille_categorie, COUNT(*)
FROM races_new
GROUP BY taille_categorie;
```

## 📁 Fichiers Créés

1. `database/init/11_new_race_schema.sql` - Schéma complet
2. `database/init/12_migrate_race_data.sql` - Script de migration
3. `database/MIGRATION_RACES_README.md` - Ce fichier

## ✅ Validation

- [x] 9 races migrées
- [x] 6 caractéristiques créées
- [x] 16 langues créées
- [x] 20 abilités raciales migrées
- [x] 17 liens bonus de caractéristiques créés
- [x] Exemples de noms ajoutés pour 9 races
- [x] Âges et tailles estimés pour toutes les races
- [x] Index de performance créés
- [x] Trigger de mise à jour automatique

**Statut Final** : ✅ Migration réussie et testée
