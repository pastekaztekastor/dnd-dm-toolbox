# TO DO / Roadmap 

<!-- TOC -->

- [TO DO / Roadmap](#to-do--roadmap)
- [DATA SQL](#data-sql)
- [WORKFLOW APP](#workflow-app)
    - [RÉPONSES AUX QUESTIONS](#r%C3%A9ponses-aux-questions)
- [CRÉATION DE PERSONNAGE](#cr%C3%A9ation-de-personnage)
- [— Post refonte archi](#-post-refonte-archi)
    - [Niveaux dans le Logger](#niveaux-dans-le-logger)
        - [RoadMap](#roadmap)
    - [Pseudo Markdown](#pseudo-markdown)
    - [Ajout d'un mode édition/session {##mode-edition-session-panification}](#ajout-dun-mode-%C3%A9ditionsession-mode-edition-session-panification)
    - [Plugin Classes](#plugin-classes)
        - [RoadMap](#roadmap)
            - [Plugin](#plugin)
            - [Graphe](#graphe)
        - [Rendu de l'arbre d'évolution](#rendu-de-larbre-d%C3%A9volution)
- [Gestion des tuiles](#gestion-des-tuiles)
- [Mode scénario](#mode-sc%C3%A9nario)
- [Gestionnaire de dons](#gestionnaire-de-dons)
- [DISTINCTION TECHNIQUE](#distinction-technique)
- [CORE / MOTEUR](#core--moteur)
- [Gestionnaire d'historique](#gestionnaire-dhistorique)
- [Gestionnaire d'équipement](#gestionnaire-d%C3%A9quipement)

<!-- /TOC -->

# DATA SQL

Pour les données des personnages, voici les tables :

- RACE :
  - Augmentation de caractéristique : liens multiples vers une table de caractéristiques avec la valeur
  - Âge min : nombre
  - Âge max : nombre
  - Taille min : nombre
  - Taille max : nombre
  - Langue : référence vers la table de toutes les langues
  - Description longue : texte formaté
  - Description courte : pour l'aide contextuelle
  - Ce qu'en disent les autres races : liens multiples vers les autres races avec un champ de texte
  - Exemple de nom de femme : txt
  - Exemple de nom d'homme : txt
  - Type de la catégorie de nom : txt
  - Exemple de nom pour la catégorie spéciale : txt
  - Variante raciale : nombre de variantes raciales
  - Race parente : rien si race parente, référence de la race parente si race enfant
  - Aptitude raciale : lien vers la table de compétences
- CARACTÉRISTIQUE :
  - Nom : txt
  - Description : txt
- LANGUE :
  - Nom : txt
  - Description : txt
  - Alphabet : txt (je ne sais pas comment gérer ça... peut-être une image)
- APTITUDE RACIALE :
  - Nom de l'aptitude
  - Description de l'aptitude
  - Mod. de carac. : table de liens entre compétence et caractéristique avec la valeur de modification
  - Sorts acquis : table de liens vers la table de sorts
  - Jets de sauvegarde : table de liens vers la table de jets de sauvegarde avec booléen avantage/désavantage
  - Compétences maîtrisées : lien vers la table de compétences
  - Attaque spéciale : lien vers la table des armes

- CLASSE :
  - Nom : txt
  - Description : txt formaté
  - Dé de vie : lien vers la table de dés
  - PV niveau 1 : formule de jet de dé (en attendant : txt)
  - Caractéristique principale : liens vers la table de caractéristiques
  - Maîtrise et jets de sauvegarde : liens vers la table de caractéristiques
  - Maîtrise des armes : liens vers la table des types d'arme
  - Maîtrise des armures : liens vers la table des types d'armures
  - Maîtrise d'outils : liens vers la table des outils
  - Compétences : liens vers la table des compétences
  - Nombre de compétences à la création : nombre
  - Équipement : table de liens vers équipement
  - Équipement à la création : table de liens vers StarterPackEquipement
  - Évolution : liens vers la table évolution
  - Utilisation de l'option 2 pour les évolutions : booléen
  - Nom de l'option 1 dans les évolutions : txt
  - Nom de l'option 2 dans les évolutions : txt
  - Utilisation des sorts : booléen
- STARTERPACKEQUIPEMENT :
  - Nom du pack :
  - Liste des éléments qu'il contient :
- ÉVOLUTION :
  - Niveau : nombre
  - Bonus de maîtrise : nombre
  - Aptitude : liens vers la table aptitude
  - Option 1 : nombre
  - Option 2 : nombre
  - Emplacement de sort 1 : nombre
  - Emplacement de sort 2 : nombre
  - Emplacement de sort 3 : nombre
  - Emplacement de sort 4 : nombre
  - Emplacement de sort 5 : nombre
  - Emplacement de sort 6 : nombre
  - Emplacement de sort 7 : nombre
  - Emplacement de sort 8 : nombre
  - Emplacement de sort 9 : nombre
- APTITUDE :
  - Nom : text
  - Description : rich text

---

# WORKFLOW APP

- C'est un bureau de MJ avec des « tools » qui s'affichent. L'app en tant que telle n'est qu'un gestionnaire de tools qui journalise toutes les actions réalisées dans les tools et permet d'enregistrer tout ce qui a été fait dans les tools à un moment donné. Exemple : dans une sauvegarde il peut n'y avoir qu'une fiche perso, ou des maps, ou un story board, ou les trois.
- On peut ajouter une sauvegarde que l'on charge à une sauvegarde ouverte en cours.
- Le format de fichier de save sera un XML compressé de tous les objets instanciés, nommé `.dnd`.
- Un tool + une ouverture = une instance. Exemple : le tool « fiche perso » + fiche A = une instance objet.
- La liste des tools :
  - Fiche perso (création + évolution)
  - Dice roller
  - Note
  - Gestionnaire de campagne / story board
  - Gestionnaire de PNJ
  - Gestionnaire de rencontre / mode de combat
  - Créateur de rencontre
  - Créateur de PNJ
  - Créateur/éditeur de carte
  - Gestionnaire de maps
  - Aide contextuelle
  - Créateur de campagne
  - Gestionnaire d'items
  - Gestionnaire d'armes/armures
  - Gestionnaire de sorts
  - Créateur d'item
  - Créateur d'armes/armures
  - Créateur de sort

## RÉPONSES AUX QUESTIONS

- 1) Option B. On a les derniers fichiers ouverts (donc les grosses campagnes mais aussi les sauvegardes plus simples comme une simple fiche perso) et les boutons [Nouveau] [Ouvrir] [Importer].
- 2) Option A et B, plus un menu au clic droit dans le fond « vide » de l'app.
- 3) Option A, mais comme ImGui gère la stackabilité, c'est bon.
- 4) Option B.
- 5) Option B. Ce qui fait que si je pars d'un fichier vide (genre j'ouvre l'app), il y a une différence entre Ouvrir et Importer.
- 6) Normalement tu as tout bon. Juste : dans l'app c'est pas un tool, mais il doit y avoir une fenêtre de log avec toutes les modifs de tous les tools (genre le dice roller et son historique, mais aussi les actions de combat et les ouvertures de map ou les éditions — BREF TOUT, la moindre action sur l'UI y est renseignée).
- 7) Tu soulèves un point important : les différents outils sont utiles donc je propose un « tool » qui a plusieurs onglets : les options A, B et C, mais aussi Recherche.
- 8) Je valide la Road map mais mets en « 0 » : réalisation de l'app en tant que gestionnaire de tools avec tout ce que ça implique (communication inter-tool, log, chargement, etc.).

RÉPONSES AUX QUESTIONS 2

- 1) Je n'ai pas validé le XML, j'avais juste proposé, mais le SQLite non compressé me va aussi. Je veux un fichier non éditable depuis une app tierce, et non formaté (brut).
- 2) Pour le log, oui on le save. Oui ça va faire de gros fichiers. Mais j'accepte jusqu'à la dizaine de giga — c'est une campagne complète de DD, c'est pas un simple Word. Pour le format, fais-moi une fusion de B et C. Je veux que seules les actions qui ont un impact sur la campagne aient un log. Idéalement, en lisant les logs on doit pouvoir avoir l'équivalent de l'histoire qui nous est racontée. Donc je pense que ce seront les tools qui diront « ça c'est loggé et ça se log comme ça » — on verra dans les tools.
- 3) Je ne suis pas à l'aise avec ces concepts, j'ai toujours travaillé avec de l'accès direct, mais c'est peut-être pas le plus efficace.
- 4) Tu as tout compris. Et dans les menus on groupera par « but » — genre Gestionnaire d'items + Créateur d'items dans un espace à eux.
- 5) Pour moi la DB SQL contient les règles, l'équivalent des livres DnD. Et le fichier .dnd contient la partie avec les instances des créations. Mais par exemple si on crée une arme custom elle n'apparaît que dans le .dnd, alors que les armes de base sont dans le SQL et toujours là. Et je me rends compte que tu avais suggéré exactement ça.
- 6) Pour moi un Créateur de Campagne te permet de faire une liste todo/événements de tout ce que les personnages vivront. Mais le Gestionnaire dit où tu en es et les points à surveiller. Exemple : dans le Créateur de Campagne tu mets un boss avec un FP de 20 mais les joueurs n'ont pas le niveau, et le Gestionnaire de campagne te met un warning.
- 7) Toutes les 10 min c'est bien — on parle beaucoup dans une partie de DD. Pour les saves, prends le système d'autosave, plus intuitif.

ATTENTION : avant de faire quoi que ce soit, il faut aussi qu'on parle de l'archi du TOOL. Pour moi ils sont INDÉPENDANTS — donc avec leur propre DB et tout et tout. Dans le `src` il y a une liste de dossiers (un par tool) avec leur nom dessus, un petit fichier d'implémentation dans l'app (genre le nom de la fenêtre et sa position dans les menus), et ensuite ils sont auto-ajoutés dans l'app. Les menus de l'app sont donc dynamiques. Tu as capté ? En écrivant ça je me dis que les ressources partagées seront plus simples avec un EventBus.

RÉPONSES AUX QUESTIONS 3

- 1) Je ne suis pas contre dans l'idée de faire simple au début, mais si c'est pour me tirer une balle dans le pied pour migrer après, c'est moins chaud. Je ne sais pas la vitesse ni la quantité de tools que je vais faire et recompiler me fait chier, donc en runtime ça me semble mieux.
- 2) Ça me semble correct et on pourra l'enrichir au besoin plus tard.
- 3) Pour le manifest, mieux vaut trop que pas assez.
- 4) Pour les tools, ils seront auto-ordonnés en fonction du manifest. Donc il y aura un menu Outils et Affichage — c'est bon — mais l'Aide n'est pas un tool, c'est une feature de base.
- 5) Les events sont ajoutés par les tools. Chaque tool embarque ses events.
- 6) OK dans les grandes lignes. Juste : tu penses qu'il est plus intéressant de faire from scratch ou de repartir de l'existant ? Je pense le from scratch plus simple pour toi.

- Ajouter au pattern dans le manifest une option « singleton » qui indique si le tool a une instance unique persistante ou s'il peut en avoir plusieurs. EXEMPLE : la différence entre le tool pour gérer une fiche perso (il peut y en avoir plusieurs différentes) et le tool pour le turn order en combat, forcément unique.

---

# CRÉATION DE PERSONNAGE

Pour ce qui est du workflow, on doit avoir la cinématique suivante :

- 1. CHOIX DE LA RACE : un onglet avec des tuiles de toutes les races avec des détails donnés pour aider à choisir (short_description de la DB par exemple).
- Une fois la race choisie, détail de la race avec des boutons radio ou des boutons à cocher si on a des choix à faire. Puis des zones de texte pour entrer le nom, prénom et autre (différent en fonction des races).
- 2. CHOIX DE LA CLASSE : même fonctionnement que la race.
- Deuxième page comme la race, mais avec la classe on nous détaille tout et on nous demande de choisir directement ce qui nous intéresse dans la page. Donc aussi les choix de stuff, de langue, et tout.
- Choix de la sous-classe s'il y en a... tout comme la classe avec ces 2 étapes.
- 3. Onglet de lancer de dés pour les caractéristiques. Plus le niveau à la création.
- Choix de l'historique (tu connais la chanson).
- Options de personnalisation.
- Choix des sorts.
- Évolution (à la montée de niveau).

Je suis sur la création du plugin de perso que je reprends depuis 0 — je n'ai pas de fichier qui me crée le template. Bref, pas de bol. Il faudra que je recompile, je n'ai plus les plugins qui sont chargés. Par contre c'est bien moins moche.

Il faut vraiment que mon texte en base gère les alias et les tables.

---

# 20260616 — Post refonte archi

## Niveaux dans le Logger

Après avoir aussi refondu le fonctionnement du bus event, je pense qu'il faut aussi faire une priorité pour les logs. Dans le sens où il y a des logs de modif, certains sont nécessaires pour comprendre les actions de la campagne, d'autres non. Bref, il y a plusieurs niveaux de lecture. Pour moi il faut :

- Compréhension de la campagne : s'ils sont remontés, on doit avoir le déroulé de la campagne
- Édition de data
- Mise en place de campagne
- Tout le reste

Je pense que c'est pas vraiment bien découpé, mais c'est un découpage fonctionnel.

### RoadMap

- [ ] Faire la liste des différents niveaux/utilisations de log
- [ ] Réimplémenter la nouvelle utilisation
- [ ] Surcharge pour que l'ancienne version co-existe
- [ ] Adapter le plugin de log

## Pseudo Markdown

- [ ] Ajouter le module de formules math. Pas pressé, mais plus sympa pour les formules.

## Ajout d'un mode édition/session {##mode-edition-session-panification}

Peut-être mettre un mode édition/session pour différencier les logs des deux phases qui sont différentes, avec un retour visuel dans la barre de l'app pour toujours savoir dans quel mode on est. Les deux modes pourraient changer le comportement des plugins — par exemple, tu ne peux pas modifier les races en mode session. Je propose donc 3 mode : 
- **Edition** : Tu peux toucher à la data .univ
- **Planification** : tu peux toucher à la data .dnd
- **Session** : tout est protégé sauf ce qui est définit comme nescéssaire pour la scéance.

## Plugin Classes

- [ ] Définir le périmètre des différents modes et les fonctions
- [ ] Ajouter la feature dans le bandeau de l'app
- [ ] Déterminer comment on switche de mode
- [ ] MAJ des anciens plugins pour compatibilité (si non pris en charge, aucune incidence sur le comportement)

### RoadMap

#### Plugin

- [x] Un panneau de consultation : on choisit la classe qu'on consulte et ça affiche tout ce qui la concerne
- [x] Un panneau d'édition
- [x] On peut reprendre la structure de présentation du gestionnaire de race
- [ ] Voir avec Claude la liste des fonctions de communication — je pense qu'il y en aura bien plus que pour les races, plus dynamique.
  - [ ] `getClasseEvolveRequired` : qui retourne tous les attributs/compétences d'une classe en fonction des prérequis : niveau min, choix de build, etc.
  - [ ] Mettre au point les protocoles de communication des classes.
- [x] Erreur lors du chargement de la seed : `UNIQUE constraint failed : classes.nom`
- [x] Un bouton Modifier pour les sous-tables de classe (les autres tabs)
- [x] Créer les seeds de toutes les autres classes. Commencer par les classes Druide, Paladin, Roublard.
  - [x] Faire la seed de barde au format SQL dans `src/plugins/classe_manager/` à partir du markdown dans `_setaside/resources/books/player handbook/03_CLASSES/BARDE/`. Respecter le schéma `src/plugins/classe_manager/classe_manager_schema.sql`.
  - [x] Faire la seed de clerc au format SQL dans `src/plugins/classe_manager/` à partir du markdown dans `_setaside/resources/books/player handbook/03_CLASSES/CLERC/`. Respecter le schéma `src/plugins/classe_manager/classe_manager_schema.sql`.
  - [x] Faire la seed d'ensorceleur au format SQL dans `src/plugins/classe_manager/` à partir du markdown dans `_setaside/resources/books/player handbook/03_CLASSES/ENSORCELEUR/`. Respecter le schéma `src/plugins/classe_manager/classe_manager_schema.sql`.
  - [x] Faire la seed de guerrier au format SQL dans `src/plugins/classe_manager/` à partir du markdown dans `_setaside/resources/books/player handbook/03_CLASSES/GUERRIER/`. Respecter le schéma `src/plugins/classe_manager/classe_manager_schema.sql`.
  - [x] Faire la seed de magicien au format SQL dans `src/plugins/classe_manager/` à partir du markdown dans `_setaside/resources/books/player handbook/03_CLASSES/MAGICIEN/`. Respecter le schéma `src/plugins/classe_manager/classe_manager_schema.sql`.
  - [x] Faire la seed de moine au format SQL dans `src/plugins/classe_manager/` à partir du markdown dans `_setaside/resources/books/player handbook/03_CLASSES/MOINE/`. Respecter le schéma `src/plugins/classe_manager/classe_manager_schema.sql`.
  - [x] Faire la seed d'occultiste au format SQL dans `src/plugins/classe_manager/` à partir du markdown dans `_setaside/resources/books/player handbook/03_CLASSES/OCCULTISTE/`. Respecter le schéma `src/plugins/classe_manager/classe_manager_schema.sql`.
  - [x] Faire la seed de rôdeur au format SQL dans `src/plugins/classe_manager/` à partir du markdown dans `_setaside/resources/books/player handbook/03_CLASSES/RODEUR/`. Respecter le schéma `src/plugins/classe_manager/classe_manager_schema.sql`.

- [x] Tab Aptitude : je ne pense pas que le layout soit le plus efficace.
- [x] Tab Évolution — colonne aptitude c'est moche... il faudrait les noms, pas les IDs, et mettre des retours à la ligne.
- [x] Tab Évolution : pourquoi la ligne LVL 3 est jaune ? C'est parce qu'elle implique un choix ? Si c'est le cas, il le faut aussi au LVL 14 car le choix totémique est différent de celui du niveau 3.
- [x] Peut-être découper le code par tab pour que ce soit plus lisible ? Dans plusieurs fichiers.
- [x] Je dois pouvoir modifier TOUTES les aptitudes, même celles des sous-classes.
- [x] Le bouton Modifier tout en haut ne devrait pas y être — il se contente de modifier la classe, donc on inclut les modifs/suppression dans le tab Classe comme pour les autres classes.
- [x] Supprimer une classe entière se fait depuis le panneau latéral gauche.
- [x] Erreur lors du chargement de la seed druide : `table classe_evolutions has no column named sort_slot_1` — le schéma SQL n'a pas été respecté strictement.
- [x] À côté de Barbare il y a écrit [barbare] — je pense que c'est l'alias. Si ce n'est pas le cas, il faut que ça soit l'alias sous la forme `[@classe.barbare]`. Plus utile pour l'utilisateur.
- [x] L'affichage des aptitudes utilise-t-il le pseudo markdown du moteur ? Sinon il faut l'ajouter car ce sera obligatoire pour les nombreux tableaux de sorts des autres classes.
- [x] Même si ça ne s'affiche pas dans le tab Classe, il faut un récap des choix de build et de leur niveau ainsi que l'aptitude à l'origine, et je dois pouvoir paramétrer les différents choix/réponses dans ce panneau d'édition. L'édition des aptitudes aura donc dans les prérequis des combos en fonction de ce qui est paramétré dans ce panneau. Je pense que cette modification peut justifier une refonte de la DB : ajouter une table `classe_choix_champ` et `classe_choix_valeur`, avec des tables de liens entre les différentes tables, y compris avec celle des aptitudes qui aurait donc comme champs supplémentaires : `choix_champ_id` si c'est une aptitude où l'on doit faire un choix, et `choix_valeur_id` si c'est une aptitude qui dépend d'un choix. Ça remplacerait les prérequis en JSON et permettrait une meilleure édition avec des combos plus faciles à mettre en place. Par contre ça implique pas mal de fixes dans le plugin.
- [X] Tous les boutons Supprimer doivent avoir une popup de confirmation — on va migrer la popup dans `src/core/utils.cpp` et l'implémenter partout où on a un bouton Supprimer. Titre : `[Confirmation de suppression]`, le contenu de la popup me va.
- [ ] On est en train de refaire la structure de la database, on passe donc la la V 0.2 du plugin et de la DB: 
  - [X] Il faut faire un test de la classe druide. On prend comme origine de la data `_setaside/resources/texte/player handbook/classes/classes_druide_DD5e.md` et comme schéma de formatage `src/plugins/classe_manager/classe_manager_schema.sql`.
  - [ ] on vas réorganisé un peu l'interface qui est bordélique.
    - [ ] on sépare les `.h` des `.cpp` dans 2 dossiers 
    - [ ] on fait un mode sur l'archi moteur comme le le pensait [ici](##mode-edition-session-panification)
    - [ ] on fait les différent table qui change de comportement en fonction du mode. En Edition touts les champs bascule en éditable avec les bouton suppr ou add sinon simple consultation. 
    - [ ] les tabs et leur contenu : 
      - [ ] **Classe** : On doit consulté et modifier les table SQL : classes, classe_evolution_colonnes, classe_presentations, classe_choix_champ, classe_choix_valeur
      - [ ] **Evolution** : On doit consulté et modifier les table SQL : classe_evolution_valeurs, classe_evolutions
      - [ ] **Aptitude** : On doit consulté et modifier les table SQL : classe_aptitudes
      - [ ] **Equipement** On doit consulté et modifier les table SQL : classe_choix_equipement
      - [ ] **Arbre** : on verra plus tard.

#### Graphe

MISE EN PAUSE : je ferai un truc plus propre plus tard avec le système de graphe et le moteur dédié.

- [x] J'aimerais avoir un rendu de type Arbre d'évolution — je pense que ma data permet de le générer.
- [x] Différence d'affichage entre les 3 premiers niveaux (nom du niveau sur le côté) et les niveaux suivants (nom du niveau en haut de la tuile). Je préfère l'affichage des niveaux suivants, mais en mettant un encart de titre comme un UML.
- [x] Pourquoi le choix du totem spirituel s'affiche en liste et pas comme le choix de la voie primitive ?
- [x] J'aimerais des informations au survol et au clic. Je pense que la description au survol et tout au clic c'est bien (tout s'affiche dans la tuile au clic).
- [ ] Pour les tuiles du tab Arbre : taille fixe en hauteur et en largeur. Alignement par le centre des tuiles d'une même branche. Liens en courbes de Bézier.

### Rendu de l'arbre d'évolution

Je veux un rendu comme ça :

```
             +-----------------------------+
LVL 1        | liste des aptitudes de lvl 1 |
             +-------------+---------------+
                           |
             +-------------+---------------+
LVL 2        | liste des aptitudes de lvl 2 |
             +-------------+---------------+
                           |
             +-------------+---------------+
LVL 3        | liste des aptitudes de lvl 3 |
             | Choix                        |
             +-------------+----------------+
                           |
                  +--------+-------+
                  |                |
            +-----+------+  +------+-----+
            |  A         |  |  B         |
            +-----+------+  +------+-----+
 ...
```

---

# Gestion des tuiles

Je pense que pour les affichages par tuile en flow on va faire un moteur de rendu. Plus permissif même si plus lourd à faire. Il sera massivement utilisé dans la partie scénario ultérieurement. Inspiration principale : le no-code dans Unreal Engine. Donc chaque tuile a :

- Un titre
- Une liste d'entrées
- Une liste de sorties
- Des composantes internes

Il existe plusieurs types de tuiles qu'on définira plus tard ensemble. Là on se concentre sur le render tuile qui est générique.

Quelques spécifications d'affichage :

- Les tuiles sont dans un espace infini grillagé.
- Les tuiles peuvent être repositionnées dans l'espace.
- Elles ne peuvent pas se chevaucher — donc leur taille est fixe.
- Elles ont des caractéristiques affichées de base mais peuvent être développées dans un menu contextuel (exemple : tuile de niveau avec le nom de l'aptitude apprise → remplit la fenêtre contextuelle de choix de la tuile).
- Les liens entre les tuiles sont des courbes de Bézier.
- Les entrées sont en haut, les sorties en bas.

Pour le moment pas d'édition possible — c'est uniquement de l'affichage. L'édition se fait dans la fenêtre contextuelle.

D'un point de vue dev, une classe abstraite qui définit le rendu. Les classes enfant sont propres au plugin. Donc dans le plugin Classe il y a une classe `tuile_classes` qui spécialise la classe tuile.

Je pense que regénérer les arbres dynamiquement à chaque ouverture c'est pas ouf — on fait un espace DB qui enregistre les choses et permet de les retrouver comme elles étaient plus tard. Juste un fact check à l'ouverture s'il y a des MAJ de data. De plus, les arbres, qui sont des ensembles de tuiles, sont sauvés dans le `.dnd`.

Donc dans la logique il y aura :

- La classe Graphe qui gère l'affichage
- Elle comprend des classes abstraites Tuile qui sont spécialisées dans les plugins

J'ai pensé à :

- CLASSE GRAPHE
  - Attributs
    - Liste des tuiles
    - Valeur de zoom
  - Méthodes
    - Auto-replace
- CLASSE ABSTRAITE TUILE
  - Attributs
    - Nom
    - Couleur
    - Dim x, y
    - Position x, y
    - Liste d'entrées
    - Liste de sorties [ptr sur tuiles]
    - Label des sorties
    - Bézier des sorties (peut être automatique)
    - Liste de contenu [table de pseudo md]
  - Méthodes
    - Render tuile
    - Render détail
    - Render édition
- CLASSE TUILE_APTITUDE
  - Gère mieux l'édition d'une aptitude de classe
  - Surcharge des méthodes
- CLASSE TUILE_RENCONTRE
  - Intègre le résumé d'une rencontre gérée par le générateur de rencontre/gestionnaire de rencontre (nécessite le générateur de rencontre, turn order, gestionnaire de rencontre)
  - Surcharge des méthodes
- CLASSE TUILE_HISTOIRE
  - Booléen pour savoir si la branche est visitée
  - Surcharge des méthodes
- CLASSE TUILE_LIEUX
  - Intègre une minimap d'un lieu (nécessite la map)
  - Surcharge des méthodes

---

# Mode scénario

Le mode scénario est l'outil de création de campagne — il doit donc gérer :

- Le lore de l'univers
- Le lore des PJ
  - PJ attachés
  - Liste d'événements (date, lieu, description)
- Le lore des PNJ
  - PNJ attachés
  - Liste d'événements (date, lieu, description)
- Une frise chronologique — c'est simplement une vue (peut-être faire un moteur de rendu pour ça aussi, mais pas core, c'est spécifique à lui)
- La gestion de la campagne
  - Les actes
    - Le but de l'acte
    - Description
    - Liste des lieux attachés (nécessite map) — récursif
    - Liste des PNJ attachés (nécessite gestionnaire de PNJ) — récursif
    - Liste des rencontres attachées (nécessite gestionnaire de rencontre, générateur de rencontre) — récursif
    - Les scènes
      - Liste des lieux attachés (nécessite map) — récursif
      - Liste des PNJ attachés (nécessite gestionnaire de PNJ) — récursif
      - Liste des rencontres attachées (nécessite gestionnaire de rencontre, générateur de rencontre) — récursif
      - Tableau d'XP par accomplissement
      - Un GRAPHE avec le détail de la scène

Ici pas de SQL à écrire, tout est géré dans le `.dnd` (qui en est techniquement un, mais je le différencie des SQL qui ne reprennent que les livres).

---

# Gestionnaire de dons

Ne pas oublier.

---

# DISTINCTION TECHNIQUE

Deux types de fichiers chargeables :

- `.univ` : seeds de data scrapées des livres. On doit pouvoir les charger depuis un SQL ou un CSV, et en `.univ` qui est une sorte de SQL brut pour gagner de la place. On exporte dans tous les formats de lecture. Un fichier `.univ` peut comporter plusieurs tables de différents plugins. Il faut donc que le format précise à quel plugin et quelle version appartient la table pour la charger.
- `.dnd` : données du jeu (aventure, fiche de perso, rencontre, map, histoire...) — uniquement chargeable et exportable en `.dnd`, qui est une sorte de SQL brut pour gagner de la place. On ne sait pas à l'avance ce que va contenir ce type de fichier — peut-être une simple fiche de perso ou toute une campagne. Il faut que le format de fichier indique les plugins et leurs versions nécessaires à son chargement. S'ils ne sont pas tous présents, il propose de ne charger que ce qu'il peut.

---

# CORE / MOTEUR

Est-ce qu'il serait utile de mettre dans le manifest quelque chose pour l'auto-complétion ? Peut-être même un fichier généré à la volée à l'ouverture de la DB pour le rendre plus fluide. Un JSON ferait l'affaire. Exemple :

```json
auto_completion [
  race [
    elfe, nain, ...
  ],
  equipement [
    arme [
      hache, épée, ...
    ],
    armure [
      maille, cuir
    ]
  ]
]
```

---

# Gestionnaire d'historique

Je n'ai pas encore travaillé sur le gestionnaire d'historique. Basiquement je pense qu'un historique est de la forme :

Avant de me lancer là-dedans, il va falloir que je passe en MD puis en SQL les scans.

```sql
TABLE historique (
  id TEXT,
  nom TEXT,
  alias TEXT,
  competences_maitrisees TEXT,
  outils_maitrises TEXT,
  langues_maitrisees TEXT,
  equipement_maitrise TEXT
)
TABLE historique_personnalite (
  id TEXT,
  rang INTEGER,
  titre TEXT,
  description TEXT,
  historique_id TEXT
)
TABLE historique_ideal (
  id TEXT,
  rang INTEGER,
  titre TEXT,
  description TEXT,
  historique_id TEXT
)
TABLE historique_obligation (
  id TEXT,
  rang INTEGER,
  titre TEXT,
  description TEXT,
  historique_id TEXT
)
TABLE historique_faille (
  id TEXT,
  rang INTEGER,
  titre TEXT,
  description TEXT,
  historique_id TEXT
)
TABLE historique_description (
  id TEXT,
  titre TEXT,
  est_un_choix BOOLEAN, -- s'il y a un choix à faire dans ce paragraphe
  nom_colonne_choix_1 TEXT,
  nom_colonne_choix_2 TEXT,
  nom_colonne_choix_3 TEXT
)
TABLE historique_description_choix (
  id TEXT,
  historique_id TEXT,
  valeur_1 TEXT,
  valeur_2 TEXT,
  valeur_3 TEXT
)
```

Il faudra probablement donner ça à Claude pour qu'il me dise ce qu'il en pense et s'il pense qu'il manque des trucs.

Normalement je n'ai rien oublié. La présentation se fera comme les races (relativement simple, tout comme les races). L'édition sera comme les races aussi.

---

# Gestionnaire d'équipement

Dans ma gestion de la fiche de perso, je considère que tout ce qui est dans l'inventaire est un équipement. Ainsi les armes, armures, outils, objets, etc. sont des équipements. La table équipement est donc lourde car en fonction du type, elle intègre tout un tas de champs pas toujours nécessaires.

Cela dit, j'ai déjà fait un MD de ces pages donc ce sera plus rapide.
