#!/bin/bash
set -e

echo "=== DnD DM Toolbox - Build via Docker ==="

# Vérifier que Docker est installé
if ! command -v docker &> /dev/null; then
    echo "ERREUR: Docker n'est pas installé."
    echo "Installer Docker: https://docs.docker.com/get-docker/"
    exit 1
fi

mkdir -p build-output

echo ">>> Compilation en cours (peut prendre quelques minutes la première fois)..."
docker compose --profile build up --build build
docker compose --profile build rm -f build

echo ""
echo "=== Build terminé ==="
echo "Binaire disponible dans: build-output/"
echo ""
echo "Pour lancer l'application:"
echo "  ./build-output/DnD_DM_Toolbox"
