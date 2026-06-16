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

## Ajout d'un mode edition/session

Peu être mêttre un mode edition / session pour différencier les logs des deux phase qui sont défférent avec un retour visuel dans la barre de l'app pour toujours savoir dans quelle mode on est. Maybe les deux mode peuvent changer le comportement des plugin genre tu peux pas modifié les race en mode session. 

## Pour le plugin de classe mettre en event

- getClasseEvolveRequired : qui retourne toute les attribut / compétence d'une classe en fonction des prérqui : niveau min, choix de build, etc. 
