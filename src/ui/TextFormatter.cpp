#include "../../include/ui/TextFormatter.h"
#include <sstream>
#include <algorithm>
#include <regex>

namespace UI {

// ============================================================================
// DÉFINITION DES COULEURS
// ============================================================================

const ImVec4 TextFormatter::COLOR_TITLE_1(1.0f, 0.8f, 0.0f, 1.0f);       // Or
const ImVec4 TextFormatter::COLOR_TITLE_2(0.4f, 0.8f, 1.0f, 1.0f);       // Cyan clair
const ImVec4 TextFormatter::COLOR_TITLE_3(1.0f, 1.0f, 1.0f, 1.0f);       // Blanc
const ImVec4 TextFormatter::COLOR_DICE(0.2f, 1.0f, 0.2f, 1.0f);          // Vert
const ImVec4 TextFormatter::COLOR_ABILITY(0.5f, 0.8f, 1.0f, 1.0f);       // Cyan
const ImVec4 TextFormatter::COLOR_LINK(0.3f, 0.6f, 1.0f, 1.0f);          // Bleu
const ImVec4 TextFormatter::COLOR_LINK_HOVER(0.5f, 0.8f, 1.0f, 1.0f);    // Bleu clair
const ImVec4 TextFormatter::COLOR_BOLD(1.0f, 1.0f, 1.0f, 1.0f);          // Blanc pur
const ImVec4 TextFormatter::COLOR_ITALIC(0.8f, 0.8f, 0.8f, 1.0f);        // Gris clair
const ImVec4 TextFormatter::COLOR_TABLE_HEADER(0.2f, 0.2f, 0.3f, 1.0f);  // Gris foncé
const ImVec4 TextFormatter::COLOR_TABLE_ROW_BG(0.1f, 0.1f, 0.15f, 0.5f); // Gris très foncé

// ============================================================================
// CONSTRUCTEUR / DESTRUCTEUR
// ============================================================================

TextFormatter::TextFormatter()
    : inTable(false)
{
}

TextFormatter::~TextFormatter() {
}

// ============================================================================
// PARSING PRINCIPAL
// ============================================================================

ParsedDocument* TextFormatter::Parse(const std::string& formattedText) {
    ParsedDocument* doc = new ParsedDocument();

    std::istringstream stream(formattedText);
    std::string line;

    while (std::getline(stream, line)) {
        ParseLine(line, doc);
    }

    // Finaliser le tableau si on était dedans
    if (inTable && !currentTableRows.empty()) {
        auto* segment = new TextSegment(ContentType::TableRow);
        segment->tableData = currentTableRows;
        doc->segments.push_back(segment);
        currentTableRows.clear();
        inTable = false;
    }

    return doc;
}

void TextFormatter::ParseLine(const std::string& line, ParsedDocument* doc) {
    // Ligne vide = espacement
    if (line.empty() || line.find_first_not_of(' ') == std::string::npos) {
        if (inTable) {
            // Fin du tableau
            if (!currentTableRows.empty()) {
                auto* segment = new TextSegment(ContentType::TableRow);
                segment->tableData = currentTableRows;
                doc->segments.push_back(segment);
                currentTableRows.clear();
            }
            inTable = false;
        }
        auto* segment = new TextSegment(ContentType::Separator);
        doc->segments.push_back(segment);
        return;
    }

    // Titres
    int titleLevel = 0;
    if (IsTitle(line, titleLevel)) {
        if (inTable) {
            // Fin du tableau
            if (!currentTableRows.empty()) {
                auto* segment = new TextSegment(ContentType::TableRow);
                segment->tableData = currentTableRows;
                doc->segments.push_back(segment);
                currentTableRows.clear();
            }
            inTable = false;
        }

        std::string titleText = line.substr(titleLevel);
        // Trim spaces
        size_t start = titleText.find_first_not_of(' ');
        if (start != std::string::npos) {
            titleText = titleText.substr(start);
        }

        auto* segment = new TextSegment(
            titleLevel == 1 ? ContentType::Title1 :
            titleLevel == 2 ? ContentType::Title2 :
            ContentType::Title3,
            titleText
        );
        doc->segments.push_back(segment);
        return;
    }

    // Bullet points
    if (IsBullet(line)) {
        if (inTable) {
            if (!currentTableRows.empty()) {
                auto* segment = new TextSegment(ContentType::TableRow);
                segment->tableData = currentTableRows;
                doc->segments.push_back(segment);
                currentTableRows.clear();
            }
            inTable = false;
        }

        std::string bulletText = line.substr(line.find('-') + 1);
        size_t start = bulletText.find_first_not_of(' ');
        if (start != std::string::npos) {
            bulletText = bulletText.substr(start);
        }

        auto* segment = new TextSegment(ContentType::BulletPoint, bulletText);
        doc->segments.push_back(segment);

        // Parser le contenu inline du bullet
        ParseInlineFormatting(bulletText, doc);
        return;
    }

    // Tableaux
    if (IsTableRow(line)) {
        auto cells = SplitTableCells(line);

        // Vérifier si c'est un séparateur (ligne de tirets)
        if (IsTableSeparator(line)) {
            // Ignorer la ligne de séparation
            return;
        }

        currentTableRows.push_back(cells);
        inTable = true;
        return;
    } else if (inTable) {
        // Fin du tableau
        if (!currentTableRows.empty()) {
            auto* segment = new TextSegment(ContentType::TableRow);
            segment->tableData = currentTableRows;
            doc->segments.push_back(segment);
            currentTableRows.clear();
        }
        inTable = false;
    }

    // Texte normal avec formatage inline
    ParseInlineFormatting(line, doc);
}

// ============================================================================
// PARSING INLINE (formules, liens, gras, italique)
// ============================================================================

void TextFormatter::ParseInlineFormatting(const std::string& text, ParsedDocument* doc) {
    auto matches = FindInlineMatches(text);

    if (matches.empty()) {
        // Pas de formatage spécial, juste du texte
        auto* segment = new TextSegment(ContentType::Text, text);
        doc->segments.push_back(segment);
        return;
    }

    // Parser les segments entre les matches
    size_t lastPos = 0;
    for (const auto& match : matches) {
        // Texte avant le match
        if (match.start > lastPos) {
            std::string before = text.substr(lastPos, match.start - lastPos);
            if (!before.empty()) {
                auto* segment = new TextSegment(ContentType::Text, before);
                doc->segments.push_back(segment);
            }
        }

        // Le match lui-même
        if (match.type == InlineMatch::Formula) {
            auto* segment = new TextSegment(ContentType::Formula, match.content);
            doc->segments.push_back(segment);
        } else if (match.type == InlineMatch::Link) {
            Link* link = ParseLink(match.content);
            if (link) {
                auto* segment = new TextSegment(ContentType::Link);
                segment->link = link;
                doc->segments.push_back(segment);
            }
        } else if (match.type == InlineMatch::Bold) {
            auto* segment = new TextSegment(ContentType::Bold, match.content);
            doc->segments.push_back(segment);
        } else if (match.type == InlineMatch::Italic) {
            auto* segment = new TextSegment(ContentType::Italic, match.content);
            doc->segments.push_back(segment);
        }

        lastPos = match.end;
    }

    // Texte après le dernier match
    if (lastPos < text.length()) {
        std::string after = text.substr(lastPos);
        if (!after.empty()) {
            auto* segment = new TextSegment(ContentType::Text, after);
            doc->segments.push_back(segment);
        }
    }
}

std::vector<TextFormatter::InlineMatch> TextFormatter::FindInlineMatches(const std::string& text) {
    std::vector<InlineMatch> matches;

    // Chercher les formules {xxx}
    size_t pos = 0;
    while ((pos = text.find('{', pos)) != std::string::npos) {
        size_t end = text.find('}', pos);
        if (end != std::string::npos) {
            InlineMatch match;
            match.type = InlineMatch::Formula;
            match.start = pos;
            match.end = end + 1;
            match.content = text.substr(pos + 1, end - pos - 1);
            matches.push_back(match);
            pos = end + 1;
        } else {
            break;
        }
    }

    // Chercher les liens [xxx](yyy)
    pos = 0;
    while ((pos = text.find('[', pos)) != std::string::npos) {
        size_t endBracket = text.find(']', pos);
        if (endBracket != std::string::npos && endBracket + 1 < text.length() && text[endBracket + 1] == '(') {
            size_t endParen = text.find(')', endBracket + 2);
            if (endParen != std::string::npos) {
                InlineMatch match;
                match.type = InlineMatch::Link;
                match.start = pos;
                match.end = endParen + 1;
                match.content = text.substr(pos, endParen + 1 - pos);
                matches.push_back(match);
                pos = endParen + 1;
            } else {
                pos++;
            }
        } else {
            pos++;
        }
    }

    // Chercher **gras**
    pos = 0;
    while ((pos = text.find("**", pos)) != std::string::npos) {
        size_t end = text.find("**", pos + 2);
        if (end != std::string::npos) {
            InlineMatch match;
            match.type = InlineMatch::Bold;
            match.start = pos;
            match.end = end + 2;
            match.content = text.substr(pos + 2, end - pos - 2);
            matches.push_back(match);
            pos = end + 2;
        } else {
            break;
        }
    }

    // Chercher *italique* (mais pas **)
    pos = 0;
    while ((pos = text.find('*', pos)) != std::string::npos) {
        // Vérifier que ce n'est pas **
        if (pos + 1 < text.length() && text[pos + 1] == '*') {
            pos += 2;
            continue;
        }

        size_t end = pos + 1;
        while (end < text.length()) {
            if (text[end] == '*' && (end + 1 >= text.length() || text[end + 1] != '*')) {
                InlineMatch match;
                match.type = InlineMatch::Italic;
                match.start = pos;
                match.end = end + 1;
                match.content = text.substr(pos + 1, end - pos - 1);
                matches.push_back(match);
                pos = end + 1;
                break;
            }
            end++;
        }
        if (end >= text.length()) break;
    }

    // Trier les matches par position de départ
    std::sort(matches.begin(), matches.end(),
        [](const InlineMatch& a, const InlineMatch& b) { return a.start < b.start; });

    return matches;
}

Link* TextFormatter::ParseLink(const std::string& linkText) {
    // Format: [Texte](type:id)
    size_t bracketEnd = linkText.find(']');
    size_t parenStart = linkText.find('(');

    if (bracketEnd == std::string::npos || parenStart == std::string::npos) {
        return nullptr;
    }

    std::string displayText = linkText.substr(1, bracketEnd - 1);
    std::string target = linkText.substr(parenStart + 1, linkText.find(')') - parenStart - 1);

    // Parser type:id
    size_t colonPos = target.find(':');
    if (colonPos == std::string::npos) {
        return nullptr;
    }

    std::string typeStr = target.substr(0, colonPos);
    std::string targetId = target.substr(colonPos + 1);

    LinkType type = ParseLinkType(typeStr);

    return new Link(displayText, type, targetId);
}

LinkType TextFormatter::ParseLinkType(const std::string& typeStr) {
    if (typeStr == "spell") return LinkType::Spell;
    if (typeStr == "skill") return LinkType::Skill;
    if (typeStr == "racial_ability") return LinkType::RacialAbility;
    if (typeStr == "class_feature") return LinkType::ClassFeature;
    if (typeStr == "weapon") return LinkType::Weapon;
    if (typeStr == "armor") return LinkType::Armor;
    if (typeStr == "item") return LinkType::Item;
    if (typeStr == "ability") return LinkType::Ability;
    if (typeStr == "saving_throw") return LinkType::SavingThrow;
    return LinkType::Unknown;
}

// ============================================================================
// DÉTECTION DE FORMATS
// ============================================================================

bool TextFormatter::IsTitle(const std::string& line, int& level) {
    if (line.empty()) return false;

    int hashCount = 0;
    for (char c : line) {
        if (c == '#') {
            hashCount++;
        } else if (c == ' ') {
            break;
        } else {
            return false;
        }
    }

    if (hashCount >= 1 && hashCount <= 3) {
        level = hashCount;
        return true;
    }

    return false;
}

bool TextFormatter::IsBullet(const std::string& line) {
    size_t firstNonSpace = line.find_first_not_of(' ');
    if (firstNonSpace == std::string::npos) return false;
    return line[firstNonSpace] == '-' && (firstNonSpace + 1 >= line.length() || line[firstNonSpace + 1] == ' ');
}

bool TextFormatter::IsTableRow(const std::string& line) {
    return !line.empty() && line[0] == '|';
}

bool TextFormatter::IsTableSeparator(const std::string& line) {
    // Ligne de type |---|---|---|
    if (!IsTableRow(line)) return false;

    for (char c : line) {
        if (c != '|' && c != '-' && c != ' ') {
            return false;
        }
    }
    return true;
}

std::vector<std::string> TextFormatter::SplitTableCells(const std::string& line) {
    std::vector<std::string> cells;
    std::string cell;
    bool inCell = false;

    for (size_t i = 0; i < line.length(); ++i) {
        if (line[i] == '|') {
            if (inCell) {
                // Trim spaces
                size_t start = cell.find_first_not_of(' ');
                size_t end = cell.find_last_not_of(' ');
                if (start != std::string::npos) {
                    cells.push_back(cell.substr(start, end - start + 1));
                } else {
                    cells.push_back("");
                }
                cell.clear();
            }
            inCell = true;
        } else {
            if (inCell) {
                cell += line[i];
            }
        }
    }

    return cells;
}

// ============================================================================
// RENDERING
// ============================================================================

void TextFormatter::Render(ParsedDocument* doc, LinkClickCallback onLinkClick) {
    for (auto* segment : doc->segments) {
        RenderTextSegment(segment, onLinkClick);
    }
}

void TextFormatter::RenderFormatted(const std::string& formattedText, LinkClickCallback onLinkClick) {
    ParsedDocument* doc = Parse(formattedText);
    Render(doc, onLinkClick);
    delete doc;
}

void TextFormatter::RenderTextSegment(TextSegment* segment, LinkClickCallback& callback) {
    switch (segment->type) {
        case ContentType::Title1:
        case ContentType::Title2:
        case ContentType::Title3:
            RenderTitle(segment->content,
                segment->type == ContentType::Title1 ? 1 :
                segment->type == ContentType::Title2 ? 2 : 3);
            break;

        case ContentType::Text:
            ImGui::TextWrapped("%s", segment->content.c_str());
            break;

        case ContentType::BulletPoint:
            // Ne rien faire ici, c'est géré par ParseInlineFormatting
            break;

        case ContentType::Formula:
            RenderFormula(segment->content);
            break;

        case ContentType::Link:
            if (segment->link) {
                RenderLink(segment->link, callback);
            }
            break;

        case ContentType::TableRow:
            RenderTable(segment->tableData);
            break;

        case ContentType::Bold:
            ImGui::TextColored(COLOR_BOLD, "%s", segment->content.c_str());
            ImGui::SameLine(0, 0);
            break;

        case ContentType::Italic:
            ImGui::TextColored(COLOR_ITALIC, "%s", segment->content.c_str());
            ImGui::SameLine(0, 0);
            break;

        case ContentType::Separator:
            ImGui::Spacing();
            break;
    }
}

void TextFormatter::RenderTitle(const std::string& text, int level) {
    ImGui::Separator();

    const ImVec4& color = level == 1 ? COLOR_TITLE_1 :
                          level == 2 ? COLOR_TITLE_2 :
                          COLOR_TITLE_3;

    ImGui::TextColored(color, "%s", text.c_str());
    ImGui::Separator();
}

void TextFormatter::RenderFormula(const std::string& formula) {
    // Déterminer si c'est un dé ou une caractéristique
    bool isDice = formula.find('d') != std::string::npos || formula.find('D') != std::string::npos;

    const ImVec4& color = isDice ? COLOR_DICE : COLOR_ABILITY;
    const char* icon = isDice ? "🎲 " : "";

    ImGui::TextColored(color, "%s%s", icon, formula.c_str());
    ImGui::SameLine(0, 2);
}

void TextFormatter::RenderLink(Link* link, LinkClickCallback& callback) {
    bool hovered = false;

    ImGui::TextColored(COLOR_LINK, "%s", link->displayText.c_str());

    if (ImGui::IsItemHovered()) {
        hovered = true;
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::SetTooltip("Cliquer pour voir les détails");

        // Changer la couleur au survol
        ImGui::SameLine(0, 0);
        ImGui::TextColored(COLOR_LINK_HOVER, "");
    }

    if (ImGui::IsItemClicked() && callback) {
        callback(link->type, link->targetId);
    }

    ImGui::SameLine(0, 2);
}

void TextFormatter::RenderTable(const std::vector<std::vector<std::string>>& rows) {
    if (rows.empty()) return;

    int colCount = rows[0].size();

    if (ImGui::BeginTable("formatted_table", colCount,
                         ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {

        // Header row
        for (const auto& cell : rows[0]) {
            ImGui::TableSetupColumn(cell.c_str());
        }
        ImGui::TableHeadersRow();

        // Data rows
        for (size_t i = 1; i < rows.size(); ++i) {
            ImGui::TableNextRow();
            for (const auto& cell : rows[i]) {
                ImGui::TableNextColumn();
                ImGui::Text("%s", cell.c_str());
            }
        }

        ImGui::EndTable();
    }
}

} // namespace UI
