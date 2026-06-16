#include "../../include/ui/MarkdownRenderer.h"
#include <imgui.h>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>

namespace UI {

// ============================================================
// Tokens inline
// ============================================================

struct Token {
    enum class Kind { Text, Bold, Italic, Link };
    Kind        kind    = Kind::Text;
    std::string content;   // texte affiché
    std::string plugin;    // pour Kind::Link
    std::string alias;     // pour Kind::Link
};

static bool IsAlphaNumOrUnderscore(char c) {
    return std::isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '-';
}

// Parse une ligne en liste de tokens inline
static std::vector<Token> ParseInline(const std::string& line) {
    std::vector<Token> tokens;
    size_t i = 0;
    size_t n = line.size();
    std::string buf;

    auto flushText = [&]() {
        if (!buf.empty()) {
            tokens.push_back({ Token::Kind::Text, buf });
            buf.clear();
        }
    };

    while (i < n) {
        // Bold **...**
        if (i + 1 < n && line[i] == '*' && line[i + 1] == '*') {
            flushText();
            i += 2;
            size_t end = line.find("**", i);
            if (end != std::string::npos) {
                tokens.push_back({ Token::Kind::Bold, line.substr(i, end - i) });
                i = end + 2;
            } else {
                // Non fermé → texte brut
                buf += "**";
            }
            continue;
        }

        // Italic *...*
        if (line[i] == '*') {
            flushText();
            i += 1;
            size_t end = line.find('*', i);
            if (end != std::string::npos) {
                tokens.push_back({ Token::Kind::Italic, line.substr(i, end - i) });
                i = end + 1;
            } else {
                buf += '*';
            }
            continue;
        }

        // Alias @plugin.alias
        if (line[i] == '@') {
            flushText();
            i += 1;
            size_t pluginStart = i;
            while (i < n && IsAlphaNumOrUnderscore(line[i])) ++i;
            std::string plugin = line.substr(pluginStart, i - pluginStart);

            if (i < n && line[i] == '.' && !plugin.empty()) {
                ++i;
                size_t aliasStart = i;
                while (i < n && IsAlphaNumOrUnderscore(line[i])) ++i;
                std::string alias = line.substr(aliasStart, i - aliasStart);
                if (!alias.empty()) {
                    Token t;
                    t.kind    = Token::Kind::Link;
                    t.content = alias; // texte affiché = alias
                    t.plugin  = plugin;
                    t.alias   = alias;
                    tokens.push_back(t);
                    continue;
                }
            }
            // Fallback : pas un alias valide
            buf += '@';
            buf += plugin;
            continue;
        }

        buf += line[i++];
    }

    flushText();
    return tokens;
}

static bool HasFormatting(const std::vector<Token>& tokens) {
    for (auto& t : tokens)
        if (t.kind != Token::Kind::Text) return true;
    return false;
}

// ============================================================
// Rendu inline avec wrapping manuel
// ============================================================

struct InlineState {
    bool  firstOnLine = true;
    float curX        = 0.0f;  // bord droit du dernier item rendu (screen space)
    float spaceW      = 0.0f;
};

// Rend un mot individuel avec gestion du wrapping
// wrapX = bord droit du contenu en screen space (calculé une fois par ligne)
static void RenderWord(const char* word, const ImVec4& color, bool useColor,
                       InlineState& s, float wrapX, bool spaceBefore = true) {
    float wordW = ImGui::CalcTextSize(word).x;

    if (!s.firstOnLine) {
        float gap  = spaceBefore ? s.spaceW : 0.0f;
        float endX = s.curX + gap + wordW;
        if (endX > wrapX) {
            // TextUnformatted a déjà avancé le curseur à la ligne suivante — pas de NewLine()
            s.firstOnLine = true;
            s.curX = ImGui::GetCursorScreenPos().x;
        } else {
            ImGui::SameLine(0, gap);
        }
    } else {
        s.curX = ImGui::GetCursorScreenPos().x;
    }

    if (useColor)
        ImGui::TextColored(color, "%s", word);
    else
        ImGui::TextUnformatted(word);

    s.curX        = ImGui::GetItemRectMax().x;
    s.firstOnLine = false;
    s.spaceW      = ImGui::CalcTextSize(" ").x;
}

static void RenderInlineLine(const std::vector<Token>& tokens, const LinkCallback& onLink) {
    InlineState s;
    s.spaceW = ImGui::CalcTextSize(" ").x;
    float wrapX = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

    for (auto& tok : tokens) {
        switch (tok.kind) {
            case Token::Kind::Text: {
                std::istringstream ss(tok.content);
                std::string word;
                bool first = true;
                while (std::getline(ss, word, ' ')) {
                    if (word.empty()) continue;
                    RenderWord(word.c_str(), {}, false, s, wrapX, !first || !s.firstOnLine);
                    first = false;
                }
                break;
            }
            case Token::Kind::Bold: {
                static const ImVec4 cBold{1.0f, 0.95f, 0.6f, 1.0f};
                RenderWord(tok.content.c_str(), cBold, true, s, wrapX);
                break;
            }
            case Token::Kind::Italic: {
                static const ImVec4 cItalic{0.75f, 0.9f, 1.0f, 1.0f};
                RenderWord(tok.content.c_str(), cItalic, true, s, wrapX);
                break;
            }
            case Token::Kind::Link: {
                static const ImVec4 cLink{0.35f, 0.75f, 1.0f, 1.0f};
                float segW = ImGui::CalcTextSize(tok.content.c_str()).x
                           + ImGui::CalcTextSize("[]").x;

                if (!s.firstOnLine) {
                    float endX = s.curX + s.spaceW + segW;
                    if (endX > wrapX) {
                        s.firstOnLine = true;
                        s.curX = ImGui::GetCursorScreenPos().x;
                    } else {
                        ImGui::SameLine(0, s.spaceW);
                    }
                } else {
                    s.curX = ImGui::GetCursorScreenPos().x;
                }

                ImGui::TextColored(cLink, "[%s]", tok.content.c_str());
                if (ImGui::IsItemHovered()) {
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    ImGui::SetTooltip("%s.%s", tok.plugin.c_str(), tok.alias.c_str());
                }
                if (ImGui::IsItemClicked() && onLink)
                    onLink(tok.plugin, tok.alias);
                s.curX        = ImGui::GetItemRectMax().x;
                s.firstOnLine = false;
                s.spaceW      = ImGui::CalcTextSize(" ").x;
                break;
            }
        }
    }
}

// ============================================================
// Tables markdown
// ============================================================

static std::vector<std::string> SplitCells(const std::string& row) {
    std::vector<std::string> cells;
    std::istringstream ss(row);
    std::string cell;
    while (std::getline(ss, cell, '|')) {
        // Trim
        size_t start = cell.find_first_not_of(' ');
        size_t end   = cell.find_last_not_of(' ');
        if (start == std::string::npos) continue;
        cells.push_back(cell.substr(start, end - start + 1));
    }
    return cells;
}

static bool IsSeparatorRow(const std::string& row) {
    for (char c : row)
        if (c != '|' && c != '-' && c != ' ' && c != ':') return false;
    return row.find('-') != std::string::npos;
}

static void RenderTable(const std::vector<std::string>& tableLines, int tableIdx) {
    if (tableLines.empty()) return;

    auto headers = SplitCells(tableLines[0]);
    if (headers.empty()) return;

    int nbCols = static_cast<int>(headers.size());
    std::string tableId = "##mdtbl_" + std::to_string(tableIdx);

    if (ImGui::BeginTable(tableId.c_str(), nbCols,
                          ImGuiTableFlags_Borders |
                          ImGuiTableFlags_SizingFixedFit |
                          ImGuiTableFlags_RowBg)) {
        for (auto& h : headers)
            ImGui::TableSetupColumn(h.c_str());
        ImGui::TableHeadersRow();

        for (size_t r = 1; r < tableLines.size(); ++r) {
            if (IsSeparatorRow(tableLines[r])) continue;
            auto cells = SplitCells(tableLines[r]);
            ImGui::TableNextRow();
            for (int c = 0; c < nbCols; ++c) {
                ImGui::TableNextColumn();
                if (c < (int)cells.size())
                    ImGui::TextUnformatted(cells[c].c_str());
            }
        }
        ImGui::EndTable();
    }
}

// ============================================================
// Entrée publique
// ============================================================

void RenderMarkdown(const std::string& text, const LinkCallback& onLink) {
    if (text.empty()) return;

    // Couleurs des titres
    static const ImVec4 cH1{1.0f, 1.0f, 1.0f, 1.0f};
    static const ImVec4 cH2{0.82f, 0.87f, 1.0f, 1.0f};
    static const ImVec4 cH3{0.62f, 0.72f, 0.82f, 1.0f};

    std::istringstream stream(text);
    std::string line;

    std::vector<std::string> tableBuffer;
    int tableIdx = 0;

    auto flushTable = [&]() {
        if (!tableBuffer.empty()) {
            RenderTable(tableBuffer, tableIdx++);
            tableBuffer.clear();
        }
    };

    while (std::getline(stream, line)) {
        // Enlever le \r terminal si présent (CRLF)
        if (!line.empty() && line.back() == '\r') line.pop_back();

        // Accumulation de table
        if (!line.empty() && line.front() == '|') {
            tableBuffer.push_back(line);
            continue;
        }
        flushTable(); // fin de bloc table

        // Ligne vide
        if (line.empty()) {
            ImGui::Spacing();
            continue;
        }

        // Séparateur ---
        if (line == "---" || line == "***") {
            ImGui::Separator();
            continue;
        }

        // Titres ###
        if (line.rfind("### ", 0) == 0) {
            ImGui::Spacing();
            ImGui::TextColored(cH3, "%s", line.substr(4).c_str());
            continue;
        }
        if (line.rfind("## ", 0) == 0) {
            ImGui::Spacing();
            ImGui::TextColored(cH2, "%s", line.substr(3).c_str());
            ImGui::Separator();
            continue;
        }
        if (line.rfind("# ", 0) == 0) {
            ImGui::Spacing();
            ImGui::TextColored(cH1, "%s", line.substr(2).c_str());
            ImGui::Separator();
            continue;
        }

        // Bullet - item
        if (line.rfind("- ", 0) == 0) {
            auto tokens = ParseInline(line.substr(2));
            ImGui::TextUnformatted("\xe2\x80\xa2"); // •
            ImGui::SameLine(0, ImGui::CalcTextSize(" ").x);
            if (HasFormatting(tokens)) {
                RenderInlineLine(tokens, onLink);
            } else {
                ImGui::TextWrapped("%s", line.substr(2).c_str());
            }
            continue;
        }

        // Ligne ordinaire
        auto tokens = ParseInline(line);
        if (HasFormatting(tokens)) {
            RenderInlineLine(tokens, onLink);
        } else {
            ImGui::TextWrapped("%s", line.c_str());
        }
    }

    flushTable(); // table en fin de texte
}

} // namespace UI
