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

WORKFLOW APP
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
  
RÉPONSE AU QUESTIONS
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

●User approved Claude's plan:
  ⎿  Plan: Plugin CharacterCreator - CRÉATION UNIQUEMENT (Étape 1 d'abord)

     Clarifications

     - Outil de CRÉATION uniquement (pas d'évolution/visualisation - ce sera un autre outil)
     - Multi-instance: Chaque création = nouvelle instance de personnage
     - Navigation bidirectionnelle: Retour en arrière possible, forward si rien modifié
     - Fil d'Ariane: Indicateur visuel des étapes (Step 1/8)
     - Développement itératif: On fait UNIQUEMENT l'Étape 1 (Race) d'abord, on validera avant de passer à 
     l'Étape 2

     Structure des 8 Étapes (Architecture)

     enum CreationStep {
         STEP_RACE_SELECTION,      // 0: Grille de tuiles des races
         STEP_RACE_DETAILS,        // 1: Détails + choix variantes + nom
         STEP_CLASS_SELECTION,     // 2: Grille de tuiles des classes
         STEP_CLASS_DETAILS,       // 3: Détails + compétences + équipement
         STEP_STATS,               // 4: Génération caractéristiques (4d6 drop lowest)
         STEP_BACKGROUND,          // 5: Histoire + alignement
         STEP_SPELLS,              // 6: Sorts (si classe lanceur)
         STEP_SUMMARY              // 7: Résumé final + bouton "Créer"
     };

     int currentStep = STEP_RACE_SELECTION;
     std::vector<bool> stepCompleted(8, false);  // Track validation

     Fil d'Ariane (Breadcrumb)

     ┌─────────────────────────────────────────────────────────┐
     │ ● Race  →  ○ Classe  →  ○ Stats  →  ○ Histoire  →  ○ Résumé │
     └─────────────────────────────────────────────────────────┘
     ● = Étape actuelle (vert)
     ○ = Non complétée (gris)
     ◉ = Complétée (bleu)

     Navigation:
     - Clic sur étape complétée → retour direct
     - Boutons "Précédent" / "Suivant"
     - "Suivant" grisé si étape non validée

     SCOPE ACTUEL: Étape 1 - Choix de la Race

     Étape 1a: Grille de Sélection (Tuiles)

     Données depuis DB (DatabaseManager::LoadRaces()):
     struct RaceData {
         string id;           // "elf", "dwarf"...
         string name_fr;      // "Elfe", "Nain"...
         string description;  // Description courte (pour tuile)
         // ... autres champs
     };

     UI - Grille 3 colonnes:
     ┌─────────────────────────────────────────┐
     │        CHOISISSEZ VOTRE RACE           │
     ├─────────────────────────────────────────┤
     │  ┌────────┐  ┌────────┐  ┌────────┐   │
     │  │  ELFE  │  │  NAIN  │  │ HUMAIN │   │
     │  │ Agile  │  │Robuste │  │Adaptable│   │
     │  └────────┘  └────────┘  └────────┘   │
     │  ┌────────┐  ┌────────┐  ┌────────┐   │
     │  │ ...    │  │  ...   │  │  ...   │   │
     └─────────────────────────────────────────┘

     Comportement:
     - Hover → Highlight
     - Clic → currentStep = STEP_RACE_DETAILS
     - Stocke selectedRaceId

     Étape 1b: Détails de la Race

     Affichage (scrollable):
     ┌─────────────────────────────────────────┐
     │  ◀ Retour          ELFE                │
     ├─────────────────────────────────────────┤
     │  Description:                          │
     │  [Texte formaté long depuis DB]       │
     │                                        │
     │  Bonus de Caractéristiques:           │
     │  • Dextérité: +2                      │
     │                                        │
     │  Vitesse: 30 ft                       │
     │  Taille: Moyenne                      │
     │  Langues: Commun, Elfique            │
     │                                        │
     │  Traits Raciaux:                      │
     │  • Vision dans le noir (60 ft)       │
     │  • Sens aiguisés (Perception)        │
     │  • Ascendance féerique               │
     │                                        │
     │  ─────────────────────────────────    │
     │  Variantes Raciales:                  │
     │  ○ Elfe des bois (+1 Sagesse)        │
     │  ○ Haut-Elfe (+1 Intelligence)       │
     │  ○ Elfe noir (+1 Charisme)           │
     │                                        │
     │  ─────────────────────────────────    │
     │  Informations Personnelles:           │
     │  Prénom:  [______________]            │
     │  Nom:     [______________]            │
     │  Âge:     [____] ans                  │
     │                                        │
     │         [Précédent]  [Suivant]        │
     └─────────────────────────────────────────┘

     Validation:
     - Variante sélectionnée (si applicable)
     - Nom/Prénom remplis
     - "Suivant" activé seulement si validé
     - Stocke tout dans DnD::Character character;

     Étape 1c: Boutons Navigation

     Logique:
     // Bouton Précédent
     if (ImGui::Button("Précédent")) {
         if (currentStep > 0) currentStep--;
     }

     // Bouton Suivant
     bool canProceed = ValidateCurrentStep();
     ImGui::BeginDisabled(!canProceed);
     if (ImGui::Button("Suivant")) {
         stepCompleted[currentStep] = true;
         currentStep++;
     }
     ImGui::EndDisabled();

     Persistence (Multi-instance)

     OnSave

     {
       "instance_id": "char_creator_abc123",
       "current_step": 1,
       "character_data": {
         "name": "Legolas",
         "race_id": "elf",
         "race_variant": "wood_elf",
         // ... autres champs au fur et à mesure
       },
       "steps_completed": [true, true, false, false, false, false, false, false]
     }

     OnLoad

     - Restaure l'étape en cours
     - Restaure les données déjà saisies
     - Permet de reprendre la création où on l'a laissée

     Fichiers à Créer (Pour Étape 1)

     plugins_src/character_creator/
     ├── CharacterCreatorTool.h
     │   - CreationStep enum
     │   - currentStep, stepCompleted[]
     │   - DnD::Character character
     │   - Database::RaceData selectedRace
     │   - vector<RaceData> availableRaces
     │   
     ├── CharacterCreatorTool.cpp
     │   - OnCreate(): Charger races depuis DB
     │   - Render():
     │       - RenderBreadcrumb()
     │       - switch(currentStep) {
     │           case STEP_RACE_SELECTION: RenderRaceGrid(); break;
     │           case STEP_RACE_DETAILS: RenderRaceDetails(); break;
     │           // ... stubs pour autres étapes
     │         }
     │   - RenderRaceGrid(): Tuiles 3 colonnes
     │   - RenderRaceDetails(): Détails + variantes + nom
     │   - ValidateStep1(): Vérifie nom rempli + variante choisie
     │   - OnSave/OnLoad: Persistence
     │   
     └── character_creator.manifest.json
         - singleton: false
         - requires: database, logger

     Étapes d'Implémentation (Étape 1 uniquement)

     1. Créer le plugin avec create_plugin.sh
       - ID: character_creator
       - Nom: "Créateur de Personnage"
       - Catégorie: "Création"
     2. Structure de base
       - Enum CreationStep
       - Variables: currentStep, stepCompleted[], character
       - Breadcrumb basique (affichage texte)
     3. OnCreate: Charger les races
       - availableRaces = dbManager->LoadRaces()
       - Gérer le cas où DB non connectée
     4. RenderRaceGrid(): Grille de tuiles
       - Loop sur availableRaces
       - 3 colonnes avec ImGui::Columns(3)
       - Tuiles cliquables (BeginChild + Selectable)
     5. RenderRaceDetails(): Page de détails
       - Affichage texte formaté (description)
       - Bonus de stats, traits
       - Boutons radio pour variantes
       - InputText pour nom/prénom/âge
     6. Navigation + Validation
       - Boutons Précédent/Suivant
       - ValidateStep1(): nom rempli + variante OK
       - Breadcrumb avec indicateur visuel
     7. OnSave/OnLoad
       - Sauvegarder état de création en cours
       - Restaurer au reopening
     8. Tests
       - Parcourir Étape 1a → 1b → retour → forward
       - Vérifier persistence
       - Valider avec toi avant de passer à l'Étape 2

     Notes pour Plus Tard

     Les autres étapes (2-7) suivront le même pattern:
     - Grille de sélection → Détails + choix
     - Validation avant "Suivant"
     - Stockage dans character
     - Mais on ne les code PAS maintenant
  ⎿

● Parfait! Je commence l'implémentation. D'abord le TodoList:

● Maintenant je crée le plugin:

● Bash(echo -e "character_creator\nCharacterCreator\nCréateur de Personnage\nOutil de création de    timeout: 
      personnage D&D 5e\n1\ny" | ./create_plugin.sh)                                                 30s
  ⎿  Running…

  Todos
  ☐ Créer le plugin character_creator avec le script
  ☐ Implémenter la structure de navigation (enum, breadcrumb)
  ☐ OnCreate: Charger les races depuis la DB
  ☐ RenderRaceGrid: Grille de tuiles 3 colonnes
  ☐ RenderRaceDetails: Page de détails de la race
  ☐ Implémenter navigation + validation Étape 1
  ☐ OnSave/OnLoad: Persistence multi-instance
  ☐ Compiler et tester Étape  1
