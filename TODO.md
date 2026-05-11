# CRÉATEUR DE PERSONNAGE

## ✅ Améliorations de l'interface (Complété)
- [x] **Menu déroulant pour la race** avec bonus de stats affichés
  - Format: "Humain (+1 à toutes)", "Elfe (+2 DEX)", etc.
  - Aide contextuelle au survol de chaque race
  - Affichage de la vitesse en dessous du menu

- [x] **Menu déroulant pour la classe** avec informations utiles
  - Format: "Guerrier (DV: d10, Tank/DPS)", etc.
  - Aide contextuelle au survol de chaque classe
  - Affichage du dé de vie et du nombre de compétences à choisir

- [x] **Menu déroulant pour l'alignement** (cohérence UI)

- [x] **Données en base de données**
  - 9 races avec bonus de stats dans `database/init/02_seed_races.sql`
  - 12 classes avec dé de vie dans `database/init/03_seed_classes.sql`
  - Traits raciaux disponibles
  - Compétences par classe disponibles

## 📝 Prochaines améliorations possibles
- [ ] Séparer l'étape "Race & Classe" en deux étapes distinctes (Race d'abord, puis Classe)
- [ ] Connecter à la base de données PostgreSQL pour charger dynamiquement les races/classes
- [ ] Afficher les traits raciaux complets dans l'aide contextuelle
- [ ] Afficher les capacités de classe de niveau 1
- [ ] Système de sauvegarde des personnages créés
