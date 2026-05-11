# Outils de Scraping D&D 5e

## 📚 Description

Ces scripts Python permettent de récupérer automatiquement toutes les données D&D 5e depuis le site **aidedd.org**, qui met à disposition gratuitement le contenu sous licence OGL 1.0a.

## 🎯 Données récupérables

- **Sorts** : Tous les sorts D&D 5e avec descriptions complètes en français
- **Monstres** : Toutes les créatures avec leurs statistiques
- **Objets magiques** : Tous les objets magiques par rareté
- (À venir) Classes, races, backgrounds, etc.

## 🛠️ Prérequis

```bash
# Installer les dépendances Python
pip3 install requests beautifulsoup4
```

## 🚀 Utilisation

### 1. Scraper les données depuis aidedd.org

```bash
# Tout récupérer
python3 tools/scraper_aidedd.py

# Uniquement les sorts
python3 tools/scraper_aidedd.py --spells-only

# Uniquement les monstres
python3 tools/scraper_aidedd.py --monsters-only

# Uniquement les objets magiques
python3 tools/scraper_aidedd.py --items-only

# Spécifier un répertoire de sortie
python3 tools/scraper_aidedd.py --output mon_dossier
```

**Sortie** : Fichiers JSON dans `scraped_data/`
- `spells.json` : Tous les sorts
- `monsters.json` : Tous les monstres
- `magic_items.json` : Tous les objets magiques

### 2. Convertir en SQL

```bash
# Convertir les données JSON en fichiers SQL
python3 tools/json_to_sql.py

# Avec répertoires personnalisés
python3 tools/json_to_sql.py --input scraped_data --output database/init
```

**Sortie** : Fichiers SQL dans `database/init/`
- `06_aidedd_spells.sql` : INSERT pour tous les sorts
- `07_aidedd_monsters.sql` : INSERT pour tous les monstres

### 3. Importer dans la base de données

```bash
# Démarrer PostgreSQL
docker-compose up -d postgres

# Importer les données
cat database/init/06_aidedd_spells.sql | docker-compose exec -T postgres psql -U dnd_user -d dnd_toolbox
```

## 📊 Exemple de données récupérées

### Sort (JSON)

```json
{
  "name": "Boule de feu",
  "level": "3",
  "school": "Évocation",
  "ritual": false,
  "casting_time": "1 action",
  "range": "45 mètres",
  "components": "V, S, M (une petite boule de guano de chauve-souris et du soufre)",
  "has_verbal": true,
  "has_somatic": true,
  "has_material": true,
  "material": "une petite boule de guano de chauve-souris et du soufre",
  "duration": "Instantanée",
  "concentration": false,
  "description": "Un éclair brillant file depuis votre doigt pointé vers un point que vous choisissez à portée...",
  "at_higher_levels": "Lorsque vous lancez ce sort en utilisant un emplacement de sort de niveau 4 ou supérieur, les dégâts augmentent de 1d6 pour chaque niveau d'emplacement au-delà du niveau 3.",
  "classes": ["Ensorceleur", "Magicien"]
}
```

### Sort (SQL généré)

```sql
INSERT INTO game_spells (
    name, name_fr, level, school, school_fr,
    casting_time, casting_time_fr, range, range_fr,
    has_verbal, has_somatic, has_material, material_component,
    duration, duration_fr, concentration, ritual,
    description, description_fr,
    at_higher_levels_fr,
    available_to_classes
) VALUES (
    'Boule de feu',
    'Boule de feu',
    3,
    'Evocation',
    'Évocation',
    '1 action',
    '1 action',
    '45 mètres',
    '45 mètres',
    true,
    true,
    true,
    'une petite boule de guano de chauve-souris et du soufre',
    'Instantanée',
    'Instantanée',
    false,
    false,
    'Un éclair brillant file depuis votre doigt pointé...',
    'Un éclair brillant file depuis votre doigt pointé...',
    'Lorsque vous lancez ce sort...',
    ARRAY['Sorcerer', 'Wizard']
);
```

## 🔧 Fonctionnalités du scraper

### Scraper respectueux
- Délai de 0.5-1 seconde entre chaque requête
- User-Agent identifié comme outil éducatif
- Gestion des erreurs et timeouts

### Parsing intelligent
- Extraction automatique des composantes (V, S, M)
- Détection de la concentration
- Parsing des matériaux coûteux
- Séparation description / niveaux supérieurs
- Mapping classes FR → EN

### Conversion SQL
- Échappement correct des apostrophes
- Support des valeurs NULL
- Arrays PostgreSQL pour les classes
- Commentaires avec statistiques

## 📝 Structure des fichiers

```
tools/
├── README.md                 # Ce fichier
├── scraper_aidedd.py        # Script de scraping
├── json_to_sql.py           # Conversion JSON → SQL
└── requirements.txt         # (à créer) Dépendances Python

scraped_data/                # Données récupérées (gitignored)
├── spells.json
├── monsters.json
└── magic_items.json

database/init/               # Scripts SQL générés
├── 06_aidedd_spells.sql
└── 07_aidedd_monsters.sql
```

## ⚡ Performances

| Opération | Nombre d'éléments | Temps estimé |
|-----------|-------------------|--------------|
| Liste sorts | ~500 | 2 secondes |
| Détails sorts | ~500 | ~5 minutes |
| Liste monstres | ~400 | 2 secondes |
| Détails monstres | ~400 | ~4 minutes |
| Conversion SQL | ~900 | 1 seconde |

**Total** : ~10 minutes pour récupérer toutes les données

## 🎯 Roadmap

- [x] Scraping des sorts
- [x] Scraping des monstres (basique)
- [x] Scraping des objets magiques (liste)
- [x] Conversion SQL des sorts
- [ ] Parsing détaillé des stats de monstres
- [ ] Scraping des classes et capacités
- [ ] Scraping des races et traits
- [ ] Scraping des backgrounds
- [ ] Scraping des dons (feats)
- [ ] Mode incrémental (ne récupérer que les nouveautés)
- [ ] Cache local pour éviter de re-télécharger

## ⚠️ Avertissements

1. **Respectez le site** : Ne lancez pas le scraper plusieurs fois par jour
2. **Vérifiez les données** : Les données parsées peuvent contenir des erreurs
3. **Licence** : Le contenu d'aidedd.org est sous OGL 1.0a
4. **Usage** : À des fins éducatives et personnelles uniquement

## 🔗 Ressources

- **Site source** : https://www.aidedd.org/adj/
- **Licence OGL 1.0a** : https://www.opengamingfoundation.org/ogl.html
- **BeautifulSoup** : https://www.crummy.com/software/BeautifulSoup/
- **Requests** : https://requests.readthedocs.io/

## 🐛 Dépannage

### Erreur "Module not found"

```bash
pip3 install requests beautifulsoup4
```

### Timeout lors du scraping

```bash
# Modifier le délai dans scraper_aidedd.py
time.sleep(2)  # Au lieu de 0.5
```

### Encodage incorrect

Le script gère automatiquement l'UTF-8, mais si vous avez des problèmes :
```python
response.encoding = 'utf-8'
```

## 💡 Améliorations possibles

1. **Parallélisation** : Utiliser asyncio pour scraper plus vite
2. **Cache** : SQLite local pour éviter de re-télécharger
3. **Validation** : Schéma JSON pour valider les données
4. **Tests** : Unit tests pour le parsing
5. **CLI améliorée** : Barre de progression avec tqdm
6. **Export CSV** : Pour import dans Excel/Google Sheets

## 📄 Licence

Code sous licence MIT. Données D&D 5e sous licence OGL 1.0a (Wizards of the Coast).
