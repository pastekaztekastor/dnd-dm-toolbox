# DATA SQL

pour ls Data des personnage voila les table
 - RACE :
    - Augmentation de caractéristique : Liens multible vers une table de caractéristique avec la valeur
    - Age min : nomre
    - Age max : nombre
    - Taille min : nombre
    - Taille max : Nombre
    - Langue : Ref vers la table de toutes les langue
    - Descrpition longue : Texte Formaté
    - Descrpition courte : c'est pour l'aide
    - Ce qu'en disent les autre race : Liens multiple vers les autre race avec un champs de texte
    - Exemple de nom de femme : Txt
    - Exemple de nom d'homme :txt
    - type de la caté de nom :txt
    - Exemple de nom pour la caté spé :txt
    - Variante raciale : nombre de var racial 
    - Race parent : rien si rce parente, ref de la race parent si race enfante.
    - Abilité racial : lien vers table de compétence.
  - CARACT2RISTIQUE.
    - Nom : txt
    - Description : txt
  - LANGUE :
    - Nom : txt
    - Description : txt
    - Alphabet : txt (Je sais pas comment faire ça ... pete img)
  - ABILITÉ RACIAL :
    - Nom de l'abilité
    - Descpription de l'abilité 
    - Mod de carac : Table de lien entre Compétence et caractéristique avec la valeur de modification
    - Sorte aquit : Table de lien vers la table de sort
    - Jet de sauvegarde : Table de lien vers la table JEt de sauvegarde avec Booléen avantage/desavantage
    - Compétence maitrisé : Lien vers la table de compétence.
    - Attaque spéciale : Lien vers la table des armes

  - CLASSE
    - nom : Ttx
    - Desprition : txt formaté
    - Dé de vie : lien vers table de dé
    - PV niveau 1 : fomule de jet de dé (en attendent txt)
    - Caratéristique principale : Liens vers table de caractéristique 
    - Maitrise et jet de sauvegarde Liens vers table de caractéristique 
    - Maitrise des armes : liens vers table type d'arme 
    - Maitrise des armures : Liens vers type d'armures
    - MAitrise d'outils : Liens vers la table des outils
    - Compétence : Liens vers la table des compétence
    - Nombre de compétence a la création: nombre
    - Equipement : Table de liens vers equipement
    - Equipement à la création : Table de liens vers StarterPackEquipement 
    - Evolution : Liens vers la table évolution
    - Utilisation de l'option 2 pour les evolution : booleen
    - Nom de l'option 1 dans les écolution : txt
    - Nom de l'option 2 dans les evolution : txt
    - Utilisation des sort : booleen
  - STARTERPACKEQUIPEMENT
    - nom du pack : 
    - liste des élément quil contient : 
  - ÉVOLUTION
    - Niveau : nombre
    - Bonuse de maitrise : nmobre
    - Aptitude : liens vers table aptitude
    - option 1: Nombre
    - Option 2: Nombre
    - Emplacement de sort 1 : nombre
    - Emplacement de sort 2 : nombre
    - Emplacement de sort 3 : nombre
    - Emplacement de sort 4 : nombre
    - Emplacement de sort 5 : nombre
    - Emplacement de sort 6 : nombre
    - Emplacement de sort 7 : nombre
    - Emplacement de sort 8 : nombre
    - Emplacement de sort 9 : nombre
  - APTITUDE
    - Nom : text
    - Descprition : richtexe

# WORKFLOW APP

- C'est un bureau de MJ avec des "tool" qui s'affiche. L'app en temps que telle n'est qu'un gestionnaire de tools qui journalise toutes les actions réalisé dans les tools et permet d'enregistrer tout ce qui à été fait dans ules tools à un moment. Exemple : Dans une sauvegarde il peux n'y avoir q'une fiche perso. Ou des maps, ou un story board, ou les 3.
- On peux ajouter une sauvegard que l'on charge à une sauvegarde ouvert en cours.
- Le format de fichier de save sera un XML compresser de tout les objet instancier nommer .dnd.
- Un tools + une ouverture = une instance. Exemple : le Tool "fiche perso" + fiche A = une instance objet.
- la liste des tool :
  - Fiche perso (création + evolution)
  - Dice roller
  - Note
  - gestionnaire de campagne Story board
  - GEstionnaire de PNJ
  - GEstionnaire de rencontre / mode de combat 
  - Créateur de rencontre
  - créateur de PNJ
  - créateur/editeur de Carte
  - gestionnaire de maps
  - Aide contextuelle
  - Créateur de campagne
  - Gestionnaire d'item
  - Gestionnaire d'arme/armure
  - Gestionnaire de sort
  - créateur d'item
  - créateur d'arme/armures
  - créateur de sorte
  
## RÉPONSE AU QUESTIONS
- 1) Option B. ON à les derniers fichier ouvert (donc les grosse campagne mais aussi les sauvegarde plus simple comme une simple fiche perso) et les boutons [Nouveau] [Ouvrir] [Importé]
- 2) Option A et B plus menu au clique droit dans le fond "vide" de l'app.
- 3) Option A mais comme IMGUI gère la stackabilité des c'est bon.
- 4) Option B
- 5) Option B. Ce qui fait que si je part d'un fichier vide (genre j'ouvre l'app) il y a une diff entre ouvrir et importé.
- 6) Normalement tu a tout bon. Juste dans l'app c'est pas un tools mais il dois y avoire un fenêtre de log avec toute les modif de tout les tool. (genre le dice roller et son historique mais aussi les action de combas et les ouverture de map ou edition BREF TOUT, la moindre avtion sur l'ui y est rensaigné )
- 7) Tu souligne un point important les différent outils sont utils donc Je propose un "tools" qui à plusieur onglet : LEs option A,B et C mais aussi Recherche
- 8) Je valide la Road map mais met en "0" Realisation de l'app en temps que gestionnaire de tools avec tout ce que ça implique (communicaiton intertool, log, chargement, etc)

RÉPONSE AU QUESTION 2
- 1) J'ai pas validé le XML j'avais juste propsé mais le SQLlite comprésé me vas aussi. Je veux un fichier non éditable depuis une app tièrec. et pas compréssé de je dire en non formaté.
- 2) Pour le Log oui on le save. Oui ça vas faire de gros fichier. Mais j'accepte jusqua la Dizaine de giga. C'est une campagne complète DD c'est pas un simple word. Pour le format tu me fait une fusion de B et C. Je veux que seul les action qui ont un impacte sur la campagne ait un log. Idéalement en lisant les log on doit pouvoir avoir l'equivalent de l'hisoitre qui nous est raconté. Donc je pesne que se sront les tools qui diront "ca c'est logé et ça se log comme ça" donc on verra dans les tools.
- 3) je ne suis pas à l'aise avec ces concepte j'ai toujours travaillé avec de l'acces direct mais c'est ptet pas le plus efficace.
- 4) T'as tout compris. Et dans les menu on groupera par "but" genre Gestionnaire d'item + Créateur d'item dans un espace à eux.
- 5) Pour moi la DB SQL contient les règle, l'équivalent des livre DnD. Et le Fichier .dnd contient la partie avec les instance des création. Mais par exemple si on cré une arme custom elle n'apparait que dans le .dnd alors que les armes de base sont dans le SQL et toujour la. Et je me rend compte que tu avait sugéré exactement ça.
- 6) Pour moi un Créateur de Campagne te permet de faire une "todo list/event" de tout ce que les personngae viveront. mais le gestionnaire de dis ou tu en est et les point à faire attention. Exemple : Dans le créateur de campagne tu met un boss avec un FP de 20 mais les joueur n'ont pas le niveau et il y arive le gestionnaire de campagne te met un warning
- 7) Tout les 10 min c'est bien. On parle beaucoup dans un DD. Pour les save prend le système d'autosave. plus intuitif.

ATTENTION avant de faire quoi que ce soit il faut aussi que l'on parle de l'archi de TOOL. Pour moi il sont IND2PENDENT donc avec leur propre DB et tout et tout. Limite dans le SRC il y a une liste de dossier (un par tool) avec leur nom dessus, un petit fichier d'implemmentation dans l'app genre le nom de la fenetre et dans les menu et enssuite il sont auto ajouté dans l'app. Genre les menu de l'app sont dynamique quoi. t'as capté ? Du coup en ecrivatn ça je me dis que les ressoucre partage seront plus simple avec un EventBus. 

RÉPONSE AU QUESTIONS 3
- 1) Je suis pas contre dans lidée de faire simple au début mais si c'est pour me tiré une ball dans le pied pour migré apres moins chaud. je ne sais pas la vitesse qui la qqt de tool que je vais faire et recompiler me fait chier donc en runtime cç me semble mieux..
- 2) ça me semble correct et on pourra l'enrichire au besoin plus tard
- 3) Pour le manifest mieux vaut trop que pas asser.
- 4) Pour les tools il seront auto ordoné en fontion du manifest. Dons il y aura un menu outils et affichage c'est bon mais L'aide n'est pas un tool mais une feature de base.
- 5) Les Events sont ajouté par les tools. Chaque tools embarque ces event.
- 6) ok dans les grande ligne juste tu pense qu'il est plus intéréssant de faire from scratch ou de repartire de l'existant ? Je pense le from scratch plus simple pour toi.

- Ajouter au paterne dans le manifest un option "singleton" qui dit si le tool à une instance unique persistante ou s'il peut avoir plusieur parterne EXEMPLE : la diff entre le tool pour géré une ficher perso (il peut y en avoir plusieur différent) et le tool pour le turn order en combat forcément unique. 

# CRÉATION DE personnage
Pour ce qui est du Workflow on dois avoir la cinématique suivante : 
- 1 CHOIX DE LA RACE : Un onglet avec des tuile de toutes les race Avec des détail qui sont donné pour aidé à choisir (shrt_descritpion de la DB par exemple)
- Une foix la race choisi détail de la race avec de bouton radio ou des bouton à coché si on à des choix a faire. puis des zone de texte pour entré le nom, prénom et autre (diférent en fonciton des race.)
- 2 CHOIX DE LA CLASSE : Même foncitonnement que la race
- Deuxième page comme la race mais avec la classe on nous détaille tout et on nous demande de choisire directement ce qui nous intéresse dans la page. Donc aussi les choix de stuf de langue, et tout et tout. 
- Choix de la sous classe s'il y en a ... tout comme la classe avec ces 2 étape.
- 3 Onglet de lancé de dées pour les caractéristiques. Plus niveau à la création 
- Choix de l'histoire (tu connais la chanson)
- Option de personnalisation
- Choix des sorts
- Evolution (à la monté de niveau)

Je suis sur la création du plugin de perso que je reprden depuis 0 j'ai pas de fichier qui me créer le template. Bref zebi. bon il faudra que je recompile j'ai plus les plugin qui sont chargé. par contre c'est bien moins moche. 

il faut vraiement que mon text en base gère les aliase et les table

# 20260616 post refont archi

## Niveau dans le Logger

Après avoir aussi refont le fonctionnement du bus event je pense qu'il faut aussi faire une prio pour les log. Dans le sence ou il y a des log de modif, certain sont nescéssaire pour conprendre les action de la campagne d'autre non. Bref il y a plusieur niveaux de lecture. Pour moi il faut 

- Comprehension de la campagne, s'ils sont remonter ont doit avoir le déroulé de la campagne 
- Edite de data
- Mise en place de campagne 
- Tout le reste 

Je pense que c'est pas vraiment pas bien découpé mais c'est un découpage fonctionnel. 

### RoadMap

- [ ] Faire la liste des différent niveau/utilisation de log
- [ ] Réimplémenté la nouvelle utilisation
- [ ] Surcharge pour quel l'ancienne version co-exsiste
- [ ] Adapté le plugin de log

## Pseudo Markdown

- [ ] Ajouter le mod formul de math. pas pressnt mais plus simpa pour les formule. 

## Ajout d'un mode edition/session

Peu être mêttre un mode edition / session pour différencier les logs des deux phase qui sont défférent avec un retour visuel dans la barre de l'app pour toujours savoir dans quelle mode on est. Maybe les deux mode peuvent changer le comportement des plugin genre tu peux pas modifié les race en mode session. 

## Plugin Classes

- [ ] Définire le périmètre des différents mode et les fonction
- [ ] ajouté la features dans le bandeau de l'app
- [ ] déterminé comment on switche de mode
- [ ] Maj des ancien plugin pour compatibilité avec (si non pris en charge aucune incidence sur le comprtement)

### RoadMap

#### Plugin
- [x] Un pannel de consultation, on choisi la classe qu'on consulte et ça affiche tout ce qu'il la concerne 
- [x] un pannel édition 
- [x] on peut reprendre la tructure de présentation du gestionnaire de race. 
- [ ] Voire avec Claude la liste des fonctionne de communication, je pense qu'il y en aura bien plus que pour les races. plus dynamique. 
  - [ ] getClasseEvolveRequired : qui retourne toute  les attribut / compétence d'une classe en fonction des prérqui : niveau min, choix de build, etc. 
  - [ ] mettre au point les propocole de communication des classes. 
- [X] Il y a une erreur quand je charge la seed : Erreur : UNIQUE consteaint failes : classes.nom
- [x] un bouton modifier pour les sous table de classe (les autre tab)
- [x] créer les seeds de toutes les autre classes. Commencer par les classes Druide, Paladin Roublard
  - [X] faire la seed de barde au format sql dans `src/plugin/classe_manager` à partir du markdown dans  : `_setaside/ressources/book/player handbook/03_CLASSES/BARDE/`. Veille à respecter le schéma de data `src/plugins/classe_manager/classe_manager_schema.sql`
  - [x] faire la seed de clerc au format sql dans `src/plugin/classe_manager` à partir du markdown dans  : `_setaside/ressources/book/player handbook/03_CLASSES/CLERC/`. Veille à respecter le schéma de data `src/plugins/classe_manager/classe_manager_schema.sql`
  - [x] faire la seed de ensorceleur au format sql dans `src/plugin/classe_manager` à partir du markdown dans  : `_setaside/ressources/book/player handbook/03_CLASSES/ENSORCELEUR/`. Veille à respecter le schéma de data `src/plugins/classe_manager/classe_manager_schema.sql`
  - [x] faire la seed de guerrier au format sql dans `src/plugin/classe_manager` à partir du markdown dans  : `_setaside/ressources/book/player handbook/03_CLASSES/GUERRIER/`. Veille à respecter le schéma de data `src/plugins/classe_manager/classe_manager_schema.sql`
  - [x] faire la seed de magicien au format sql dans `src/plugin/classe_manager` à partir du markdown dans  : `_setaside/ressources/book/player handbook/03_CLASSES/MAGICIEN/`. Veille à respecter le schéma de data `src/plugins/classe_manager/classe_manager_schema.sql`
  - [x] faire la seed de moine au format sql dans `src/plugin/classe_manager` à partir du markdown dans  : `_setaside/ressources/book/player handbook/03_CLASSES/MOINE/`. Veille à respecter le schéma de data `src/plugins/classe_manager/classe_manager_schema.sql`
  - [x] faire la seed de occultiste au format sql dans `src/plugin/classe_manager` à partir du markdown dans  : `_setaside/ressources/book/player handbook/03_CLASSES/OCCULTISTE/`. Veille à respecter le schéma de data `src/plugins/classe_manager/classe_manager_schema.sql`
  - [ ] faire la seed de rodeur au format sql dans `src/plugin/classe_manager` à partir du markdown dans  : `_setaside/ressources/book/player handbook/03_CLASSES/RODEUR/`. Veille à respecter le schéma de data `src/plugins/classe_manager/classe_manager_schema.sql`


- [X] tab aptitude je pense pas que le layout soit le plus efficace. 
- [X] tab Evolution , collonne aptitude c'est moche ... il faudrait les noms pas les ids, mettre du retour ligne. 
- [X] Tab evolution : Pourquoi la ligne LVL 3 est jaune ? C'est parce qu'elle implique un choix ? Sic'est le cas il le faut aussi LVL 14 car choix totémique différent de celui du niveau 3. 
- [x] Peut être découpé le code par tab pour que ça soit plus lisible ? Dans plusier fichier.
- [x] Je dois pouvoir modifier TOUTES les aptitude même celle des sous classes
- [X] Le bouton modifer tout en haut ne devrait pas y être il se contente de modifier la classe donc on inclu les modif / suppression dans le tab Classe comme pour les autre classe. 
- [X] Supprimer une classe entière se fait depuis le side panel de gauche. 
- [X] Erreur lors du chargement de la seed druid. Erreur : table classe_evolutions has no column named sort_slot_1 je panse que tu n'a pas respecer strictement le schéma sql
- [X] à coté de Barbare il y a écrit [barbare] je pense que c'est l'alias. Si c'est pas le cas il faut que ça soit l'alias mais sout la forme [@classe.barbare]. Plus utils pour l'utilisateur.
- [X] L'affichage des aptitude utilise t'il le pseudo markdown du moteur ? Sinon il faut l'ajouter parce que ça sera obligatoire pour les nombreux tableux de sorts de d'autre classes.
- [X] Même si ça ne s'affiche pas dans le tab classe il faut que j'ai un récape des choix de build et de leur niveau ainsi que l'apptitude à l'origine et je dois pouvoir paramétré les différent ochoix/réponse dans ce panel d'édition. L'édition des aptitude qura donc dans les prérequi des combots en fonction de ce qui est paramétré dans ce panel la. je pense que cette modification peux justifier une refonte de la DB. Ajouter une table classe_choix_evolution_champs et classe_choix_evolution_valeur. Avec des table de liens entre les différentes table. y compris avec celle des apptitude qui aurais donc comme champs suplémentaire : classe_choix_evolution_champs si c'est une apptitude ou l'on doit faire un choix et classe_choix_evolution_valeur si c'est une apptitude qui dépende d'un choix. ça remplacerait les prerqui en JSON et permeterait une meilleur edition avec des comba plus facile à mettre en place. Par contre ça implique pas mal de fix dans le plugin.
- [ ] Tout les bouton supr doivent avoir une popup donc on va migrer la popup dans src/core/utils.cpp et on l'implemente partout ou on a un bouton suprimer. Titre de la popup [Confirmation de suppression] le contenue de la popu me vas. 

#### Graphe

MISE EN PAUSE : Je ferai un truc plus propre plus tard avec le système de graphe et le moteur dédié.

- [x] J'aimerais avoir un rendu de type Arbre d'évolution je pense que ma data pérmet de le généré. 
- [x] Différence d'affichage entre les 3 premier niveau (nom du niveau sur le coté)et les niveau suivant (non du niveau en haut de la tuile) Je préfère l'affichage des niveau suivant. Mais en mettant un encart de trite comme un UML. 
- [x] Pourquoi le choix du totem spirituel s'affiche en liste et pas comme le choix de voie de la voix primitive ? 
- [x] J'aimerais des infomartion au survole et au clique. Je penque que la description en survolle et tout eu clique c'est bien (tout s'affiche dans la tuile au clique).
- [ ] Pour les tuiles du tab Arbre. Les tuiles doivent fix contente. En heuteur et en largeur. Alignement par le centre des tuile d'une meme branche. ligne en courbe de bézier. 


### rendu de l'arbre de d'évolution.

Je veux un rendu comme ça : 
```
             +-----------------------------+
LVL 1        | liste des aptitude de lvl 1 |
             +-------------+---------------+
                           |
             +-------------+---------------+
LVL 2        | liste des aptitude de lvl 2 |
             +-------------+---------------+
                           |
             +-------------+---------------+
LVL 3        | liste des aptitude de lvl 3 |
             | Choix                       |
             +-------------+---------------+
                           |
                  +--------+-------+
                  |                |
            +-----+------+  +------+-----+  
            |  A         |  |  B         |
            +-----+------+  +------+-----+  
 ...
```


# Gestion des tuiles. 

 Je pense que pour les affichage par tuile en flow on vas faire un moteur de rendu. Plus permissif même si plus ourd à faire. Il sera massivement utilisé dans la partie scénario ultérieurement. Inspiration principal : le no code dans Unreal Engine. Donc chaque tuile à : 

- Un titre
- Liste d'entrée
- Liste de sortie
- Des composante interne. 

 il existe plusieur type de tuile qu'on définira plus tard ensemble. la on se concentre sur le render tuile qui est générique. 

 quelque spécification d'affichage : 
- Les tuile sont dans un espace infini grillagé. 
- Les tuiles peuvent être reposistionné dans l'espace. 
- Elle ne peuvent pas se chevauché. Donc leur taille est fixe 
- Elle ont des caractéritique affiché de base mais peuvent être dévelopé dans un menu contextuel (exemple : tuile de niveau avec le nom de l'apptitude apprise -> remplie la fenre contextuelle de choix de la tuile.)
- Les liens entre les tuiles sont des courbe de bézier. 
- Les entrées sont sur le dessus, les sortie en bas. 

Pour le moment pas d'édition possible c'est uniquement de l'affichage l'édition se fait dans la fenre contextuelle. 

D'un pov dev, une classe abstrait qui explique le rendu. les classe enfant sont propre au plugin. donc dans le plugin Classe il y a une classe tuile_classes qui spécialise la classe tuile.

Je pense que regénéré les arbre dynamiquement à chaque ouverture c'est pas ouf on faire un espace de DB qui enregistre les choses et permet de les retrouver comme elle était plus tard. Juste un fact check à l'ouverture s'il y a des maj data. DE plus les arbres qui sont des ensemble de tuiles sont save dans le .dnd 

donc dans la logique il y aura : 
- la classe graphe qui gère l'affichage
- elle comprend des classes abstraite tuiles qui sont spécialisé dans les plugin. 

J'ai penser à : 

- CLASSE GRAPHE
  - Atribut 
    - Liste des tuiles
    - Valeur de zoom
  - methode 
    - Auto-replace
- CLASSE ABSTRAITE TUILE
  - Atribut 
    - Nom 
    - couleur
    - Dim x, y
    - position x, y
    - (liste entrées)
    - liste sorties [ptr sur tuiles]
    - label des sorties
    - bezier des sorties (peut être automatique)
    - Liste de contenue [table de pseudo md]
  - Methode 
    - Render tuile
    - render detail
    - Render edition
- CLASSE TUILE_APTITUDE
  - Gère mieux l'édition d'une apptitude de classe
  - surcharge des méthodes
- CLASSE TUILE_RENCONTRE
  - integre le résumé d'une rencontre géré par le générateur de rencontre/gestionnaire de rencontre. (nécessite le générateur de rencontre, turn order, gestionnaire de rencontre.)
  - surcharge des méthodes
- CLASSE TUILE_HISTOIRE
  - Boolean pour savoir si branche visité 
  - surcharge des méthodes
- CLASSE TUILE_LIEUX
  - integre une minimap d'un lieux (nécésite la map)
  - surcharge des méthodes

# Mode scénario

Le mode scénario c'est l'outils de création de campagne donc il doit géré : 

- le lore de l'univers
- le lors des pj
  - pj attaché 
  - liste d'event (date, lieux, description)
- le lors des pnj 
  - pnj attaché 
  - liste d'event (date, lieux, description)
- une frise chronologique c'est simplement une vue (peut être faire un moteur de rendu pour ça aussi mais pas core c'est spécifique à lui)
- la gestion de la campagne
  - les actes 
    - le but de l'acte
    - description 
    - liste des lieux attaché (nécéssite map) - récursif
    - liste des pnj attaché (nécessite gestionnaire de pnj) - récursif
    - liste des rencontre attaché (nécessite gestionnaire de rencontre, générateur de rencontre) récursif
    - les scenes
      - liste des lieux attaché (nécéssite map) - récursif
      - liste des pnj attaché (nécessite gestionnaire de pnj) - récursif
      - liste des rencontre attaché (nécessite gestionnaire de rencontre, générateur de rencontre) récursif
      - tableau d'exp par accomplissement
      - un GRAPHE RENDER avec le detaille de la scène 
    
J'ai déjà pas mal a faire avec ça. Ici pas de SQL à écrire tout est géré dans le .dnd (qui en est techniquement un mais je le différencie des sql qui ne reprenne que les livres.)

# Gestionnaire de don ....

Ne pas oublier

# DISTINCTION TECHNIQUE 

deux type de fichier loadable
- `.univ` qui sont des seed de data scrapper des livre. Ont doit pouvoir les loads depuis un SQL ou un CSV et en `.univ` qui est une sorte de sql en brut por gagné de la place. On exporté dans tout les style de lecture. Un fichier .univ peux comporté plusieur table de différent plugin. Il faut donc que le format précise à quelle plugin et quelle version appartient la table pour le chargé. 
- `.dnd`  qui sont les données du jeu (aventure, fiche de perso, rencontre, map, histoire ...) uniquement chargable et exportable en `.dnd` qui est une sorte de sql en brute pour gagné de la place. On ne sait pas à l'avance ce que vas contenir ce type de fichier. Peut être une simple de fiche de perso ou allors toutes une campagne. Il faut que le format de fichier indique les plugins et leurs version nécessaire à son chargement. s'il ne sont pas tous présent il propose de ne chargé que ce qu'il peux. 

# CORE/MOTEUR 

Est ce qu'il serai utile de mettre dans le manifest quelque chose pour l'auto completion ? peut être même un fichier généré à la volé à louverture de la DB pour le rendre plus fluide. un Json ferai l'affaire. exemple : 

```json
auto_comletion [
  race[
    elfe, nain, ...
  ],
  equipement[
    arme[
      hache, épée, ...
    ],
    armure [
      maille, cuire
    ]
  ]
]
```

# Gestionnaire d'historique. 

J'ai pas encore taffé sur le gestionnaire d'historique. Basiquement je pense que un historique c'est de la forme 

Avant de me lancé la dedans il va falloir que je passe en md puis en sql les scannes. 

```sql
TABLE historique (
  id TEXT
  nom TEXT
  alias TEXT
  compétence maitrisé 
  outils maitrisé 
  langue maitrisé
  équipement maitrisé 
)
TABLE hitorique_ersonalité (
  id 
  rang
  titre 
  desciption
  id de l historique
)
table hitorique_déal  (
  id 
  rang
  titre 
  desciption
  id de l historique
)
table hitorique_bligation (
  id 
  rang
  titre 
  desciption
  id de l historique
)
table hitorique_faille (
  id 
  rang
  titre 
  desciption
  id de l historique
)
table historique_description (
  id 
  titre 
  estunchoix %s il y a un choix a faire dans ce paragraphe
  nom de colone choix 1
  nom de colone choix 2 
  nom de colone choix 3
)
table historique_description_choix(
  id 
  id historique
  valeur 1
  valeur 2
  valeur 3
)
```
Il faudra probablement donné ça a Claude pour qu'il me dise ce qu'il en pense et s'il pense qu'il manque des trucs dedans. 

Normalement j'ai rien oublié. 
La présentation se fera comme les races (relativement simple tout comme les races.)
l'édition sera comme les races aussi.

# Gestionnaire d'équipement

Dans ma gestion de la fiche de perso je considère que tout ce qui est dans l'inventaire est un équipement. Ainsi les armes, armures, outils, objet,etc sont des équipement. La tables equipement est donc lourde car en fonctiont du type de celui ci elle intège tout un tas de champs pas toujours necessaire. 

Cela dis j'ai déjà fait un MD de ces pages donc plus rapide. 

