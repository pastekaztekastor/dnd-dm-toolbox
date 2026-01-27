#include "../../include/ui/DiceRollerWindow.h"
#include <imgui.h>
#include <ctime>
#include <sstream>
#include <iomanip>

namespace UI {

std::string DiceRollerWindow::GetCurrentTime() const {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%H:%M:%S");
    return oss.str();
}

void DiceRollerWindow::AddToHistory(const DiceRollResult& result) {
    history.push_back(result);
    if (history.size() > maxHistorySize) {
        history.erase(history.begin());
    }
}

void DiceRollerWindow::RollDice(const std::string& notation) {
    if (notation.empty()) return;

    try {
        auto diceRoll = DnD::Dice::ParseDiceNotation(notation);

        DiceRollResult result;
        result.notation = notation;
        result.modifier = diceRoll.modifier;
        result.timestamp = GetCurrentTime();

        // Roll individual dice
        result.rolls = DnD::Dice::RollIndividual(diceRoll.count, diceRoll.sides);

        // Calculate total
        result.total = 0;
        for (int roll : result.rolls) {
            result.total += roll;
        }
        result.total += result.modifier;

        AddToHistory(result);
    } catch (...) {
        // Invalid notation - ignore
    }
}

void DiceRollerWindow::Render() {
    if (!isOpen) return;

    ImGui::SetNextWindowSize(ImVec2(500, 600), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("🎲 Lanceur de Dés", &isOpen)) {

        // === Section de saisie manuelle ===
        ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Lancer Personnalisé");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Format : XdY+Z (ex: 2d6+3, 1d20, 4d8-2)");
        ImGui::SetNextItemWidth(200);
        if (ImGui::InputText("##DiceNotation", diceNotationBuffer, sizeof(diceNotationBuffer),
                            ImGuiInputTextFlags_EnterReturnsTrue)) {
            RollDice(diceNotationBuffer);
        }

        ImGui::SameLine();
        if (ImGui::Button("Lancer", ImVec2(100, 0))) {
            RollDice(diceNotationBuffer);
        }

        ImGui::SameLine();
        if (ImGui::Button("Effacer")) {
            diceNotationBuffer[0] = '\0';
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // === Boutons de lancer rapide ===
        ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Lancers Rapides");
        ImGui::Separator();
        ImGui::Spacing();

        // Standard dice
        ImGui::Text("Dés Standard :");
        for (size_t i = 0; i < 7; ++i) {
            if (ImGui::Button(quickRolls[i].label, ImVec2(60, 35))) {
                RollDice(quickRolls[i].notation);
            }
            if (i < 6) ImGui::SameLine();
        }

        ImGui::Spacing();
        ImGui::Text("Multiples :");
        for (size_t i = 7; i < quickRolls.size(); ++i) {
            if (ImGui::Button(quickRolls[i].label, ImVec2(60, 35))) {
                RollDice(quickRolls[i].notation);
            }
            if (i < quickRolls.size() - 1) ImGui::SameLine();
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // === Dernier résultat (grand affichage) ===
        if (!history.empty()) {
            const auto& lastRoll = history.back();
            ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "Dernier Lancer");
            ImGui::Separator();

            // Display dice notation
            ImGui::Text("Formule : %s", lastRoll.notation.c_str());

            // Display individual rolls if multiple dice
            if (lastRoll.rolls.size() > 1) {
                ImGui::Text("Dés : [");
                ImGui::SameLine();
                for (size_t i = 0; i < lastRoll.rolls.size(); ++i) {
                    ImGui::Text("%d", lastRoll.rolls[i]);
                    if (i < lastRoll.rolls.size() - 1) {
                        ImGui::SameLine();
                        ImGui::Text(",");
                        ImGui::SameLine();
                    }
                }
                ImGui::SameLine();
                ImGui::Text("]");

                if (lastRoll.modifier != 0) {
                    ImGui::SameLine();
                    ImGui::Text("%s %d", lastRoll.modifier > 0 ? "+" : "-", std::abs(lastRoll.modifier));
                }
            }

            // Big result display
            ImGui::Spacing();
            ImGui::PushFont(ImGui::GetFont()); // Use default font but scaled

            // Calculate text size for centering
            char resultText[32];
            snprintf(resultText, sizeof(resultText), "Résultat : %d", lastRoll.total);

            ImVec2 textSize = ImGui::CalcTextSize(resultText);
            float windowWidth = ImGui::GetContentRegionAvail().x;
            float textPosX = (windowWidth - textSize.x * 1.5f) / 2.0f;

            if (textPosX > 0) ImGui::SetCursorPosX(ImGui::GetCursorPosX() + textPosX);

            // Display result in large, colored text
            ImVec4 resultColor = ImVec4(0.2f, 1.0f, 0.3f, 1.0f); // Green
            if (lastRoll.notation.find("d20") != std::string::npos) {
                int d20Value = lastRoll.total - lastRoll.modifier;
                if (d20Value == 20) {
                    resultColor = ImVec4(1.0f, 0.84f, 0.0f, 1.0f); // Gold for nat 20
                } else if (d20Value == 1) {
                    resultColor = ImVec4(1.0f, 0.2f, 0.2f, 1.0f); // Red for nat 1
                }
            }

            ImGui::PushStyleColor(ImGuiCol_Text, resultColor);

            // Scale up the text
            ImGui::SetWindowFontScale(2.0f);
            ImGui::Text("Résultat : %d", lastRoll.total);
            ImGui::SetWindowFontScale(1.0f);

            ImGui::PopStyleColor();
            ImGui::PopFont();

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
        }

        // === Historique ===
        ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Historique");
        ImGui::SameLine();
        if (ImGui::SmallButton("Effacer l'historique")) {
            ClearHistory();
        }
        ImGui::Separator();

        ImGui::BeginChild("HistoryScroll", ImVec2(0, 0), true);

        // Display history in reverse order (most recent first)
        for (auto it = history.rbegin(); it != history.rend(); ++it) {
            ImGui::PushID(&(*it));

            // Timestamp
            ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "[%s]", it->timestamp.c_str());
            ImGui::SameLine();

            // Result
            ImGui::Text("%s", it->ToString().c_str());

            ImGui::PopID();
        }

        ImGui::EndChild();
    }
    ImGui::End();
}

} // namespace UI
