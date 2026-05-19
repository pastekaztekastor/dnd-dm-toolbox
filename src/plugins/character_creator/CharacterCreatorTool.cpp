#include "CharacterCreatorTool.h"
#include "../../include/core/Logger.h"
#include "../../include/core/EventBus.h"
#include <imgui.h>
#include <nlohmann/json.hpp>
#include <chrono>
#include <cstdint>

CharacterCreatorTool::CharacterCreatorTool() {
    stepCompleted.resize(8, false);
    memset(firstName, 0, sizeof(firstName));
    memset(lastName, 0, sizeof(lastName));
}

CharacterCreatorTool::~CharacterCreatorTool() {
}

// ============================================================
// LIFECYCLE
// ============================================================

void CharacterCreatorTool::OnCreate() {
    if (dbManager) {
        availableRaces = dbManager->Races()->LoadAll();
        for (const auto& race : availableRaces)
            allSubRaces[race.id] = dbManager->Races()->LoadSubRaces(race.id);

        availableClasses = dbManager->Classes()->LoadAll();
        for (const auto& classe : availableClasses)
            allSubClasses[classe.id] = dbManager->Classes()->LoadSubClasses(classe.id);

        if (logger) {
            Log("character_creator_loaded", {
                {"races_count",   availableRaces.size()},
                {"classes_count", availableClasses.size()}
            });
        }
    }
}

void CharacterCreatorTool::OnDestroy() {
}

void CharacterCreatorTool::OnSave(sqlite3* db) {
    if (!db) return;

    nlohmann::json state;
    state["current_step"] = static_cast<int>(currentStep);
    state["steps_completed"] = stepCompleted;

    if (selectedRaceIndex >= 0) {
        state["selected_race_index"] = selectedRaceIndex;
        state["selected_race_id"] = selectedRace.id;
    }
    state["selected_variant_index"] = selectedVariantIndex;
    state["first_name"] = std::string(firstName);
    state["last_name"] = std::string(lastName);
    state["age"] = age;

    if (selectedClassIndex >= 0) {
        state["selected_class_index"] = selectedClassIndex;
        state["selected_class_id"] = selectedClass.id;
        state["selected_sub_class_index"] = selectedSubClassIndex;
    }

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

                if (state.contains("current_step"))
                    currentStep = static_cast<CreationStep>(state["current_step"].get<int>());

                if (state.contains("steps_completed"))
                    stepCompleted = state["steps_completed"].get<std::vector<bool>>();

                if (state.contains("selected_race_index")) {
                    selectedRaceIndex = state["selected_race_index"];
                    if (selectedRaceIndex >= 0 && selectedRaceIndex < (int)availableRaces.size())
                        selectedRace = availableRaces[selectedRaceIndex];
                }

                if (state.contains("selected_variant_index"))
                    selectedVariantIndex = state["selected_variant_index"];

                if (state.contains("first_name"))
                    strncpy(firstName, state["first_name"].get<std::string>().c_str(), sizeof(firstName) - 1);

                if (state.contains("last_name"))
                    strncpy(lastName, state["last_name"].get<std::string>().c_str(), sizeof(lastName) - 1);

                if (state.contains("age"))
                    age = state["age"];

                if (state.contains("selected_class_index")) {
                    selectedClassIndex = state["selected_class_index"];
                    if (selectedClassIndex >= 0 && selectedClassIndex < (int)availableClasses.size())
                        selectedClass = availableClasses[selectedClassIndex];
                }

                if (state.contains("selected_sub_class_index"))
                    selectedSubClassIndex = state["selected_sub_class_index"];
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
        RenderBreadcrumb();
        ImGui::Separator();

        switch (currentStep) {
            case STEP_RACE_SELECTION:
            case STEP_RACE_DETAILS:
                RenderRaceOnglet();
                break;
            case STEP_CLASS_SELECTION:
                RenderClassOnglet();
                break;
            case STEP_BACKGROUND_SELECTION:
                RenderBackgroundOnglet();
                break;
            case STEP_STUFF_OPTIONS:
                RenderStuffOptionsOnglet();
                break;
            case STEP_PERSONALISATION_OPTIONS:
                RenderPersonalisationOptionsOnglet();
                break;
            case STEP_SPELLS_SELECTION:
                RenderSpellsSelectionOnglet();
                break;
            case STEP_SUMMARY:
                RenderSummaryOnglet();
                break;
        }

        if (ValidateCurrentStep() && currentStep < STEP_SUMMARY && !stepCompleted[static_cast<int>(currentStep)])
            stepCompleted[static_cast<int>(currentStep)] = true;
    }

    ImGui::End();
}

void CharacterCreatorTool::RenderBreadcrumb() {
    // Index 1 (STEP_RACE_DETAILS) est une étape interne, masquée
    const char* stepNames[] = {
        "Race",             // 0
        "",                 // 1 - masqué
        "Classe",           // 2
        "Historique",       // 3
        "Equipement",       // 4
        "Personnalisation", // 5
        "Sorts",            // 6
        "Resume"            // 7
    };

    if (ImGui::BeginTabBar("CharacterCreationTabs", ImGuiTabBarFlags_None)) {
        for (int i = 0; i < 8; i++) {
            if (i == 1) continue; // STEP_RACE_DETAILS masqué

            bool canAccess = (i == 0)
                || (i < (int)stepCompleted.size() && stepCompleted[i - 1])
                || (i < (int)stepCompleted.size() && stepCompleted[i]);
            if (!canAccess) continue;

            int colorCount = 0;
            if (i < (int)stepCompleted.size() && stepCompleted[i]) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.70f, 0.70f, 0.70f, 1.0f));
                colorCount = 1;
            }

            ImGuiTabItemFlags flags = ImGuiTabItemFlags_None;
            if (i == static_cast<int>(currentStep))
                flags |= ImGuiTabItemFlags_SetSelected;

            if (ImGui::BeginTabItem(stepNames[i], nullptr, flags))
                ImGui::EndTabItem();

            if (ImGui::IsItemActivated() && static_cast<int>(currentStep) != i)
                currentStep = static_cast<CreationStep>(i);

            if (colorCount > 0)
                ImGui::PopStyleColor(colorCount);
        }
        ImGui::EndTabBar();
    }
}

// ============================================================
// ÉTAPE RACE
// ============================================================

void CharacterCreatorTool::RenderRaceOnglet() {
    if (availableRaces.empty()) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
            "Aucune race disponible. Verifiez la connexion a la base de donnees.");
        return;
    }

    float leftWidth = ImGui::GetContentRegionAvail().x * 0.25f;
    ImGui::Columns(2, "race_layout", true);
    ImGui::SetColumnWidth(0, leftWidth);

    // === PANNEAU GAUCHE ===
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

        bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)i, flags, "%s", race.nom.c_str());

        if (ImGui::IsItemClicked()) {
            if (!hasChildren) {
                selectedRaceIndex = i;
                selectedRace = race;
                selectedSubRaceIndex = -1;
            }
        }

        if (nodeOpen && hasChildren) {
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

    // === PANNEAU DROIT ===
    if (selectedRaceIndex >= 0) {
        Database::RaceData mergedRace;
        if (selectedSubRaceIndex >= 0) {
            mergedRace = selectedSubRace;
            if (!selectedRace.description.empty() && !mergedRace.description.empty())
                mergedRace.description = selectedRace.description + "\n\n" + mergedRace.description;
            else if (!selectedRace.description.empty())
                mergedRace.description = selectedRace.description;
            mergedRace.bonus_forces       += selectedRace.bonus_forces;
            mergedRace.bonus_dexterite    += selectedRace.bonus_dexterite;
            mergedRace.bonus_constitution += selectedRace.bonus_constitution;
            mergedRace.bonus_intelligence += selectedRace.bonus_intelligence;
            mergedRace.bonus_sagesse      += selectedRace.bonus_sagesse;
            mergedRace.bonus_charisme     += selectedRace.bonus_charisme;
            if (mergedRace.vitesse_base == 0)
                mergedRace.vitesse_base = selectedRace.vitesse_base;
            if (!selectedRace.liste_langues.empty() && !mergedRace.liste_langues.empty())
                mergedRace.liste_langues = selectedRace.liste_langues + ", " + mergedRace.liste_langues;
            else if (!selectedRace.liste_langues.empty())
                mergedRace.liste_langues = selectedRace.liste_langues;
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

        ImGui::Text("Bonus de Caracteristiques:");
        ImGui::Spacing();
        if (ImGui::BeginTable("BonusTable", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchSame)) {
            const char* headers[] = { "FOR", "DEX", "CON", "INT", "SAG", "CHA" };
            for (const char* h : headers) ImGui::TableSetupColumn(h);
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
                displayRace.bonus_forces, displayRace.bonus_dexterite,
                displayRace.bonus_constitution, displayRace.bonus_intelligence,
                displayRace.bonus_sagesse, displayRace.bonus_charisme
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

        if (!displayRace.presentations.empty()) {
            ImGui::Spacing();
            ImGui::Separator();
            for (const auto& pres : displayRace.presentations) {
                if (ImGui::CollapsingHeader(pres.titre.c_str())) {
                    ImGui::PushTextWrapPos();
                    ImGui::TextWrapped("%s", pres.texte.c_str());
                    ImGui::PopTextWrapPos();
                }
            }
        }

        if (!displayRace.race_traits.empty()) {
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Text("Traits Raciaux");
            for (const auto& trait : displayRace.race_traits) {
                if (ImGui::CollapsingHeader(trait.titre.c_str())) {
                    ImGui::PushTextWrapPos();
                    ImGui::TextWrapped("%s", trait.texte.c_str());
                    ImGui::PopTextWrapPos();
                }
            }
        }

        if (!displayRace.noms.empty()) {
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Text("Exemples de Noms");
            for (const auto& nom : displayRace.noms) {
                if (ImGui::CollapsingHeader(nom.titre.c_str())) {
                    ImGui::PushTextWrapPos();
                    ImGui::TextWrapped("%s", nom.texte.c_str());
                    ImGui::PopTextWrapPos();
                }
            }
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Informations Personnelles:");
        ImGui::InputText("Prenom", firstName, sizeof(firstName));
        ImGui::InputText("Nom", lastName, sizeof(lastName));
        ImGui::InputInt("Age", &age);
        
        ImGui::Spacing();
        bool canAdvance = selectedRaceIndex >= 0 && strlen(firstName) > 0 && age > 0;
        if (!canAdvance) ImGui::BeginDisabled();
        if (ImGui::Button("Etape suivante : Classe >>", ImVec2(-1, 0))) {
            stepCompleted[static_cast<int>(STEP_RACE_SELECTION)] = true;
            stepCompleted[static_cast<int>(STEP_RACE_DETAILS)]   = true; // étape interne
            currentStep = STEP_CLASS_SELECTION;
        }
        if (!canAdvance) {
            ImGui::EndDisabled();
            ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f),
                "Selectionnez une race, entrez un prenom et un age pour continuer.");
        }

        ImGui::EndChild();
    } else {
        ImGui::BeginChild("RaceDetails", ImVec2(0, 0), true);
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Selectionnez une race pour voir les details");
        ImGui::EndChild();
    }

    ImGui::Columns(1);
}

// ============================================================
// ÉTAPE CLASSE
// ============================================================

void CharacterCreatorTool::RenderClassOnglet() {
    if (availableClasses.empty()) {
        ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Aucune classe disponible.");
        if (dbManager && !dbManager->IsConnected())
            ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f),
                "Base de donnees non connectee. (%s)", dbManager->GetLastError().c_str());
        else
            ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f),
                "Connecte mais aucune classe trouvee dans la table 'classes'.");
        return;
    }

    float leftWidth = ImGui::GetContentRegionAvail().x * 0.25f;
    ImGui::Columns(2, "class_layout", true);
    ImGui::SetColumnWidth(0, leftWidth);

    // === PANNEAU GAUCHE : liste des classes ===
    ImGui::BeginChild("ClassesList", ImVec2(0, 0), true);

    for (size_t i = 0; i < availableClasses.size(); ++i) {
        const auto& classe = availableClasses[i];
        auto it = allSubClasses.find(classe.id);
        const std::vector<Database::ClassData>& children =
            (it != allSubClasses.end()) ? it->second : std::vector<Database::ClassData>{};
        bool hasChildren = !children.empty();
        bool isSelected  = (selectedClassIndex == (int)i && selectedSubClassIndex < 0);

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DrawLinesFull;
        if (!hasChildren) {
            flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        }
        if (isSelected) flags |= ImGuiTreeNodeFlags_Selected;

        bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)i, flags, "%s", classe.nom.c_str());

        if (ImGui::IsItemClicked()) {
            selectedClassIndex    = i;
            selectedClass         = classe;
            selectedSubClassIndex = -1;
        }

        if (nodeOpen && hasChildren) {
            for (size_t j = 0; j < children.size(); ++j) {
                const auto& sub = children[j];
                bool subIsSelected = (selectedClassIndex == (int)i && selectedSubClassIndex == (int)j);

                ImGuiTreeNodeFlags subFlags = ImGuiTreeNodeFlags_Leaf
                    | ImGuiTreeNodeFlags_NoTreePushOnOpen
                    | ImGuiTreeNodeFlags_SpanAvailWidth;
                if (subIsSelected) subFlags |= ImGuiTreeNodeFlags_Selected;

                ImGui::TreeNodeEx((void*)(intptr_t)(1000 + j), subFlags, "%s", sub.nom.c_str());

                if (ImGui::IsItemClicked()) {
                    selectedClassIndex    = i;
                    selectedClass         = classe;
                    selectedSubClassIndex = j;
                    selectedSubClass      = sub;
                }
            }
            ImGui::TreePop();
        }
    }

    ImGui::EndChild();
    ImGui::NextColumn();

    // === PANNEAU DROIT : détails de la classe ===
    if (selectedClassIndex >= 0) {
        // La sous-classe affichée hérite des champs vides depuis la classe parente
        const Database::ClassData& sub    = selectedSubClass;
        const Database::ClassData& parent = selectedClass;
        const Database::ClassData& display =
            (selectedSubClassIndex >= 0) ? sub : parent;

        // Helpers pour fallback parent → sous-classe
        int dee = display.dee_de_vie > 0 ? display.dee_de_vie : parent.dee_de_vie;
        const std::string& castStat = !display.caracteristiques_de_sorts.empty()
            ? display.caracteristiques_de_sorts : parent.caracteristiques_de_sorts;

        ImGui::BeginChild("ClassDetails", ImVec2(0, 0), true);

        // ── Tableau de synthèse ──────────────────────────────────────────
        if (ImGui::BeginTable("ClassInfoTable", 3,
            ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchSame))
        {
            ImGui::TableSetupColumn("De de vie");
            ImGui::TableSetupColumn("Jets de sauvegarde");
            ImGui::TableSetupColumn("Carac. d'incantation");
            ImGui::TableHeadersRow();
            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::Text("d%d", dee);

            ImGui::TableNextColumn();
            const std::vector<std::string>& saves =
                !display.jets_de_sauvegarde.empty()
                ? display.jets_de_sauvegarde
                : parent.jets_de_sauvegarde;
            std::string savesStr;
            for (size_t k = 0; k < saves.size(); ++k) {
                if (k > 0) savesStr += ", ";
                savesStr += saves[k];
            }
            ImGui::TextUnformatted(savesStr.empty() ? "—" : savesStr.c_str());

            ImGui::TableNextColumn();
            ImGui::TextUnformatted(castStat.empty() ? "—" : castStat.c_str());

            ImGui::EndTable();
        }

        ImGui::Spacing();

        // ── Description ─────────────────────────────────────────────────
        if (!display.description.empty()) {
            ImGui::PushTextWrapPos();
            ImGui::TextWrapped("%s", display.description.c_str());
            ImGui::PopTextWrapPos();
            ImGui::Spacing();
            ImGui::Separator();
        }

        // ── Conseil joueur ───────────────────────────────────────────────
        const std::string& aide = !display.aide_joueur.empty()
            ? display.aide_joueur : parent.aide_joueur;
        if (!aide.empty()) {
            ImGui::TextColored(ImVec4(0.55f, 0.85f, 0.55f, 1.0f), "Conseil joueur");
            ImGui::PushTextWrapPos();
            ImGui::TextWrapped("%s", aide.c_str());
            ImGui::PopTextWrapPos();
            ImGui::Spacing();
            ImGui::Separator();
        }

        // ── Paragraphes de présentation ──────────────────────────────────
        if (!display.presentations.empty()) {
            for (const auto& pres : display.presentations) {
                if (ImGui::CollapsingHeader(pres.titre.c_str())) {
                    ImGui::PushTextWrapPos();
                    ImGui::TextWrapped("%s", pres.texte.c_str());
                    ImGui::PopTextWrapPos();
                }
            }
            ImGui::Spacing();
            ImGui::Separator();
        }

        // ── Aptitudes de niveau 1 ────────────────────────────────────────
        bool hasLvl1 = false;
        for (const auto& apt : display.aptitudes)
            if (apt.niveau_acquisition == 1) { hasLvl1 = true; break; }

        if (hasLvl1) {
            ImGui::Text("Aptitudes au niveau 1");
            ImGui::Spacing();
            for (const auto& apt : display.aptitudes) {
                if (apt.niveau_acquisition != 1) continue;
                ImGui::BulletText("%s", apt.nom.c_str());
                if (!apt.description_rapide.empty()) {
                    ImGui::SameLine();
                    ImGui::TextDisabled("  %s", apt.description_rapide.c_str());
                }
            }
            ImGui::Spacing();
            ImGui::Separator();
        }

        // ── Aptitudes des autres niveaux (collapsibles) ──────────────────
        bool hasOtherLevels = false;
        for (const auto& apt : display.aptitudes)
            if (apt.niveau_acquisition > 1) { hasOtherLevels = true; break; }

        if (hasOtherLevels && ImGui::CollapsingHeader("Aptitudes des niveaux superieurs")) {
            for (const auto& apt : display.aptitudes) {
                if (apt.niveau_acquisition <= 1) continue;
                ImGui::Text("[niv. %d]", apt.niveau_acquisition);
                ImGui::SameLine();
                ImGui::BulletText("%s", apt.nom.c_str());
                if (!apt.description_rapide.empty()) {
                    ImGui::SameLine();
                    ImGui::TextDisabled("  %s", apt.description_rapide.c_str());
                }
            }
        }

        ImGui::EndChild();
    } else {
        ImGui::BeginChild("ClassDetails", ImVec2(0, 0), true);
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f),
            "Selectionnez une classe pour voir les details");
        ImGui::EndChild();
    }

    ImGui::Columns(1);
}

// ============================================================
// ÉTAPES TODO
// ============================================================

void CharacterCreatorTool::RenderBackgroundOnglet() {
    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "TODO: Afficher la grille d'historiques");
}

void CharacterCreatorTool::RenderStuffOptionsOnglet() {
    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "TODO: Afficher les options d'equipement");
}

void CharacterCreatorTool::RenderPersonalisationOptionsOnglet() {
    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "TODO: Afficher les options de personnalisation");
}

void CharacterCreatorTool::RenderSpellsSelectionOnglet() {
    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "TODO: Afficher la selection de sorts");
}

void CharacterCreatorTool::RenderSummaryOnglet() {
    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "TODO: Afficher le resume du personnage");
}

// ============================================================
// EVENTS
// ============================================================

void CharacterCreatorTool::OnEvent(const Core::Event& event) {
}

// ============================================================
// HELPERS
// ============================================================

bool CharacterCreatorTool::ValidateCurrentStep() {
    switch (currentStep) {
        case STEP_RACE_SELECTION:
        case STEP_RACE_DETAILS:
            return selectedRaceIndex >= 0
                && strlen(firstName) > 0
                && age > 0;
        case STEP_CLASS_SELECTION:
            return selectedClassIndex >= 0;
        default:
            return false;
    }
}

double CharacterCreatorTool::GetCurrentTime() {
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration<double>(now.time_since_epoch()).count();
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
