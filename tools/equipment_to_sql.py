#!/usr/bin/env python3
"""
Convertit les données d'équipement JSON en fichiers SQL
"""

import json
import re
from pathlib import Path


class EquipmentToSQL:
    def __init__(self, input_dir="scraped_data", output_dir="database/init"):
        self.input_dir = Path(input_dir)
        self.output_dir = Path(output_dir)
        self.output_dir.mkdir(parents=True, exist_ok=True)

    def escape_sql_string(self, text):
        """Échappe les caractères spéciaux pour SQL"""
        if text is None or text == '-':
            return "NULL"
        text = str(text)
        text = text.replace("'", "''")
        return f"'{text}'"

    def parse_price(self, price_str):
        """Parse le prix et retourne en pièces d'or (po/gp)"""
        if not price_str or price_str == '-':
            return 0

        # Extraire le nombre et l'unité
        match = re.search(r'(\d+(?:[,\.]\d+)?)\s*(pc|pa|pe|po|pp)', price_str.lower())
        if not match:
            return 0

        amount = float(match.group(1).replace(',', '.'))
        unit = match.group(2)

        # Conversion en pièces d'or
        conversions = {
            'pc': 0.01,   # pièces de cuivre
            'pa': 0.1,    # pièces d'argent
            'pe': 0.5,    # pièces d'électrum
            'po': 1.0,    # pièces d'or
            'pp': 10.0    # pièces de platine
        }

        return round(amount * conversions.get(unit, 1), 2)

    def parse_weight(self, weight_str):
        """Parse le poids et retourne en livres (pounds)"""
        if not weight_str or weight_str == '-':
            return None

        # Extraire le nombre
        match = re.search(r'(\d+(?:[,\.]\d+)?)', weight_str)
        if not match:
            return None

        weight = float(match.group(1).replace(',', '.'))

        # Convertir en kg d'abord
        if 'g' in weight_str.lower() and 'kg' not in weight_str.lower():
            weight = weight / 1000  # g -> kg

        # Convertir kg en livres (1 kg = 2.20462 lbs)
        weight_lbs = weight * 2.20462

        return round(weight_lbs, 2)

    def parse_damage(self, damage_str):
        """Parse les dégâts (ex: "1d6 tranchant")"""
        if not damage_str or damage_str == '-':
            return None, None

        parts = damage_str.split()
        if len(parts) >= 2:
            dice = parts[0]
            damage_type = ' '.join(parts[1:])
            return dice, damage_type

        return damage_str, None

    def convert_weapons_to_sql(self):
        """Convertit weapons.json en SQL"""
        print("\n⚔️  Conversion des armes en SQL...")

        input_file = self.input_dir / "weapons.json"
        if not input_file.exists():
            print(f"❌ Fichier {input_file} introuvable")
            return

        with open(input_file, 'r', encoding='utf-8') as f:
            weapons = json.load(f)

        output_file = self.output_dir / "08_aidedd_weapons.sql"

        with open(output_file, 'w', encoding='utf-8') as f:
            f.write("-- ============================================================================\n")
            f.write("-- ARMES RÉCUPÉRÉES DEPUIS AIDEDD.ORG\n")
            f.write(f"-- Nombre d'armes : {len(weapons)}\n")
            f.write("-- ============================================================================\n\n")

            for weapon in weapons:
                name = weapon.get('name', '')
                name_en = weapon.get('name_en', '')
                price_str = weapon.get('price', '0')
                weight_str = weapon.get('weight', '0')
                properties = weapon.get('properties', '')
                damage_str = weapon.get('damage', '')

                # Parse
                price_gp = self.parse_price(price_str)
                weight = self.parse_weight(weight_str)
                damage_dice, damage_type = self.parse_damage(damage_str)

                # Déterminer le type d'arme (basique pour l'instant)
                weapon_type = 'Simple Melee'  # Par défaut

                # SQL INSERT
                f.write(f"INSERT INTO weapons (\n")
                f.write(f"    name, name_fr, weapon_type,\n")
                f.write(f"    damage_dice, damage_type,\n")
                f.write(f"    weight, cost_gp\n")
                # f.write(f"    properties\n")  # TODO: parser properties en array
                f.write(f") VALUES (\n")
                f.write(f"    {self.escape_sql_string(name_en)},\n")
                f.write(f"    {self.escape_sql_string(name)},\n")
                f.write(f"    {self.escape_sql_string(weapon_type)},\n")
                f.write(f"    {self.escape_sql_string(damage_dice)},\n")
                f.write(f"    {self.escape_sql_string(damage_type)},\n")
                f.write(f"    {weight if weight else 'NULL'},\n")
                f.write(f"    {price_gp}\n")
                # f.write(f"    {self.escape_sql_string(properties)}\n")
                f.write(f");\n\n")

        print(f"✅ {len(weapons)} armes converties")
        print(f"💾 Sauvegardé dans {output_file}")

    def convert_armor_to_sql(self):
        """Convertit armor.json en SQL"""
        print("\n🛡️  Conversion des armures en SQL...")

        input_file = self.input_dir / "armor.json"
        if not input_file.exists():
            print(f"❌ Fichier {input_file} introuvable")
            return

        with open(input_file, 'r', encoding='utf-8') as f:
            armors = json.load(f)

        output_file = self.output_dir / "09_aidedd_armor.sql"

        with open(output_file, 'w', encoding='utf-8') as f:
            f.write("-- ============================================================================\n")
            f.write("-- ARMURES RÉCUPÉRÉES DEPUIS AIDEDD.ORG\n")
            f.write(f"-- Nombre d'armures : {len(armors)}\n")
            f.write("-- ============================================================================\n\n")

            for armor in armors:
                name = armor.get('name', '')
                name_en = armor.get('name_en', '')
                ac = armor.get('ac', '10')
                strength_req = armor.get('strength', '-')
                stealth = armor.get('stealth', '-')
                weight_str = armor.get('weight', '0')
                price_str = armor.get('price', '0')

                # Parse
                price_gp = self.parse_price(price_str)
                weight = self.parse_weight(weight_str)
                stealth_disadvantage = 'désavantage' in stealth.lower()

                # Déterminer armor_type et base_ac (simplifiéé)
                armor_type = 'Light'
                base_ac = 11

                # Parser AC (ex: "11 + Mod.Dex" -> base_ac=11, adds_dex=true)
                ac_match = re.search(r'(\d+)', ac)
                if ac_match:
                    base_ac = int(ac_match.group(1))

                # Check if adds dex bonus
                adds_dex = 'Dex' in ac

                # SQL INSERT
                f.write(f"INSERT INTO armors (\n")
                f.write(f"    name, name_fr, armor_type,\n")
                f.write(f"    base_ac, adds_dex_bonus, stealth_disadvantage,\n")
                f.write(f"    weight, cost_gp\n")
                f.write(f") VALUES (\n")
                f.write(f"    {self.escape_sql_string(name_en)},\n")
                f.write(f"    {self.escape_sql_string(name)},\n")
                f.write(f"    {self.escape_sql_string(armor_type)},\n")
                f.write(f"    {base_ac},\n")
                f.write(f"    {str(adds_dex).lower()},\n")
                f.write(f"    {str(stealth_disadvantage).lower()},\n")
                f.write(f"    {weight if weight else 'NULL'},\n")
                f.write(f"    {price_gp}\n")
                f.write(f");\n\n")

        print(f"✅ {len(armors)} armures converties")
        print(f"💾 Sauvegardé dans {output_file}")

    def convert_gear_to_sql(self):
        """Convertit gear.json en SQL"""
        print("\n🎒 Conversion de l'équipement en SQL...")

        input_file = self.input_dir / "gear.json"
        if not input_file.exists():
            print(f"❌ Fichier {input_file} introuvable")
            return

        with open(input_file, 'r', encoding='utf-8') as f:
            gear = json.load(f)

        output_file = self.output_dir / "10_aidedd_equipment.sql"

        with open(output_file, 'w', encoding='utf-8') as f:
            f.write("-- ============================================================================\n")
            f.write("-- ÉQUIPEMENT RÉCUPÉRÉ DEPUIS AIDEDD.ORG\n")
            f.write(f"-- Nombre d'objets : {len(gear)}\n")
            f.write("-- ============================================================================\n\n")

            for item in gear:
                name = item.get('name', '')
                name_en = item.get('name_en', '')
                price_str = item.get('price', '0')
                weight_str = item.get('weight', '0')

                # Parse
                price_gp = self.parse_price(price_str)
                weight = self.parse_weight(weight_str)

                # SQL INSERT
                f.write(f"INSERT INTO equipment (\n")
                f.write(f"    name, name_fr,\n")
                f.write(f"    weight, cost_gp\n")
                f.write(f") VALUES (\n")
                f.write(f"    {self.escape_sql_string(name_en)},\n")
                f.write(f"    {self.escape_sql_string(name)},\n")
                f.write(f"    {weight if weight else 'NULL'},\n")
                f.write(f"    {price_gp}\n")
                f.write(f");\n\n")

        print(f"✅ {len(gear)} objets convertis")
        print(f"💾 Sauvegardé dans {output_file}")

    def convert_all(self):
        """Convertit tous les fichiers JSON en SQL"""
        print("=" * 70)
        print("🔄 CONVERSION ÉQUIPEMENT JSON → SQL")
        print("=" * 70)

        self.convert_weapons_to_sql()
        self.convert_armor_to_sql()
        self.convert_gear_to_sql()

        print("\n" + "=" * 70)
        print("✅ CONVERSION TERMINÉE")
        print("=" * 70)


def main():
    converter = EquipmentToSQL()
    converter.convert_all()


if __name__ == "__main__":
    main()
