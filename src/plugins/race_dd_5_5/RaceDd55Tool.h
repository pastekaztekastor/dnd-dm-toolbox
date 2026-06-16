#pragma once

#include "../../include/core/ToolBase.h"
#include <string>
#include <vector>
#include <memory>
#include "RaceDd55Repository.h"

class RaceDd55Tool : public Core::ToolBase {
public:
    RaceDd55Tool();
    ~RaceDd55Tool() override;

    std::string GetID()          const override { return "race_dd_5_5"; }
    std::string GetName()        const override { return "Races D&D 5e"; }
    std::string GetCategory()    const override { return "Référence"; }
    std::string GetDescription() const override { return "Races du Manuel du joueur D&D 5e (VF)"; }

    void OnCreate() override;
    void OnDestroy() override;
    void OnSave(sqlite3* db) override;
    void OnLoad(sqlite3* db) override;
    void Render() override;
    void OnEvent(const Core::Event& event) override;

private:
    sqlite3* pluginDb = nullptr;
    std::unique_ptr<RaceDd55::RaceDd55Repository> repo;

    // Données affichées
    std::vector<RaceDd55::RaceData> races;
    std::vector<RaceDd55::RaceData> subraces;
    std::string selectedId;
    int         selectedIdx    = -1;
    int         parentRaceIdx  = -1;
    bool        selectedIsSub  = false;

    // État édition
    bool editMode  = false;
    bool isNewRace = false;
    RaceDd55::RaceData editRace;
    std::string editError;

    void SelectRace(int idx, bool isSub);
    void StartEdit(const RaceDd55::RaceData& race, bool isNew = false);
    void SaveEdit();
    void CancelEdit();
    void RenderDetailPanel(RaceDd55::RaceData& race);
    void RenderEditPanel();
    double GetCurrentTime();
};
