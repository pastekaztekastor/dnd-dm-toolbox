#include "RaceDd55Tool.h"
#include "../../include/core/Logger.h"
#include "../../include/core/EventBus.h"
#include <imgui.h>
#include <nlohmann/json.hpp>
#include <chrono>
#include <fstream>
#include <sstream>
#include <cstring>
#include <functional>

RaceDd55Tool::RaceDd55Tool() {}
RaceDd55Tool::~RaceDd55Tool() {}

// ============================================================
// LIFECYCLE
// ============================================================

void RaceDd55Tool::OnCreate() {
    sqlite3_open("race_dd_5_5.db", &pluginDb);

    bool needsInit = true;
    sqlite3_stmt* checkStmt;
    if (sqlite3_prepare_v2(pluginDb,
        "SELECT name FROM sqlite_master WHERE type='table' AND name='races'",
        -1, &checkStmt, nullptr) == SQLITE_OK) {
        needsInit = sqlite3_step(checkStmt) != SQLITE_ROW;
        sqlite3_finalize(checkStmt);
    }
    if (needsInit) {
        std::ifstream f("plugins/race_dd_5_5_schema.sql");
        if (f) { std::stringstream buf; buf << f.rdbuf();
            sqlite3_exec(pluginDb, buf.str().c_str(), nullptr, nullptr, nullptr); }
    }

    int count = 0;
    sqlite3_stmt* cnt;
    if (sqlite3_prepare_v2(pluginDb, "SELECT COUNT(*) FROM races", -1, &cnt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(cnt) == SQLITE_ROW) count = sqlite3_column_int(cnt, 0);
        sqlite3_finalize(cnt);
    }
    if (count == 0) {
        std::ifstream f("plugins/race_dd_5_5_seed.sql");
        if (f) { std::stringstream buf; buf << f.rdbuf();
            sqlite3_exec(pluginDb, buf.str().c_str(), nullptr, nullptr, nullptr); }
    }

    repo  = std::make_unique<RaceDd55::RaceDd55Repository>(pluginDb);
    races = repo->LoadAllRaces();
    if (!races.empty()) SelectRace(0, false);
}

void RaceDd55Tool::OnDestroy() {
    repo.reset();
    if (pluginDb) { sqlite3_close(pluginDb); pluginDb = nullptr; }
}

void RaceDd55Tool::OnSave(sqlite3* db) {
    if (!db) return;
    nlohmann::json state;
    state["selected_id"]     = selectedId;
    state["selected_is_sub"] = selectedIsSub;
    const char* sql = "INSERT OR REPLACE INTO tool_instances (id, tool_type, data) VALUES (?, ?, ?)";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, instanceID.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, GetID().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, state.dump().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt); sqlite3_finalize(stmt);
    }
}

void RaceDd55Tool::OnLoad(sqlite3* db) {
    if (!db) return;
    const char* sql = "SELECT data FROM tool_instances WHERE id = ?";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, instanceID.c_str(), -1, SQLITE_TRANSIENT);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* j = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            if (j && strlen(j) > 0) {
                auto state = nlohmann::json::parse(j, nullptr, false);
                if (!state.is_discarded() && state.contains("selected_id")) {
                    selectedId    = state["selected_id"].get<std::string>();
                    selectedIsSub = state.value("selected_is_sub", false);
                    auto& list = selectedIsSub ? subraces : races;
                    for (int i = 0; i < (int)list.size(); ++i)
                        if (list[i].id == selectedId) { selectedIdx = i; break; }
                }
            }
        }
        sqlite3_finalize(stmt);
    }
}

// ============================================================
// HELPERS ImGui + std::string
// ============================================================

static int str_resize_cb(ImGuiInputTextCallbackData* data) {
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
        auto* str = static_cast<std::string*>(data->UserData);
        str->resize(data->BufTextLen);
        data->Buf = str->data();
    }
    return 0;
}

static bool InputStr(const char* label, std::string& str,
    ImGuiInputTextFlags flags = 0, float width = -1.f) {
    if (width > 0) ImGui::SetNextItemWidth(width);
    str.reserve(str.size() + 64);
    return ImGui::InputText(label, str.data(), str.capacity(),
        flags | ImGuiInputTextFlags_CallbackResize, str_resize_cb, &str);
}

static bool InputStrMulti(const char* label, std::string& str, ImVec2 size = {-1, 100}) {
    str.reserve(str.size() + 64);
    return ImGui::InputTextMultiline(label, str.data(), str.capacity(), size,
        ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_WordWrap,
        str_resize_cb, &str);
}

// ============================================================
// RENDERING
// ============================================================

static const char* STAT_LABELS[] = {"FOR","DEX","CON","INT","SAG","CHA"};

static ImVec4 stat_color(int v) {
    if (v > 0) return {.3f,.9f,.3f,1};
    if (v < 0) return {.9f,.3f,.3f,1};
    return {1,1,1,.35f};
}

static void render_stat_table(int v0, int v1, int v2, int v3, int v4, int v5) {
    const int vals[6] = {v0, v1, v2, v3, v4, v5};
    if (ImGui::BeginTable("##st", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit)) {
        for (auto& l : STAT_LABELS) ImGui::TableSetupColumn(l);
        ImGui::TableHeadersRow();
        ImGui::TableNextRow();
        for (int i = 0; i < 6; ++i) {
            ImGui::TableNextColumn();
            ImGui::TextColored(stat_color(vals[i]), "%+d", vals[i]);
        }
        ImGui::EndTable();
    }
}

static void render_stat_table_combined(
    int pv0, int pv1, int pv2, int pv3, int pv4, int pv5,
    int sv0, int sv1, int sv2, int sv3, int sv4, int sv5)
{
    const int pv[6] = {pv0,pv1,pv2,pv3,pv4,pv5};
    const int sv[6] = {sv0,sv1,sv2,sv3,sv4,sv5};
    if (ImGui::BeginTable("##stc", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit)) {
        for (auto& l : STAT_LABELS) ImGui::TableSetupColumn(l);
        ImGui::TableHeadersRow();
        ImGui::TableNextRow();
        for (int i = 0; i < 6; ++i) {
            ImGui::TableNextColumn();
            int total = pv[i] + sv[i];
            ImGui::TextColored(stat_color(total), "%+d", total);
            if (pv[i] && sv[i])
                ImGui::SetItemTooltip("Parent %+d + sous-race %+d", pv[i], sv[i]);
        }
        ImGui::EndTable();
    }
}

static void render_stat_table_edit(int& v0, int& v1, int& v2, int& v3, int& v4, int& v5) {
    int* vals[6] = {&v0,&v1,&v2,&v3,&v4,&v5};
    if (ImGui::BeginTable("##ste", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit)) {
        for (auto& l : STAT_LABELS) ImGui::TableSetupColumn(l);
        ImGui::TableHeadersRow();
        ImGui::TableNextRow();
        for (int i = 0; i < 6; ++i) {
            ImGui::TableNextColumn();
            ImGui::PushID(i);
            if (ImGui::SmallButton("+")) (*vals[i])++;
            ImGui::PopID();
        }
        ImGui::TableNextRow();
        for (int i = 0; i < 6; ++i) {
            ImGui::TableNextColumn();
            ImGui::TextColored(stat_color(*vals[i]), "%+d", *vals[i]);
        }
        ImGui::TableNextRow();
        for (int i = 0; i < 6; ++i) {
            ImGui::TableNextColumn();
            ImGui::PushID(i + 100);
            if (ImGui::SmallButton("-")) (*vals[i])--;
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
}

void RaceDd55Tool::Render() {
    if (!isOpen) return;
    ImGui::SetNextWindowSize(ImVec2(960, 620), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(GetWindowTitle().c_str(), &isOpen)) { ImGui::End(); return; }

    // ---- Panneau gauche ----
    ImGui::BeginChild("##list", ImVec2(220, 0), true);

    if (!editMode) {
        if (ImGui::Button("+ Nouvelle race", ImVec2(-1, 0)))
            StartEdit({}, true);
    }
    ImGui::Spacing();

    // L'index de la race parente à étendre dans l'arbre
    int expandIdx = selectedIsSub ? parentRaceIdx : selectedIdx;

    for (int i = 0; i < (int)races.size(); ++i) {
        bool sel = (!selectedIsSub && selectedIdx == i);
        if (ImGui::Selectable(races[i].nom.c_str(), sel) && !editMode)
            SelectRace(i, false);
        // Afficher les sous-races si c'est la race expandée (sélectionnée ou parente de la sous-race active)
        if (i == expandIdx && !subraces.empty()) {
            ImGui::Indent(14.f);
            for (int j = 0; j < (int)subraces.size(); ++j) {
                bool ssel = (selectedIsSub && selectedIdx == j);
                if (ImGui::Selectable(subraces[j].nom.c_str(), ssel) && !editMode)
                    SelectRace(j, true);
            }
            ImGui::Unindent(14.f);
        }
    }
    ImGui::EndChild();
    ImGui::SameLine();

    // ---- Panneau droit ----
    ImGui::BeginChild("##detail", ImVec2(0, 0), false);
    if (editMode) {
        RenderEditPanel();
    } else if (selectedIdx >= 0) {
        auto& race = selectedIsSub ? subraces[selectedIdx] : races[selectedIdx];
        RenderDetailPanel(race);
    } else {
        ImGui::TextDisabled("Sélectionnez une race dans la liste.");
    }
    ImGui::EndChild();
    ImGui::End();
}

static void render_race_sections(RaceDd55::RaceData& race, bool defaultOpen, const char* scope) {
    ImGuiTreeNodeFlags flags = defaultOpen ? ImGuiTreeNodeFlags_DefaultOpen : 0;
    char hdr[64];
    snprintf(hdr, sizeof(hdr), "Présentation##%s", scope);
    if (!race.presentations.empty() && ImGui::CollapsingHeader(hdr, flags)) {
        for (auto& p : race.presentations) {
            if (!p.titre.empty())
                ImGui::TextColored({.8f,.7f,.4f,1}, "%s", p.titre.c_str());
            ImGui::TextWrapped("%s", p.texte.c_str());
            ImGui::Spacing();
        }
    }
    snprintf(hdr, sizeof(hdr), "Traits raciaux##%s", scope);
    if (!race.traits.empty() && ImGui::CollapsingHeader(hdr, flags)) {
        for (auto& t : race.traits) {
            ImGui::TextColored({.5f,.8f,1,1}, "%s", t.titre.c_str());
            ImGui::TextWrapped("%s", t.texte.c_str());
            ImGui::Spacing();
        }
    }
    snprintf(hdr, sizeof(hdr), "Noms##%s", scope);
    if (!race.noms.empty() && ImGui::CollapsingHeader(hdr, flags)) {
        for (auto& n : race.noms) {
            ImGui::TextColored({.8f,.7f,.4f,1}, "%s", n.titre.c_str());
            ImGui::TextWrapped("%s", n.texte.c_str());
            ImGui::Spacing();
        }
    }
}

void RaceDd55Tool::RenderDetailPanel(RaceDd55::RaceData& race) {
    // Boutons d'action
    if (ImGui::Button("Modifier"))
        StartEdit(race, false);
    ImGui::SameLine();
    if (!selectedIsSub) {
        if (ImGui::Button("+ Sous-race")) {
            RaceDd55::RaceData sub;
            sub.race_parent_id = race.id;
            StartEdit(sub, true);
        }
        ImGui::SameLine();
    }
    if (ImGui::Button("Supprimer")) {
        repo->DeleteRaceById(race.id);
        races    = repo->LoadAllRaces();
        subraces.clear();
        selectedIdx = -1; selectedId = ""; selectedIsSub = false; parentRaceIdx = -1;
        return;
    }
    ImGui::Separator();
    ImGui::Spacing();

    if (selectedIsSub && parentRaceIdx >= 0 && parentRaceIdx < (int)races.size()) {
        // ---- Vue sous-race : afficher parent + spécifiques ----
        auto& parent = races[parentRaceIdx];

        // Titre sous-race
        ImGui::Text("%s", race.nom.c_str());
        ImGui::SameLine();
        ImGui::TextDisabled("  — sous-race de %s", parent.nom.c_str());
        ImGui::Separator(); ImGui::Spacing();

        // Bonus combinés (survoler une cellule affiche la répartition parent/sous-race)
        render_stat_table_combined(
            parent.bonus_forces, parent.bonus_dexterite, parent.bonus_constitution,
            parent.bonus_intelligence, parent.bonus_sagesse, parent.bonus_charisme,
            race.bonus_forces, race.bonus_dexterite, race.bonus_constitution,
            race.bonus_intelligence, race.bonus_sagesse, race.bonus_charisme);

        // Stats héritées du parent
        if (parent.vitesse_base > 0)
            ImGui::Text("Vitesse : %.1f m", parent.vitesse_base * 0.3f);
        if (parent.age_max > 0)
            ImGui::Text("Âge adulte %d ans — Max %d ans", parent.age_adulte, parent.age_max);
        if (!parent.liste_langues.empty())
            ImGui::TextWrapped("Langues : %s", parent.liste_langues.c_str());

        ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

        // Sections spécifiques à la sous-race (ouvertes)
        if (!race.presentations.empty() || !race.traits.empty() || !race.noms.empty()) {
            ImGui::TextColored({.8f,.6f,.2f,1}, "Spécifique à %s", race.nom.c_str());
            ImGui::Spacing();
            render_race_sections(race, true, "sub");
        }

        // Sections héritées du parent (fermées par défaut)
        ImGui::Spacing();
        std::string parentHeader = std::string("Hérité de ") + parent.nom + "##par";
        if (ImGui::CollapsingHeader(parentHeader.c_str())) {
            render_race_sections(parent, false, "par");
        }

    } else {
        // ---- Vue race normale ----
        ImGui::Text("%s", race.nom.c_str());
        ImGui::Separator(); ImGui::Spacing();

        render_stat_table(race.bonus_forces, race.bonus_dexterite, race.bonus_constitution,
                          race.bonus_intelligence, race.bonus_sagesse, race.bonus_charisme);

        if (race.vitesse_base > 0)
            ImGui::Text("Vitesse : %.1f m", race.vitesse_base * 0.3f);
        if (race.age_max > 0)
            ImGui::Text("Âge adulte %d ans — Max %d ans", race.age_adulte, race.age_max);
        if (!race.liste_langues.empty())
            ImGui::TextWrapped("Langues : %s", race.liste_langues.c_str());

        ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
        render_race_sections(race, true, "main");
    }
}

void RaceDd55Tool::RenderEditPanel() {
    // Barre d'actions
    ImGui::TextColored({.8f,.6f,.2f,1}, isNewRace ? "[Nouvelle race]" : "[Modification]");
    ImGui::SameLine();
    if (ImGui::Button("Sauvegarder")) SaveEdit();
    ImGui::SameLine();
    if (ImGui::Button("Annuler")) CancelEdit();
    ImGui::Separator(); ImGui::Spacing();

    // Erreurs de validation
    if (!editError.empty()) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, .35f, .35f, 1.f));
        ImGui::TextWrapped("! %s", editError.c_str());
        ImGui::PopStyleColor();
        ImGui::Spacing();
    }

    // Infos de base
    InputStr("Nom##e", editRace.nom);
    if (isNewRace) {
        InputStr("Alias / ID##e", editRace.alias);
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Identifiant unique, ex: elfe-des-ombres\n(ne peut plus être changé après création)");
    }

    // Dropdown race parente
    {
        const char* parentLabel = "(aucune — race de base)";
        for (auto& r : races)
            if (r.id == editRace.race_parent_id) { parentLabel = r.nom.c_str(); break; }
        if (ImGui::BeginCombo("Race parente##e", parentLabel)) {
            if (ImGui::Selectable("(aucune — race de base)", editRace.race_parent_id.empty()))
                editRace.race_parent_id.clear();
            if (editRace.race_parent_id.empty()) ImGui::SetItemDefaultFocus();
            for (auto& r : races) {
                if (!isNewRace && r.id == editRace.id) continue;
                bool sel = (editRace.race_parent_id == r.id);
                if (ImGui::Selectable(r.nom.c_str(), sel))
                    editRace.race_parent_id = r.id;
                if (sel) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

    InputStr("Langues##e", editRace.liste_langues);

    ImGui::Spacing();
    ImGui::Text("Bonus de caractéristiques");
    render_stat_table_edit(editRace.bonus_forces, editRace.bonus_dexterite, editRace.bonus_constitution,
                           editRace.bonus_intelligence, editRace.bonus_sagesse, editRace.bonus_charisme);

    ImGui::Spacing();
    ImGui::SetNextItemWidth(80); ImGui::InputInt("Âge adulte##e", &editRace.age_adulte); ImGui::SameLine();
    ImGui::SetNextItemWidth(80); ImGui::InputInt("Âge max##e",    &editRace.age_max);
    float vitMs = editRace.vitesse_base * 0.3f;
    ImGui::SetNextItemWidth(80);
    if (ImGui::InputFloat("Vitesse (m)##e", &vitMs, 1.5f, 1.5f, "%.1f"))
        editRace.vitesse_base = vitMs / 0.3f;

    ImGui::Spacing();
    ImGui::Text("Description"); InputStrMulti("##desc", editRace.description, {-1, 80});
    ImGui::Text("Aide joueur"); InputStrMulti("##aide", editRace.aide_joueur,  {-1, 60});

    ImGui::Spacing(); ImGui::Separator();

    // --- Présentations ---
    if (ImGui::CollapsingHeader("Présentations", ImGuiTreeNodeFlags_DefaultOpen)) {
        for (int i = 0; i < (int)editRace.presentations.size(); ++i) {
            auto& p = editRace.presentations[i];
            ImGui::PushID(i);
            InputStr("Titre##pt", p.titre, 0, -30);
            ImGui::SameLine();
            if (ImGui::SmallButton("×")) {
                editRace.presentations.erase(editRace.presentations.begin() + i);
                ImGui::PopID(); goto next_section_p;
            }
            InputStrMulti("##ptx", p.texte, {-1, 70});
            ImGui::Spacing();
            ImGui::PopID();
        }
        next_section_p:
        if (ImGui::Button("+ Présentation"))
            editRace.presentations.push_back({"", editRace.id, "", "", 0});
    }

    // --- Traits ---
    if (ImGui::CollapsingHeader("Traits raciaux")) {
        for (int i = 0; i < (int)editRace.traits.size(); ++i) {
            auto& t = editRace.traits[i];
            ImGui::PushID(i + 1000);
            InputStr("Titre##tt", t.titre, 0, -30);
            ImGui::SameLine();
            if (ImGui::SmallButton("×")) {
                editRace.traits.erase(editRace.traits.begin() + i);
                ImGui::PopID(); goto next_section_t;
            }
            InputStrMulti("##ttx", t.texte, {-1, 60});
            ImGui::Spacing();
            ImGui::PopID();
        }
        next_section_t:
        if (ImGui::Button("+ Trait"))
            editRace.traits.push_back({"", editRace.id, "", "", 0});
    }

    // --- Noms ---
    if (ImGui::CollapsingHeader("Noms")) {
        for (int i = 0; i < (int)editRace.noms.size(); ++i) {
            auto& n = editRace.noms[i];
            ImGui::PushID(i + 2000);
            InputStr("Titre##nt", n.titre, 0, -30);
            ImGui::SameLine();
            if (ImGui::SmallButton("×")) {
                editRace.noms.erase(editRace.noms.begin() + i);
                ImGui::PopID(); goto next_section_n;
            }
            InputStrMulti("##ntx", n.texte, {-1, 50});
            ImGui::Spacing();
            ImGui::PopID();
        }
        next_section_n:
        if (ImGui::Button("+ Groupe de noms"))
            editRace.noms.push_back({"", editRace.id, "", "", 0});
    }

    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
    if (ImGui::Button("Sauvegarder##bot")) SaveEdit();
    ImGui::SameLine();
    if (ImGui::Button("Annuler##bot")) CancelEdit();
}

// ============================================================
// EVENTS
// ============================================================

void RaceDd55Tool::OnEvent(const Core::Event& /*event*/) {}

// ============================================================
// MÉTHODES PRIVÉES
// ============================================================

void RaceDd55Tool::SelectRace(int idx, bool isSub) {
    if (!repo) return;
    if (!isSub) {
        selectedIdx   = idx;
        selectedIsSub = false;
        selectedId    = races[idx].id;
        parentRaceIdx = idx;
        repo->LoadPresentations(races[idx]);
        repo->LoadTraits(races[idx]);
        repo->LoadNoms(races[idx]);
        subraces = repo->LoadSubRaces(races[idx].id);
    } else {
        selectedIdx   = idx;
        selectedIsSub = true;
        selectedId    = subraces[idx].id;
        // parentRaceIdx reste inchangé (défini lors de la sélection du parent)
        // S'assurer que les données du parent sont bien chargées
        if (parentRaceIdx >= 0 && parentRaceIdx < (int)races.size()
            && races[parentRaceIdx].presentations.empty()) {
            repo->LoadPresentations(races[parentRaceIdx]);
            repo->LoadTraits(races[parentRaceIdx]);
            repo->LoadNoms(races[parentRaceIdx]);
        }
        repo->LoadPresentations(subraces[idx]);
        repo->LoadTraits(subraces[idx]);
        repo->LoadNoms(subraces[idx]);
    }
}

void RaceDd55Tool::StartEdit(const RaceDd55::RaceData& race, bool isNew) {
    editRace   = race;
    isNewRace  = isNew;
    editMode   = true;
    editError.clear();
    // Pour une modification, on s'assure d'avoir les données secondaires
    if (!isNew && !race.id.empty() && repo) {
        repo->LoadPresentations(editRace);
        repo->LoadTraits(editRace);
        repo->LoadNoms(editRace);
    }
}

void RaceDd55Tool::SaveEdit() {
    editError.clear();

    // Validation
    if (editRace.nom.empty()) {
        editError = "Le nom est requis.";
        return;
    }
    if (isNewRace) {
        if (editRace.alias.empty()) {
            editError = "L'alias (ID) est requis pour une nouvelle race.";
            return;
        }
        if (repo->ExistsId(editRace.alias)) {
            editError = "Cet alias est déjà utilisé : " + editRace.alias;
            return;
        }
    }
    if (repo->ExistsNom(editRace.nom, isNewRace ? "" : editRace.id)) {
        editError = "Ce nom est déjà utilisé par une autre race : " + editRace.nom;
        return;
    }

    repo->BeginTransaction();

    bool ok;
    if (isNewRace) {
        editRace.id = editRace.alias;
        ok = repo->InsertRace(editRace);
    } else {
        ok = repo->UpdateRace(editRace);
    }

    if (!ok) {
        repo->Rollback();
        editError = "Erreur lors de l'enregistrement en base de données.";
        return;
    }

    repo->SavePresentations(editRace.id, editRace.presentations);
    repo->SaveTraits(editRace.id, editRace.traits);
    repo->SaveNoms(editRace.id, editRace.noms);

    repo->Commit();

    // Rechargement
    races = repo->LoadAllRaces();
    selectedIsSub = false; selectedIdx = -1;
    for (int i = 0; i < (int)races.size(); ++i)
        if (races[i].id == editRace.id) { SelectRace(i, false); break; }

    editMode = false; isNewRace = false;
}

void RaceDd55Tool::CancelEdit() {
    editMode = false; isNewRace = false; editError.clear();
}

// ============================================================
// EXPORT C ABI
// ============================================================

extern "C" {
    PLUGIN_API Core::ToolBase* CreateTool() { return new RaceDd55Tool(); }
    PLUGIN_API void DestroyTool(Core::ToolBase* tool) { delete tool; }
}

double RaceDd55Tool::GetCurrentTime() {
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration<double>(now.time_since_epoch()).count();
}
