#include "CharacterCreatorTool.h"
#include "../../include/core/Logger.h"
#include "../../include/core/EventBus.h"
#include <imgui.h>
#include <nlohmann/json.hpp>
#include <chrono>
#include <cstdint>

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
        for (const auto& race : availableRaces)
            allSubRaces[race.id] = dbManager->LoadSubRaces(race.id);

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
            case STEP_RACE_DETAILS:
                RenderRaceGrid();
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
        "Classe",
        "Classe Details",
        "Caracteristiques",
        "Histoire",
        "Sorts",
        "Resume",
        ""
    };

    if (ImGui::BeginTabBar("CharacterCreationTabs", ImGuiTabBarFlags_None)) {
        for (int i = 0; i < 8; i++) {
            // Skip STEP_RACE_DETAILS (index 1)
            if (i == 1) continue;

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
    if (availableRaces.empty()) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
            "Aucune race disponible. Verifiez la connexion a la base de donnees.");
        return;
    }

    // Layout: panneau gauche (tree) + panneau droit (détails)
    // Panel gauche plus étroit : 25% de l'espace
    float leftWidth = ImGui::GetContentRegionAvail().x * 0.25f;
    ImGui::Columns(2, "race_layout", true);
    ImGui::SetColumnWidth(0, leftWidth);

    // === PANNEAU GAUCHE: Hiérarchie des races ===

    ImGui::BeginChild("RacesList", ImVec2(0, 0), true);

    for (size_t i = 0; i < availableRaces.size(); ++i) {
        const auto& race = availableRaces[i];
        const auto& children = allSubRaces[race.id];
        bool hasChildren = !children.empty();
        bool isSelected = (selectedRaceIndex == (int)i && selectedSubRaceIndex < 0);

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DrawLinesFull;
        if (!hasChildren) {
            flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
            if (isSelected) flags |= ImGuiTreeNodeFlags_Selected;
        }

        bool isOpen = ImGui::TreeNodeEx((void*)(intptr_t)i, flags, "%s", race.nom.c_str());

        if (ImGui::IsItemClicked()) {
            if (!hasChildren) {
                selectedRaceIndex = i;
                selectedRace = race;
                selectedSubRaceIndex = -1;
            }
        }

        if (isOpen && hasChildren) {
            for (size_t j = 0; j < children.size(); ++j) {
                const auto& subRace = children[j];
                bool subIsSelected = (selectedRaceIndex == (int)i && selectedSubRaceIndex == (int)j);

                ImGuiTreeNodeFlags subFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanAvailWidth;
                if (subIsSelected) subFlags |= ImGuiTreeNodeFlags_Selected;

                ImGui::TreeNodeEx((void*)(intptr_t)(1000 + j), subFlags, "%s", subRace.nom.c_str());

                if (ImGui::IsItemClicked()) {
                    selectedRaceIndex = i;
                    selectedRace = race;
                    selectedSubRaceIndex = j;
                    selectedSubRace = subRace;
                }
            }
            ImGui::TreePop();
        }
    }

    ImGui::EndChild();
    ImGui::NextColumn();
    
    // === PANNEAU DROIT: Détails de la race ===
    if (selectedRaceIndex >= 0) {
        Database::RaceData mergedRace;
        if (selectedSubRaceIndex >= 0) {
            mergedRace = selectedSubRace;
            // Cumul description
            if (!selectedRace.description.empty() && !mergedRace.description.empty())
                mergedRace.description = selectedRace.description + "\n\n" + mergedRace.description;
            else if (!selectedRace.description.empty())
                mergedRace.description = selectedRace.description;
            // Cumul bonus
            mergedRace.bonus_forces       += selectedRace.bonus_forces;
            mergedRace.bonus_dexterite    += selectedRace.bonus_dexterite;
            mergedRace.bonus_constitution += selectedRace.bonus_constitution;
            mergedRace.bonus_intelligence += selectedRace.bonus_intelligence;
            mergedRace.bonus_sagesse      += selectedRace.bonus_sagesse;
            mergedRace.bonus_charisme     += selectedRace.bonus_charisme;
            // Vitesse: garde celle de la sous-race si définie, sinon parent
            if (mergedRace.vitesse_base == 0)
                mergedRace.vitesse_base = selectedRace.vitesse_base;
            // Cumul langues
            if (!selectedRace.liste_langues.empty() && !mergedRace.liste_langues.empty())
                mergedRace.liste_langues = selectedRace.liste_langues + ", " + mergedRace.liste_langues;
            else if (!selectedRace.liste_langues.empty())
                mergedRace.liste_langues = selectedRace.liste_langues;
            // Cumul traits : si même titre, concatène le texte ; sinon insère le trait parent en tête
            for (const auto& parentTrait : selectedRace.race_traits) {
                auto it = std::find_if(mergedRace.race_traits.begin(), mergedRace.race_traits.end(),
                    [&](const Database::RaceTrait& t) { return t.titre == parentTrait.titre; });
                if (it != mergedRace.race_traits.end())
                    it->texte = parentTrait.texte + "\n- " + it->texte;
                else
                    mergedRace.race_traits.insert(mergedRace.race_traits.begin(), parentTrait);
            }
            mergedRace.presentations.insert(mergedRace.presentations.begin(), selectedRace.presentations.begin(), selectedRace.presentations.end());
            mergedRace.noms.insert(mergedRace.noms.begin(), selectedRace.noms.begin(), selectedRace.noms.end());
        } else {
            mergedRace = selectedRace;
        }
        const auto& displayRace = mergedRace;

        ImGui::BeginChild("RaceDetails", ImVec2(0, 0), true);
        // Tableau des bonus de caractéristiques
        ImGui::Text("Bonus de Caracteristiques:");
        ImGui::Spacing();
        if (ImGui::BeginTable("BonusTable", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchSame)) {
            const char* headers[] = { "FOR", "DEX", "CON", "INT", "SAG", "CHA" };
            for (const char* h : headers)
                ImGui::TableSetupColumn(h);
            ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
            for (const char* h : headers) {
                ImGui::TableNextColumn();
                float colWidth = ImGui::GetColumnWidth();
                float textWidth = ImGui::CalcTextSize(h).x;
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (colWidth - textWidth) * 0.5f);
                ImGui::TextUnformatted(h);
            }

            ImGui::TableNextRow();
            int bonuses[] = {
                displayRace.bonus_forces,
                displayRace.bonus_dexterite,
                displayRace.bonus_constitution,
                displayRace.bonus_intelligence,
                displayRace.bonus_sagesse,
                displayRace.bonus_charisme
            };
            for (int b : bonuses) {
                ImGui::TableNextColumn();
                char buf[8] = "";
                if (b > 0)      snprintf(buf, sizeof(buf), "+%d", b);
                else if (b < 0) snprintf(buf, sizeof(buf), "%d", b);
                float colWidth = ImGui::GetColumnWidth();
                float textWidth = ImGui::CalcTextSize(buf).x;
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (colWidth - textWidth) * 0.5f);
                ImGui::TextUnformatted(buf);
            }

            ImGui::EndTable();
        }

        ImGui::Spacing();
        ImGui::Text("Vitesse: %d pieds", displayRace.vitesse_base);
        if (!displayRace.liste_langues.empty())
            ImGui::Text("Langues: %s", displayRace.liste_langues.c_str());

        
        
        // Presentations
        if (!displayRace.presentations.empty()) {
            ImGui::TextWrapped("Details de la Race");
            for (const auto& pres : displayRace.presentations) {
                if (ImGui::CollapsingHeader(pres.titre.c_str())) {
                    ImGui::PushTextWrapPos();
                    ImGui::TextWrapped("%s", pres.texte.c_str());
                    ImGui::PopTextWrapPos();
                }
            }
            ImGui::Spacing();
            ImGui::Separator();
        }

        // Traits raciaux
        if (!displayRace.race_traits.empty()) {
            ImGui::TextWrapped("Traits Raciaux");
            for (const auto& trait : displayRace.race_traits) {
                if (ImGui::CollapsingHeader(trait.titre.c_str())) {
                    ImGui::PushTextWrapPos();
                    ImGui::TextWrapped("%s", trait.texte.c_str());
                    ImGui::PopTextWrapPos();
                }
            }
            ImGui::Spacing();
            ImGui::Separator();
        }

        // Prénoms et noms
        if (!displayRace.noms.empty()) {
            ImGui::TextWrapped("Exemples de Noms");
            for (const auto& nom : displayRace.noms) {
                if (ImGui::CollapsingHeader(nom.titre.c_str())) {
                    ImGui::PushTextWrapPos();
                    ImGui::TextWrapped("%s", nom.texte.c_str());
                    ImGui::PopTextWrapPos();
                }
            }
            ImGui::Spacing();
            ImGui::Separator();
        }

        // Infos personnelles
        ImGui::Text("Informations Personnelles:");
        ImGui::InputText("Prenom", firstName, sizeof(firstName));
        ImGui::InputText("Nom", lastName, sizeof(lastName));
        ImGui::InputInt("Age", &age);
        ImGui::EndChild();
    } else {
        ImGui::BeginChild("RaceDetails", ImVec2(0, 0), true);
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Selectionnez une race pour voir les details");
        ImGui::EndChild();
    }

    ImGui::Columns(1);
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
