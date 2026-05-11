# Format de Texte Personnalisé pour D&D DM Toolbox

## 🎯 Objectif

Créer un format simple de balisage de texte pour les descriptions dans la base de données, avec support de :
1. Titres colorés (paragraphes)
2. Formules mathématiques (dés, calculs)
3. Tableaux
4. Liens cliquables vers d'autres éléments de DB

## 📝 Syntaxe

### 1. Titres / Paragraphes

```
# Titre Principal
Texte en couleur dorée, plus gros

## Sous-titre
Texte en couleur plus claire, taille moyenne

### Sous-sous-titre
Texte normal mais avec séparateur
```

**Rendu ImGui** :
- `#` → Texte doré (1.0, 0.8, 0.0) + séparateur
- `##` → Texte cyan (0.4, 0.8, 1.0) + séparateur
- `###` → Texte blanc + séparateur simple

### 2. Formules Mathématiques / Dés

```
Vous infligez {2d6+3} dégâts de feu.
Votre modificateur est {DEX}.
Le DC est {8+bonus_maitrise+MOD}.
```

**Syntaxe** : `{formule}`

**Exemples** :
- `{1d20}` → Affiche en vert avec icône de dé
- `{2d6+3}` → Affiche en vert avec icône de dé
- `{DEX}` → Affiche en cyan (référence à caractéristique)
- `{8+bonus_maitrise+SAG}` → Affiche en cyan (formule DC)

**Rendu ImGui** :
```cpp
ImGui::TextColored(vert, "🎲 2d6+3");  // ou icône personnalisée
```

### 3. Tableaux

```
|Niveau|Bonus Maîtrise|Sorts connus|
|------|--------------|------------|
|1     |+2            |4           |
|2     |+2            |5           |
|3     |+2            |6           |
```

**Règles** :
- Première ligne = headers
- Deuxième ligne = séparateur (ignorée)
- Lignes suivantes = données
- Colonnes séparées par `|`

**Rendu ImGui** :
```cpp
ImGui::BeginTable("table", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg);
// Headers en couleur
// Lignes alternées avec background
ImGui::EndTable();
```

### 4. Liens vers la DB

```
Vous apprenez le sort [Boule de feu](spell:fireball).
Vous maîtrisez la compétence [Perception](skill:perception).
Vous obtenez l'abilité raciale [Vision dans le noir](racial_ability:darkvision).
Vous pouvez utiliser [Arc long](weapon:longbow).
```

**Syntaxe** : `[Texte affiché](type:identifiant)`

**Types supportés** :
- `spell:id_sort` → Ouvre fenêtre de détail du sort
- `skill:id_competence` → Ouvre détail de la compétence
- `racial_ability:id_abilite` → Ouvre détail de l'abilité
- `class_feature:id_capacite` → Ouvre détail de la capacité
- `weapon:id_arme` → Ouvre détail de l'arme
- `armor:id_armure` → Ouvre détail de l'armure
- `item:id_objet` → Ouvre détail de l'objet

**Rendu ImGui** :
```cpp
ImGui::TextColored(bleu_lien, "Boule de feu");
if (ImGui::IsItemHovered()) {
    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    ImGui::SetTooltip("Cliquer pour voir les détails");
}
if (ImGui::IsItemClicked()) {
    // Ouvrir fenêtre modale avec détails du sort
}
```

### 5. Listes à puces

```
- Point 1
- Point 2
  - Sous-point 2.1
  - Sous-point 2.2
- Point 3
```

**Rendu ImGui** :
```cpp
ImGui::BulletText("Point 1");
ImGui::Indent();
ImGui::BulletText("Sous-point 2.1");
ImGui::Unindent();
```

### 6. Texte en gras / italique

```
**Texte en gras** pour l'emphase.
*Texte en italique* pour les notes.
```

**Rendu ImGui** :
- `**texte**` → Couleur blanche pure (1, 1, 1) ou police plus épaisse si disponible
- `*texte*` → Couleur grise claire (0.8, 0.8, 0.8)

## 📄 Exemple Complet

```
# DRUIDE

Le druide est un lanceur de sorts de la nature.

## Capacités de classe

### Points de vie
- **Dé de vie** : {1d8} par niveau de druide
- **Points de vie au niveau 1** : {8+CON}
- **Points de vie aux niveaux supérieurs** : {1d8} (ou 5) + modificateur de {CON}

### Maîtrises
Vous maîtrisez les [Jets de sauvegarde](saving_throw:intelligence) suivants :
- [Intelligence](ability:intelligence)
- [Sagesse](ability:wisdom)

### Sorts
|Niveau|Sorts mineurs connus|Sorts connus|Emplacements niveau 1|
|------|-------------------|------------|---------------------|
|1     |2                  |4           |2                    |
|2     |2                  |5           |3                    |
|3     |2                  |6           |4                    |

Au niveau 1, vous apprenez [Druidisme](spell:druidcraft) et [Gourdin magique](spell:shillelagh).

## Forme sauvage

À partir du niveau 2, vous pouvez utiliser **Forme sauvage** pour vous transformer en animal.

Vous infligez {1d6+FOR} dégâts avec vos attaques naturelles.
```

## 🎨 Couleurs Proposées

```cpp
// Titres
const ImVec4 COLOR_TITLE_1(1.0f, 0.8f, 0.0f, 1.0f);   // Or
const ImVec4 COLOR_TITLE_2(0.4f, 0.8f, 1.0f, 1.0f);   // Cyan clair
const ImVec4 COLOR_TITLE_3(1.0f, 1.0f, 1.0f, 1.0f);   // Blanc

// Formules
const ImVec4 COLOR_DICE(0.2f, 1.0f, 0.2f, 1.0f);      // Vert (dés)
const ImVec4 COLOR_ABILITY(0.5f, 0.8f, 1.0f, 1.0f);   // Cyan (caractéristiques)

// Liens
const ImVec4 COLOR_LINK(0.3f, 0.6f, 1.0f, 1.0f);      // Bleu lien
const ImVec4 COLOR_LINK_HOVER(0.5f, 0.8f, 1.0f, 1.0f); // Bleu clair au survol

// Emphase
const ImVec4 COLOR_BOLD(1.0f, 1.0f, 1.0f, 1.0f);      // Blanc pur
const ImVec4 COLOR_ITALIC(0.8f, 0.8f, 0.8f, 1.0f);    // Gris clair

// Tableaux
const ImVec4 COLOR_TABLE_HEADER(0.2f, 0.2f, 0.3f, 1.0f); // Gris foncé pour header
const ImVec4 COLOR_TABLE_ROW_BG(0.1f, 0.1f, 0.15f, 0.5f); // Gris très foncé alternance
```

## 🔧 Utilisation en DB

### Exemple pour une race

```sql
UPDATE races_new SET description_longue = '
# ELFE

Les elfes sont un peuple magique à la grâce surnaturelle.

## Traits raciaux

### Augmentation de caractéristiques
Votre {DEX} augmente de 2.

### Âge
Les elfes atteignent l''âge adulte vers 100 ans et peuvent vivre jusqu''à 750 ans.

### Taille
Votre taille est **Moyenne** (150-180 cm).

### Vitesse
Votre vitesse de base est de 30 pieds.

### Vision dans le noir
Vous voyez dans la pénombre à 60 pieds comme en pleine lumière. Consultez [Vision dans le noir](racial_ability:darkvision) pour les détails.

### Sens aiguisés
Vous maîtrisez la compétence [Perception](skill:perception).

### Ascendance féerique
Vous avez l''**avantage** aux jets de sauvegarde contre les effets de charme et la magie ne peut pas vous endormir.

### Transe
Au lieu de dormir, vous méditez pendant 4 heures. Après un repos de ce type, vous obtenez les mêmes bénéfices qu''un humain après 8 heures de sommeil.
'
WHERE nom = 'Elf';
```

### Exemple pour un sort

```sql
UPDATE game_spells SET description_fr = '
Un éclair brillant file depuis votre doigt pointé vers un point que vous choisissez à portée et explose en une gerbe de flammes.

**Jet de sauvegarde** : [Dextérité](ability:dexterity) DD {8+bonus_maitrise+MOD}

**Effet** : Chaque créature dans une sphère de 20 pieds centrée sur ce point doit réussir un jet de sauvegarde ou subir {8d6} dégâts de feu, ou la moitié en cas de réussite.

Le feu se propage dans les coins. Il embrase les objets inflammables dans la zone qui ne sont pas portés ou transportés.
'
WHERE name = 'Fireball';
```

## 🎯 Avantages

1. **Lisible en DB** : Le format reste lisible même sans parser
2. **Flexible** : Facile d'ajouter de nouveaux types de liens
3. **Cohérent** : Même syntaxe partout (races, classes, sorts, etc.)
4. **Interactif** : Liens cliquables pour navigation rapide
5. **Visuel** : Couleurs et formatage pour une meilleure lecture

## 📦 Fichiers à créer

1. `include/ui/TextFormatter.h` - Header avec struct Link, struct ParsedText, etc.
2. `src/ui/TextFormatter.cpp` - Parser et renderer
3. `include/ui/DetailWindows.h` - Fenêtres modales pour liens
4. `src/ui/DetailWindows.cpp` - Implémentation des fenêtres de détail

## 🧪 Tests à faire

1. Parser titre simple : `# Titre`
2. Parser formule : `{2d6+3}`
3. Parser tableau 3x3
4. Parser lien vers sort : `[Fireball](spell:fireball)`
5. Mélange de tout : titre + texte + formule + lien
