# D&D DM Toolbox - Roadmap

Cette roadmap décrit les fonctionnalités planifiées et leur ordre de priorité.

## Phase 1 : MVP ✅ (COMPLÉTÉ)

### Infrastructure
- [x] Setup du projet C++ avec CMake
- [x] Intégration de Dear ImGui avec docking
- [x] Configuration Docker pour dev et production
- [x] Docker Compose avec PostgreSQL
- [x] GitHub Actions CI/CD
- [x] Documentation (README, QUICKSTART)

### Système de base de données
- [x] Schéma PostgreSQL complet
- [x] Tables pour personnages, campagnes, combat
- [x] Données de démo (Lost Mines of Phandelver)
- [x] Sorts de base (Magic Missile, Fireball, etc.)

### Modèles de données D&D 5e
- [x] Ability scores et modificateurs
- [x] Skills et proficiencies
- [x] Races et classes
- [x] Système de dés (rolls, advantage, disadvantage)
- [x] Structure de personnage complète

### Interface utilisateur
- [x] Application principale avec docking
- [x] Menu et navigation
- [x] Fenêtre de bienvenue
- [x] Character Creator (wizard en 5 étapes)
- [x] Point Buy, Standard Array, et Rolling pour les abilities

## Phase 2 : Character Management (EN COURS)

### Character Sheet Viewer
- [ ] Affichage complet de la feuille de personnage
- [ ] Modification en temps réel des HP
- [ ] Gestion des HP temporaires
- [ ] Interface pour les jets de sauvegarde
- [ ] Interface pour les jets de compétences
- [ ] Calculateur de dégâts rapide

### Inventory System
- [ ] Interface pour gérer l'inventaire
- [ ] Ajout/suppression d'items
- [ ] Équipement d'armes et armures
- [ ] Calcul automatique de l'AC basé sur l'équipement
- [ ] Poids total et encombrement
- [ ] Gestion de l'or et des richesses

### Spell Management
- [ ] Base de données complète des sorts D&D 5e
- [ ] Interface de sélection de sorts
- [ ] Préparation de sorts
- [ ] Tracking des emplacements de sorts
- [ ] Description détaillée des sorts
- [ ] Filtres par niveau, école, classe

### Character Persistence
- [ ] Sauvegarde dans PostgreSQL
- [ ] Chargement de personnages existants
- [ ] Export en JSON
- [ ] Import depuis JSON
- [ ] Backup automatique

## Phase 3 : Combat Tracker

### Initiative & Turn Order
- [ ] Interface de combat avec timeline
- [ ] Roll d'initiative automatique
- [ ] Tri par ordre d'initiative
- [ ] Gestion des tours (Next Turn, End Round)
- [ ] Support pour la surprise
- [ ] Délai/retard d'initiative

### Combat Management
- [ ] Ajout de PJ et PNJ au combat
- [ ] Tracking HP en temps réel
- [ ] Application de dégâts/soins
- [ ] HP temporaires
- [ ] Death saving throws
- [ ] Interface pour les jets d'attaque

### Conditions & Effects
- [ ] Application de conditions (poisoned, paralyzed, etc.)
- [ ] Durée des conditions (rounds, minutes)
- [ ] Rappels automatiques
- [ ] Icônes visuelles des conditions
- [ ] Effets des conditions sur les stats

### Monster Database
- [ ] Import du Monster Manual (SRD)
- [ ] Recherche et filtrage de monstres
- [ ] Ajout rapide au combat
- [ ] Génération de stat blocks
- [ ] Templates de monstres personnalisés

## Phase 4 : Dungeon Generator

### Procedural Generation
- [ ] Algorithme BSP pour la génération
- [ ] Génération de salles et couloirs
- [ ] Paramètres personnalisables (taille, densité)
- [ ] Seed pour reproductibilité
- [ ] Différents styles de donjons

### Map Editor
- [ ] Grille customisable (5ft, 10ft)
- [ ] Outils de dessin (mur, porte, fenêtre)
- [ ] Placement de tokens
- [ ] Ajout de notes et markers
- [ ] Sauvegarde/chargement de maps

### Encounter Placement
- [ ] Tables de rencontres aléatoires
- [ ] Budget XP pour équilibrage
- [ ] Placement automatique de monstres
- [ ] Zones de piéges
- [ ] Trésors et récompenses

### Visualization
- [ ] Rendu 2D de la map
- [ ] Zoom et pan
- [ ] Fog of War
- [ ] Export en image (PNG)
- [ ] Mode joueur vs mode DM

## Phase 5 : Campaign Manager

### Campaign Creation
- [ ] Création de campagnes
- [ ] Associer des personnages à une campagne
- [ ] Description et notes
- [ ] Timeline des événements
- [ ] Gestion des sessions

### Quest System
- [ ] Création de quêtes
- [ ] États (active, completed, failed)
- [ ] Récompenses
- [ ] Dépendances entre quêtes
- [ ] Tracking de progression

### NPC Management
- [ ] Base de données de NPCs
- [ ] Relations entre NPCs et PJs
- [ ] Notes et backstory
- [ ] Localisation
- [ ] Génération aléatoire de NPCs

### World Building
- [ ] Création de lieux
- [ ] Hiérarchie (continent > région > ville)
- [ ] Descriptions et lore
- [ ] Connexions entre lieux
- [ ] Carte du monde (basique)

### Session Notes
- [ ] Prise de notes par session
- [ ] Historique des sessions
- [ ] Export en Markdown
- [ ] Recherche dans les notes
- [ ] Recap automatique

## Phase 6 : Advanced Features

### Homebrew Content
- [ ] Création de races personnalisées
- [ ] Création de classes personnalisées
- [ ] Items magiques personnalisés
- [ ] Sorts personnalisés
- [ ] Monsters personnalisés
- [ ] Import/Export de homebrew

### Automation
- [ ] Calculs automatiques de dégâts
- [ ] Application automatique d'effets de sorts
- [ ] Rappels pour concentration
- [ ] Initiative automatique
- [ ] Level up wizard

### Advanced Dice Rolling
- [ ] Dice roller visuel
- [ ] Historique des rolls
- [ ] Macros de dés
- [ ] Statistiques de rolls
- [ ] Support pour formules complexes

### Character Builder Enhancements
- [ ] Multiclassing
- [ ] Feats
- [ ] Backgrounds détaillés
- [ ] Subclasses (archetypes)
- [ ] Leveling automatique
- [ ] Character templates

## Phase 7 : Multiplayer & Integration

### Network Play
- [ ] Serveur pour sessions multijoueur
- [ ] Synchronisation en temps réel
- [ ] Chat intégré
- [ ] Partage de maps
- [ ] Contrôle d'accès (DM vs Players)

### Virtual Tabletop Integration
- [ ] Support pour Roll20 (import/export)
- [ ] Support pour Foundry VTT
- [ ] Support pour D&D Beyond
- [ ] API REST pour intégrations tierces

### Mobile Support
- [ ] Version responsive
- [ ] App mobile (React Native?)
- [ ] Companion app pour joueurs

## Phase 8 : Polish & Performance

### UI/UX Improvements
- [ ] Thèmes personnalisables
- [ ] Dark/Light mode
- [ ] Raccourcis clavier
- [ ] Drag & drop amélioré
- [ ] Animations et transitions

### Performance
- [ ] Optimisation du rendu ImGui
- [ ] Caching de la base de données
- [ ] Lazy loading
- [ ] Compression des sauvegardes

### Testing
- [ ] Unit tests pour les modèles
- [ ] Integration tests
- [ ] UI tests
- [ ] Performance benchmarks
- [ ] Coverage > 80%

### Documentation
- [ ] Documentation complète de l'API
- [ ] Guide du développeur
- [ ] Tutoriels vidéo
- [ ] Wiki utilisateur
- [ ] Documentation des algorithmes

## Contribuer

Vous souhaitez contribuer ? Voici comment :

1. Choisissez une feature de la roadmap
2. Ouvrez une issue pour discuter de l'implémentation
3. Fork le projet
4. Implémentez la fonctionnalité
5. Soumettez une Pull Request

Les contributions sont les bienvenues à tous les niveaux !

## Notes

- Cette roadmap est flexible et peut changer selon les besoins
- Les priorités peuvent être ajustées en fonction des retours
- Certaines features peuvent être combinées ou divisées
- Les dates de livraison seront ajoutées une fois que le développement sera plus avancé

---

Dernière mise à jour : 2026-01-25
