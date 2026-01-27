#include "../../include/ui/ContextualHelpWindow.h"
#include <imgui.h>

namespace UI {

void ContextualHelpWindow::InitializeHelpDatabase() {
    // === RACES ===
    helpDatabase["race_human"] = {
        "Humain",
        "Les humains sont polyvalents et adaptables. Ils reçoivent +1 à toutes les caractéristiques.",
        "💡 Excellent choix pour les débutants ! Les humains sont équilibrés et fonctionnent bien avec toutes les classes.",
        ""
    };

    helpDatabase["race_elf"] = {
        "Elfe",
        "Les elfes sont agiles et gracieux. +2 Dextérité. Vision dans le noir, avantage contre le charme, immunité au sommeil magique.",
        "💡 Parfait pour : Roublard, Rôdeur, Moine. La Dextérité aide pour l'AC et l'initiative.",
        "⚠️ Évitez si vous voulez jouer un Barbare ou Guerrier en armure lourde."
    };

    helpDatabase["race_dwarf"] = {
        "Nain",
        "Les nains sont robustes et résistants. +2 Constitution. Vision dans le noir, résistance au poison, maîtrise des haches et marteaux.",
        "💡 Excellent pour : Guerrier, Clerc, Paladin. La Constitution donne plus de PV !",
        "⚠️ Vitesse réduite (25 ft) mais non affectée par l'armure lourde."
    };

    helpDatabase["race_halfling"] = {
        "Halfelin",
        "Les halfelins sont chanceux et agiles. +2 Dextérité. Chanceux (relancer les 1), brave, petite taille.",
        "💡 Idéal pour : Roublard, Barde. La capacité Chanceux peut vous sauver la vie !",
        "⚠️ Petite taille = armes lourdes avec désavantage."
    };

    helpDatabase["race_dragonborn"] = {
        "Drakéide",
        "Descendants de dragons. +2 Force, +1 Charisme. Souffle draconique, résistance aux dégâts.",
        "💡 Bon pour : Paladin, Guerrier, Ensorceleur. Le souffle est une attaque de zone !",
        ""
    };

    helpDatabase["race_gnome"] = {
        "Gnome",
        "+2 Intelligence. Petite taille, vision dans le noir, ruse gnome (avantage aux JdS mentaux).",
        "💡 Parfait pour : Magicien, Artificier. L'Intelligence est votre caractéristique principale !",
        ""
    };

    helpDatabase["race_halfelf"] = {
        "Demi-Elfe",
        "+2 Charisme, +1 à deux autres caractéristiques au choix. Très polyvalent !",
        "💡 Excellent pour : Barde, Sorcier, Paladin. La flexibilité des bonus est précieuse !",
        ""
    };

    helpDatabase["race_halforc"] = {
        "Demi-Orc",
        "+2 Force, +1 Constitution. Ténacité (survivre à 1 PV au lieu de tomber à 0), attaques critiques brutales.",
        "💡 Idéal pour : Barbare, Guerrier. Combinaison parfaite pour un combattant au corps à corps !",
        ""
    };

    helpDatabase["race_tiefling"] = {
        "Tieffelin",
        "+2 Charisme, +1 Intelligence. Résistance au feu, sorts innés (thaumaturgie, représailles infernales).",
        "💡 Bon pour : Sorcier, Ensorceleur, Barde. La résistance au feu est très utile !",
        ""
    };

    // === CLASSES ===
    helpDatabase["class_barbarian"] = {
        "Barbare",
        "Guerrier sauvage qui entre en rage. DV: d12 (le plus élevé !). Caractéristique principale: Force.",
        "💡 Débutant friendly ! Simple à jouer, très résistant. Foncez au combat et frappez fort !",
        "⚠️ Pas d'armure lourde en rage, privilégiez Constitution et Dextérité pour l'AC."
    };

    helpDatabase["class_bard"] = {
        "Barde",
        "Lanceur de sorts basé sur le Charisme, touche-à-tout. DV: d8. Inspiration bardique pour aider les alliés.",
        "💡 Très polyvalent ! Peut soigner, infliger des dégâts, et contrôler le champ de bataille.",
        "⚠️ Classe complexe pour débutants. Beaucoup de sorts à gérer."
    };

    helpDatabase["class_cleric"] = {
        "Clerc",
        "Lanceur de sorts divin. DV: d8. Peut soigner et combattre. Caractéristique principale: Sagesse.",
        "💡 Excellent pour débutants ! Polyvalent, peut porter armure moyenne et bouclier.",
        "⚠️ Préparez vos sorts sagement. Gardez toujours des sorts de soin !"
    };

    helpDatabase["class_druid"] = {
        "Druide",
        "Lanceur de sorts naturels, peut se transformer en animaux. DV: d8. Caractéristique principale: Sagesse.",
        "💡 Très flexible ! La forme sauvage vous donne des PV temporaires en combat.",
        "⚠️ Pas d'armure en métal. Complexe pour les débutants."
    };

    helpDatabase["class_fighter"] = {
        "Guerrier",
        "Maître du combat. DV: d10. Caractéristique principale: Force ou Dextérité.",
        "💡 PARFAIT POUR DÉBUTER ! Simple, efficace, résistant. Second souffle pour se soigner.",
        "⚠️ Choisissez entre Force (armure lourde) ou Dextérité (armure légère) dès le départ."
    };

    helpDatabase["class_monk"] = {
        "Moine",
        "Combattant martial sans armes. DV: d8. Caractéristiques principales: Dextérité et Sagesse.",
        "💡 Mobile et amusant à jouer ! Peut étourdir les ennemis.",
        "⚠️ Fragile au début. Évitez d'être le premier au combat. Classe avancée."
    };

    helpDatabase["class_paladin"] = {
        "Paladin",
        "Guerrier saint. DV: d10. Caractéristiques principales: Force et Charisme.",
        "💡 Très résistant ! Peut soigner et infliger de gros dégâts avec Divine Smite.",
        "⚠️ Dépendant de plusieurs caractéristiques. Gardez vos emplacements de sorts pour Smite !"
    };

    helpDatabase["class_ranger"] = {
        "Rôdeur",
        "Chasseur et pisteur. DV: d10. Caractéristiques principales: Dextérité et Sagesse.",
        "💡 Bon pour le combat à distance. Compagnon animal au niveau 3 (certaines sous-classes).",
        "⚠️ Moins puissant que le Guerrier en combat pur. Spécialisé contre certains ennemis."
    };

    helpDatabase["class_rogue"] = {
        "Roublard",
        "Expert en discrétion et attaques sournoises. DV: d8. Caractéristique principale: Dextérité.",
        "💡 EXCELLENT pour débutants ! Attaque sournoise = gros dégâts. Expertise en compétences.",
        "⚠️ Fragile ! Restez à distance ou caché. Évitez le combat frontal."
    };

    helpDatabase["class_sorcerer"] = {
        "Ensorceleur",
        "Lanceur de sorts inné (Charisme). DV: d6. Points de sorcellerie pour modifier les sorts.",
        "💡 Puissant ! Peut lancer 2 sorts en 1 tour avec Quicken Spell.",
        "⚠️ Très fragile (d6) et peu de sorts connus. Classe avancée. Restez loin du combat !"
    };

    helpDatabase["class_warlock"] = {
        "Sorcier (Warlock)",
        "Lanceur de sorts via pacte. DV: d8. Caractéristique principale: Charisme.",
        "💡 Unique ! Peu d'emplacements de sorts mais se rechargent au repos court. Eldritch Blast = meilleur cantrip.",
        "⚠️ Système de sorts différent. Dépendant du patron choisi."
    };

    helpDatabase["class_wizard"] = {
        "Magicien",
        "Maître des arcanes. DV: d6. Caractéristique principale: Intelligence.",
        "💡 Le plus de sorts disponibles ! Peut apprendre des sorts de parchemins.",
        "⚠️ TRÈS fragile (d6). Restez en arrière ! Complexe pour débutants (beaucoup de sorts)."
    };

    // === ABILITIES ===
    helpDatabase["ability_strength"] = {
        "Force (FOR)",
        "Puissance physique. Utilisée pour : attaques au corps à corps, dégâts d'armes, soulever/pousser.",
        "💡 Essentielle pour : Barbare, Guerrier, Paladin (combat rapproché).",
        "⚠️ Peu utile pour les lanceurs de sorts ou combattants à distance."
    };

    helpDatabase["ability_dexterity"] = {
        "Dextérité (DEX)",
        "Agilité et réflexes. Utilisée pour : CA (sans armure/légère), initiative, attaques à distance, discrétion.",
        "💡 TRÈS IMPORTANTE pour tous ! Même les Guerriers en armure lourde bénéficient de l'initiative.",
        "⚠️ Ne négligez jamais la Dextérité, sauf si vous portez une armure lourde."
    };

    helpDatabase["ability_constitution"] = {
        "Constitution (CON)",
        "Endurance et santé. Points de vie et concentration sur les sorts.",
        "💡 CRITIQUE POUR TOUS ! Plus de PV = survivre plus longtemps. Minimum 14 recommandé.",
        "⚠️ Ne JAMAIS mettre la Constitution en caractéristique faible. Vous le regretterez !"
    };

    helpDatabase["ability_intelligence"] = {
        "Intelligence (INT)",
        "Mémoire et raisonnement. Caractéristique principale pour Magicien.",
        "💡 Essentielle pour : Magicien. Utile pour : Investigation, Histoire, Arcanes.",
        "⚠️ Sauf Magicien, peut être une stat de dump (8-10) pour la plupart des classes."
    };

    helpDatabase["ability_wisdom"] = {
        "Sagesse (SAG)",
        "Conscience et intuition. Caractéristique principale pour Clerc, Druide, Moine, Rôdeur.",
        "💡 Important pour TOUS : Perception (repérer les ennemis) dépend de la Sagesse !",
        "⚠️ Même si ce n'est pas votre stat principale, gardez 12-14 pour la Perception."
    };

    helpDatabase["ability_charisma"] = {
        "Charisme (CHA)",
        "Force de personnalité. Caractéristique principale pour Barde, Paladin, Ensorceleur, Sorcier.",
        "💡 Utile pour : négociations, tromperies, performances. Le 'face' du groupe.",
        "⚠️ Peut être une stat de dump si vous n'êtes pas un lanceur de sorts de Charisme."
    };

    // === ABILITY METHODS ===
    helpDatabase["method_pointbuy"] = {
        "Achat de Points (27 points)",
        "Vous avez 27 points pour acheter vos caractéristiques (8-15). Système équilibré et stratégique.",
        "💡 RECOMMANDÉ ! Permet de construire exactement le personnage que vous voulez.",
        "⚠️ Pas de stats exceptionnelles (max 15 avant bonus raciaux). Équilibré mais pas de chance de 18."
    };

    helpDatabase["method_standard"] = {
        "Tableau Standard",
        "Valeurs prédéfinies : 15, 14, 13, 12, 10, 8. Répartissez-les comme vous voulez.",
        "💡 Simple et rapide ! Bon pour les débutants. Mettez 15-14 dans vos stats principales.",
        "⚠️ Le 8 doit aller dans une stat peu importante pour votre classe !"
    };

    helpDatabase["method_roll"] = {
        "Lancer de Dés (4d6, sans le plus bas)",
        "Lancez 4d6, retirez le plus bas dé, 6 fois. Peut donner des stats très hautes ou basses !",
        "💡 Amusant et traditionnel ! Possibilité de stats héroïques (16-18).",
        "⚠️ RISQUÉ ! Vous pouvez avoir de mauvais jets. Pas recommandé pour les débutants."
    };

    // === SKILLS ===
    helpDatabase["skill_general"] = {
        "Compétences",
        "Les maîtrises de compétences ajoutent votre bonus de maîtrise aux jets correspondants.",
        "💡 Choisissez des compétences qui correspondent à votre rôle dans le groupe !",
        "⚠️ Chaque classe a un nombre limité de compétences. Choisissez avec soin !"
    };

    helpDatabase["skill_perception"] = {
        "Perception (Sagesse)",
        "Repérer les ennemis cachés, trouver des pièges, remarquer des détails.",
        "💡 LA compétence la plus importante ! Au moins un membre du groupe devrait l'avoir.",
        "⚠️ Essentielle pour éviter les embuscades et pièges. Très recommandée !"
    };

    helpDatabase["skill_stealth"] = {
        "Discrétion (Dextérité)",
        "Se déplacer silencieusement, se cacher, éviter la détection.",
        "💡 Critique pour : Roublard, Rôdeur. Permet de surprendre les ennemis.",
        "⚠️ Inutile si vous portez une armure lourde (désavantage automatique) !"
    };

    helpDatabase["skill_athletics"] = {
        "Athlétisme (Force)",
        "Escalade, natation, sauts, bousculades, lutte.",
        "💡 Utile pour : Barbare, Guerrier, Paladin. Pour les actions physiques.",
        ""
    };

    helpDatabase["skill_acrobatics"] = {
        "Acrobaties (Dextérité)",
        "Équilibre, pirouettes, rester debout sur terrain difficile.",
        "💡 Alternative à Athlétisme pour les personnages agiles.",
        "⚠️ Moins utile que Discrétion pour la plupart des Roublards."
    };

    helpDatabase["skill_investigation"] = {
        "Investigation (Intelligence)",
        "Chercher des indices, déduire, analyser.",
        "💡 Utile pour résoudre des énigmes et enquêter.",
        "⚠️ Différent de Perception ! Investigation = analyse, Perception = remarquer."
    };

    helpDatabase["skill_insight"] = {
        "Intuition (Sagesse)",
        "Détecter les mensonges, comprendre les intentions.",
        "💡 Très utile en roleplay ! Savoir si quelqu'un ment.",
        ""
    };

    helpDatabase["skill_persuasion"] = {
        "Persuasion (Charisme)",
        "Convaincre, négocier, diplomatie.",
        "💡 Essentielle pour le 'face' du groupe. Éviter les combats !",
        ""
    };

    helpDatabase["skill_deception"] = {
        "Tromperie (Charisme)",
        "Mentir, bluffer, déguisement.",
        "💡 Pour les personnages sournois ou espions.",
        ""
    };

    helpDatabase["skill_intimidation"] = {
        "Intimidation (Charisme)",
        "Menacer, effrayer les ennemis.",
        "💡 Alternative à Persuasion. Pour les personnages brutaux.",
        ""
    };
}

void ContextualHelpWindow::SetHelp(const std::string& key) {
    auto it = helpDatabase.find(key);
    if (it != helpDatabase.end()) {
        currentHelp = it->second;
        hasContent = true;
    }
}

void ContextualHelpWindow::ClearHelp() {
    hasContent = false;
}

void ContextualHelpWindow::Render() {
    if (!isOpen) return;

    ImGui::SetNextWindowSize(ImVec2(400, 500), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("💡 Aide Contextuelle", &isOpen)) {
        if (!hasContent) {
            ImGui::TextWrapped("📖 Survolez ou cliquez sur les éléments avec le symbole (?) pour obtenir de l'aide.");
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            ImGui::TextWrapped("Cette fenêtre affichera des conseils, descriptions et avertissements pour vous aider à créer le meilleur personnage possible !");
            ImGui::Spacing();
            ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "💡 Conseil");
            ImGui::TextWrapped("Les conseils vous orientent vers les meilleurs choix pour votre style de jeu.");
            ImGui::Spacing();
            ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.0f, 1.0f), "⚠️ Avertissement");
            ImGui::TextWrapped("Les avertissements vous évitent les erreurs courantes de débutants.");
        } else {
            // Title
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.3f, 0.7f, 1.0f, 1.0f));
            ImGui::TextWrapped("%s", currentHelp.title.c_str());
            ImGui::PopStyleColor();
            ImGui::Separator();
            ImGui::Spacing();

            // Description
            ImGui::TextWrapped("%s", currentHelp.description.c_str());
            ImGui::Spacing();

            // Advice
            if (!currentHelp.advice.empty()) {
                ImGui::Separator();
                ImGui::Spacing();
                ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "%s", currentHelp.advice.c_str());
                ImGui::Spacing();
            }

            // Warnings
            if (!currentHelp.warnings.empty()) {
                ImGui::Separator();
                ImGui::Spacing();
                ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.0f, 1.0f), "%s", currentHelp.warnings.c_str());
                ImGui::Spacing();
            }
        }
    }
    ImGui::End();
}

void ContextualHelpWindow::ShowTooltip(const char* text) {
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort)) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(text);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void ContextualHelpWindow::ShowHelpMarker(const std::string& helpKey) {
    ImGui::SameLine();
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNone)) {
        SetHelp(helpKey);
    }
}

} // namespace UI
