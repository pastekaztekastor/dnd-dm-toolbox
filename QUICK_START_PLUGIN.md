# Quick Start : Créer un Plugin en 3 Minutes

## 🚀 Méthode Rapide

### 1. Lancer le script de génération

```bash
./create_plugin.sh
```

### 2. Répondre aux questions

```
ID du plugin (snake_case): my_tool
Nom de la classe (PascalCase): MyTool
Nom affiché: My Tool
Description courte: Mon premier plugin
Catégorie (1-4): 4
Icône (emoji): 🔧
```

### 3. Le script crée automatiquement

```
plugins_src/my_tool/
├── MyToolTool.h
├── MyToolTool.cpp
└── my_tool.manifest.json
```

Et ajoute `add_plugin(my_tool)` au `CMakeLists.txt` !

### 4. Compiler

```bash
cmake --build build
```

### 5. Tester

```bash
./build/DnD_DM_Toolbox
```

Votre plugin apparaît dans **Outils** > **Outils** > **🔧 My Tool** !

---

## 📝 Personnaliser le Plugin

### Ajouter des variables d'état

Dans `MyToolTool.h`, section `private:` :

```cpp
private:
    std::string myText = "Hello";
    int myCounter = 0;
    std::vector<std::string> myList;
```

### Implémenter l'interface

Dans `MyToolTool.cpp`, méthode `Render()` :

```cpp
void MyToolTool::Render() {
    if (!isOpen) return;

    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);

    if (ImGui::Begin(GetWindowTitle().c_str(), &isOpen)) {
        ImGui::Text("Mon Plugin!");

        // Input texte
        char buffer[256];
        strncpy(buffer, myText.c_str(), sizeof(buffer));
        if (ImGui::InputText("Texte", buffer, sizeof(buffer))) {
            myText = buffer;
        }

        // Compteur
        if (ImGui::Button("Incrementer")) {
            myCounter++;

            // Logger l'action
            Log("increment", {{"value", myCounter}});
        }
        ImGui::SameLine();
        ImGui::Text("Compteur: %d", myCounter);

        // Liste
        if (ImGui::Button("Ajouter à la liste")) {
            myList.push_back("Item " + std::to_string(myList.size() + 1));
        }

        if (ImGui::BeginChild("List")) {
            for (size_t i = 0; i < myList.size(); i++) {
                ImGui::Text("%s", myList[i].c_str());
            }
        }
        ImGui::EndChild();
    }

    ImGui::End();
}
```

### Sauvegarder/Charger l'état

Dans `OnSave()` :

```cpp
nlohmann::json state;
state["text"] = myText;
state["counter"] = myCounter;
state["list"] = myList;

// Le reste est déjà implémenté dans le template
```

Dans `OnLoad()` :

```cpp
if (state.contains("text")) myText = state["text"];
if (state.contains("counter")) myCounter = state["counter"];
if (state.contains("list")) myList = state["list"].get<std::vector<std::string>>();
```

### Recompiler et tester

```bash
cmake --build build
./build/DnD_DM_Toolbox
```

---

## 📚 Pour aller plus loin

Consultez **PLUGIN_DEVELOPMENT.md** pour :
- Communication entre plugins (EventBus)
- Accès à la base PostgreSQL
- Widgets ImGui avancés
- Bonnes pratiques
- Exemples complets

---

## 🎯 Exemples de Plugins à Créer

Voici des idées de plugins simples pour commencer :

### 1. Note Tool (📝)
- Input texte multiline
- Liste de notes
- Sauvegarde automatique

### 2. Timer Tool (⏱️)
- Countdown timer
- Notification quand terminé
- Événement publié

### 3. Music Player Tool (🎵)
- Liste de lecture
- Contrôles play/pause
- Volume

### 4. Weather Tool (🌦️)
- État météo de la campagne
- Effets sur le gameplay
- Logs des changements

Amusez-vous bien ! 🚀
