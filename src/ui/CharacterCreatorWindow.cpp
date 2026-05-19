#include "../../include/ui/CharacterCreatorWindow.h"
#include "../../include/ui/DiceRollerWindow.h"
#include "../../include/ui/ContextualHelpWindow.h"
#include <imgui.h>
#include <algorithm>

namespace UI {

void CharacterCreatorWindow::Render() {
    if (!isOpen) return;

    ImGui::SetNextWindowSize(ImVec2(1000, 700), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Créateur de Personnage", &isOpen)) {
        // Step indicator
        const char* steps[] = {"Infos de Base", "Race & Classe", "Caractéristiques", "Compétences", "Récapitulatif"};
        ImGui::Text("Étape %d sur 5 : %s", creationStep + 1, steps[creationStep]);
        ImGui::Separator();

        // Render current step
        switch (creationStep) {
            case 0: RenderBasicInfo(); break;
            case 1: RenderRaceClass(); break;
            case 2: RenderAbilities(); break;
            case 3: RenderSkills(); break;
            case 4: RenderReview(); break;
        }

        ImGui::Separator();

        // Navigation buttons
        if (creationStep > 0) {
            if (ImGui::Button("< Précédent")) {
                creationStep--;
            }
            ImGui::SameLine();
        }

        if (creationStep < 4) {
            if (ImGui::Button("Suivant >")) {
                creationStep++;
            }
        } else {
            if (ImGui::Button("Terminer & Créer le Personnage")) {
                // TODO: Save character
                Hide();
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Annuler")) {
            Hide();
        }
    }
    ImGui::End();
}

void CharacterCreatorWindow::RenderBasicInfo() {
    ImGui::Text("Informations de Base du Personnage");
    ImGui::Spacing();

    ImGui::InputText("Nom", nameBuffer, sizeof(nameBuffer));
    if (strlen(nameBuffer) > 0) {
        character->name = nameBuffer;
    }

    ImGui::InputText("Historique", backgroundBuffer, sizeof(backgroundBuffer));
    if (strlen(backgroundBuffer) > 0) {
        character->background = backgroundBuffer;
    }

    ImGui::Spacing();
    ImGui::Text("Niveau de Départ");
    int level = character->level;
    if (ImGui::SliderInt("##Level", &level, 1, 20)) {
        character->level = level;
    }

    ImGui::Spacing();
    ImGui::Checkbox("Est un PNJ ?", &character->isNPC);
}

void CharacterCreatorWindow::RenderRaceClass() {
    ImGui::Text("Sélection de Race et Classe");
    ImGui::Spacing();

    // Race selection
    ImGui::Text("Race :");
    ImGui::SameLine();
    if (helpWindow) {
        helpWindow->ShowHelpMarker("race_general");
    }

    // Utiliser les données de la DB si disponibles, sinon fallback sur enums
    if (!dbRaces.empty()) {
        // VERSION DB: Affichage dynamique depuis PostgreSQL
        std::string racePreview = selectedRace < static_cast<int>(dbRaces.size())
            ? FormatRaceLabel(dbRaces[selectedRace])
            : "Sélectionner une race";

        if (ImGui::BeginCombo("##RaceCombo", racePreview.c_str())) {
            for (size_t i = 0; i < dbRaces.size(); ++i) {
                const bool isSelected = (selectedRace == static_cast<int>(i));
                std::string label = FormatRaceLabel(dbRaces[i]);

                if (ImGui::Selectable(label.c_str(), isSelected)) {
                    selectedRace = i;
                    // TODO: Stocker l'ID de la race dans character
                    character->speed = dbRaces[i].vitesse_base;
                }

                // Afficher la description au survol
                if (ImGui::IsItemHovered() && !dbRaces[i].description.empty()) {
                    ImGui::SetTooltip("%s", dbRaces[i].description.c_str());
                }

                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        // Afficher les détails de la race sélectionnée
        if (selectedRace >= 0 && selectedRace < static_cast<int>(dbRaces.size())) {
            ImGui::Spacing();
            const auto& race = dbRaces[selectedRace];
            ImGui::TextWrapped("Vitesse : %d pieds", race.vitesse_base);
        }
    } else {
        // FALLBACK: Version hardcodée si DB non disponible
        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "⚠ Base de données non connectée");
        auto races = DnD::CharacterHelper::GetAllRaces();
        if (selectedRace >= 0 && selectedRace < static_cast<int>(races.size())) {
            ImGui::Text("Race : %s", DnD::CharacterHelper::RaceToString(races[selectedRace]).c_str());
        }
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Class selection
    ImGui::Text("Classe :");
    ImGui::SameLine();
    if (helpWindow) {
        helpWindow->ShowHelpMarker("class_general");
    }

    // Utiliser les données de la DB si disponibles
    if (!dbClasses.empty()) {
        // VERSION DB: Affichage dynamique depuis PostgreSQL
        std::string classPreview = selectedClass < static_cast<int>(dbClasses.size())
            ? FormatClassLabel(dbClasses[selectedClass])
            : "Sélectionner une classe";

        if (ImGui::BeginCombo("##ClassCombo", classPreview.c_str())) {
            for (size_t i = 0; i < dbClasses.size(); ++i) {
                const bool isSelected = (selectedClass == static_cast<int>(i));
                std::string label = FormatClassLabel(dbClasses[i]);

                if (ImGui::Selectable(label.c_str(), isSelected)) {
                    selectedClass = i;
                    // TODO: Stocker l'ID de la classe dans character
                    // TODO: Charger les saving throw proficiencies depuis la DB
                }

                // Afficher la description au survol
                if (ImGui::IsItemHovered() && !dbClasses[i].description.empty()) {
                    ImGui::SetTooltip("%s", dbClasses[i].description.c_str());
                }

                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        // Afficher des infos sur la classe sélectionnée
        if (selectedClass >= 0 && selectedClass < static_cast<int>(dbClasses.size())) {
            ImGui::Spacing();
            const auto& cls = dbClasses[selectedClass];
            ImGui::TextWrapped("Dé de vie : d%d", cls.dee_de_vie);

            if (!cls.caracteristiques_de_sorts.empty()) {
                ImGui::TextWrapped("Caractéristique d'incantation : %s", cls.caracteristiques_de_sorts.c_str());
            }

            if (!cls.jets_de_sauvegarde.empty()) {
                std::string saves;
                for (size_t k = 0; k < cls.jets_de_sauvegarde.size(); ++k) {
                    if (k > 0) saves += ", ";
                    saves += cls.jets_de_sauvegarde[k];
                }
                ImGui::TextWrapped("Jets de sauvegarde : %s", saves.c_str());
            }
        }
    } else {
        // FALLBACK: Version hardcodée si DB non disponible
        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "⚠ Base de données non connectée");
        auto classes = DnD::CharacterHelper::GetAllClasses();
        if (selectedClass >= 0 && selectedClass < static_cast<int>(classes.size())) {
            ImGui::Text("Classe : %s", DnD::CharacterHelper::ClassToString(classes[selectedClass]).c_str());
        }
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Alignment
    ImGui::Text("Alignement :");
    ImGui::SameLine();
    if (helpWindow) {
        helpWindow->ShowHelpMarker("alignment_general");
    }

    const char* alignments[] = {
        "Loyal Bon", "Neutre Bon", "Chaotique Bon",
        "Loyal Neutre", "Neutre Strict", "Chaotique Neutre",
        "Loyal Mauvais", "Neutre Mauvais", "Chaotique Mauvais"
    };

    if (ImGui::BeginCombo("##AlignmentCombo", alignments[selectedAlignment])) {
        for (int i = 0; i < 9; ++i) {
            const bool isSelected = (selectedAlignment == i);
            if (ImGui::Selectable(alignments[i], isSelected)) {
                selectedAlignment = i;
                character->alignment = static_cast<DnD::Alignment>(i);
            }

            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    ImGui::Spacing();
    ImGui::TextWrapped("L'alignement représente les valeurs morales et éthiques de votre personnage.");
}

void CharacterCreatorWindow::RenderAbilities() {
    ImGui::Text("Valeurs de Caractéristiques");
    ImGui::Spacing();

    // Method selection
    if (ImGui::RadioButton("Achat de Points (27 points)", (int*)&abilityMethod, 0)) {
        abilityMethod = AbilityMethod::PointBuy;
    }
    if (helpWindow && ImGui::IsItemHovered()) {
        helpWindow->SetHelp("method_pointbuy");
    }

    ImGui::SameLine();
    if (ImGui::RadioButton("Tableau Standard", (int*)&abilityMethod, 1)) {
        abilityMethod = AbilityMethod::StandardArray;
        // Set standard array: 15, 14, 13, 12, 10, 8
        character->abilityScores.strength = 15;
        character->abilityScores.dexterity = 14;
        character->abilityScores.constitution = 13;
        character->abilityScores.intelligence = 12;
        character->abilityScores.wisdom = 10;
        character->abilityScores.charisma = 8;
    }
    if (helpWindow && ImGui::IsItemHovered()) {
        helpWindow->SetHelp("method_standard");
    }

    ImGui::SameLine();
    if (ImGui::RadioButton("Lancer de Dés (4d6 sans le plus bas)", (int*)&abilityMethod, 2)) {
        abilityMethod = AbilityMethod::Roll;
    }
    if (helpWindow && ImGui::IsItemHovered()) {
        helpWindow->SetHelp("method_roll");
    }

    // Button to roll ability scores
    if (abilityMethod == AbilityMethod::Roll) {
        if (ImGui::Button("🎲 Lancer les 6 Caractéristiques")) {
            rolledScores = DnD::Dice::RollAbilityScoreSet();
            // Auto-assign to abilities in order
            if (rolledScores.size() >= 6) {
                character->abilityScores.strength = rolledScores[0];
                character->abilityScores.dexterity = rolledScores[1];
                character->abilityScores.constitution = rolledScores[2];
                character->abilityScores.intelligence = rolledScores[3];
                character->abilityScores.wisdom = rolledScores[4];
                character->abilityScores.charisma = rolledScores[5];
            }
        }
        if (diceRoller) {
            ImGui::SameLine();
            if (ImGui::SmallButton("Ouvrir le Lanceur")) {
                diceRoller->Show();
            }
        }
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (abilityMethod == AbilityMethod::PointBuy) {
        int pointsUsed = CalculatePointBuyTotal();
        ImGui::Text("Points Utilisés : %d / 27", pointsUsed);
        if (pointsUsed > 27) {
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "Vous avez dépassé votre budget de points !");
        }
        ImGui::Spacing();
    }

    // Ability score sliders/editors
    auto renderAbility = [this](const char* label, DnD::Ability ability, const std::string& helpKey) {
        int score = character->abilityScores.Get(ability);
        int modifier = character->abilityScores.GetModifier(ability);

        ImGui::Text("%s:", label);
        if (helpWindow && ImGui::IsItemHovered()) {
            helpWindow->SetHelp(helpKey);
        }
        ImGui::SameLine(140);

        ImGui::SetNextItemWidth(200);
        if (abilityMethod == AbilityMethod::PointBuy) {
            if (ImGui::SliderInt(("##" + std::string(label)).c_str(), &score, 8, 15)) {
                character->abilityScores.Set(ability, score);
            }
        } else {
            if (ImGui::InputInt(("##" + std::string(label)).c_str(), &score)) {
                character->abilityScores.Set(ability, score);
            }
        }

        ImGui::SameLine();
        ImGui::Text("Mod: %+d", modifier);
    };

    renderAbility("Force", DnD::Ability::Strength, "ability_strength");
    renderAbility("Dextérité", DnD::Ability::Dexterity, "ability_dexterity");
    renderAbility("Constitution", DnD::Ability::Constitution, "ability_constitution");
    renderAbility("Intelligence", DnD::Ability::Intelligence, "ability_intelligence");
    renderAbility("Sagesse", DnD::Ability::Wisdom, "ability_wisdom");
    renderAbility("Charisme", DnD::Ability::Charisma, "ability_charisma");

    if (!rolledScores.empty() && abilityMethod == AbilityMethod::Roll) {
        ImGui::Spacing();
        ImGui::Text("Résultats des dés :");
        for (int score : rolledScores) {
            ImGui::SameLine();
            ImGui::Text("%d", score);
        }
    }
}

void CharacterCreatorWindow::RenderSkills() {
    ImGui::Text("Maîtrises de Compétences");
    if (helpWindow) {
        ContextualHelpWindow::ShowTooltip("Les compétences représentent ce en quoi votre personnage excelle.");
    }
    ImGui::Spacing();

    // Get available skills for the current class
    auto availableSkills = DnD::CharacterHelper::GetAvailableSkills(character->characterClass);
    int maxSkills = DnD::CharacterHelper::GetSkillChoices(character->characterClass);

    // Count currently selected skills
    int selectedCount = 0;
    for (const auto& [skill, prof] : character->skillProficiencies) {
        if (prof != DnD::ProficiencyLevel::None) {
            selectedCount++;
        }
    }

    // Display skill counter
    ImGui::TextWrapped("Sélectionnez vos maîtrises de compétences.");
    if (helpWindow && ImGui::IsItemHovered()) {
        helpWindow->SetHelp("skill_general");
    }
    ImGui::Spacing();
    if (selectedCount > maxSkills) {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Compétences : %d / %d (Trop de compétences !)", selectedCount, maxSkills);
    } else if (selectedCount == maxSkills) {
        ImGui::TextColored(ImVec4(0, 1, 0, 1), "Compétences : %d / %d", selectedCount, maxSkills);
    } else {
        ImGui::Text("Compétences : %d / %d", selectedCount, maxSkills);
    }
    ImGui::Spacing();

    // Skill help keys mapping
    static const std::map<DnD::Skill, std::string> skillHelpKeys = {
        {DnD::Skill::Perception, "skill_perception"},
        {DnD::Skill::Stealth, "skill_stealth"},
        {DnD::Skill::Athletics, "skill_athletics"},
        {DnD::Skill::Acrobatics, "skill_acrobatics"},
        {DnD::Skill::Investigation, "skill_investigation"},
        {DnD::Skill::Insight, "skill_insight"},
        {DnD::Skill::Persuasion, "skill_persuasion"},
        {DnD::Skill::Deception, "skill_deception"},
        {DnD::Skill::Intimidation, "skill_intimidation"}
    };

    // Render all skills
    auto skills = DnD::SkillHelper::GetAllSkills();
    for (auto skill : skills) {
        DnD::Ability ability = DnD::SkillHelper::GetAbility(skill);
        std::string skillName = DnD::SkillHelper::ToString(skill);
        std::string abilityName = DnD::AbilityHelper::ToShortString(ability);

        // Check if this skill is available for the current class
        bool isAvailable = std::find(availableSkills.begin(), availableSkills.end(), skill) != availableSkills.end();

        bool isProficient = character->skillProficiencies[skill] == DnD::ProficiencyLevel::Proficient;
        bool isExpert = character->skillProficiencies[skill] == DnD::ProficiencyLevel::Expert;

        // Disable checkbox if skill is not available or if max skills reached (and this skill is not already selected)
        bool disabled = !isAvailable || (selectedCount >= maxSkills && !isProficient && !isExpert);

        if (disabled) {
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        }

        if (ImGui::Checkbox((skillName + " (" + abilityName + ")").c_str(), &isProficient)) {
            if (isProficient) {
                character->skillProficiencies[skill] = DnD::ProficiencyLevel::Proficient;
            } else {
                character->skillProficiencies[skill] = DnD::ProficiencyLevel::None;
            }
        }

        // Show help for important skills
        if (helpWindow && ImGui::IsItemHovered() && skillHelpKeys.count(skill) > 0) {
            helpWindow->SetHelp(skillHelpKeys.at(skill));
        }

        if (disabled) {
            ImGui::PopItemFlag();
            ImGui::PopStyleVar();
        }

        // Expert checkbox (for Rogues, Bards, etc.)
        if (character->characterClass == DnD::CharacterClass::Rogue && character->level >= 1) {
            ImGui::SameLine();

            bool expertDisabled = !isProficient && !isExpert;
            if (expertDisabled) {
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
                ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            }

            if (ImGui::Checkbox(("Expert##" + skillName).c_str(), &isExpert)) {
                if (isExpert) {
                    character->skillProficiencies[skill] = DnD::ProficiencyLevel::Expert;
                } else {
                    character->skillProficiencies[skill] = isProficient ?
                        DnD::ProficiencyLevel::Proficient : DnD::ProficiencyLevel::None;
                }
            }

            if (expertDisabled) {
                ImGui::PopItemFlag();
                ImGui::PopStyleVar();
            }
        }

        // Show calculated bonus - use flexible positioning
        ImGui::SameLine();
        float cursorX = ImGui::GetCursorPosX();
        float availWidth = ImGui::GetContentRegionAvail().x;
        if (availWidth > 50) {
            ImGui::SetCursorPosX(cursorX + availWidth - 50);
        }
        int bonus = character->GetSkillModifier(skill);
        ImGui::Text("Bonus: %+d", bonus);
    }
}

void CharacterCreatorWindow::RenderReview() {
    ImGui::Text("Character Summary");
    ImGui::Spacing();

    // Basic info
    ImGui::Text("Name: %s", character->name.c_str());
    ImGui::Text("Race: %s", DnD::CharacterHelper::RaceToString(character->race).c_str());
    ImGui::Text("Class: %s", DnD::CharacterHelper::ClassToString(character->characterClass).c_str());
    ImGui::Text("Level: %d", character->level);
    ImGui::Text("Background: %s", character->background.c_str());
    ImGui::Text("Alignment: %s", DnD::CharacterHelper::AlignmentToString(character->alignment).c_str());

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Ability scores
    ImGui::Text("Ability Scores:");
    ImGui::Columns(3, "AbilityColumns");
    for (int i = 0; i < 6; ++i) {
        DnD::Ability ability = static_cast<DnD::Ability>(i);
        int score = character->abilityScores.Get(ability);
        int modifier = character->abilityScores.GetModifier(ability);
        ImGui::Text("%s: %d (%+d)",
            DnD::AbilityHelper::ToString(ability).c_str(),
            score,
            modifier);
        ImGui::NextColumn();
    }
    ImGui::Columns(1);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Calculate and set HP
    int hitDie = DnD::CharacterHelper::GetHitDie(character->characterClass);
    int conMod = character->abilityScores.GetModifier(DnD::Ability::Constitution);
    int calculatedHP = hitDie + conMod + (character->level - 1) * (hitDie / 2 + 1 + conMod);
    character->maxHitPoints = std::max(1, calculatedHP);
    character->currentHitPoints = character->maxHitPoints;

    // Combat stats
    ImGui::Text("Combat Stats:");
    ImGui::Text("Hit Points: %d", character->maxHitPoints);
    ImGui::Text("Armor Class: %d", character->armorClass);
    ImGui::Text("Initiative: %+d", character->GetInitiativeModifier());
    ImGui::Text("Speed: %d ft.", character->speed);
    ImGui::Text("Proficiency Bonus: +%d", character->GetProficiencyBonus());
    ImGui::Text("Passive Perception: %d", character->GetPassivePerception());

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Proficient skills
    ImGui::Text("Proficient Skills:");
    for (auto& [skill, proficiency] : character->skillProficiencies) {
        if (proficiency != DnD::ProficiencyLevel::None) {
            std::string profType = proficiency == DnD::ProficiencyLevel::Expert ? " (Expert)" : "";
            ImGui::BulletText("%s%s: %+d",
                DnD::SkillHelper::ToString(skill).c_str(),
                profType.c_str(),
                character->GetSkillModifier(skill));
        }
    }
}

} // namespace UI
