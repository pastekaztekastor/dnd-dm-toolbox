#!/bin/bash
# Initialise la base de données SQLite depuis le schéma
set -e

DB_PATH="${1:-dnd_toolbox.db}"
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

if [ -f "$DB_PATH" ]; then
    echo "La base '$DB_PATH' existe déjà."
    read -p "La réinitialiser ? (o/N) " confirm
    [ "$confirm" = "o" ] || exit 0
    rm "$DB_PATH"
fi

sqlite3 "$DB_PATH" < "$SCRIPT_DIR/init_sqlite.sql"
echo "Base de données initialisée : $DB_PATH"
