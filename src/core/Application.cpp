#include "../../include/core/Application.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <nfd.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <filesystem>

namespace Core {

Application::Application() {
}

Application::~Application() {
    Shutdown();
}

bool Application::Init() {
    std::cout << "=== D&D DM Toolbox - Initialisation ===" << std::endl;

    // Initialiser NFD (file dialogs)
    if (NFD_Init() != NFD_OKAY) {
        std::cerr << "Erreur: Échec de l'initialisation de NFD: " << NFD_GetError() << std::endl;
        return false;
    }
    std::cout << "File dialogs initialisés" << std::endl;

    // Initialiser la fenêtre
    if (!InitWindow()) {
        std::cerr << "Erreur: Échec de l'initialisation de la fenêtre" << std::endl;
        return false;
    }

    // Initialiser ImGui
    if (!InitImGui()) {
        std::cerr << "Erreur: Échec de l'initialisation d'ImGui" << std::endl;
        return false;
    }

    // Initialiser les systèmes core
    eventBus = std::make_unique<EventBus>();
    logger = std::make_unique<Logger>();
    saveFileManager = std::make_unique<SaveFileManager>();
    toolRegistry = std::make_unique<ToolRegistry>();

    // Injecter les dépendances dans le registry
    toolRegistry->SetEventBus(eventBus.get());
    toolRegistry->SetLogger(logger.get());

    // Initialiser la base de données
    if (!InitDatabase()) {
        std::cerr << "Attention: Base de données non disponible" << std::endl;
        // Non-bloquant, on continue sans DB
    }

    // Charger les plugins
    if (!LoadPlugins()) {
        std::cerr << "Attention: Aucun plugin chargé" << std::endl;
    }

    // Charger les fichiers récents
    LoadRecentFiles();

    initialized = true;
    std::cout << "Application initialisée avec succès" << std::endl;

    return true;
}

void Application::Run() {
    if (!initialized) {
        std::cerr << "Erreur: Application non initialisée" << std::endl;
        return;
    }

    std::cout << "=== Démarrage de la boucle principale ===" << std::endl;

    double lastFrameTime = GetCurrentTime();

    while (!ShouldClose()) {
        // Calculer delta time
        double currentTime = GetCurrentTime();
        double deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        // Traiter les événements GLFW
        ProcessEvents();

        // Mettre à jour la logique
        Update(deltaTime);

        // Rendu
        Render();
    }
}

void Application::Shutdown() {
    std::cout << "=== Arrêt de l'application ===" << std::endl;

    // Sauvegarder les fichiers récents
    SaveRecentFiles();

    // Cleanup NFD
    NFD_Quit();

    // Fermer tous les tools
    for (auto* tool : toolInstances) {
        toolRegistry->DestroyToolInstance(tool);
    }
    toolInstances.clear();

    // Fermer le fichier de campagne
    saveFileManager->Close();

    // Décharger les plugins
    toolRegistry->UnloadAllPlugins();

    // Déconnecter la DB
    if (dbManager) {
        dbManager->Disconnect();
    }

    // Cleanup ImGui
    if (window) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    // Cleanup GLFW
    if (window) {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    initialized = false;
    std::cout << "Application arrêtée proprement" << std::endl;
}

bool Application::ShouldClose() const {
    return window && glfwWindowShouldClose(window);
}

// ============================================================
// INITIALISATION
// ============================================================

bool Application::InitWindow() {
    if (!glfwInit()) {
        std::cerr << "Erreur: Échec de l'initialisation de GLFW" << std::endl;
        return false;
    }

    // Configuration OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Créer la fenêtre
    window = glfwCreateWindow(1600, 900, "D&D DM Toolbox", nullptr, nullptr);
    if (!window) {
        std::cerr << "Erreur: Échec de la création de la fenêtre GLFW" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // VSync

    std::cout << "Fenêtre créée (1600x900)" << std::endl;
    return true;
}

bool Application::InitImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // Configuration
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Charger la police Nimbus Sans (équivalent libre d'Arial)
    const char* fontPaths[] = {
        "/usr/share/fonts/gsfonts/NimbusSans-Regular.otf",
        "/usr/share/fonts/dejavu/DejaVuSans.ttf"
    };

    for (const char* path : fontPaths) {
        if (std::filesystem::exists(path)) {
            io.Fonts->AddFontFromFileTTF(path, 13.0f);
            std::cout << "Police chargée depuis: " << path << std::endl;
            break;
        }
    }

    // Style - Thème Dark en nuances de gris (COMPLET)
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // Texte
    colors[ImGuiCol_Text]                   = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.35f, 0.35f, 0.35f, 0.35f);

    // Couleurs de fond
    colors[ImGuiCol_WindowBg]               = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.10f, 0.10f, 0.10f, 0.98f);
    colors[ImGuiCol_Border]                 = ImVec4(0.30f, 0.30f, 0.30f, 0.50f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

    // Frames (input boxes, etc)
    colors[ImGuiCol_FrameBg]                = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);

    // Titres
    colors[ImGuiCol_TitleBg]                = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.10f, 0.10f, 0.10f, 0.75f);

    // Menu Bar
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);

    // Scrollbar
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);

    // Checkboxes
    colors[ImGuiCol_CheckMark]              = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);

    // Sliders
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);

    // Boutons
    colors[ImGuiCol_Button]                 = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);

    // Headers (onglets, menus, collapsing headers)
    colors[ImGuiCol_Header]                 = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);

    // Séparateurs
    colors[ImGuiCol_Separator]              = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);

    // Resize Grip
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.25f, 0.25f, 0.25f, 0.50f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.35f, 0.35f, 0.35f, 0.67f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.45f, 0.45f, 0.45f, 0.95f);

    // Tabs
    colors[ImGuiCol_Tab]                    = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    colors[ImGuiCol_TabActive]              = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
    colors[ImGuiCol_TabUnfocused]           = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);

    // Docking
    colors[ImGuiCol_DockingPreview]         = ImVec4(0.40f, 0.40f, 0.40f, 0.70f);
    colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);

    // Plot (graphiques)
    colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);

    // Tables
    colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
    colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_TableBorderLight]       = ImVec4(0.23f, 0.23f, 0.23f, 1.00f);
    colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);

    // Drag and Drop
    colors[ImGuiCol_DragDropTarget]         = ImVec4(0.60f, 0.60f, 0.60f, 0.90f);

    // Navigation
    colors[ImGuiCol_NavHighlight]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);

    // Modal
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    std::cout << "ImGui initialisé" << std::endl;
    return true;
}

bool Application::InitDatabase() {
    dbManager = std::make_unique<Database::DatabaseManager>();

    bool connected = dbManager->Connect("dnd_toolbox.db");

    if (connected) {
        toolRegistry->SetDatabaseManager(dbManager.get());
        std::cout << "Base de données SQLite connectée" << std::endl;
        return true;
    } else {
        std::cerr << "Base de données non disponible (mode hors-ligne)" << std::endl;
        return false;
    }
}

bool Application::LoadPlugins() {
    // Essayer d'abord "plugins" (si lancé depuis build/)
    std::string pluginsDir = "plugins";

    // Sinon essayer "build/plugins" (si lancé depuis la racine)
    if (!std::filesystem::exists(pluginsDir)) {
        pluginsDir = "build/plugins";
    }

    // Vérifier que le dossier existe
    if (!std::filesystem::exists(pluginsDir)) {
        std::cerr << "Dossier plugins introuvable" << std::endl;
        return false;
    }

    int count = toolRegistry->LoadAllPlugins(pluginsDir);

    if (count > 0) {
        std::cout << "" << count << " plugin(s) chargé(s)" << std::endl;
        return true;
    } else {
        std::cout << "Aucun plugin trouvé dans " << pluginsDir << std::endl;
        return false;
    }
}

void Application::LoadRecentFiles() {
    std::string configPath = "recent_files.txt";

    if (!std::filesystem::exists(configPath)) {
        return;
    }

    std::ifstream file(configPath);
    std::string line;

    while (std::getline(file, line) && recentFiles.size() < MAX_RECENT_FILES) {
        if (!line.empty() && std::filesystem::exists(line)) {
            recentFiles.push_back(line);
        }
    }

    std::cout << "" << recentFiles.size() << " fichier(s) récent(s) chargé(s)" << std::endl;
}

void Application::SaveRecentFiles() {
    std::ofstream file("recent_files.txt");

    for (const auto& filepath : recentFiles) {
        file << filepath << "\n";
    }
}

// ============================================================
// BOUCLE PRINCIPALE
// ============================================================

void Application::ProcessEvents() {
    glfwPollEvents();
}

void Application::Update(double deltaTime) {
    // Traiter les événements du bus
    eventBus->ProcessEvents();

    // Vérifier l'auto-save
    CheckAutoSave();
}

void Application::Render() {
    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Dockspace (fenêtre principale)
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("DockSpace", nullptr, window_flags);
    ImGui::PopStyleVar(3);

    // DockSpace
    ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

    // Menu bar
    RenderMenuBar();

    // Menu contextuel (clic droit sur la zone du dockspace)
    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        ImGui::OpenPopup("BackgroundContextMenu");
    }

    if (ImGui::BeginPopup("BackgroundContextMenu")) {
        ImGui::SeparatorText("Ouvrir un outil");

        auto pluginsByCategory = toolRegistry->GetPluginsByCategory();
        for (const auto& [category, manifests] : pluginsByCategory) {
            ImGui::SeparatorText(category.c_str());
            for (const auto* manifest : manifests) {
                if (ImGui::MenuItem(manifest->name.c_str())) {
                    OpenTool(manifest->id);
                }
            }
        }

        ImGui::EndPopup();
    }

    ImGui::End();

    // Écran d'accueil
    if (showWelcomeScreen) {
        RenderWelcomeScreen();
    }

    // Tools ouverts
    RenderTools();

    // ImGui Demo (debug)
    if (showDemoWindow) {
        ImGui::ShowDemoWindow(&showDemoWindow);
    }

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}

void Application::CheckAutoSave() {
    if (!saveFileManager->IsOpen()) {
        return;
    }

    double currentTime = GetCurrentTime();

    if (currentTime - lastAutoSaveTime >= AUTO_SAVE_INTERVAL) {
        std::cout << "Auto-save..." << std::endl;
        saveFileManager->AutoSave(toolInstances, logger.get());
        lastAutoSaveTime = currentTime;
    }
}

// ============================================================
// UI - MENU BAR
// ============================================================

void Application::RenderMenuBar() {
    if (ImGui::BeginMenuBar()) {
        // Menu Fichier
        if (ImGui::BeginMenu("Fichier")) {
            if (ImGui::MenuItem("Nouvelle Campagne", "Ctrl+N")) {
                NewCampaign();
            }
            if (ImGui::MenuItem("Ouvrir Campagne", "Ctrl+O")) {
                OpenCampaign();
            }
            if (ImGui::BeginMenu("Récement ouvert", "Ctrl+Maj+O")) {
                if (recentFiles.empty()) {
                    ImGui::TextDisabled("Aucun fichier récent");
                } else {
                    for (const auto& filepath : recentFiles) {
                        std::string filename = std::filesystem::path(filepath).filename().string();

                        if (ImGui::MenuItem(filename.c_str())) {
                            OpenCampaignFile(filepath);
                            showWelcomeScreen = false;
                        }
                    }
                }
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Importer", "Ctrl+I")) {
                ImportCampaign();
            }
            if (ImGui::MenuItem("Sauvegarder", "Ctrl+S", false, saveFileManager->IsOpen())) {
                SaveCampaign();
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Quitter", "Alt+F4")) {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }

            ImGui::EndMenu();
        }

        // Menu Outils (généré dynamiquement depuis les plugins)
        if (ImGui::BeginMenu("Outils")) {
            auto pluginsByCategory = toolRegistry->GetPluginsByCategory();

            for (const auto& [category, manifests] : pluginsByCategory) {
                ImGui::SeparatorText(category.c_str());
                for (const auto* manifest : manifests) {
                    if (ImGui::MenuItem(manifest->name.c_str())) {
                        OpenTool(manifest->id);
                    }
                }
            }

            ImGui::EndMenu();
        }

        // Menu Affichage
        if (ImGui::BeginMenu("Affichage")) {
            ImGui::MenuItem("ImGui Demo", nullptr, &showDemoWindow);

            ImGui::EndMenu();
        }

        // Menu Aide
        if (ImGui::BeginMenu("Aide")) {
            if (ImGui::MenuItem("À propos")) {
                // TODO: Fenêtre À propos
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}

// ============================================================
// UI - WELCOME SCREEN
// ============================================================

void Application::RenderWelcomeScreen() {
    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f),
                           ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));

    if (ImGui::Begin("Bienvenue dans D&D DM Toolbox", &showWelcomeScreen)) {
        ImGui::TextWrapped("Gestionnaire de campagne D&D 5e avec système de plugins");
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Boutons d'action
        if (ImGui::Button("Nouvelle Campagne")) {
            NewCampaign();
            showWelcomeScreen = false;
        }

        ImGui::SameLine();

        if (ImGui::Button("Ouvrir Campagne")) {
            OpenCampaign();
        }

        ImGui::SameLine();

        if (ImGui::Button("Importer")) {
            ImportCampaign();
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Fichiers récents
        ImGui::Text("Fichiers récents:");

        if (recentFiles.empty()) {
            ImGui::TextDisabled("Aucun fichier récent");
        } else {
            for (const auto& filepath : recentFiles) {
                std::string filename = std::filesystem::path(filepath).filename().string();

                if (ImGui::Selectable(filename.c_str())) {
                    OpenCampaignFile(filepath);
                    showWelcomeScreen = false;
                }

                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("%s", filepath.c_str());
                }
            }
        }
    }
    ImGui::End();
}

// ============================================================
// UI - TOOLS
// ============================================================

void Application::RenderTools() {
    // Render tous les tools ouverts
    for (auto* tool : toolInstances) {
        if (tool && tool->IsOpen()) {
            tool->Render();
        }
    }

    // Fermer les tools fermés
    toolInstances.erase(
        std::remove_if(toolInstances.begin(), toolInstances.end(),
                      [this](ToolBase* tool) {
                          if (!tool->IsOpen()) {
                              // Sauvegarder l'état du tool avant de le détruire
                              if (saveFileManager->IsOpen()) {
                                  tool->OnSave(saveFileManager->GetDatabase());
                              }
                              toolRegistry->DestroyToolInstance(tool);
                              return true;
                          }
                          return false;
                      }),
        toolInstances.end()
    );
}

// ============================================================
// ACTIONS
// ============================================================

void Application::NewCampaign() {
    // Ouvrir un dialog "Save As" natif
    nfdchar_t* outPath = nullptr;
    nfdfilteritem_t filters[1] = { { "D&D Campaign", "dnd" } };
    nfdresult_t result = NFD_SaveDialog(&outPath, filters, 1, nullptr, "nouvelle_campagne.dnd");

    if (result == NFD_OKAY) {
        std::string filepath(outPath);
        NFD_FreePath(outPath);

        // S'assurer que l'extension est .dnd
        if (!filepath.ends_with(".dnd")) {
            filepath += ".dnd";
        }

        // Extraire le nom de la campagne depuis le nom de fichier
        std::filesystem::path path(filepath);
        std::string name = path.stem().string();

        if (saveFileManager->New(filepath, name)) {
            AddToRecentFiles(filepath);
            showWelcomeScreen = false;
            std::cout << "Nouvelle campagne créée: " << filepath << std::endl;
        }
    } else if (result == NFD_CANCEL) {
        std::cout << "Création de campagne annulée" << std::endl;
    } else {
        std::cerr << "Erreur lors du dialog: " << NFD_GetError() << std::endl;
    }
}

void Application::OpenCampaign() {
    // Ouvrir un dialog "Open" natif
    nfdchar_t* outPath = nullptr;
    nfdfilteritem_t filters[1] = { { "D&D Campaign", "dnd" } };
    nfdresult_t result = NFD_OpenDialog(&outPath, filters, 1, nullptr);

    if (result == NFD_OKAY) {
        std::string filepath(outPath);
        NFD_FreePath(outPath);

        OpenCampaignFile(filepath);
        showWelcomeScreen = false;
    } else if (result == NFD_CANCEL) {
        std::cout << "Ouverture de campagne annulée" << std::endl;
    } else {
        std::cerr << "Erreur lors du dialog: " << NFD_GetError() << std::endl;
    }
}

void Application::OpenCampaignFile(const std::string& filepath) {
    if (saveFileManager->Open(filepath)) {
        // Charger les instances de tools
        auto instances = saveFileManager->LoadToolInstances();

        for (const auto& [instanceID, data] : instances) {
            const auto& [toolType, json] = data;

            ToolBase* tool = toolRegistry->CreateToolInstance(toolType);
            if (tool) {
                tool->SetInstanceID(instanceID);
                tool->OnLoad(saveFileManager->GetDatabase());
                toolInstances.push_back(tool);
            }
        }

        // Charger les logs
        logger->LoadFromDatabase(saveFileManager->GetDatabase());

        AddToRecentFiles(filepath);
        std::cout << "Campagne ouverte: " << filepath << std::endl;
    }
}

void Application::ImportCampaign() {
    // Ouvrir un dialog "Open" pour sélectionner le fichier à importer
    nfdchar_t* outPath = nullptr;
    nfdfilteritem_t filters[1] = { { "D&D Campaign", "dnd" } };
    nfdresult_t result = NFD_OpenDialog(&outPath, filters, 1, nullptr);

    if (result == NFD_OKAY) {
        std::string filepath(outPath);
        NFD_FreePath(outPath);

        // TODO: Implémenter la logique d'import (merge avec la campagne actuelle)
        std::cout << "Import depuis: " << filepath << std::endl;
        std::cout << "Import non encore implémenté (merge des données)" << std::endl;
    } else if (result == NFD_CANCEL) {
        std::cout << "Import annulé" << std::endl;
    } else {
        std::cerr << "Erreur lors du dialog: " << NFD_GetError() << std::endl;
    }
}

void Application::SaveCampaign() {
    if (saveFileManager->IsOpen()) {
        saveFileManager->Save(toolInstances, logger.get());
        std::cout << "Campagne sauvegardée" << std::endl;
    }
}

void Application::CloseCampaign() {
    // Fermer tous les tools
    for (auto* tool : toolInstances) {
        toolRegistry->DestroyToolInstance(tool);
    }
    toolInstances.clear();

    // Fermer le fichier
    saveFileManager->Close();

    // Afficher l'écran d'accueil
    showWelcomeScreen = true;

    std::cout << "Campagne fermée" << std::endl;
}

void Application::OpenTool(const std::string& toolType) {
    // Créer l'instance
    ToolBase* tool = toolRegistry->CreateToolInstance(toolType);

    if (tool) {
        // Charger les données sauvegardées si une campagne est ouverte
        if (saveFileManager->IsOpen()) {
            tool->OnLoad(saveFileManager->GetDatabase());
        }

        toolInstances.push_back(tool);
        std::cout << "Tool ouvert: " << toolType << std::endl;
    }
}

void Application::CloseTool(ToolBase* tool) {
    if (!tool) return;

    auto it = std::find(toolInstances.begin(), toolInstances.end(), tool);
    if (it != toolInstances.end()) {
        toolRegistry->DestroyToolInstance(*it);
        toolInstances.erase(it);
    }
}

void Application::AddToRecentFiles(const std::string& filepath) {
    // Retirer si déjà présent
    recentFiles.erase(
        std::remove(recentFiles.begin(), recentFiles.end(), filepath),
        recentFiles.end()
    );

    // Ajouter en tête
    recentFiles.insert(recentFiles.begin(), filepath);

    // Limiter à MAX_RECENT_FILES
    if (recentFiles.size() > MAX_RECENT_FILES) {
        recentFiles.resize(MAX_RECENT_FILES);
    }
}

// ============================================================
// HELPERS
// ============================================================

double Application::GetCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration<double>(duration).count();
}

} // namespace Core
