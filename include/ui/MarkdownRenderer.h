#pragma once

#include <functional>
#include <string>

namespace UI {

using LinkCallback = std::function<void(const std::string& plugin, const std::string& alias)>;

/**
 * @brief Rendu d'un texte pseudo-markdown dans une fenêtre ImGui courante.
 *
 * Syntaxe supportée :
 *   # / ## / ###  Titres
 *   **gras**       Couleur distincte (pas de vraie police bold)
 *   *italique*     Couleur distincte
 *   - item         Bullet point
 *   ---            Séparateur horizontal
 *   | col | col |  Tableau (2e ligne = séparateurs, ignorée)
 *   @plugin.alias  Lien cliquable → onLink("plugin", "alias")
 *
 * Doit être appelé à l'intérieur d'un ImGui::Begin/End.
 *
 * @param text   Contenu à rendre
 * @param onLink Callback déclenché au clic sur un alias (@plugin.alias). Peut être nullptr.
 */
void RenderMarkdown(const std::string& text, const LinkCallback& onLink = nullptr);

} // namespace UI
