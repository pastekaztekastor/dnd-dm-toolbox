#!/usr/bin/env python3
"""
generate_plugin.py — Scaffold un nouveau plugin (Core::ToolBase) et,
si un schéma est fourni, un repository SQLite pour la DB propre au plugin.

Usage:
    python3 generate_plugin.py <plugin_id> --name "Nom affiché" [options]

Options:
    --name        Nom affiché (obligatoire)
    --category    Catégorie de menu (défaut: "Outils")
    --description Description courte (défaut: "")
    --schema      Fichier .sql (CREATE TABLE, syntaxe SQLite) décrivant la DB
                   propre au plugin : génère un repository SQLite en plus
                   du tool.

Exemple:
    python3 generate_plugin.py race_manager --name "Gestionnaire de Races" \\
        --category "Création" --description "Gère les races jouables" \\
        --schema race_manager_schema.sql

Le plugin est créé dans src/plugins/<plugin_id>/ et enregistré dans
CMakeLists.txt via add_plugin(<plugin_id>).
"""

import argparse
import re
import sys
from pathlib import Path

# ============================================================
# Mapping SQL (SQLite) -> C++
# ============================================================

SQLITE_TYPE_MAP = {
    'TEXT':    ('std::string', '""'),
    'VARCHAR': ('std::string', '""'),
    'CHAR':    ('std::string', '""'),
    'CLOB':    ('std::string', '""'),
    'BLOB':    ('std::string', '""'),
    'INTEGER': ('int', '0'),
    'INT':     ('int', '0'),
    'BOOLEAN': ('bool', 'false'),
    'BOOL':    ('bool', 'false'),
    'REAL':    ('double', '0.0'),
    'FLOAT':   ('double', '0.0'),
    'DOUBLE':  ('double', '0.0'),
    'NUMERIC': ('double', '0.0'),
}

SQL_KEYWORDS = {
    'PRIMARY', 'FOREIGN', 'UNIQUE', 'CHECK', 'CONSTRAINT', 'KEY',
    'REFERENCES', 'ON', 'DELETE', 'UPDATE', 'CASCADE', 'NOT', 'NULL',
    'DEFAULT', 'AUTOINCREMENT',
}


def sql_to_cpp(sql_type: str) -> tuple:
    base = re.sub(r'\(\d+(?:,\s*\d+)?\)', '', sql_type.strip().upper()).strip()
    base = base.split()[0] if base else 'TEXT'
    return SQLITE_TYPE_MAP.get(base, ('std::string', '""'))


def parse_columns(body: str) -> list:
    columns = []
    for raw in body.split('\n'):
        line = re.sub(r'--.*$', '', raw).strip().rstrip(',').strip()
        if not line:
            continue
        m = re.match(
            r'^["\']?(\w+)["\']?\s+'
            r'((?:TEXT|VARCHAR|CHAR|CLOB|BLOB|INTEGER|INT|BOOLEAN|BOOL|REAL|FLOAT|DOUBLE|NUMERIC)'
            r'(?:\(\d+(?:,\s*\d+)?\))?)',
            line, re.IGNORECASE
        )
        if not m:
            continue
        col_name = m.group(1)
        if col_name.upper() in SQL_KEYWORDS:
            continue
        cpp_type, default = sql_to_cpp(m.group(2))
        columns.append({'name': col_name.lower(), 'cpp_type': cpp_type, 'default': default})
    return columns


def parse_tables(sql: str) -> list:
    pattern = re.compile(
        r'CREATE\s+TABLE\s+(?:IF\s+NOT\s+EXISTS\s+)?["\']?(\w+)["\']?\s*\((.*?)\)\s*;',
        re.DOTALL | re.IGNORECASE
    )
    tables = []
    for m in pattern.finditer(sql):
        cols = parse_columns(m.group(2))
        if cols:
            tables.append({'name': m.group(1).lower(), 'columns': cols})
    return tables


# ============================================================
# Helpers de nommage
# ============================================================

def pascal(name: str) -> str:
    return ''.join(w.capitalize() for w in name.split('_'))


def singular(name: str) -> str:
    return name[:-1] if name.endswith('s') and len(name) > 1 else name


# ============================================================
# Génération du plugin (Tool + manifest)
# ============================================================

def generate_plugin_files(plugin_dir: Path, templates_dir: Path, plugin_id: str,
                           name: str, category: str, description: str, class_name: str):
    replacements = {
        'PLUGIN_ID': plugin_id,
        'PLUGIN_NAME_DISPLAY': name,
        'PLUGIN_CATEGORY': category,
        'PLUGIN_DESCRIPTION': description,
        'PLUGIN_CLASS_NAME': class_name,
    }

    def render(text: str) -> str:
        for key, value in replacements.items():
            text = text.replace(key, value)
        return text

    header = render((templates_dir / 'TemplateTool.h').read_text(encoding='utf-8'))
    source = render((templates_dir / 'TemplateTool.cpp').read_text(encoding='utf-8'))
    manifest = render((templates_dir / 'template.manifest.json').read_text(encoding='utf-8'))

    (plugin_dir / f'{class_name}Tool.h').write_text(header, encoding='utf-8')
    (plugin_dir / f'{class_name}Tool.cpp').write_text(source, encoding='utf-8')
    (plugin_dir / f'{plugin_id}.manifest.json').write_text(manifest, encoding='utf-8')


# ============================================================
# Génération du repository SQLite (optionnel, depuis --schema)
# ============================================================

def generate_repository(plugin_dir: Path, templates_dir: Path, tables: list, class_name: str):
    repo_class = f'{class_name}Repository'
    namespace = class_name

    structs = ''
    method_decls = ''
    method_impls = ''

    for table in tables:
        cols = table['columns']
        struct_name = pascal(singular(table['name'])) + 'Data'
        load_all = f'LoadAll{pascal(table["name"])}'
        load_one = f'Load{pascal(singular(table["name"]))}ById'

        # Struct
        structs += f'struct {struct_name} {{\n'
        for c in cols:
            structs += f'    {c["cpp_type"]} {c["name"]} = {c["default"]};\n'
        structs += '};\n\n'

        # Déclarations
        method_decls += f'    std::vector<{struct_name}> {load_all}();\n'
        method_decls += f'    {struct_name} {load_one}(const std::string& id);\n'

        select_cols = ', '.join(c['name'] for c in cols)

        def column_assign(var: str, indent: str) -> str:
            lines = []
            for i, c in enumerate(cols):
                if c['cpp_type'] == 'std::string':
                    lines.append(
                        f'{indent}{var}.{c["name"]} = sqlite3_column_text(stmt, {i}) '
                        f'? reinterpret_cast<const char*>(sqlite3_column_text(stmt, {i})) : "";'
                    )
                elif c['cpp_type'] == 'int':
                    lines.append(f'{indent}{var}.{c["name"]} = sqlite3_column_int(stmt, {i});')
                elif c['cpp_type'] == 'bool':
                    lines.append(f'{indent}{var}.{c["name"]} = sqlite3_column_int(stmt, {i}) != 0;')
                elif c['cpp_type'] == 'double':
                    lines.append(f'{indent}{var}.{c["name"]} = sqlite3_column_double(stmt, {i});')
            return '\n'.join(lines)

        method_impls += (
            f'std::vector<{struct_name}> {repo_class}::{load_all}() {{\n'
            f'    std::vector<{struct_name}> results;\n'
            f'    const char* sql = "SELECT {select_cols} FROM {table["name"]}";\n'
            f'    sqlite3_stmt* stmt;\n'
            f'    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {{\n'
            f'        while (sqlite3_step(stmt) == SQLITE_ROW) {{\n'
            f'            {struct_name} obj;\n'
            f'{column_assign("obj", " " * 12)}\n'
            f'            results.push_back(obj);\n'
            f'        }}\n'
            f'        sqlite3_finalize(stmt);\n'
            f'    }}\n'
            f'    return results;\n'
            f'}}\n\n'
            f'{struct_name} {repo_class}::{load_one}(const std::string& id) {{\n'
            f'    {struct_name} obj;\n'
            f'    const char* sql = "SELECT {select_cols} FROM {table["name"]} WHERE id = ?";\n'
            f'    sqlite3_stmt* stmt;\n'
            f'    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {{\n'
            f'        sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);\n'
            f'        if (sqlite3_step(stmt) == SQLITE_ROW) {{\n'
            f'{column_assign("obj", " " * 12)}\n'
            f'        }}\n'
            f'        sqlite3_finalize(stmt);\n'
            f'    }}\n'
            f'    return obj;\n'
            f'}}\n\n'
        )

    header = (templates_dir / 'repository.h.template').read_text(encoding='utf-8')
    source = (templates_dir / 'repository.cpp.template').read_text(encoding='utf-8')

    header = (header
              .replace('PLUGIN_NAMESPACE', namespace)
              .replace('REPO_CLASS_NAME', repo_class)
              .replace('REPO_STRUCTS', structs)
              .replace('REPO_METHOD_DECLS', method_decls))

    source = (source
              .replace('PLUGIN_NAMESPACE', namespace)
              .replace('REPO_CLASS_NAME', repo_class)
              .replace('REPO_METHOD_IMPLS', method_impls))

    (plugin_dir / f'{repo_class}.h').write_text(header, encoding='utf-8')
    (plugin_dir / f'{repo_class}.cpp').write_text(source, encoding='utf-8')

    return repo_class


# ============================================================
# CMakeLists.txt
# ============================================================

def register_plugin_in_cmake(root: Path, plugin_id: str):
    cmake_path = root / 'CMakeLists.txt'
    content = cmake_path.read_text(encoding='utf-8')

    line = f'add_plugin({plugin_id})'
    if line in content:
        return False

    m = re.search(r'(add_plugin\([\w]+\)\n)(?!.*add_plugin)', content, re.DOTALL)
    if not m:
        return False

    insert_at = m.end()
    content = content[:insert_at] + line + '\n' + content[insert_at:]
    cmake_path.write_text(content, encoding='utf-8')
    return True


# ============================================================
# Point d'entrée
# ============================================================

def find_project_root() -> Path:
    p = Path(__file__).resolve()
    for parent in p.parents:
        if (parent / 'CMakeLists.txt').exists():
            return parent
    return Path.cwd()


def main():
    parser = argparse.ArgumentParser(
        description='Scaffold un nouveau plugin Core::ToolBase (et son repository SQLite).',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__
    )
    parser.add_argument('plugin_id', help='ID du plugin (ex: race_manager)')
    parser.add_argument('--name', required=True, help='Nom affiché du plugin')
    parser.add_argument('--category', default='Outils', help='Catégorie de menu (défaut: Outils)')
    parser.add_argument('--description', default='', help='Description courte')
    parser.add_argument('--schema', help='Fichier .sql (SQLite) décrivant la DB du plugin')
    args = parser.parse_args()

    if not re.match(r'^[a-z][a-z0-9_]*$', args.plugin_id):
        print('Erreur : <plugin_id> doit être en snake_case (ex: race_manager)', file=sys.stderr)
        sys.exit(1)

    root = find_project_root()
    templates_dir = Path(__file__).resolve().parent / 'templates'
    plugin_dir = root / 'src' / 'plugins' / args.plugin_id
    class_name = pascal(args.plugin_id)

    if plugin_dir.exists():
        print(f'Erreur : {plugin_dir} existe déjà.', file=sys.stderr)
        sys.exit(1)

    plugin_dir.mkdir(parents=True)

    generate_plugin_files(plugin_dir, templates_dir, args.plugin_id, args.name,
                           args.category, args.description, class_name)

    print(f'Plugin créé : {plugin_dir}')
    print(f'  {class_name}Tool.h / {class_name}Tool.cpp')
    print(f'  {args.plugin_id}.manifest.json')

    if args.schema:
        schema_path = Path(args.schema)
        if not schema_path.exists():
            print(f'Erreur : fichier non trouvé : {schema_path}', file=sys.stderr)
            sys.exit(1)

        tables = parse_tables(schema_path.read_text(encoding='utf-8'))
        if not tables:
            print('Erreur : aucune CREATE TABLE trouvée dans le schéma.', file=sys.stderr)
            sys.exit(1)

        repo_class = generate_repository(plugin_dir, templates_dir, tables, class_name)

        schema_dest = plugin_dir / f'{args.plugin_id}_schema.sql'
        schema_dest.write_text(schema_path.read_text(encoding='utf-8'), encoding='utf-8')

        print(f'  {repo_class}.h / {repo_class}.cpp')
        print(f'  {args.plugin_id}_schema.sql')
        print(f'\nTables : {", ".join(t["name"] for t in tables)}')

    if register_plugin_in_cmake(root, args.plugin_id):
        print(f'\nCMakeLists.txt mis à jour : add_plugin({args.plugin_id})')
    else:
        print(f'\nAttention : ajoutez manuellement add_plugin({args.plugin_id}) dans CMakeLists.txt')

    print('\nProchaines étapes :')
    print(f'  1. Implémenter {class_name}Tool::Render() (et OnCreate/OnSave/OnLoad)')
    if args.schema:
        print(f'  2. Dans OnCreate(), ouvrir {args.plugin_id}.db (sqlite3_open), exécuter')
        print(f'     {args.plugin_id}_schema.sql si la DB est neuve, puis instancier {class_name}Repository')
    print('  3. Relancer cmake (le plugin est auto-détecté via GLOB)')


if __name__ == '__main__':
    main()
