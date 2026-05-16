#!/usr/bin/env python3
"""
generate_repo.py — Génère un Repository C++ depuis un fichier SQL d'init.

Usage:
    python3 generate_repo.py <sql_file> [--out-include <dir>] [--out-src <dir>]

Exemple:
    python3 generate_repo.py ../../../database/init/05_create_historique_schema.sql
"""

import sys
import re
import os
import argparse
from pathlib import Path

# ============================================================
# Mapping SQL -> C++
# ============================================================

SQL_KEYWORDS = {
    'PRIMARY', 'FOREIGN', 'UNIQUE', 'CHECK', 'CONSTRAINT', 'INDEX',
    'KEY', 'REFERENCES', 'ON', 'DELETE', 'CASCADE', 'NOT', 'NULL',
    'DEFAULT', 'CREATE', 'TABLE', 'IF', 'EXISTS',
}

def sql_to_cpp(sql_type: str) -> tuple:
    """Retourne (cpp_type, valeur_defaut)."""
    base = re.sub(r'\(\d+(?:,\s*\d+)?\)', '', sql_type.strip().upper()).strip()
    mapping = {
        'UUID':             ('std::string',              '""'),
        'VARCHAR':          ('std::string',              '""'),
        'TEXT':             ('std::string',              '""'),
        'CHAR':             ('std::string',              '""'),
        'INTEGER':          ('int',                      '0'),
        'BOOLEAN':          ('bool',                     'false'),
        'FLOAT':            ('float',                    '0.0f'),
        'REAL':             ('float',                    '0.0f'),
        'DOUBLE PRECISION': ('double',                   '0.0'),
        'UUID[]':           ('std::vector<std::string>', '{}'),
        'TEXT[]':           ('std::vector<std::string>', '{}'),
        'VARCHAR[]':        ('std::vector<std::string>', '{}'),
        'INTEGER[]':        ('std::vector<int>',         '{}'),
    }
    return mapping.get(base, ('std::string', '""'))


def coalesce_expr(col: dict) -> str:
    name, cpp, sql = col['name'], col['cpp_type'], col['sql_type'].upper()
    if name == 'id':                    return 'id::text'
    if '[]' in sql:                     return f"COALESCE({name}::text, '{{}}')"
    if cpp == 'std::string':            return f"COALESCE({name}, '')"
    if cpp == 'int':                    return f"COALESCE({name}, 0)"
    if cpp == 'bool':                   return f"COALESCE({name}, false)"
    return f"COALESCE({name}, 0)"


def parse_expr(col: dict, idx: int) -> str:
    name, cpp = col['name'], col['cpp_type']
    v = f'PQgetvalue(result, i, {idx})'
    if cpp == 'std::string':            return f'        obj.{name} = {v};'
    if cpp == 'int':                    return f'        obj.{name} = atoi({v});'
    if cpp == 'bool':                   return f'        obj.{name} = std::string({v}) == "t";'
    if cpp in ('float', 'double'):      return f'        obj.{name} = atof({v});'
    if 'vector' in cpp:                 return f'        // TODO: obj.{name} = ParseArray({v});  // array — parsing manuel requis'
    return f'        obj.{name} = {v};'

# ============================================================
# Parser SQL
# ============================================================

def parse_columns(body: str) -> list:
    columns = []
    for raw in body.split('\n'):
        line = re.sub(r'--.*$', '', raw).strip().rstrip(',').strip()
        if not line:
            continue
        m = re.match(
            r'^(\w+)\s+((?:UUID|TEXT|VARCHAR|CHAR|INTEGER|BOOLEAN|FLOAT|REAL|DOUBLE\s+PRECISION)(?:\[\])?(?:\(\d+(?:,\s*\d+)?\))?)',
            line, re.IGNORECASE
        )
        if not m:
            continue
        col_name = m.group(1)
        sql_type = m.group(2).strip()
        if col_name.upper() in SQL_KEYWORDS:
            continue
        cpp_type, default_val = sql_to_cpp(sql_type)
        columns.append({
            'name':      col_name.lower(),
            'sql_type':  sql_type,
            'cpp_type':  cpp_type,
            'default':   default_val,
        })
    return columns


def parse_tables(sql: str) -> list:
    pattern = re.compile(
        r'CREATE\s+TABLE\s+(?:IF\s+NOT\s+EXISTS\s+)?(\w+)\s*\((.*?)\)\s*;',
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


def find_fk(related_cols: list, main_table: str) -> str:
    candidates = {main_table.rstrip('s') + '_id', main_table + '_id'}
    for c in related_cols:
        if c['name'] in candidates:
            return c['name']
    for c in related_cols:
        if c['name'].endswith('_id'):
            return c['name']
    return 'id'

# ============================================================
# Génération
# ============================================================

def load_template(name: str) -> str:
    tpl_dir = Path(__file__).parent / 'templates'
    return (tpl_dir / name).read_text(encoding='utf-8')


def generate_header(main: dict, related: list, class_name: str, struct_name: str) -> str:
    tpl = load_template('repository.h.template')

    # Structs tables liées
    related_structs = ''
    for rt in related:
        rs = pascal(rt['name'])
        related_structs += f'struct {rs} {{\n'
        for c in rt['columns']:
            if c['name'].endswith('_id') or c['name'] == 'id':
                continue
            related_structs += f'    {c["cpp_type"]} {c["name"]};\n'
        related_structs += '};\n\n'

    # Champs struct principal
    struct_fields = ''
    for c in main['columns']:
        d = c['default']
        if d and d not in ('""', '{}'):
            struct_fields += f'    {c["cpp_type"]} {c["name"]} = {d};\n'
        else:
            struct_fields += f'    {c["cpp_type"]} {c["name"]};\n'
    for rt in related:
        struct_fields += f'    std::vector<{pascal(rt["name"])}> {rt["name"]};\n'

    # Déclarations privées Load*
    related_load_decls = ''
    for rt in related:
        related_load_decls += f'    void Load{pascal(rt["name"])}({struct_name}& obj);\n'

    return (tpl
        .replace('{{RELATED_STRUCTS}}',    related_structs)
        .replace('{{MAIN_TABLE}}',         main['name'])
        .replace('{{STRUCT_NAME}}',        struct_name)
        .replace('{{STRUCT_FIELDS}}',      struct_fields)
        .replace('{{CLASS_NAME}}',         class_name)
        .replace('{{RELATED_LOAD_DECLS}}', related_load_decls)
    )


def generate_cpp(main: dict, related: list, class_name: str, struct_name: str, include_path: str) -> str:
    tpl = load_template('repository.cpp.template')

    cols = main['columns']

    select_cols      = ',\n               '.join(coalesce_expr(c) for c in cols)
    select_cols_flat = ', '.join(coalesce_expr(c) for c in cols)
    parse_fields     = '\n'.join(parse_expr(c, i) for i, c in enumerate(cols)) + '\n'

    related_load_calls = ''.join(f'        Load{pascal(rt["name"])}(obj);\n' for rt in related)

    related_impls = ''
    for rt in related:
        rs      = pascal(rt['name'])
        fk      = find_fk(rt['columns'], main['name'])
        sub_cols = [c for c in rt['columns'] if c['name'] not in ('id', fk)]
        sub_sel  = ', '.join(c['name'] for c in sub_cols) if sub_cols else '*'
        sub_parse = '\n'.join(
            f'            sub.{c["name"]} = PQgetvalue(result, i, {i});'
            for i, c in enumerate(sub_cols)
        )

        related_impls += (
            f'void {class_name}::Load{rs}({struct_name}& obj) {{\n'
            f'    std::string query = "SELECT {sub_sel} FROM {rt["name"]} "\n'
            f'                        "WHERE {fk} = \'" + obj.id + "\' ORDER BY 1";\n'
            f'    PGresult* result = PQexec(conn, query.c_str());\n'
            f'    if (PQresultStatus(result) == PGRES_TUPLES_OK) {{\n'
            f'        for (int i = 0; i < PQntuples(result); ++i) {{\n'
            f'            {rs} sub;\n'
            f'{sub_parse}\n'
            f'            obj.{rt["name"]}.push_back(sub);\n'
            f'        }}\n'
            f'    }}\n'
            f'    PQclear(result);\n'
            f'}}\n\n'
        )

    return (tpl
        .replace('{{INCLUDE_PATH}}',        include_path)
        .replace('{{CLASS_NAME}}',          class_name)
        .replace('{{STRUCT_NAME}}',         struct_name)
        .replace('{{MAIN_TABLE}}',          main['name'])
        .replace('{{SELECT_COLUMNS}}',      select_cols)
        .replace('{{SELECT_COLUMNS_FLAT}}', select_cols_flat)
        .replace('{{PARSE_FIELDS}}',        parse_fields)
        .replace('{{RELATED_LOAD_CALLS}}',  related_load_calls)
        .replace('{{RELATED_IMPLS}}',       related_impls)
    )

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
        description='Génère un Repository C++ depuis un fichier SQL d\'init.',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__
    )
    parser.add_argument('sql_file')
    parser.add_argument('--out-include', help='Dossier .h  (défaut: <racine>/include/database/repositories)')
    parser.add_argument('--out-src',     help='Dossier .cpp (défaut: <racine>/src/database/repositories)')
    args = parser.parse_args()

    sql_path = Path(args.sql_file)
    if not sql_path.exists():
        print(f'Erreur : fichier non trouvé : {sql_path}', file=sys.stderr)
        sys.exit(1)

    tables = parse_tables(sql_path.read_text(encoding='utf-8'))
    if not tables:
        print('Erreur : aucune CREATE TABLE trouvée.', file=sys.stderr)
        sys.exit(1)

    main_table     = tables[0]
    related_tables = tables[1:]
    class_name     = pascal(main_table['name']) + 'Repository'
    struct_name    = pascal(main_table['name']) + 'Data'

    root        = find_project_root()
    out_include = Path(args.out_include) if args.out_include else root / 'include' / 'database' / 'repositories'
    out_src     = Path(args.out_src)     if args.out_src     else root / 'src'     / 'database' / 'repositories'

    out_include.mkdir(parents=True, exist_ok=True)
    out_src.mkdir(parents=True, exist_ok=True)

    try:
        rel = out_include.relative_to(root / 'include')
        include_path = '../../../include/' + str(rel).replace(os.sep, '/') + '/'
    except ValueError:
        include_path = ''

    print(f'Table principale  : {main_table["name"]} ({len(main_table["columns"])} colonnes)')
    if related_tables:
        print(f'Tables liées      : {", ".join(t["name"] for t in related_tables)}')
    print(f'Génération de     : {class_name}.h / {class_name}.cpp')

    header_path = out_include / f'{class_name}.h'
    src_path    = out_src     / f'{class_name}.cpp'

    header_path.write_text(generate_header(main_table, related_tables, class_name, struct_name),              encoding='utf-8')
    src_path.write_text(   generate_cpp(   main_table, related_tables, class_name, struct_name, include_path), encoding='utf-8')

    print(f'\nFichiers générés :')
    print(f'  {header_path}')
    print(f'  {src_path}')
    print(f'\nProchaines étapes :')
    print(f'  1. Ajouter dans DatabaseManager.h  :  {class_name}* {pascal(main_table["name"])}s();')
    print(f'  2. Instancier {class_name} dans DatabaseManager.cpp (Connect / Disconnect)')
    print(f'  3. Relancer cmake .. (le .cpp est auto-détecté via GLOB_RECURSE)')


if __name__ == '__main__':
    main()
