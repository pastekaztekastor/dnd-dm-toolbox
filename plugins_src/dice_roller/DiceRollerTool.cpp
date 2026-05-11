#include "DiceRollerTool.h"
#include "../../include/core/Logger.h"
#include <imgui.h>
#include <chrono>
#include <regex>
#include <sstream>

DiceRollerTool::DiceRollerTool() {
    // Initialiser le générateur aléatoire
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    rng.seed(static_cast<unsigned int>(seed));
}

DiceRollerTool::~DiceRollerTool() {
}

void DiceRollerTool::OnCreate() {
    // Enregistrer le formatteur de log
    RegisterLogFormatter();
}

void DiceRollerTool::OnDestroy() {
    // Rien à faire
}

void DiceRollerTool::OnSave(sqlite3* db) {
    if (!db) return;

    // Sauvegarder l'historique dans le .dnd
    nlohmann::json state;
    state["formula"] = std::string(formulaBuffer);

    // Sauvegarder les 20 derniers jets
    nlohmann::json historyJson = nlohmann::json::array();
    size_t start = history.size() > 20 ? history.size() - 20 : 0;

    for (size_t i = start; i < history.size(); ++i) {
        const auto& roll = history[i];
        nlohmann::json rollJson;
        rollJson["formula"] = roll.formula;
        rollJson["result"] = roll.result;
        rollJson["rolls"] = roll.rolls;
        rollJson["timestamp"] = roll.timestamp;
        historyJson.push_back(rollJson);
    }

    state["history"] = historyJson;

    // ID singleton pour dice_roller
    std::string diceID = "dice_roller_singleton";

    // Sauvegarder dans la DB
    const char* sql = "INSERT OR REPLACE INTO tool_instances (id, tool_type, data) VALUES (?, ?, ?)";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, diceID.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, GetID().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, state.dump().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
}

void DiceRollerTool::OnLoad(sqlite3* db) {
    if (!db) return;

    // ID singleton pour dice_roller
    std::string diceID = "dice_roller_singleton";

    // Charger l'historique depuis le .dnd
    const char* sql = "SELECT data FROM tool_instances WHERE id = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, diceID.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* jsonStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));

            if (jsonStr && strlen(jsonStr) > 0) {
                auto state = nlohmann::json::parse(jsonStr);

                if (state.contains("formula")) {
                    std::string formula = state["formula"];
                    strncpy(formulaBuffer, formula.c_str(), sizeof(formulaBuffer) - 1);
                }

                if (state.contains("history")) {
                    for (const auto& rollJson : state["history"]) {
                        DiceRoll roll;
                        roll.formula = rollJson["formula"];
                        roll.result = rollJson["result"];
                        roll.rolls = rollJson["rolls"].get<std::vector<int>>();
                        roll.timestamp = rollJson["timestamp"];
                        history.push_back(roll);
                    }
                }
            }
        }

        sqlite3_finalize(stmt);
    }
}

void DiceRollerTool::Render() {
    if (!isOpen) return;

    ImGui::SetNextWindowSize(ImVec2(400, 500), ImGuiCond_FirstUseEver);

    if (ImGui::Begin(GetWindowTitle().c_str(), &isOpen)) {
        ImGui::TextWrapped("Lanceur de dés D&D 5e");
        ImGui::Separator();

        // Input de formule
        ImGui::Text("Formule:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1);
        ImGui::InputText("##formula", formulaBuffer, sizeof(formulaBuffer));

        // Bouton Roll
        if (ImGui::Button("🎲 Lancer", ImVec2(-1, 40))) {
            DiceRoll roll;
            if (RollDice(formulaBuffer, roll)) {
                history.push_back(roll);

                // Log
                Log("roll_dice", {
                    {"formula", roll.formula},
                    {"result", roll.result},
                    {"rolls", roll.rolls}
                });

                // Publish event
                PublishEvent("dice.rolled", {
                    {"formula", roll.formula},
                    {"result", roll.result}
                });
            }
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Historique
        ImGui::Text("Historique:");

        if (ImGui::BeginChild("History")) {
            // Afficher en ordre inverse (plus récent en haut)
            for (int i = static_cast<int>(history.size()) - 1; i >= 0; --i) {
                const auto& roll = history[i];

                ImGui::PushID(i);

                // Formule et résultat
                ImGui::Text("%s = ", roll.formula.c_str());
                ImGui::SameLine();
                ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "%d", roll.result);

                // Détails des jets individuels
                if (!roll.rolls.empty()) {
                    ImGui::SameLine();
                    ImGui::TextDisabled("(");
                    for (size_t j = 0; j < roll.rolls.size(); ++j) {
                        ImGui::SameLine();
                        ImGui::TextDisabled("%d", roll.rolls[j]);
                        if (j < roll.rolls.size() - 1) {
                            ImGui::SameLine();
                            ImGui::TextDisabled("+");
                        }
                    }
                    ImGui::SameLine();
                    ImGui::TextDisabled(")");
                }

                ImGui::PopID();
            }
        }
        ImGui::EndChild();
    }

    ImGui::End();
}

bool DiceRollerTool::RollDice(const std::string& formula, DiceRoll& outRoll) {
    // Parser la formule avec regex
    // Formats supportés: XdY, XdY+Z, XdY-Z
    std::regex diceRegex(R"((\d+)d(\d+)([\+\-]\d+)?)");
    std::smatch matches;

    if (!std::regex_match(formula, matches, diceRegex)) {
        return false;
    }

    int numDice = std::stoi(matches[1].str());
    int numSides = std::stoi(matches[2].str());
    int modifier = 0;

    if (matches[3].matched) {
        modifier = std::stoi(matches[3].str());
    }

    // Vérifications
    if (numDice <= 0 || numDice > 100) return false;
    if (numSides <= 0 || numSides > 1000) return false;

    // Lancer les dés
    outRoll.formula = formula;
    outRoll.rolls.clear();
    outRoll.result = modifier;

    for (int i = 0; i < numDice; ++i) {
        int roll = RollSingleDie(numSides);
        outRoll.rolls.push_back(roll);
        outRoll.result += roll;
    }

    // Timestamp
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    outRoll.timestamp = std::chrono::duration<double>(duration).count();

    return true;
}

int DiceRollerTool::RollSingleDie(int sides) {
    std::uniform_int_distribution<int> dist(1, sides);
    return dist(rng);
}

void DiceRollerTool::RegisterLogFormatter() {
    if (!logger) return;

    logger->RegisterFormatter("roll_dice", [](const Core::LogEntry& entry) {
        std::string formula = entry.data["formula"];
        int result = entry.data["result"];

        std::stringstream ss;
        ss << "Lance " << formula << " → " << result;

        return ss.str();
    });

    logger->RegisterToolDisplayName(GetID(), "🎲 Dice Roller");
}

// ============================================================
// EXPORT DU PLUGIN (C ABI)
// ============================================================

extern "C" {
    PLUGIN_API Core::ToolBase* CreateTool() {
        return new DiceRollerTool();
    }

    PLUGIN_API void DestroyTool(Core::ToolBase* tool) {
        delete tool;
    }
}
