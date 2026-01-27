#include "../../include/ui/CharacterCreatorWindow.h"
#include <imgui.h>
#include <algorithm>

namespace UI {

void CharacterCreatorWindow::Render() {
    if (!isOpen) return;

    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Character Creator", &isOpen)) {
        // Step indicator
        const char* steps[] = {"Basic Info", "Race & Class", "Abilities", "Skills", "Review"};
        ImGui::Text("Step %d of 5: %s", creationStep + 1, steps[creationStep]);
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
            if (ImGui::Button("< Previous")) {
                creationStep--;
            }
            ImGui::SameLine();
        }

        if (creationStep < 4) {
            if (ImGui::Button("Next >")) {
                creationStep++;
            }
        } else {
            if (ImGui::Button("Finish & Create Character")) {
                // TODO: Save character
                Hide();
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            Hide();
        }
    }
    ImGui::End();
}

void CharacterCreatorWindow::RenderBasicInfo() {
    ImGui::Text("Character Basic Information");
    ImGui::Spacing();

    ImGui::InputText("Name", nameBuffer, sizeof(nameBuffer));
    if (strlen(nameBuffer) > 0) {
        character->name = nameBuffer;
    }

    ImGui::InputText("Background", backgroundBuffer, sizeof(backgroundBuffer));
    if (strlen(backgroundBuffer) > 0) {
        character->background = backgroundBuffer;
    }

    ImGui::Spacing();
    ImGui::Text("Starting Level");
    int level = character->level;
    if (ImGui::SliderInt("##Level", &level, 1, 20)) {
        character->level = level;
    }

    ImGui::Spacing();
    ImGui::Checkbox("Is NPC?", &character->isNPC);
}

void CharacterCreatorWindow::RenderRaceClass() {
    ImGui::Text("Select Race and Class");
    ImGui::Spacing();

    // Race selection
    ImGui::Text("Race:");
    auto races = DnD::CharacterHelper::GetAllRaces();
    for (size_t i = 0; i < races.size(); ++i) {
        if (ImGui::RadioButton(DnD::CharacterHelper::RaceToString(races[i]).c_str(), &selectedRace, i)) {
            character->race = races[i];
            character->speed = DnD::CharacterHelper::GetRacialSpeed(races[i]);
        }
        if (i % 3 != 2) ImGui::SameLine();
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Class selection
    ImGui::Text("Class:");
    auto classes = DnD::CharacterHelper::GetAllClasses();
    for (size_t i = 0; i < classes.size(); ++i) {
        if (ImGui::RadioButton(DnD::CharacterHelper::ClassToString(classes[i]).c_str(), &selectedClass, i)) {
            character->characterClass = classes[i];

            // Set class-based proficiencies
            // This is simplified - real implementation would need full class data
            switch (classes[i]) {
                case DnD::CharacterClass::Fighter:
                    character->savingThrowProficiencies[DnD::Ability::Strength] = true;
                    character->savingThrowProficiencies[DnD::Ability::Constitution] = true;
                    break;
                case DnD::CharacterClass::Wizard:
                    character->savingThrowProficiencies[DnD::Ability::Intelligence] = true;
                    character->savingThrowProficiencies[DnD::Ability::Wisdom] = true;
                    break;
                case DnD::CharacterClass::Rogue:
                    character->savingThrowProficiencies[DnD::Ability::Dexterity] = true;
                    character->savingThrowProficiencies[DnD::Ability::Intelligence] = true;
                    break;
                case DnD::CharacterClass::Cleric:
                    character->savingThrowProficiencies[DnD::Ability::Wisdom] = true;
                    character->savingThrowProficiencies[DnD::Ability::Charisma] = true;
                    break;
                default:
                    break;
            }
        }
        if (i % 3 != 2) ImGui::SameLine();
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Alignment
    ImGui::Text("Alignment:");
    const char* alignments[] = {
        "Lawful Good", "Neutral Good", "Chaotic Good",
        "Lawful Neutral", "True Neutral", "Chaotic Neutral",
        "Lawful Evil", "Neutral Evil", "Chaotic Evil"
    };
    for (int i = 0; i < 9; ++i) {
        if (ImGui::RadioButton(alignments[i], &selectedAlignment, i)) {
            character->alignment = static_cast<DnD::Alignment>(i);
        }
        if (i % 3 != 2) ImGui::SameLine();
    }
}

void CharacterCreatorWindow::RenderAbilities() {
    ImGui::Text("Ability Scores");
    ImGui::Spacing();

    // Method selection
    if (ImGui::RadioButton("Point Buy (27 points)", (int*)&abilityMethod, 0)) {
        abilityMethod = AbilityMethod::PointBuy;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Standard Array", (int*)&abilityMethod, 1)) {
        abilityMethod = AbilityMethod::StandardArray;
        // Set standard array: 15, 14, 13, 12, 10, 8
        character->abilityScores.strength = 15;
        character->abilityScores.dexterity = 14;
        character->abilityScores.constitution = 13;
        character->abilityScores.intelligence = 12;
        character->abilityScores.wisdom = 10;
        character->abilityScores.charisma = 8;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Roll (4d6 drop lowest)", (int*)&abilityMethod, 2)) {
        abilityMethod = AbilityMethod::Roll;
        if (ImGui::Button("Roll Scores")) {
            rolledScores = DnD::Dice::RollAbilityScoreSet();
        }
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (abilityMethod == AbilityMethod::PointBuy) {
        int pointsUsed = CalculatePointBuyTotal();
        ImGui::Text("Points Used: %d / 27", pointsUsed);
        if (pointsUsed > 27) {
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "You have exceeded your point budget!");
        }
        ImGui::Spacing();
    }

    // Ability score sliders/editors
    auto renderAbility = [this](const char* label, DnD::Ability ability) {
        int score = character->abilityScores.Get(ability);
        int modifier = character->abilityScores.GetModifier(ability);

        ImGui::Text("%s:", label);
        ImGui::SameLine(120);

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
        ImGui::Text("Modifier: %+d", modifier);
    };

    renderAbility("Strength", DnD::Ability::Strength);
    renderAbility("Dexterity", DnD::Ability::Dexterity);
    renderAbility("Constitution", DnD::Ability::Constitution);
    renderAbility("Intelligence", DnD::Ability::Intelligence);
    renderAbility("Wisdom", DnD::Ability::Wisdom);
    renderAbility("Charisma", DnD::Ability::Charisma);

    if (!rolledScores.empty() && abilityMethod == AbilityMethod::Roll) {
        ImGui::Spacing();
        ImGui::Text("Rolled scores:");
        for (int score : rolledScores) {
            ImGui::SameLine();
            ImGui::Text("%d", score);
        }
    }
}

void CharacterCreatorWindow::RenderSkills() {
    ImGui::Text("Skill Proficiencies");
    ImGui::Spacing();

    ImGui::TextWrapped("Select your skill proficiencies. The number of skills you can choose depends on your class and background.");
    ImGui::Spacing();

    // Render all skills
    auto skills = DnD::SkillHelper::GetAllSkills();
    for (auto skill : skills) {
        DnD::Ability ability = DnD::SkillHelper::GetAbility(skill);
        std::string skillName = DnD::SkillHelper::ToString(skill);
        std::string abilityName = DnD::AbilityHelper::ToShortString(ability);

        bool isProficient = character->skillProficiencies[skill] == DnD::ProficiencyLevel::Proficient;
        bool isExpert = character->skillProficiencies[skill] == DnD::ProficiencyLevel::Expert;

        if (ImGui::Checkbox((skillName + " (" + abilityName + ")").c_str(), &isProficient)) {
            if (isProficient) {
                character->skillProficiencies[skill] = DnD::ProficiencyLevel::Proficient;
            } else {
                character->skillProficiencies[skill] = DnD::ProficiencyLevel::None;
            }
        }

        // Expert checkbox (for Rogues, Bards, etc.)
        if (character->characterClass == DnD::CharacterClass::Rogue && character->level >= 1) {
            ImGui::SameLine();
            if (ImGui::Checkbox(("Expert##" + skillName).c_str(), &isExpert)) {
                if (isExpert) {
                    character->skillProficiencies[skill] = DnD::ProficiencyLevel::Expert;
                } else {
                    character->skillProficiencies[skill] = isProficient ?
                        DnD::ProficiencyLevel::Proficient : DnD::ProficiencyLevel::None;
                }
            }
        }

        // Show calculated bonus
        int bonus = character->GetSkillModifier(skill);
        ImGui::SameLine(400);
        ImGui::Text("%+d", bonus);
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
