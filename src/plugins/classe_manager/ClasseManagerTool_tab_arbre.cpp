#include "ClasseManagerTool.h"
#include <imgui.h>
#include <algorithm>
#include <cstring>
#include <set>
#include <map>

using namespace ClasseManager;

// ============================================================
// BUILD TREE
// ============================================================
// Layout :
//   • Pré-choix  : colonne centrale (boîtes UML, 1 par niveau, classe de base)
//   • Choix      : boîte "▼ Choix de voie" → éventail vers N colonnes SC
//   • Post-choix : N colonnes SC ;
//       – aptitudes normales  → nœud principal centré dans la lane SC
//       – aptitudes à prerequi → nœud principal "▼" → fan vers sous-colonnes variantes
//       – aptitudes à conditions.type=prerequi → sous-nœuds variantes

void ClasseManagerTool::BuildTree(const ClasseData& c) {
    treeNodes.clear();
    selectedTreeNode = -1;

    const float BOX_W  = 220.f;
    const float LINE_H =  18.f;
    const float PAD_V  =   6.f;
    const float GAP_Y  =  20.f;
    const float GAP_X  =  40.f;
    const float MARG_L =  60.f;
    const float STEM_H =  48.f;
    const float HDR_H  = LINE_H + PAD_V * 1.5f;   // compartiment UML (≈ 27 px)

    auto makeH = [&](int bodyLines) -> float {
        return HDR_H + PAD_V + bodyLines * LINE_H + PAD_V;
    };

    int N = (int)c.sous_classes.size();

    // Niveau de choix de sous-classe
    int choix_niveau = 0;
    for (auto& e : c.evolutions)
        if (e.choix_sous_classe) { choix_niveau = e.niveau; break; }

    // Ordre des variantes par SC (depuis choix_champs → valeurs de la classe de base)
    std::vector<std::string> globalValeurOrder;
    for (auto& cc : c.choix_champs)
        for (auto& cv : cc.valeurs)
            globalValeurOrder.push_back(cv.id);

    std::vector<std::vector<std::string>> scVariantOrder(N);
    for (int sci = 0; sci < N; ++sci) {
        std::set<std::string> seen;
        for (auto& valId : globalValeurOrder) {
            for (auto& a : c.sous_classes[sci].aptitudes) {
                if (a.choix_valeur_id == valId && seen.find(valId) == seen.end()) {
                    scVariantOrder[sci].push_back(valId);
                    seen.insert(valId);
                    break;
                }
            }
        }
    }

    // Largeurs et positions X des lanes SC
    std::vector<int>   scNV(N, 1);
    std::vector<float> scEffW(N);
    std::vector<float> scLaneX(N);

    for (int sci = 0; sci < N; ++sci)
        scNV[sci] = std::max(1, (int)scVariantOrder[sci].size());
    for (int sci = 0; sci < N; ++sci)
        scEffW[sci] = scNV[sci] * BOX_W + (scNV[sci]-1) * GAP_X;
    {
        float x = MARG_L;
        for (int sci = 0; sci < N; ++sci) { scLaneX[sci] = x; x += scEffW[sci] + GAP_X; }
    }

    float total_sc_w = N > 0 ? (scLaneX[N-1] + scEffW[N-1] - MARG_L) : BOX_W;
    float centerBoxX = MARG_L + (total_sc_w - BOX_W) * 0.5f;

    auto scCenterX = [&](int sci) -> float {
        return scLaneX[sci] + (scEffW[sci] - BOX_W) * 0.5f;
    };
    auto scSubX = [&](int sci, int vi) -> float {
        return scLaneX[sci] + vi * (BOX_W + GAP_X);
    };
    auto getVI = [&](int sci, const std::string& val) -> int {
        auto& vo = scVariantOrder[sci];
        auto it = std::find(vo.begin(), vo.end(), val);
        return it != vo.end() ? (int)(it - vo.begin()) : 0;
    };

    // Classification des aptitudes SC
    struct SCLevel {
        std::vector<const AptitudeData*> mainApts;
        std::map<std::string, std::vector<const AptitudeData*>> variantApts;
        bool hasMain()     const { return !mainApts.empty(); }
        bool hasVariants() const { return !variantApts.empty(); }
    };
    std::vector<std::map<int, SCLevel>> scLevels(N);

    for (int sci = 0; sci < N; ++sci) {
        for (auto& a : c.sous_classes[sci].aptitudes) {
            bool isVar = !a.choix_valeur_id.empty();
            std::string varVal = a.choix_valeur_id;
            if (isVar)
                scLevels[sci][a.niveau_acquisition].variantApts[varVal].push_back(&a);
            else
                scLevels[sci][a.niveau_acquisition].mainApts.push_back(&a);
        }
    }

    // Aptitudes de la classe de base par niveau
    std::map<int, std::vector<const AptitudeData*>> baseByNiv;
    for (auto& a : c.aptitudes) baseByNiv[a.niveau_acquisition].push_back(&a);

    // Constructeur de contenu (label / tooltip / description)
    struct NodeContent { std::string label, tooltip, description; };
    auto buildContent = [](const std::vector<const AptitudeData*>& apts) -> NodeContent {
        NodeContent nc;
        for (auto* a : apts) {
            if (!nc.label.empty()) nc.label += '\n';
            nc.label += a->nom;
            if (!a->choix_champ_id.empty()) nc.label += " \xe2\x96\xbc";  // ▼
            if (!nc.tooltip.empty())     nc.tooltip     += " | ";
            nc.tooltip     += a->description_rapide;
            if (!nc.description.empty()) nc.description += "\n\n";
            nc.description += a->nom + "\n" + a->description;
        }
        return nc;
    };
    auto countLines = [](const std::string& s) -> int {
        return 1 + (int)std::count(s.begin(), s.end(), '\n');
    };

    // Nœuds pré-choix
    float curY = 20.f;
    int choiceNodeIdx = -1;

    std::set<int> preLevels;
    for (auto& e : c.evolutions)
        if (choix_niveau == 0 || e.niveau <= choix_niveau)
            preLevels.insert(e.niveau);

    for (int niv : preLevels) {
        std::vector<const AptitudeData*> apts;
        auto it = baseByNiv.find(niv); if (it != baseByNiv.end()) apts = it->second;

        bool isChoice = (N > 0 && niv == choix_niveau);
        NodeContent nc = buildContent(apts);
        if (isChoice) {
            if (!nc.label.empty()) nc.label += '\n';
            nc.label += "\xe2\x96\xbc Choix de voie";
        }
        if (nc.label.empty()) nc.label = "\xe2\x80\x94";

        TreeNode node;
        node.header      = "Niv " + std::to_string(niv) + " \xe2\x80\x94 " + c.nom;
        node.label       = nc.label;
        node.tooltip     = nc.tooltip;
        node.description = nc.description;
        node.x           = centerBoxX;
        node.y           = curY;
        node.w           = BOX_W;
        node.h           = makeH(countLines(nc.label));
        node.hHeader     = HDR_H;
        node.niveau      = niv;
        node.isChoice    = isChoice;
        node.colIdx      = -1;

        int idx = (int)treeNodes.size();
        treeNodes.push_back(node);

        if (idx > 0 && treeNodes[idx-1].colIdx == -1)
            treeNodes[idx-1].seqChildIdx.push_back(idx);

        if (isChoice) choiceNodeIdx = idx;
        curY += node.h + GAP_Y;
    }

    if (N == 0) return;

    // Layout post-choix
    std::set<int> postLevels;
    for (int sci = 0; sci < N; ++sci)
        for (auto& [niv, _] : scLevels[sci]) postLevels.insert(niv);

    struct LevelLayout {
        float mainH=0, subH=0, totalH=0;
        float y=0, subY=0;
        bool  hasStem=false;
    };
    std::map<int, LevelLayout> levelLayouts;

    for (int niv : postLevels) {
        float maxMainH=0, maxSubH=0; bool anyStem=false;
        for (int sci = 0; sci < N; ++sci) {
            auto it = scLevels[sci].find(niv); if (it == scLevels[sci].end()) continue;
            const auto& lev = it->second;
            if (lev.hasMain())
                maxMainH = std::max(maxMainH, makeH((int)lev.mainApts.size()));
            if (lev.hasVariants()) {
                anyStem |= lev.hasMain();
                int mv = 0;
                for (auto& [v,a] : lev.variantApts) mv = std::max(mv,(int)a.size());
                maxSubH = std::max(maxSubH, makeH(mv));
            }
        }
        LevelLayout ll;
        ll.mainH   = maxMainH;
        ll.subH    = maxSubH;
        ll.hasStem = anyStem;
        ll.totalH  = anyStem ? (maxMainH + STEM_H + maxSubH)
                              : std::max(maxMainH, maxSubH);
        levelLayouts[niv] = ll;
    }

    {
        float y = curY - GAP_Y + STEM_H;
        for (int niv : postLevels) {
            auto& ll = levelLayouts[niv];
            ll.y    = y;
            ll.subY = ll.hasStem ? (y + ll.mainH + STEM_H) : y;
            y += ll.totalH + GAP_Y;
        }
    }

    // Nœuds SC
    std::vector<int> lastMainSC(N,-1), firstMainSC(N,-1);
    std::vector<std::map<std::string,int>> lastVariantSC(N);

    for (int niv : postLevels) {
        const auto& ll = levelLayouts[niv];

        for (int sci = 0; sci < N; ++sci) {
            auto it = scLevels[sci].find(niv); if (it == scLevels[sci].end()) continue;
            const auto& lev = it->second;
            const std::string& scNom = c.sous_classes[sci].nom;

            int mainNodeIdx = -1;
            if (lev.hasMain()) {
                NodeContent nc = buildContent(lev.mainApts);
                bool isC = false;
                for (auto* a : lev.mainApts) if (!a->choix_champ_id.empty()) { isC=true; break; }

                TreeNode node;
                node.header      = "Niv " + std::to_string(niv) + " \xe2\x80\x94 " + scNom;
                node.label       = nc.label;
                node.tooltip     = nc.tooltip;
                node.description = nc.description;
                node.x           = scCenterX(sci);
                node.y           = ll.y;
                node.w           = BOX_W;
                node.h           = ll.mainH > 0 ? ll.mainH : makeH(countLines(nc.label));
                node.hHeader     = HDR_H;
                node.niveau      = niv;
                node.isChoice    = isC;
                node.colIdx      = sci;

                mainNodeIdx = (int)treeNodes.size();
                treeNodes.push_back(node);

                if (lastMainSC[sci] >= 0)
                    treeNodes[lastMainSC[sci]].seqChildIdx.push_back(mainNodeIdx);
                lastMainSC[sci] = mainNodeIdx;
                if (firstMainSC[sci] < 0) firstMainSC[sci] = mainNodeIdx;
            }

            for (auto& [valeur, apts] : lev.variantApts) {
                int vi = getVI(sci, valeur);
                NodeContent nc = buildContent(apts);

                TreeNode node;
                node.header      = "Niv " + std::to_string(niv) + " \xe2\x80\x94 " + scNom;
                node.label       = nc.label;
                node.tooltip     = nc.tooltip;
                node.description = nc.description;
                node.x           = scSubX(sci, vi);
                node.y           = ll.subY;
                node.w           = BOX_W;
                node.h           = makeH(countLines(nc.label));
                node.hHeader     = HDR_H;
                node.niveau      = niv;
                node.isChoice    = false;
                node.colIdx      = sci;

                int vIdx = (int)treeNodes.size();
                treeNodes.push_back(node);

                if (mainNodeIdx >= 0) {
                    treeNodes[mainNodeIdx].childIdx.push_back(vIdx);
                } else {
                    auto vit = lastVariantSC[sci].find(valeur);
                    if (vit != lastVariantSC[sci].end())
                        treeNodes[vit->second].seqChildIdx.push_back(vIdx);
                    else if (lastMainSC[sci] >= 0)
                        treeNodes[lastMainSC[sci]].childIdx.push_back(vIdx);
                }
                lastVariantSC[sci][valeur] = vIdx;
            }
        }
    }

    // Relier le nœud "Choix de voie" aux premières boîtes SC
    if (choiceNodeIdx >= 0)
        for (int sci = 0; sci < N; ++sci)
            if (firstMainSC[sci] >= 0)
                treeNodes[choiceNodeIdx].childIdx.push_back(firstMainSC[sci]);
}

// ============================================================
// TAB ARBRE
// ============================================================

void ClasseManagerTool::RenderTabArbre() {
    if (treeNodes.empty()) { ImGui::TextDisabled("Aucun arbre disponible."); return; }

    const float LINE_H = 18.f;
    const float HDR_H  = LINE_H + 9.f;   // doit correspondre à BuildTree (≈ 27 px)
    const float PAD_V  =  6.f;

    float maxX = 0.f, maxY = 0.f;
    for (auto& n : treeNodes) {
        if (n.x + n.w > maxX) maxX = n.x + n.w;
        if (n.y + n.h > maxY) maxY = n.y + n.h;
    }
    maxX += 20.f; maxY += 20.f;

    const float INFO_H = 160.f;
    float availH  = ImGui::GetContentRegionAvail().y;
    float canvasH = (selectedTreeNode >= 0)
                    ? std::max(80.f, availH - INFO_H - 8.f)
                    : availH;

    ImGui::BeginChild("##arbre_scroll", ImVec2(0, canvasH), false,
        ImGuiWindowFlags_HorizontalScrollbar);
    ImVec2 origin = ImGui::GetCursorScreenPos();
    ImDrawList* dl = ImGui::GetWindowDrawList();

    // Palette
    const ImU32 COLOR_HDR_BASE   = IM_COL32( 28,  52,  76, 245);
    const ImU32 COLOR_BOX_BASE   = IM_COL32( 50,  80, 110, 220);
    const ImU32 COLOR_HDR_CHOICE = IM_COL32( 85,  72,   8, 245);
    const ImU32 COLOR_BOX_CHOICE = IM_COL32(130, 110,  20, 240);
    const ImU32 COLOR_EDGE       = IM_COL32(160, 160, 160, 180);
    const ImU32 COLOR_TXT        = IM_COL32(230, 230, 230, 255);
    const ImU32 COLOR_OUTLINE    = IM_COL32(255, 255, 255,  40);
    const ImU32 COLOR_SEP_LINE   = IM_COL32(255, 255, 255,  55);
    const ImU32 SC_HDR[]  = {
        IM_COL32( 82,  42,  10, 245),
        IM_COL32( 28,  68,  38, 245),
        IM_COL32( 58,  28,  78, 245),
        IM_COL32( 58,  58,  12, 245),
    };
    const ImU32 SC_BOX[]  = {
        IM_COL32(120,  70,  25, 220),
        IM_COL32( 50, 100,  60, 220),
        IM_COL32( 90,  50, 110, 220),
        IM_COL32( 90,  90,  30, 220),
    };

    // 1. Arêtes (fan + séquentielles)
    auto drawFanFrom = [&](const TreeNode& n) {
        if (n.childIdx.empty()) return;
        float srcCX = origin.x + n.x + n.w * 0.5f;
        float srcBY = origin.y + n.y + n.h;
        if (n.childIdx.size() == 1) {
            auto& ch = treeNodes[n.childIdx[0]];
            dl->AddLine({srcCX, srcBY},
                {origin.x + ch.x + ch.w * 0.5f, origin.y + ch.y}, COLOR_EDGE, 1.5f);
        } else {
            float minTY = FLT_MAX;
            for (int ci : n.childIdx)
                minTY = std::min(minTY, origin.y + treeNodes[ci].y);
            float midY = (srcBY + minTY) * 0.5f;
            dl->AddLine({srcCX, srcBY}, {srcCX, midY}, COLOR_EDGE, 1.5f);
            float minCX = FLT_MAX, maxCX = -FLT_MAX;
            for (int ci : n.childIdx) {
                float cx = origin.x + treeNodes[ci].x + treeNodes[ci].w * 0.5f;
                if (cx < minCX) minCX = cx;
                if (cx > maxCX) maxCX = cx;
            }
            dl->AddLine({minCX, midY}, {maxCX, midY}, COLOR_EDGE, 1.5f);
            for (int ci : n.childIdx) {
                float cx = origin.x + treeNodes[ci].x + treeNodes[ci].w * 0.5f;
                dl->AddLine({cx, midY}, {cx, origin.y + treeNodes[ci].y}, COLOR_EDGE, 1.5f);
            }
        }
    };

    for (auto& n : treeNodes) {
        drawFanFrom(n);
        float srcCX = origin.x + n.x + n.w * 0.5f;
        float srcBY = origin.y + n.y + n.h;
        for (int ci : n.seqChildIdx) {
            auto& ch = treeNodes[ci];
            dl->AddLine({srcCX, srcBY},
                {origin.x + ch.x + ch.w * 0.5f, origin.y + ch.y}, COLOR_EDGE, 1.5f);
        }
    }

    // 2. Boîtes UML
    int btnId = 0;
    for (int ni = 0; ni < (int)treeNodes.size(); ++ni) {
        auto& n = treeNodes[ni];
        ImVec2 p0 = { origin.x + n.x,     origin.y + n.y       };
        ImVec2 p1 = { p0.x + n.w,         p0.y + n.h           };
        ImVec2 sl = { p0.x,               p0.y + n.hHeader      };
        ImVec2 sr = { p0.x + n.w,         p0.y + n.hHeader      };

        ImU32 hdrC = (n.colIdx < 0)
                   ? (n.isChoice ? COLOR_HDR_CHOICE : COLOR_HDR_BASE)
                   : SC_HDR[n.colIdx % 4];
        ImU32 boxC = (n.colIdx < 0)
                   ? (n.isChoice ? COLOR_BOX_CHOICE : COLOR_BOX_BASE)
                   : SC_BOX[n.colIdx % 4];

        dl->AddRectFilled(p0, sr, hdrC, 5.f, ImDrawFlags_RoundCornersTop);
        dl->AddRectFilled(sl, p1, boxC, 5.f, ImDrawFlags_RoundCornersBottom);
        dl->AddRect(p0, p1, COLOR_OUTLINE, 5.f, 0, 1.f);
        dl->AddLine(sl, sr, COLOR_SEP_LINE, 1.f);

        if (!n.header.empty()) {
            ImVec2 ts = ImGui::CalcTextSize(n.header.c_str());
            dl->AddText(
                {p0.x + (n.w - ts.x) * 0.5f, p0.y + (n.hHeader - LINE_H) * 0.5f},
                COLOR_TXT, n.header.c_str());
        }

        float ty = p0.y + n.hHeader + PAD_V;
        const char* cur = n.label.c_str();
        while (*cur) {
            const char* nl = strchr(cur, '\n');
            if (!nl) nl = cur + strlen(cur);
            if (nl > cur) {
                ImVec2 ts = ImGui::CalcTextSize(cur, nl);
                dl->AddText({p0.x + (n.w - ts.x) * 0.5f, ty}, COLOR_TXT, cur, nl);
            }
            ty += LINE_H;
            cur = (*nl == '\n') ? nl + 1 : nl;
        }

        ImGui::SetCursorScreenPos(p0);
        ImGui::PushID(btnId++);
        ImGui::InvisibleButton("##tn", {n.w, n.h});
        if (ImGui::IsItemHovered() && !n.tooltip.empty())
            ImGui::SetTooltip("%s", n.tooltip.c_str());
        if (ImGui::IsItemClicked())
            selectedTreeNode = (selectedTreeNode == ni) ? -1 : ni;
        ImGui::PopID();

        if (selectedTreeNode == ni)
            dl->AddRect(p0, p1, IM_COL32(255, 200, 50, 220), 5.f, 0, 2.5f);
    }

    ImGui::SetCursorScreenPos({origin.x + maxX, origin.y + maxY});
    ImGui::Dummy({0.f, 0.f});
    ImGui::EndChild();

    // Panneau d'info (nœud sélectionné)
    if (selectedTreeNode >= 0 && selectedTreeNode < (int)treeNodes.size()) {
        ImGui::Separator();
        ImGui::BeginChild("##arbre_info", ImVec2(0, INFO_H), false);
        const auto& n = treeNodes[selectedTreeNode];
        ImGui::TextColored({.85f, .72f, .35f, 1.f}, "%s", n.header.c_str());
        ImGui::Separator();
        if (n.description.empty())
            ImGui::TextDisabled("(pas de description)");
        else
            ImGui::TextWrapped("%s", n.description.c_str());
        ImGui::EndChild();
    }
}
