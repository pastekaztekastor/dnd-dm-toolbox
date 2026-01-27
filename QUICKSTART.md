# Quick Start Guide

Bienvenue dans le D&D DM Toolbox ! Ce guide vous aidera à démarrer rapidement.

## Option 1 : Build natif (Recommandé pour le développement)

### Prérequis
```bash
# Sur Arch/Manjaro
sudo pacman -S cmake gcc glfw-x11 mesa git

# Sur Ubuntu/Debian
sudo apt-get install cmake build-essential libglfw3-dev libgl1-mesa-dev libx11-dev git
```

### Installation et build
```bash
cd dnd-dm-toolbox

# Télécharger les dépendances (Dear ImGui, nlohmann/json)
./setup_dependencies.sh

# Build le projet
./build.sh

# Lancer l'application
cd build
./DnD_DM_Toolbox
```

## Option 2 : Docker (Recommandé pour la production)

### Environnement de développement
```bash
cd dnd-dm-toolbox

# Démarrer tous les services
docker-compose up -d

# Entrer dans le conteneur de développement
docker-compose exec dev bash

# À l'intérieur du conteneur
./setup_dependencies.sh
mkdir build && cd build
cmake ..
make -j$(nproc)

# Permettre X11 (sur l'hôte)
xhost +local:docker

# Lancer l'application
export DISPLAY=:0
./DnD_DM_Toolbox
```

### Production
```bash
# Build et lancer le conteneur de production
docker-compose --profile production up -d

# Permettre X11 (sur l'hôte)
xhost +local:docker
```

## Accès aux services

### Application principale
- L'application s'ouvre dans une fenêtre graphique
- Utilisez le menu "Tools" pour accéder aux différentes fonctionnalités
- Les données sont sauvegardées dans le dossier `data/`

### Base de données PostgreSQL
- **Host**: localhost
- **Port**: 5432
- **Database**: dnd_toolbox
- **Username**: dnduser
- **Password**: dndpass

### pgAdmin (Interface web pour la DB)
- **URL**: http://localhost:5050
- **Email**: admin@dnd.local
- **Password**: admin

## Utilisation de l'application

### 1. Créer un personnage
1. Cliquez sur "Tools" → "Character Creator"
2. Suivez l'assistant en 5 étapes :
   - **Étape 1** : Informations de base (nom, niveau)
   - **Étape 2** : Race et classe
   - **Étape 3** : Scores de caractéristiques (Point Buy, Standard Array, ou Roll)
   - **Étape 4** : Compétences
   - **Étape 5** : Révision et finalisation

### 2. Gérer un combat (Bientôt disponible)
- Tracker d'initiative
- Gestion des HP et conditions
- Jets d'attaque et de dégâts

### 3. Générer un donjon (Bientôt disponible)
- Génération procédurale
- Éditeur de maps
- Rencontres aléatoires

## Structure du projet

```
dnd-dm-toolbox/
├── src/                    # Code source C++
│   ├── main.cpp           # Point d'entrée de l'application
│   └── ui/                # Composants d'interface
├── include/               # Headers
│   ├── dnd/              # Modèles de données D&D
│   └── ui/               # Headers UI
├── database/             # Scripts SQL
│   └── init/            # Initialisation de la DB
├── data/                # Données utilisateur (sauvegardes)
├── resources/           # Assets (fonts, icons)
├── build/              # Build artifacts (généré)
└── libs/               # Bibliothèques externes (généré)
```

## Développement

### Recompiler après modification
```bash
cd build
make -j$(nproc)
```

### Nettoyer le build
```bash
rm -rf build
./build.sh
```

### Formater le code
```bash
find src include -name "*.cpp" -o -name "*.h" | xargs clang-format -i
```

## Résolution de problèmes

### Erreur : "Cannot open display"
Assurez-vous d'avoir autorisé X11 :
```bash
xhost +local:docker
export DISPLAY=:0
```

### Erreur : "GLFW: Failed to initialize"
Vérifiez que vos drivers OpenGL sont installés :
```bash
# Sur Arch/Manjaro
sudo pacman -S mesa

# Sur Ubuntu
sudo apt-get install libgl1-mesa-dri
```

### Les dépendances ne se téléchargent pas
Assurez-vous d'avoir git installé et une connexion internet :
```bash
git --version
./setup_dependencies.sh
```

## Prochaines étapes

Une fois l'application lancée, vous pouvez :

1. **Créer des personnages** et les sauvegarder
2. **Explorer la base de données** avec pgAdmin
3. **Contribuer au développement** en ajoutant de nouvelles fonctionnalités

Pour contribuer, consultez le README.md pour plus de détails sur l'architecture et les conventions de code.

## Support

Pour toute question ou problème :
- Consultez le README.md
- Ouvrez une issue sur GitHub
- Vérifiez les logs de l'application

Bon jeu ! 🎲
