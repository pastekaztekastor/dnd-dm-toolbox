#!/usr/bin/env python3
"""
Convertit les données JSON scrapées en fichiers SQL pour insertion dans PostgreSQL
"""

import json
import re
from pathlib import Path
import argparse


class JSONToSQL:
    def __init__(self, input_dir="scraped_data", output_dir="database/init"):
        self.input_dir = Path(input_dir)
        self.output_dir = Path(output_dir)
        self.output_dir.mkdir(parents=True, exist_ok=True)

    def escape_sql_string(self, text):
        """Échappe les caractères spéciaux pour SQL"""
        if text is None:
            return "NULL"
        text = str(text)
        # Remplacer les apostrophes simples par deux apostrophes
        text = text.replace("'", "''")
        return f"'{text}'"

    def extract_spell_level(self, level_text):
        """Extrait le niveau numérique d'un sort"""
        if not level_text:
            return 0
        # Chercher un nombre dans le texte
        match = re.search(r'\d+', str(level_text))
        if match:
            return int(match.group())
        # Cantrips = niveau 0
        if 'mineur' in level_text.lower() or 'cantrip' in level_text.lower():
            return 0
        return 0

    def map_class_name(self, fr_name):
        """Mappe les noms de classes FR vers EN"""
        mapping = {
            'Barde': 'Bard',
            'Clerc': 'Cleric',
            'Druide': 'Druid',
            'Ensorceleur': 'Sorcerer',
            'Guerrier': 'Fighter',
            'Magicien': 'Wizard',
            'Moine': 'Monk',
            'Paladin': 'Paladin',
            'Rôdeur': 'Ranger',
            'Roublard': 'Rogue',
            'Sorcier': 'Warlock',
            'Barbare': 'Barbarian'
        }
        return mapping.get(fr_name, fr_name)

    def map_school_name(self, fr_school):
        """Mappe les écoles de magie FR vers EN"""
        mapping = {
            'Abjuration': 'Abjuration',
            'Invocation': 'Conjuration',
            'Divination': 'Divination',
            'Enchantement': 'Enchantment',
            'Évocation': 'Evocation',
            'Illusion': 'Illusion',
            'Nécromancie': 'Necromancy',
            'Transmutation': 'Transmutation'
        }
        return mapping.get(fr_school, fr_school)

    def convert_spells_to_sql(self):
        """Convertit spells.json en SQL"""
        print("\n🔮 Conversion des sorts en SQL...")

        input_file = self.input_dir / "spells.json"
        if not input_file.exists():
            print(f"❌ Fichier {input_file} introuvable")
            return

        with open(input_file, 'r', encoding='utf-8') as f:
            spells = json.load(f)

        output_file = self.output_dir / "06_aidedd_spells.sql"

        with open(output_file, 'w', encoding='utf-8') as f:
            f.write("-- ============================================================================\n")
            f.write("-- SORTS RÉCUPÉRÉS DEPUIS AIDEDD.ORG\n")
            f.write(f"-- Nombre de sorts : {len(spells)}\n")
            f.write("-- ============================================================================\n\n")

            for spell in spells:
                name = spell.get('name', '')
                level = self.extract_spell_level(spell.get('level', '0'))
                school_fr = spell.get('school', 'Évocation')
                school_en = self.map_school_name(school_fr)

                # Composantes
                comp_text = spell.get('components', '')
                has_v = 'V' in comp_text
                has_s = 'S' in comp_text
                has_m = 'M' in comp_text
                material = spell.get('material', '')

                # Classes
                classes_fr = spell.get('classes', [])
                classes_en = [self.map_class_name(c) for c in classes_fr]

                # Concentration et rituel
                concentration = spell.get('concentration', False)
                ritual = spell.get('ritual', False)

                # Description
                description_fr = spell.get('description', '').strip()
                at_higher_levels_fr = spell.get('at_higher_levels', '')

                # SQL INSERT
                f.write(f"INSERT INTO game_spells (\n")
                f.write(f"    name, name_fr, level, school, school_fr,\n")
                f.write(f"    casting_time, casting_time_fr, range, range_fr,\n")
                f.write(f"    has_verbal, has_somatic, has_material, material_component,\n")
                f.write(f"    duration, duration_fr, concentration, ritual,\n")
                f.write(f"    description, description_fr")

                if at_higher_levels_fr:
                    f.write(f",\n    at_higher_levels_fr")

                f.write(f",\n    available_to_classes\n")
                f.write(f") VALUES (\n")
                f.write(f"    {self.escape_sql_string(name)},\n")
                f.write(f"    {self.escape_sql_string(name)},\n")
                f.write(f"    {level},\n")
                f.write(f"    {self.escape_sql_string(school_en)},\n")
                f.write(f"    {self.escape_sql_string(school_fr)},\n")
                f.write(f"    {self.escape_sql_string(spell.get('casting_time', '1 action'))},\n")
                f.write(f"    {self.escape_sql_string(spell.get('casting_time', '1 action'))},\n")
                f.write(f"    {self.escape_sql_string(spell.get('range', 'Personnelle'))},\n")
                f.write(f"    {self.escape_sql_string(spell.get('range', 'Personnelle'))},\n")
                f.write(f"    {str(has_v).lower()},\n")
                f.write(f"    {str(has_s).lower()},\n")
                f.write(f"    {str(has_m).lower()},\n")
                f.write(f"    {self.escape_sql_string(material) if material else 'NULL'},\n")
                f.write(f"    {self.escape_sql_string(spell.get('duration', 'Instantanée'))},\n")
                f.write(f"    {self.escape_sql_string(spell.get('duration', 'Instantanée'))},\n")
                f.write(f"    {str(concentration).lower()},\n")
                f.write(f"    {str(ritual).lower()},\n")
                f.write(f"    {self.escape_sql_string(description_fr)},\n")
                f.write(f"    {self.escape_sql_string(description_fr)}")

                if at_higher_levels_fr:
                    f.write(f",\n    {self.escape_sql_string(at_higher_levels_fr)}")

                # Array de classes
                classes_array = "ARRAY[" + ", ".join([self.escape_sql_string(c) for c in classes_en]) + "]"
                f.write(f",\n    {classes_array}\n")
                f.write(f");\n\n")

        print(f"✅ {len(spells)} sorts convertis")
        print(f"💾 Sauvegardé dans {output_file}")

    def convert_monsters_to_sql(self):
        """Convertit monsters.json en SQL"""
        print("\n👹 Conversion des monstres en SQL...")

        input_file = self.input_dir / "monsters.json"
        if not input_file.exists():
            print(f"❌ Fichier {input_file} introuvable")
            return

        with open(input_file, 'r', encoding='utf-8') as f:
            monsters = json.load(f)

        output_file = self.output_dir / "07_aidedd_monsters.sql"

        with open(output_file, 'w', encoding='utf-8') as f:
            f.write("-- ============================================================================\n")
            f.write("-- MONSTRES RÉCUPÉRÉS DEPUIS AIDEDD.ORG\n")
            f.write(f"-- Nombre de monstres : {len(monsters)}\n")
            f.write("-- ============================================================================\n\n")

            f.write("-- TODO: Parser les stats des monstres pour remplir la table monsters\n")
            f.write("-- Les données brutes sont disponibles dans scraped_data/monsters.json\n\n")

        print(f"✅ {len(monsters)} monstres trouvés (conversion détaillée à faire)")
        print(f"💾 Fichier créé : {output_file}")

    def convert_all(self):
        """Convertit tous les fichiers JSON en SQL"""
        print("=" * 60)
        print("🔄 CONVERSION JSON → SQL")
        print("=" * 60)

        self.convert_spells_to_sql()
        self.convert_monsters_to_sql()

        print("\n" + "=" * 60)
        print("✅ CONVERSION TERMINÉE")
        print("=" * 60)


def main():
    parser = argparse.ArgumentParser(
        description="Convertit les données JSON en SQL"
    )
    parser.add_argument(
        '--input', '-i',
        default='scraped_data',
        help='Répertoire d\'entrée (défaut: scraped_data)'
    )
    parser.add_argument(
        '--output', '-o',
        default='database/init',
        help='Répertoire de sortie (défaut: database/init)'
    )

    args = parser.parse_args()

    converter = JSONToSQL(input_dir=args.input, output_dir=args.output)
    converter.convert_all()


if __name__ == "__main__":
    main()
