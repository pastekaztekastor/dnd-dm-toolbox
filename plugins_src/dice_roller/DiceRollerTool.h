#pragma once

#include "../../include/core/ToolBase.h"
#include <string>
#include <vector>
#include <random>

/**
 * @brief Plugin Dice Roller - Lanceur de dés D&D
 *
 * Fonctionnalités:
 * - Lance des dés avec formule (ex: 2d6+3, 1d20+5)
 * - Historique des jets
 * - Publish des événements "dice.rolled"
 * - Log de tous les jets
 */
class DiceRollerTool : public Core::ToolBase {
public:
    DiceRollerTool();
    ~DiceRollerTool() override;

    // Interface ToolBase
    std::string GetID() const override { return "dice_roller"; }
    std::string GetName() const override { return "Dice Roller"; }
    std::string GetCategory() const override { return "Jeu"; }
    std::string GetDescription() const override { return "Lanceur de dés pour D&D 5e"; }

    void OnCreate() override;
    void OnDestroy() override;
    void OnSave(sqlite3* db) override;
    void OnLoad(sqlite3* db) override;
    void Render() override;

private:
    struct DiceRoll {
        std::string formula;        // Formule (ex: "2d6+3")
        int result;                 // Résultat
        std::vector<int> rolls;     // Détails des jets individuels
        double timestamp;           // Quand le jet a été fait
    };

    std::vector<DiceRoll> history;  // Historique des jets
    char formulaBuffer[256] = "1d20"; // Buffer pour l'input de formule
    std::mt19937 rng;               // Générateur aléatoire

    /**
     * @brief Parse et exécute une formule de dé
     *
     * @param formula Formule (ex: "2d6+3", "1d20", "3d8-2")
     * @param outRoll Résultat (output)
     * @return true si la formule est valide
     */
    bool RollDice(const std::string& formula, DiceRoll& outRoll);

    /**
     * @brief Lance un dé simple (1dX)
     *
     * @param sides Nombre de faces
     * @return Résultat (1 à sides)
     */
    int RollSingleDie(int sides);

    /**
     * @brief Enregistre un formatteur custom pour le log
     */
    void RegisterLogFormatter();
};
