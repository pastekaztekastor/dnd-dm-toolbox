#include "ClasseManagerTool.h"
#include "../../include/core/EventBus.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>
#include <cstring>

using namespace ClasseManager;

void ClasseManagerTool::OnCreate() {
    sqlite3_open("plugins/classe_manager.db", &pluginDb);

    bool needsSchema = true;
    sqlite3_stmt* ck;
    if (sqlite3_prepare_v2(pluginDb,
            "SELECT name FROM sqlite_master WHERE type='table' AND name='classes'",
            -1, &ck, nullptr) == SQLITE_OK) {
        needsSchema = sqlite3_step(ck) != SQLITE_ROW;
        sqlite3_finalize(ck);
    }
    if (needsSchema) {
        std::ifstream f("plugins/classe_manager_schema.sql");
        if (f) { std::stringstream buf; buf << f.rdbuf();
            sqlite3_exec(pluginDb, buf.str().c_str(), nullptr, nullptr, nullptr); }
    }

    // Migration idempotente : nouvelles tables et colonnes (ignorées si déjà présentes)
    sqlite3_exec(pluginDb,
        "CREATE TABLE IF NOT EXISTS classe_choix_champ ("
        "  id TEXT PRIMARY KEY,"
        "  classe_id TEXT NOT NULL REFERENCES classes(id) ON DELETE CASCADE,"
        "  nom TEXT NOT NULL,"
        "  alias TEXT NOT NULL,"
        "  niveau INTEGER NOT NULL DEFAULT 1,"
        "  description TEXT,"
        "  UNIQUE(classe_id, alias));"
        "CREATE TABLE IF NOT EXISTS classe_choix_valeur ("
        "  id TEXT PRIMARY KEY,"
        "  champ_id TEXT NOT NULL REFERENCES classe_choix_champ(id) ON DELETE CASCADE,"
        "  nom TEXT NOT NULL,"
        "  valeur TEXT NOT NULL,"
        "  description TEXT,"
        "  UNIQUE(champ_id, valeur));",
        nullptr, nullptr, nullptr);
    sqlite3_exec(pluginDb,
        "ALTER TABLE classe_aptitudes ADD COLUMN choix_champ_id TEXT;",
        nullptr, nullptr, nullptr);
    sqlite3_exec(pluginDb,
        "ALTER TABLE classe_aptitudes ADD COLUMN choix_valeur_id TEXT;",
        nullptr, nullptr, nullptr);
    sqlite3_exec(pluginDb,
        "ALTER TABLE classe_aptitudes ADD COLUMN aptitude_prerequisite_id TEXT;",
        nullptr, nullptr, nullptr);

    int count = 0;
    sqlite3_stmt* cnt;
    if (sqlite3_prepare_v2(pluginDb, "SELECT COUNT(*) FROM classes", -1, &cnt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(cnt) == SQLITE_ROW) count = sqlite3_column_int(cnt, 0);
        sqlite3_finalize(cnt);
    }
    if (count == 0) {
        for (const char* seed : {
                "plugins/seed_barbare.sql",
                "plugins/seed_barde.sql",
                "plugins/seed_clerc.sql",
                "plugins/seed_druide.sql",
                "plugins/seed_ensorceleur.sql",
                "plugins/seed_guerrier.sql",
                "plugins/seed_magicien.sql",
                "plugins/seed_moine.sql",
                "plugins/seed_occultiste.sql",
                "plugins/seed_paladin.sql",
                "plugins/seed_rodeur.sql",
                "plugins/seed_roublard.sql"}) {
            std::ifstream f(seed);
            if (f) { std::stringstream buf; buf << f.rdbuf();
                sqlite3_exec(pluginDb, buf.str().c_str(), nullptr, nullptr, nullptr); }
        }
    }

    repo = std::make_unique<ClasseManagerRepository>(pluginDb);
    LoadClasses();
    if (!classes.empty()) SelectClasse(0);
}

void ClasseManagerTool::OnDestroy() {
    repo.reset();
    if (pluginDb) { sqlite3_close(pluginDb); pluginDb = nullptr; }
}

void ClasseManagerTool::OnSave(sqlite3* db) {
    if (!db) return;
    nlohmann::json state;
    state["selected_idx"] = selectedIdx;
    const char* sql = "INSERT OR REPLACE INTO tool_instances (id, tool_type, data) VALUES (?, ?, ?)";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, instanceID.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, GetID().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, state.dump().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt); sqlite3_finalize(stmt);
    }
}

void ClasseManagerTool::OnLoad(sqlite3* db) {
    if (!db) return;
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, "SELECT data FROM tool_instances WHERE id = ?", -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, instanceID.c_str(), -1, SQLITE_TRANSIENT);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* j = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            if (j && strlen(j) > 0) {
                auto state = nlohmann::json::parse(j, nullptr, false);
                if (!state.is_discarded() && state.contains("selected_idx")) {
                    int idx = state["selected_idx"].get<int>();
                    if (idx >= 0 && idx < (int)classes.size()) SelectClasse(idx);
                }
            }
        }
        sqlite3_finalize(stmt);
    }
}

void ClasseManagerTool::OnEvent(const Core::Event& /*event*/) {}

void ClasseManagerTool::LoadClasses() {
    if (!repo) return;
    classes = repo->LoadBaseClasses();
}

void ClasseManagerTool::SelectClasse(int idx) {
    if (!repo || idx < 0 || idx >= (int)classes.size()) return;
    selectedIdx  = idx;
    detailLoaded = true;
    repo->LoadChildData(classes[idx]);
    BuildTree(classes[idx]);
}
