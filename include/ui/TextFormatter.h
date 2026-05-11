#pragma once

#include <string>
#include <vector>
#include <functional>
#include <imgui.h>

namespace UI {

// ============================================================================
// TYPES DE CONTENU
// ============================================================================

enum class ContentType {
    Text,           // Texte normal
    Title1,         // # Titre principal
    Title2,         // ## Sous-titre
    Title3,         // ### Sous-sous-titre
    BulletPoint,    // - Liste à puces
    Formula,        // {2d6+3} formules de dés
    Link,           // [texte](type:id) lien cliquable
    TableRow,       // Ligne de tableau
    Bold,           // **texte**
    Italic,         // *texte*
    Separator       // Ligne horizontale
};

// Types de liens supportés
enum class LinkType {
    Spell,          // spell:fireball
    Skill,          // skill:perception
    RacialAbility,  // racial_ability:darkvision
    ClassFeature,   // class_feature:rage
    Weapon,         // weapon:longbow
    Armor,          // armor:chainmail
    Item,           // item:potion_healing
    Ability,        // ability:strength
    SavingThrow,    // saving_throw:dexterity
    Unknown
};

// ============================================================================
// STRUCTURES DE DONNÉES
// ============================================================================

// Représente un lien cliquable
struct Link {
    std::string displayText;  // Texte affiché
    LinkType type;            // Type de lien
    std::string targetId;     // ID de l'élément ciblé

    Link(const std::string& text, LinkType t, const std::string& id)
        : displayText(text), type(t), targetId(id) {}
};

// Segment de texte avec son type
struct TextSegment {
    ContentType type;
    std::string content;
    Link* link;  // Non-null si type == Link
    std::vector<std::vector<std::string>> tableData;  // Si type == TableRow

    TextSegment(ContentType t, const std::string& c = "")
        : type(t), content(c), link(nullptr) {}

    ~TextSegment() {
        if (link) delete link;
    }
};

// Document parsé complet
struct ParsedDocument {
    std::vector<TextSegment*> segments;

    ~ParsedDocument() {
        for (auto seg : segments) delete seg;
    }
};

// ============================================================================
// CLASSE PRINCIPALE: TEXT FORMATTER
// ============================================================================

class TextFormatter {
public:
    // Couleurs par défaut
    static const ImVec4 COLOR_TITLE_1;      // Or
    static const ImVec4 COLOR_TITLE_2;      // Cyan clair
    static const ImVec4 COLOR_TITLE_3;      // Blanc
    static const ImVec4 COLOR_DICE;         // Vert (formules de dés)
    static const ImVec4 COLOR_ABILITY;      // Cyan (caractéristiques)
    static const ImVec4 COLOR_LINK;         // Bleu lien
    static const ImVec4 COLOR_LINK_HOVER;   // Bleu clair au survol
    static const ImVec4 COLOR_BOLD;         // Blanc pur
    static const ImVec4 COLOR_ITALIC;       // Gris clair
    static const ImVec4 COLOR_TABLE_HEADER; // Gris foncé
    static const ImVec4 COLOR_TABLE_ROW_BG; // Gris très foncé

    // Callback appelé quand on clique sur un lien
    using LinkClickCallback = std::function<void(LinkType type, const std::string& targetId)>;

    TextFormatter();
    ~TextFormatter();

    // Parser un texte formaté
    ParsedDocument* Parse(const std::string& formattedText);

    // Rendre un document parsé avec ImGui
    void Render(ParsedDocument* doc, LinkClickCallback onLinkClick = nullptr);

    // Shortcut: parser et rendre directement
    void RenderFormatted(const std::string& formattedText,
                        LinkClickCallback onLinkClick = nullptr);

private:
    // Parsing
    void ParseLine(const std::string& line, ParsedDocument* doc);
    void ParseInlineFormatting(const std::string& text, ParsedDocument* doc);

    bool IsTitle(const std::string& line, int& level);
    bool IsBullet(const std::string& line);
    bool IsTableSeparator(const std::string& line);
    bool IsTableRow(const std::string& line);

    std::vector<std::string> SplitTableCells(const std::string& line);

    // Parsing inline (formules, liens, gras, italique)
    void ParseInlineSegment(const std::string& text,
                           size_t start,
                           size_t end,
                           ParsedDocument* doc);

    Link* ParseLink(const std::string& linkText);
    LinkType ParseLinkType(const std::string& typeStr);

    // Rendering
    void RenderTextSegment(TextSegment* segment, LinkClickCallback& callback);
    void RenderTitle(const std::string& text, int level);
    void RenderFormula(const std::string& formula);
    void RenderLink(Link* link, LinkClickCallback& callback);
    void RenderTable(const std::vector<std::vector<std::string>>& rows);

    // État du parser
    bool inTable;
    std::vector<std::vector<std::string>> currentTableRows;

    // Détection de formules/liens dans le texte
    struct InlineMatch {
        enum Type { Formula, Link, Bold, Italic };
        Type type;
        size_t start;
        size_t end;
        std::string content;
    };

    std::vector<InlineMatch> FindInlineMatches(const std::string& text);
};

} // namespace UI
