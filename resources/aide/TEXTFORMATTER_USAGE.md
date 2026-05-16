# 📖 TextFormatter - Guide d'Utilisation

## ✅ Compilation Réussie

Le TextFormatter a été compilé avec succès sans erreurs ni warnings !

## 🎯 Utilisation en C++

### 1. Include

```cpp
#include "ui/TextFormatter.h"
```

### 2. Utilisation Simple (sans liens cliquables)

```cpp
UI::TextFormatter formatter;

std::string text = R"(
# DRUIDE

Le druide est un lanceur de sorts de la nature.

## Points de vie
- **Dé de vie** : {1d8} par niveau
- **PV au niveau 1** : {8+CON}

## Sorts
Vous apprenez [Druidisme](spell:druidcraft).
)";

// Dans votre boucle ImGui::Begin/End
formatter.RenderFormatted(text);
```

### 3. Utilisation avec Liens Cliquables

```cpp
UI::TextFormatter formatter;

std::string raceDescription = R"(
# ELFE

## Traits raciaux

### Augmentation de caractéristiques
Votre {DEX} augmente de 2.

### Vision dans le noir
Consultez [Vision dans le noir](racial_ability:darkvision) pour les détails.

### Sens aiguisés
Vous maîtrisez la compétence [Perception](skill:perception).
)";

// Callback appelé quand on clique sur un lien
auto onLinkClick = [](UI::LinkType type, const std::string& targetId) {
    switch (type) {
        case UI::LinkType::Spell:
            // Ouvrir une fenêtre modale avec le sort
            std::cout << "Ouvrir sort: " << targetId << std::endl;
            // TODO: OpenSpellDetailWindow(targetId);
            break;

        case UI::LinkType::Skill:
            std::cout << "Ouvrir compétence: " << targetId << std::endl;
            // TODO: OpenSkillDetailWindow(targetId);
            break;

        case UI::LinkType::RacialAbility:
            std::cout << "Ouvrir abilité raciale: " << targetId << std::endl;
            // TODO: OpenRacialAbilityWindow(targetId);
            break;

        default:
            std::cout << "Lien non géré: " << targetId << std::endl;
            break;
    }
};

// Dans votre boucle ImGui::Begin/End
formatter.RenderFormatted(raceDescription, onLinkClick);
```

### 4. Parsing et Rendering Séparés (pour optimisation)

```cpp
UI::TextFormatter formatter;

// Parser une seule fois (dans le constructeur)
UI::ParsedDocument* parsedDoc = formatter.Parse(description);

// Render plusieurs fois (dans la boucle de rendu)
void Render() {
    formatter.Render(parsedDoc, onLinkClick);
}

// Ne pas oublier de delete à la fin
~MyWindow() {
    delete parsedDoc;
}
```

## 📝 Exemples de Textes Formatés

### Exemple 1: Description de Race

```cpp
std::string elfDescription = R"(
# ELFE

Les elfes sont un peuple magique à la grâce surnaturelle.

## Traits raciaux

### Augmentation de caractéristiques
Votre {DEX} augmente de 2.

### Âge
Les elfes atteignent l'âge adulte vers 100 ans et peuvent vivre jusqu'à 750 ans.

### Taille
Votre taille est **Moyenne** (150-180 cm).

### Vitesse
Votre vitesse de base est de 30 pieds.

### Vision dans le noir
Vous voyez dans la pénombre à 60 pieds comme en pleine lumière.
Voir [Vision dans le noir](racial_ability:darkvision) pour plus de détails.

### Sens aiguisés
Vous maîtrisez la compétence [Perception](skill:perception).

### Ascendance féerique
Vous avez l'**avantage** aux jets de sauvegarde contre les effets de charme.

### Transe
Au lieu de dormir, vous méditez pendant *4 heures*.
)";

formatter.RenderFormatted(elfDescription, onLinkClick);
```

### Exemple 2: Description de Sort

```cpp
std::string fireballDescription = R"(
# BOULE DE FEU

*Sort d'évocation de niveau 3*

## Caractéristiques
- **Temps d'incantation** : 1 action
- **Portée** : 150 pieds
- **Composantes** : V, S, M (une petite boule de guano de chauve-souris et du soufre)
- **Durée** : Instantanée

## Description
Un éclair brillant file depuis votre doigt pointé vers un point que vous choisissez à portée et explose en une gerbe de flammes.

**Jet de sauvegarde** : [Dextérité](ability:dexterity) DD {8+bonus_maitrise+MOD}

**Effet** : Chaque créature dans une sphère de 20 pieds doit réussir un jet de sauvegarde ou subir {8d6} dégâts de feu, ou la moitié en cas de réussite.

## Aux niveaux supérieurs
Lorsque vous lancez ce sort avec un emplacement de niveau 4 ou supérieur, les dégâts augmentent de {1d6} par niveau au-delà du niveau 3.
)";

formatter.RenderFormatted(fireballDescription, onLinkClick);
```

### Exemple 3: Tableau de Progression

```cpp
std::string wizardTable = R"(
# PROGRESSION DU MAGICIEN

## Table de progression

|Niveau|Bonus Maîtrise|Sorts connus|Emplacements 1|Emplacements 2|Emplacements 3|
|------|--------------|------------|--------------|--------------|--------------|
|1     |+2            |6           |2             |0             |0             |
|2     |+2            |7           |3             |0             |0             |
|3     |+2            |8           |4             |2             |0             |
|4     |+2            |9           |4             |3             |0             |
|5     |+3            |10          |4             |3             |2             |

Au niveau 1, vous apprenez automatiquement [Projectile magique](spell:magic_missile) et [Bouclier](spell:shield).
)";

formatter.RenderFormatted(wizardTable, onLinkClick);
```

## 🔗 Intégration dans CharacterCreatorWindow

### Dans le header (CharacterCreatorWindow.h)

```cpp
#include "../ui/TextFormatter.h"

class CharacterCreatorWindow {
private:
    UI::TextFormatter textFormatter;

    void OnLinkClick(UI::LinkType type, const std::string& targetId);
};
```

### Dans le cpp (CharacterCreatorWindow.cpp)

```cpp
void CharacterCreatorWindow::RenderRaceClass() {
    // ...code existant...

    // Afficher la description de la race sélectionnée
    if (selectedRace >= 0 && selectedRace < static_cast<int>(dbRaces.size())) {
        const auto& race = dbRaces[selectedRace];

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Description:");
        ImGui::Separator();

        // Render avec le formatter
        textFormatter.RenderFormatted(
            race.description_longue,
            [this](UI::LinkType type, const std::string& id) {
                this->OnLinkClick(type, id);
            }
        );
    }
}

void CharacterCreatorWindow::OnLinkClick(UI::LinkType type, const std::string& targetId) {
    switch (type) {
        case UI::LinkType::Spell:
            // TODO: Ouvrir fenêtre de détail du sort
            std::cout << "Clic sur sort: " << targetId << std::endl;
            break;

        case UI::LinkType::Skill:
            // TODO: Ouvrir fenêtre de compétence
            std::cout << "Clic sur compétence: " << targetId << std::endl;
            break;

        case UI::LinkType::RacialAbility:
            // TODO: Ouvrir fenêtre d'abilité raciale
            std::cout << "Clic sur abilité: " << targetId << std::endl;
            break;

        default:
            break;
    }
}
```

## 🎨 Personnalisation des Couleurs

Vous pouvez modifier les couleurs en changeant les constantes dans `TextFormatter.cpp` :

```cpp
const ImVec4 TextFormatter::COLOR_TITLE_1(1.0f, 0.8f, 0.0f, 1.0f);  // Or → changer en rouge ?
const ImVec4 TextFormatter::COLOR_DICE(0.2f, 1.0f, 0.2f, 1.0f);    // Vert → bleu ?
// etc.
```

## 🚀 Prochaines Étapes

### 1. Créer les Fenêtres de Détail

Créer `DetailWindows.h` et `DetailWindows.cpp` pour afficher :
- Détail d'un sort
- Détail d'une compétence
- Détail d'une abilité raciale
- Etc.

### 2. Mettre à Jour la DB

Ajouter les descriptions formatées en DB :

```sql
UPDATE races_new SET description_longue = '
# ELFE

Les elfes sont un peuple magique...

## Traits raciaux

### Augmentation de caractéristiques
Votre {DEX} augmente de 2.

### Vision dans le noir
Voir [Vision dans le noir](racial_ability:darkvision).
'
WHERE nom = 'Elf';
```

### 3. Ajouter des Icônes (optionnel)

Remplacer les émojis par des vraies icônes :

```cpp
// Au lieu de 🎲
const char* ICON_DICE = "\uF6CF";  // FontAwesome dice icon
```

## 📊 Performance

Le TextFormatter est optimisé pour :
- **Parser une fois** : Gardez le `ParsedDocument*` en cache
- **Render plusieurs fois** : Le rendering est rapide
- **Mémoire** : Les documents parsés sont petits (~quelques KB)

Exemple de pattern optimal :

```cpp
class RaceInfoWindow {
private:
    std::map<std::string, UI::ParsedDocument*> parsedDescriptions;
    UI::TextFormatter formatter;

public:
    void ShowRace(const std::string& raceId, const std::string& description) {
        // Parser une seule fois par race
        if (parsedDescriptions.find(raceId) == parsedDescriptions.end()) {
            parsedDescriptions[raceId] = formatter.Parse(description);
        }

        // Render (rapide)
        formatter.Render(parsedDescriptions[raceId], onLinkClick);
    }

    ~RaceInfoWindow() {
        for (auto& pair : parsedDescriptions) {
            delete pair.second;
        }
    }
};
```

## ✅ Checklist d'Intégration

- [x] TextFormatter compilé
- [x] Documentation du format créée
- [ ] Intégrer dans CharacterCreatorWindow
- [ ] Créer DetailWindows pour les liens cliquables
- [ ] Ajouter descriptions formatées en DB
- [ ] Tester avec toutes les races
- [ ] Tester avec toutes les classes
- [ ] Tester avec les sorts

## 🐛 Troubleshooting

### Les liens ne s'affichent pas
Vérifier le format : `[Texte](type:id)` sans espaces

### Les formules ne sont pas colorées
Vérifier que les accolades sont bien fermées : `{2d6+3}`

### Le tableau ne s'affiche pas
- Première ligne = headers
- Deuxième ligne = séparateur `|---|---|`
- Lignes suivantes = données

### Crash au clic sur un lien
Vérifier que le callback `onLinkClick` n'est pas `nullptr`
