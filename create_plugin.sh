#!/bin/bash
# Script de création de nouveau plugin pour D&D DM Toolbox
# Usage: ./create_plugin.sh

set -e

# Couleurs pour le terminal
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}"
echo "╔═══════════════════════════════════════════════╗"
echo "║                                               ║"
echo "║      D&D DM Toolbox - Plugin Generator        ║"
echo "║                                               ║"
echo "╚═══════════════════════════════════════════════╝"
echo -e "${NC}"

# ============================================================
# Demander les informations du plugin
# ============================================================

echo -e "${YELLOW}Veuillez fournir les informations du plugin:${NC}"
echo ""

# ID du plugin (snake_case, ex: dice_roller)
read -p "ID du plugin (snake_case, ex: dice_roller): " PLUGIN_ID
if [ -z "$PLUGIN_ID" ]; then
    echo -e "${RED}Erreur: L'ID du plugin ne peut pas être vide${NC}"
    exit 1
fi

# Nom de la classe (PascalCase, ex: DiceRoller)
read -p "Nom de la classe (PascalCase, ex: DiceRoller): " PLUGIN_CLASS_NAME
if [ -z "$PLUGIN_CLASS_NAME" ]; then
    echo -e "${RED}Erreur: Le nom de la classe ne peut pas être vide${NC}"
    exit 1
fi

# Nom affiché (ex: Dice Roller)
read -p "Nom affiché (ex: Dice Roller): " PLUGIN_NAME_DISPLAY
if [ -z "$PLUGIN_NAME_DISPLAY" ]; then
    PLUGIN_NAME_DISPLAY="$PLUGIN_CLASS_NAME"
fi

# Description
read -p "Description courte: " PLUGIN_DESCRIPTION
if [ -z "$PLUGIN_DESCRIPTION" ]; then
    PLUGIN_DESCRIPTION="Nouveau plugin"
fi

# Catégorie
echo ""
echo "Catégories disponibles:"
echo "  1) Création"
echo "  2) Gestion"
echo "  3) Jeu"
echo "  4) Outils"
read -p "Catégorie (1-4, défaut: 4): " CATEGORY_CHOICE
case $CATEGORY_CHOICE in
    1) PLUGIN_CATEGORY="Création" ;;
    2) PLUGIN_CATEGORY="Gestion" ;;
    3) PLUGIN_CATEGORY="Jeu" ;;
    *) PLUGIN_CATEGORY="Outils" ;;
esac

# ============================================================
# Vérifications
# ============================================================

PLUGIN_DIR="plugins_src/$PLUGIN_ID"

if [ -d "$PLUGIN_DIR" ]; then
    echo -e "${RED}Erreur: Le plugin '$PLUGIN_ID' existe déjà dans $PLUGIN_DIR${NC}"
    exit 1
fi

# ============================================================
# Résumé
# ============================================================

echo ""
echo -e "${GREEN}═══════════════════════════════════════════${NC}"
echo -e "${GREEN}Résumé du plugin:${NC}"
echo -e "${GREEN}═══════════════════════════════════════════${NC}"
echo "  ID:          $PLUGIN_ID"
echo "  Classe:      ${PLUGIN_CLASS_NAME}Tool"
echo "  Nom:         $PLUGIN_NAME_DISPLAY"
echo "  Description: $PLUGIN_DESCRIPTION"
echo "  Catégorie:   $PLUGIN_CATEGORY"
echo "  Dossier:     $PLUGIN_DIR"
echo -e "${GREEN}═══════════════════════════════════════════${NC}"
echo ""

read -p "Créer ce plugin ? (y/n): " CONFIRM
if [ "$CONFIRM" != "y" ] && [ "$CONFIRM" != "Y" ]; then
    echo -e "${YELLOW}Annulé.${NC}"
    exit 0
fi

# ============================================================
# Création du plugin
# ============================================================

echo ""
echo -e "${BLUE}Création du plugin...${NC}"

# Créer le dossier
mkdir -p "$PLUGIN_DIR"

# Copier et modifier les fichiers template
echo -e "${BLUE}  -> Création de ${PLUGIN_CLASS_NAME}Tool.h${NC}"
sed -e "s/PLUGIN_ID/$PLUGIN_ID/g" \
    -e "s/PLUGIN_CLASS_NAME/$PLUGIN_CLASS_NAME/g" \
    -e "s/PLUGIN_NAME_DISPLAY/$PLUGIN_NAME_DISPLAY/g" \
    -e "s/PLUGIN_DESCRIPTION/$PLUGIN_DESCRIPTION/g" \
    -e "s/PLUGIN_CATEGORY/$PLUGIN_CATEGORY/g" \
    plugin_template/TemplateTool.h > "$PLUGIN_DIR/${PLUGIN_CLASS_NAME}Tool.h"

echo -e "${BLUE}  -> Création de ${PLUGIN_CLASS_NAME}Tool.cpp${NC}"
sed -e "s/PLUGIN_ID/$PLUGIN_ID/g" \
    -e "s/PLUGIN_CLASS_NAME/$PLUGIN_CLASS_NAME/g" \
    -e "s/PLUGIN_NAME_DISPLAY/$PLUGIN_NAME_DISPLAY/g" \
    -e "s/PLUGIN_DESCRIPTION/$PLUGIN_DESCRIPTION/g" \
    -e "s/PLUGIN_CATEGORY/$PLUGIN_CATEGORY/g" \
    plugin_template/TemplateTool.cpp > "$PLUGIN_DIR/${PLUGIN_CLASS_NAME}Tool.cpp"

echo -e "${BLUE}  -> Création de ${PLUGIN_ID}.manifest.json${NC}"
sed -e "s/PLUGIN_ID/$PLUGIN_ID/g" \
    -e "s/PLUGIN_NAME_DISPLAY/$PLUGIN_NAME_DISPLAY/g" \
    -e "s/PLUGIN_DESCRIPTION/$PLUGIN_DESCRIPTION/g" \
    -e "s/PLUGIN_CATEGORY/$PLUGIN_CATEGORY/g" \
    plugin_template/template.manifest.json > "$PLUGIN_DIR/${PLUGIN_ID}.manifest.json"

# ============================================================
# Modification du CMakeLists.txt
# ============================================================

echo -e "${BLUE}  -> Ajout du plugin au CMakeLists.txt${NC}"

# Vérifier si le plugin est déjà dans CMakeLists.txt
if grep -q "add_plugin($PLUGIN_ID)" CMakeLists.txt; then
    echo -e "${YELLOW}    Le plugin est déjà dans CMakeLists.txt${NC}"
else
    # Ajouter la ligne add_plugin() après la ligne "# Compiler les plugins"
    sed -i "/# Compiler les plugins/a add_plugin($PLUGIN_ID)" CMakeLists.txt
    echo -e "${GREEN}    OK - Ajouté à CMakeLists.txt${NC}"
fi

# ============================================================
# Succès !
# ============================================================

echo ""
echo -e "${GREEN}╔═══════════════════════════════════════════════╗${NC}"
echo -e "${GREEN}║                                               ║${NC}"
echo -e "${GREEN}║          Plugin créé avec succès !            ║${NC}"
echo -e "${GREEN}║                                               ║${NC}"
echo -e "${GREEN}╚═══════════════════════════════════════════════╝${NC}"
echo ""
echo -e "${YELLOW}Prochaines étapes:${NC}"
echo ""
echo "  1. Modifier les fichiers dans $PLUGIN_DIR/"
echo "     - ${PLUGIN_CLASS_NAME}Tool.h : Déclarer vos variables et méthodes"
echo "     - ${PLUGIN_CLASS_NAME}Tool.cpp : Implémenter votre logique"
echo "     - ${PLUGIN_ID}.manifest.json : Ajuster les permissions/events"
echo ""
echo "  2. Compiler le projet:"
echo "     cmake --build build"
echo ""
echo "  3. Lancer l'app et tester votre plugin:"
echo "     cd build && ./DnD_DM_Toolbox"
echo ""
echo -e "${GREEN}Bon développement !${NC}"
