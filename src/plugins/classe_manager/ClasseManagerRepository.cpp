#include "ClasseManagerRepository.h"

// TODO : implémentation à écrire ensemble (cf. ClasseManagerRepository.cpp.old
// pour la logique v1 de référence, à adapter au schéma v2).

namespace ClasseManager {

ClasseManagerRepository::ClasseManagerRepository(sqlite3* db) : db(db) {}

// ── Lecture ─────────────────────────────────────────────────────────────────

std::vector<ClasseData> ClasseManagerRepository::LoadBaseClasses() { return {}; }
std::vector<ClasseData> ClasseManagerRepository::LoadSubClasses(const std::string& /*parent_id*/) { return {}; }
void ClasseManagerRepository::LoadChildData(ClasseData& /*c*/) {}

// ── Classe ──────────────────────────────────────────────────────────────────

bool ClasseManagerRepository::InsertClasse(const ClasseData& /*c*/) { return false; }
bool ClasseManagerRepository::UpdateClasse(const ClasseData& /*c*/) { return false; }
bool ClasseManagerRepository::DeleteClasse(const std::string& /*id*/) { return false; }

// ── Colonnes d'évolution dynamiques ─────────────────────────────────────────

bool ClasseManagerRepository::InsertEvolutionColonne(const EvolutionColonneData& /*col*/) { return false; }
bool ClasseManagerRepository::UpdateEvolutionColonne(const EvolutionColonneData& /*col*/) { return false; }
bool ClasseManagerRepository::DeleteEvolutionColonne(const std::string& /*id*/) { return false; }

// ── Évolution ───────────────────────────────────────────────────────────────

bool ClasseManagerRepository::InsertEvolution(const EvolutionData& /*e*/) { return false; }
bool ClasseManagerRepository::UpdateEvolution(const EvolutionData& /*e*/) { return false; }
bool ClasseManagerRepository::DeleteEvolution(const std::string& /*id*/) { return false; }

// ── Valeurs de colonnes dynamiques ───────────────────────────────────────────

bool ClasseManagerRepository::UpsertEvolutionValeur(const EvolutionValeurData& /*v*/) { return false; }
bool ClasseManagerRepository::DeleteEvolutionValeur(const std::string& /*id*/) { return false; }

// ── Choix de build — champ ──────────────────────────────────────────────────

bool ClasseManagerRepository::InsertChoixChamp(const ChoixChampData& /*c*/) { return false; }
bool ClasseManagerRepository::UpdateChoixChamp(const ChoixChampData& /*c*/) { return false; }
bool ClasseManagerRepository::DeleteChoixChamp(const std::string& /*id*/) { return false; }

// ── Choix de build — valeur ──────────────────────────────────────────────────

bool ClasseManagerRepository::InsertChoixValeur(const ChoixValeurData& /*v*/) { return false; }
bool ClasseManagerRepository::UpdateChoixValeur(const ChoixValeurData& /*v*/) { return false; }
bool ClasseManagerRepository::DeleteChoixValeur(const std::string& /*id*/) { return false; }

// ── Aptitude ────────────────────────────────────────────────────────────────

bool ClasseManagerRepository::InsertAptitude(const AptitudeData& /*a*/) { return false; }
bool ClasseManagerRepository::UpdateAptitude(const AptitudeData& /*a*/) { return false; }
bool ClasseManagerRepository::DeleteAptitude(const std::string& /*id*/) { return false; }

// ── Présentation ────────────────────────────────────────────────────────────

bool ClasseManagerRepository::InsertPresentation(const PresentationData& /*p*/) { return false; }
bool ClasseManagerRepository::UpdatePresentation(const PresentationData& /*p*/) { return false; }
bool ClasseManagerRepository::DeletePresentation(const std::string& /*id*/) { return false; }

// ── Équipement ──────────────────────────────────────────────────────────────

bool ClasseManagerRepository::InsertChoixEquipement(const ChoixEquipementData& /*e*/) { return false; }
bool ClasseManagerRepository::UpdateChoixEquipement(const ChoixEquipementData& /*e*/) { return false; }
bool ClasseManagerRepository::DeleteChoixEquipement(const std::string& /*id*/) { return false; }

// ── Transactions ────────────────────────────────────────────────────────────

void ClasseManagerRepository::BeginTransaction() {}
void ClasseManagerRepository::Commit() {}
void ClasseManagerRepository::Rollback() {}

// ── Utilitaires ─────────────────────────────────────────────────────────────

bool        ClasseManagerRepository::ExistsNom(const std::string& /*nom*/, const std::string& /*excludeId*/) { return false; }
std::string ClasseManagerRepository::GenerateId() { return {}; }

// ── row_to_* ────────────────────────────────────────────────────────────────

ClasseData           ClasseManagerRepository::row_to_classe(sqlite3_stmt* /*s*/) { return {}; }
EvolutionColonneData ClasseManagerRepository::row_to_evolution_colonne(sqlite3_stmt* /*s*/) { return {}; }
EvolutionData        ClasseManagerRepository::row_to_evolution(sqlite3_stmt* /*s*/) { return {}; }
AptitudeData         ClasseManagerRepository::row_to_aptitude(sqlite3_stmt* /*s*/) { return {}; }

} // namespace ClasseManager
