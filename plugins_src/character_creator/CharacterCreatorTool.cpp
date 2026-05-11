#include "CharacterCreatorTool.h"
#include "../../include/core/Logger.h"
#include "../../include/core/EventBus.h"
#include <imgui.h>
#include <nlohmann/json.hpp>
#include <chrono>

CharacterCreatorTool::CharacterCreatorTool() {
    // Initialiser stepCompleted pour 8 étapes
    stepCompleted.resize(8, false);

    // Initialiser les buffers
    memset(firstName, 0, sizeof(firstName));
    memset(lastName, 0, sizeof(lastName));
}

CharacterCreatorTool::~CharacterCreatorTool() {
}

// ============================================================
// LIFECYCLE
// ============================================================

void CharacterCreatorTool::OnCreate() {
    // Charger les races depuis la base de données
    if (dbManager) {
        availableRaces = dbManager->LoadRaces();

        if (logger) {
            Log("character_creator_loaded", {
                {"races_count", availableRaces.size()}
            });
        }
    }
}

void CharacterCreatorTool::OnDestroy() {
    // Cleanup
}

void CharacterCreatorTool::OnSave(sqlite3* db) {
    if (!db) return;

    // Multi-instance: utiliser l'instanceID unique pour chaque création
    nlohmann::json state;
    state["current_step"] = static_cast<int>(currentStep);
    state["steps_completed"] = stepCompleted;

    // Données de l'étape 1
    if (selectedRaceIndex >= 0) {
        state["selected_race_index"] = selectedRaceIndex;
        state["selected_race_id"] = selectedRace.id;
    }
    state["selected_variant_index"] = selectedVariantIndex;
    state["first_name"] = std::string(firstName);
    state["last_name"] = std::string(lastName);
    state["age"] = age;

    // TODO: Sauvegarder le personnage complet (character)

    const char* sql = "INSERT OR REPLACE INTO tool_instances (id, tool_type, data) VALUES (?, ?, ?)";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, instanceID.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, GetID().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, state.dump().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
}

void CharacterCreatorTool::OnLoad(sqlite3* db) {
    if (!db) return;

    const char* sql = "SELECT data FROM tool_instances WHERE id = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, instanceID.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* jsonStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));

            if (jsonStr && strlen(jsonStr) > 0) {
                auto state = nlohmann::json::parse(jsonStr);

                if (state.contains("current_step")) {
                    currentStep = static_cast<CreationStep>(state["current_step"].get<int>());
                }

                if (state.contains("steps_completed")) {
                    stepCompleted = state["steps_completed"].get<std::vector<bool>>();
                }

                if (state.contains("selected_race_index")) {
                    selectedRaceIndex = state["selected_race_index"];
                    if (selectedRaceIndex >= 0 && selectedRaceIndex < availableRaces.size()) {
                        selectedRace = availableRaces[selectedRaceIndex];
                    }
                }

                if (state.contains("selected_variant_index")) {
                    selectedVariantIndex = state["selected_variant_index"];
                }

                if (state.contains("first_name")) {
                    strncpy(firstName, state["first_name"].get<std::string>().c_str(), sizeof(firstName) - 1);
                }

                if (state.contains("last_name")) {
                    strncpy(lastName, state["last_name"].get<std::string>().c_str(), sizeof(lastName) - 1);
                }

                if (state.contains("age")) {
                    age = state["age"];
                }
            }
        }

        sqlite3_finalize(stmt);
    }
}

// ============================================================
// RENDERING
// ============================================================

void CharacterCreatorTool::Render() {
    if (!isOpen) return;

    ImGui::SetNextWindowSize(ImVec2(900, 700), ImGuiCond_FirstUseEver);

    if (ImGui::Begin(GetWindowTitle().c_str(), &isOpen)) {
        // Tabs pour navigation
        RenderBreadcrumb();

        ImGui::Separator();

        // Contenu selon l'étape actuelle
        switch (currentStep) {
            case STEP_RACE_SELECTION:
                RenderRaceGrid();
                break;

            case STEP_RACE_DETAILS:
                RenderRaceDetails();
                break;

            case STEP_CLASS_SELECTION:
            case STEP_CLASS_DETAILS:
            case STEP_STATS:
            case STEP_BACKGROUND:
            case STEP_SPELLS:
            case STEP_SUMMARY:
                ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "TODO: Cette etape n'est pas encore implementee");
                break;
        }

        // Auto-advance: vérifier à la fin du rendu si l'étape est complète
        bool currentStepValid = ValidateCurrentStep();
        if (currentStepValid && currentStep < STEP_SUMMARY && !stepCompleted[static_cast<int>(currentStep)]) {
            // Marquer l'étape comme complétée
            stepCompleted[static_cast<int>(currentStep)] = true;
        }
    }

    ImGui::End();
}

void CharacterCreatorTool::RenderBreadcrumb() {
    const char* stepNames[] = {
        "Race",
        "Race Details",
        "Classe",
        "Classe Details",
        "Caracteristiques",
        "Histoire",
        "Sorts",
        "Resume"
    };

    if (ImGui::BeginTabBar("CharacterCreationTabs", ImGuiTabBarFlags_None)) {
        for (int i = 0; i < 8; i++) {
            // Déterminer si l'onglet est accessible
            bool canAccess = false;
            if (i == 0) {
                // Premier onglet toujours accessible
                canAccess = true;
            } else if (i < stepCompleted.size() && stepCompleted[i - 1]) {
                // Onglet accessible si l'étape précédente est complétée
                canAccess = true;
            } else if (i < stepCompleted.size() && stepCompleted[i]) {
                // Onglet accessible si déjà complété (retour en arrière)
                canAccess = true;
            }

            // Ne créer l'onglet que s'il est accessible
            if (!canAccess) {
                continue;  // Sauter les onglets non accessibles
            }

            // Appliquer couleur pour les étapes complétées
            int colorCount = 0;
            if (i < stepCompleted.size() && stepCompleted[i]) {
                // Étape complétée (gris clair)
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.70f, 0.70f, 0.70f, 1.0f));
                colorCount = 1;
            }

            // Flags pour l'onglet
            ImGuiTabItemFlags flags = ImGuiTabItemFlags_None;
            if (i == static_cast<int>(currentStep)) {
                flags |= ImGuiTabItemFlags_SetSelected;
            }

            // Créer l'onglet
            if (ImGui::BeginTabItem(stepNames[i], nullptr, flags)) {
                ImGui::EndTabItem();
            }

            // Détecter si l'utilisateur a cliqué sur cet onglet (via ImGui::IsItemClicked)
            // On ne change currentStep que si c'est un clic utilisateur, pas juste l'onglet actif
            if (ImGui::IsItemActivated() && static_cast<int>(currentStep) != i) {
                currentStep = static_cast<CreationStep>(i);
            }

            // Restaurer les couleurs
            if (colorCount > 0) {
                ImGui::PopStyleColor(colorCount);
            }
        }
        ImGui::EndTabBar();
    }
}

void CharacterCreatorTool::RenderRaceGrid() {
    ImGui::TextWrapped("Choisissez votre race");
    ImGui::Spacing();

    if (availableRaces.empty()) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
            "Aucune race disponible. Verifiez la connexion a la base de donnees.");
        return;
    }

    // Grille 3 colonnes
    int columns = 3;
    float tileWidth = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x * (columns - 1)) / columns;

    for (size_t i = 0; i < availableRaces.size(); i++) {
        const auto& race = availableRaces[i];

        ImGui::BeginChild(("race_tile_" + std::to_string(i)).c_str(),
                         ImVec2(tileWidth, 120),
                         true);

        // Nom de la race (centré et en gras)
        ImGui::PushFont(ImGui::GetFont());  // TODO: utiliser une police plus grande
        ImGui::Text("%s", race.name_fr.c_str());
        ImGui::PopFont();

        ImGui::Separator();

        // Description courte (wrapped)
        ImGui::PushTextWrapPos();
        ImGui::TextWrapped("%s", race.description.c_str());
        ImGui::PopTextWrapPos();

        ImGui::Spacing();

        // Bouton de sélection
        if (ImGui::Button(("Choisir##" + std::to_string(i)).c_str(), ImVec2(-1, 0))) {
            selectedRaceIndex = i;
            selectedRace = race;
            selectedVariantIndex = -1;  // Reset variante
            currentStep = STEP_RACE_DETAILS;
        }

        ImGui::EndChild();

        // Nouvelle ligne tous les 3 éléments
        if ((i + 1) % columns != 0 && i + 1 < availableRaces.size()) {
            ImGui::SameLine();
        }
    }
}

void CharacterCreatorTool::RenderRaceDetails() {
    if (selectedRaceIndex < 0) {
        ImGui::Text("Aucune race selectionnee");
        return;
    }

    ImGui::Text("Race: %s", selectedRace.name_fr.c_str());
    ImGui::Separator();

    // Description longue (scrollable)
    ImGui::BeginChild("RaceDescription", ImVec2(0, 300), true);
    ImGui::PushTextWrapPos();
    ImGui::TextWrapped("%s", selectedRace.description.c_str());
    ImGui::PopTextWrapPos();
    ImGui::EndChild();

    ImGui::Spacing();

    // Bonus de caractéristiques
    ImGui::Text("Bonus de Caracteristiques:");
    if (selectedRace.str_bonus > 0) ImGui::BulletText("Force: +%d", selectedRace.str_bonus);
    if (selectedRace.dex_bonus > 0) ImGui::BulletText("Dexterite: +%d", selectedRace.dex_bonus);
    if (selectedRace.con_bonus > 0) ImGui::BulletText("Constitution: +%d", selectedRace.con_bonus);
    if (selectedRace.int_bonus > 0) ImGui::BulletText("Intelligence: +%d", selectedRace.int_bonus);
    if (selectedRace.wis_bonus > 0) ImGui::BulletText("Sagesse: +%d", selectedRace.wis_bonus);
    if (selectedRace.cha_bonus > 0) ImGui::BulletText("Charisme: +%d", selectedRace.cha_bonus);

    ImGui::Spacing();

    // Autres infos
    ImGui::Text("Vitesse: %d ft", selectedRace.base_speed);
    ImGui::Text("Taille: %s", selectedRace.size.c_str());

    ImGui::Spacing();

    // Langues
    if (!selectedRace.languages.empty()) {
        ImGui::Text("Langues:");
        for (const auto& lang : selectedRace.languages) {
            ImGui::BulletText("%s", lang.c_str());
        }
        ImGui::Spacing();
    }

    // TODO: Variantes raciales (boutons radio)
    // TODO: Traits raciaux

    ImGui::Separator();

    // Informations personnelles
    ImGui::Text("Informations Personnelles:");
    ImGui::InputText("Prenom", firstName, sizeof(firstName));
    ImGui::InputText("Nom", lastName, sizeof(lastName));
    ImGui::InputInt("Age", &age);
}


bool CharacterCreatorTool::ValidateCurrentStep() {
    switch (currentStep) {
        case STEP_RACE_SELECTION:
            // Validé seulement si une race a été sélectionnée
            return selectedRaceIndex >= 0;

        case STEP_RACE_DETAILS:
            // Nom/Prénom remplis et variante choisie (TODO: variante)
            return strlen(firstName) > 0 && strlen(lastName) > 0 && age > 0;

        case STEP_CLASS_SELECTION:
        case STEP_CLASS_DETAILS:
        case STEP_STATS:
        case STEP_BACKGROUND:
        case STEP_SPELLS:
        case STEP_SUMMARY:
            // TODO: Validation pour les autres étapes
            return false;

        default:
            return false;
    }
}

// ============================================================
// EVENTS
// ============================================================

void CharacterCreatorTool::OnEvent(const Core::Event& event) {
    // TODO: Gérer les événements si nécessaire
}

// ============================================================
// HELPERS
// ============================================================

double CharacterCreatorTool::GetCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration<double>(duration).count();
}

// ============================================================
// EXPORT DU PLUGIN (C ABI) - NE PAS MODIFIER
// ============================================================

extern "C" {
    PLUGIN_API Core::ToolBase* CreateTool() {
        return new CharacterCreatorTool();
    }

    PLUGIN_API void DestroyTool(Core::ToolBase* tool) {
        delete tool;
    }
}
