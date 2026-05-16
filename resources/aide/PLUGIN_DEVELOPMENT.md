# Guide de Développement de Plugins

Ce guide explique comment créer des plugins (tools) pour D&D DM Toolbox.

## Architecture des Plugins

Chaque plugin est une **shared library** (.so sur Linux, .dll sur Windows) chargée dynamiquement au démarrage de l'application.

### Structure d'un Plugin

```
plugins_src/
└── mon_plugin/
    ├── MonPluginTool.h          # Header avec la déclaration de la classe
    ├── MonPluginTool.cpp         # Implémentation
    └── mon_plugin.manifest.json  # Métadonnées du plugin
```

Le plugin compilé sera placé dans `build/plugins/`:
```
build/plugins/
├── libmon_plugin.so            # Bibliothèque compilée
└── mon_plugin.manifest.json    # Copie du manifest
```

---

## Méthode 1 : Utiliser le Script de Génération (Recommandé)

### Étape 1 : Lancer le script

```bash
./create_plugin.sh
```

Le script vous demandera :
- **ID du plugin** (snake_case, ex: `dice_roller`, `character_sheet`)
- **Nom de la classe** (PascalCase, ex: `DiceRoller`, `CharacterSheet`)
- **Nom affiché** (ex: "Dice Roller", "Fiche Personnage")
- **Description** (courte description de ce que fait le plugin)
- **Catégorie** (Création / Gestion / Jeu / Outils)
- **Icône** (emoji, ex: 🎲 🧙 ⚔️)

### Étape 2 : Implémenter votre plugin

Le script génère automatiquement les 3 fichiers avec des TODOs. Vous devez :

1. **Modifier le .h** : Ajouter vos variables et méthodes privées
2. **Modifier le .cpp** : Implémenter votre logique
3. **Modifier le .manifest.json** : Ajuster les permissions/events si nécessaire

### Étape 3 : Compiler

```bash
cmake --build build
```

Le plugin sera automatiquement compilé et placé dans `build/plugins/`.

### Étape 4 : Tester

```bash
./build/DnD_DM_Toolbox
```

Votre plugin apparaîtra dans le menu **Outils** > **[Catégorie]** > **[Nom]**.

---

## Méthode 2 : Création Manuelle

### 1. Créer le dossier

```bash
mkdir plugins_src/mon_plugin
```

### 2. Copier les templates

```bash
cp plugin_template/TemplateTool.h plugins_src/mon_plugin/MonPluginTool.h
cp plugin_template/TemplateTool.cpp plugins_src/mon_plugin/MonPluginTool.cpp
cp plugin_template/template.manifest.json plugins_src/mon_plugin/mon_plugin.manifest.json
```

### 3. Remplacer les placeholders

Dans tous les fichiers, remplacer :
- `PLUGIN_ID` → l'ID de votre plugin (ex: `mon_plugin`)
- `PLUGIN_CLASS_NAME` → le nom de la classe (ex: `MonPlugin`)
- `PLUGIN_NAME_DISPLAY` → le nom affiché (ex: "Mon Plugin")
- `PLUGIN_DESCRIPTION` → la description
- `PLUGIN_CATEGORY` → la catégorie
- `PLUGIN_ICON` → l'icône

### 4. Ajouter au CMakeLists.txt

Dans `CMakeLists.txt`, après la ligne `# Compiler les plugins`, ajouter :

```cmake
add_plugin(mon_plugin)
```

### 5. Compiler et tester

```bash
cmake --build build
./build/DnD_DM_Toolbox
```

---

## Anatomie d'un Plugin

### Le Header (.h)

```cpp
class MonPluginTool : public Core::ToolBase {
public:
    // Métadonnées (obligatoires)
    std::string GetID() const override { return "mon_plugin"; }
    std::string GetName() const override { return "Mon Plugin"; }
    std::string GetCategory() const override { return "Outils"; }
    std::string GetDescription() const override { return "Description"; }

    // Lifecycle (obligatoires)
    void OnCreate() override;   // Appelé à la création
    void OnDestroy() override;  // Appelé à la destruction
    void OnSave(sqlite3* db) override;   // Sauvegarder l'état
    void OnLoad(sqlite3* db) override;   // Charger l'état
    void Render() override;     // Rendu ImGui (chaque frame)

    // Events (optionnel)
    void OnEvent(const Core::Event& event) override;

private:
    // Vos variables d'état
    std::string myData;
    int myCounter = 0;
};
```

### Le Manifest (.manifest.json)

```json
{
  "id": "mon_plugin",
  "name": "Mon Plugin",
  "description": "Description de mon plugin",
  "version": "1.0.0",
  "author": "Votre Nom",

  "menu": {
    "category": "Outils",
    "order": 10,
    "icon": "🔧",
    "shortcuts": ["Ctrl+M"]
  },

  "requires": {
    "database": true,      // Besoin de PostgreSQL ?
    "event_bus": true,     // Besoin de l'EventBus ?
    "logger": true         // Besoin du Logger ?
  },

  "permissions": {
    "read_database": true,     // Lire depuis PostgreSQL
    "write_campaign": true,    // Écrire dans le .dnd
    "publish_events": true,    // Publier des événements
    "subscribe_events": true   // S'abonner à des événements
  },

  "events": {
    "publishes": ["mon_plugin.action_done"],
    "subscribes": ["dice.rolled"]
  },

  "ui": {
    "window_title": "Mon Plugin",
    "default_width": 600,
    "default_height": 400,
    "resizable": true,
    "can_dock": true
  }
}
```

---

## Ressources Disponibles dans un Plugin

Chaque plugin a accès à ces ressources (injectées automatiquement) :

### 1. EventBus (`eventBus`)

Communication pub/sub entre plugins.

```cpp
// S'abonner à un événement (dans OnCreate)
eventBus->Subscribe("dice.rolled", [this](const Core::Event& e) {
    int result = e.data["result"];
    // Faire quelque chose
});

// Publier un événement
PublishEvent("mon_plugin.action_done", {
    {"data", "valeur"}
});
```

### 2. Logger (`logger`)

Système de log global.

```cpp
// Logger une action
Log("action_name", {
    {"key", "value"},
    {"number", 42}
});

// Enregistrer un formateur custom pour les logs
logger->RegisterFormatter("action_name", [](const Core::LogEntry& entry) {
    return "Mon log: " + entry.data["key"].get<std::string>();
});
```

### 3. DatabaseManager (`dbManager`)

Accès read-only à PostgreSQL (règles D&D).

```cpp
// Récupérer toutes les races
auto races = dbManager->GetRaces();

// Récupérer toutes les classes
auto classes = dbManager->GetClasses();

// Récupérer les traits raciaux
auto traits = dbManager->GetRacialTraits("elf");
```

### 4. Instance ID (`instanceID`)

UUID unique de cette instance du plugin.

```cpp
std::string id = GetInstanceID();
// → "instance_42"
```

### 5. SQLite (fichier .dnd)

Dans `OnSave()` et `OnLoad()`, vous avez accès à la base SQLite du fichier de campagne.

```cpp
void OnSave(sqlite3* db) {
    nlohmann::json state;
    state["my_data"] = myData;

    const char* sql = "INSERT OR REPLACE INTO tool_instances (id, tool_type, data) VALUES (?, ?, ?)";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, instanceID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, GetID().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, state.dump().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}
```

---

## Interface ImGui

Le rendu se fait avec **Dear ImGui**. Voici les widgets les plus utiles :

```cpp
void Render() {
    if (!isOpen) return;

    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);

    if (ImGui::Begin(GetWindowTitle().c_str(), &isOpen)) {
        // Texte
        ImGui::Text("Hello World");
        ImGui::TextWrapped("Texte long qui wrap automatiquement...");

        // Bouton
        if (ImGui::Button("Cliquez-moi")) {
            // Action
        }

        // Input texte
        char buffer[256] = "";
        ImGui::InputText("Label", buffer, sizeof(buffer));

        // Checkbox
        bool checked = false;
        ImGui::Checkbox("Option", &checked);

        // Combo box
        const char* items[] = { "Option 1", "Option 2" };
        int current = 0;
        ImGui::Combo("Choix", &current, items, 2);

        // Zone scrollable
        if (ImGui::BeginChild("ScrollZone", ImVec2(0, 200))) {
            for (int i = 0; i < 100; i++) {
                ImGui::Text("Ligne %d", i);
            }
        }
        ImGui::EndChild();

        // Tableau
        if (ImGui::BeginTable("MyTable", 3, ImGuiTableFlags_Borders)) {
            ImGui::TableSetupColumn("Col 1");
            ImGui::TableSetupColumn("Col 2");
            ImGui::TableSetupColumn("Col 3");
            ImGui::TableHeadersRow();

            for (int i = 0; i < 10; i++) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Cellule %d", i);
                ImGui::TableNextColumn();
                ImGui::Text("Data");
                ImGui::TableNextColumn();
                if (ImGui::Button("Action")) {
                    // ...
                }
            }

            ImGui::EndTable();
        }
    }

    ImGui::End();
}
```

Documentation complète : https://github.com/ocornut/imgui

---

## Bonnes Pratiques

### 1. Gestion de l'état

- Toutes les variables d'état doivent être **membres de la classe**
- Sauvegarder TOUT l'état dans `OnSave()` (sinon perdu à la réouverture)
- Charger l'état dans `OnLoad()`

### 2. Communication entre plugins

- **NE PAS** appeler directement les méthodes d'un autre plugin
- **TOUJOURS** utiliser l'EventBus pour communiquer
- Déclarer les événements dans le manifest

### 3. Logs

- Logger les actions importantes (pas les actions UI mineures)
- Les logs racontent "l'histoire" de la campagne
- Exemple : Logger un jet de dé, pas un clic sur un bouton

### 4. Performance

- `Render()` est appelé **chaque frame** (60 FPS)
- Éviter les calculs lourds dans `Render()`
- Faire les calculs dans `Update()` ou lors d'actions utilisateur

### 5. Nommage

- **ID du plugin** : snake_case (ex: `character_sheet`)
- **Classe** : PascalCase + "Tool" (ex: `CharacterSheetTool`)
- **Fichiers** : Nom de la classe (ex: `CharacterSheetTool.h`)
- **Manifest** : `{id}.manifest.json` (ex: `character_sheet.manifest.json`)

---

## Exemples

Consultez les plugins existants pour des exemples complets :

- **`plugins_src/dice_roller/`** : Plugin simple avec input, historique, events
- (À venir : character_sheet, notes, combat_tracker, etc.)

---

## Debugging

### Logs de chargement

Au démarrage, l'app affiche :
```
ToolRegistry: Loading plugin from plugins/libmon_plugin.so
ToolRegistry: ✅ Loaded plugin 'Mon Plugin' (ID: mon_plugin)
```

Si le plugin ne se charge pas :
```
ToolRegistry: Error reading manifest for plugins/libmon_plugin.so
```
→ Vérifier que le manifest est bien nommé et au bon endroit

### Erreurs de compilation

Si le plugin ne compile pas, vérifier :
- Les includes sont corrects
- Le manifest est valide (JSON bien formaté)
- La classe hérite bien de `Core::ToolBase`
- Les méthodes virtuelles sont toutes implémentées
- Les symboles `CreateTool` et `DestroyTool` sont exportés

### Le plugin ne s'affiche pas dans le menu

Vérifier :
- Le plugin est bien dans `build/plugins/`
- Le manifest est bien copié
- L'ID dans le .h correspond à l'ID dans le manifest
- La catégorie existe

---

## Aide

Si vous rencontrez des problèmes, consultez :
- Le code du plugin `dice_roller` (exemple de référence)
- La doc ImGui : https://github.com/ocornut/imgui
- Les headers dans `include/core/` (ToolBase, EventBus, Logger, etc.)

Bon développement ! 🚀
