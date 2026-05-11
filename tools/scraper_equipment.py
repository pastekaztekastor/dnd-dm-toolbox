#!/usr/bin/env python3
"""
Scraper pour les armes, armures et équipement depuis aidedd.org
"""

import requests
from bs4 import BeautifulSoup
import json
from pathlib import Path
import time


class EquipmentScraper:
    def __init__(self, output_dir="scraped_data"):
        self.base_url = "https://www.aidedd.org/regles/equipement/"
        self.output_dir = Path(output_dir)
        self.output_dir.mkdir(exist_ok=True)
        self.session = requests.Session()
        self.session.headers.update({
            'User-Agent': 'Mozilla/5.0 (DnD Toolbox Educational Scraper)'
        })

    def get_page(self, url):
        """Récupère une page web"""
        try:
            response = self.session.get(url, timeout=30)
            response.raise_for_status()
            response.encoding = 'utf-8'
            return BeautifulSoup(response.text, 'html.parser')
        except Exception as e:
            print(f"❌ Erreur lors du chargement de {url}: {e}")
            return None

    def scrape_weapons(self):
        """Récupère toutes les armes"""
        print("\n⚔️  Récupération des armes...")
        url = self.base_url + "armes/"
        soup = self.get_page(url)

        if not soup:
            return []

        weapons = []
        table = soup.find('table')

        if not table:
            print("❌ Tableau d'armes non trouvé")
            return []

        rows = table.find_all('tr')[1:]  # Skip header

        for row in rows:
            cols = row.find_all('td')
            if len(cols) >= 6:
                weapon = {
                    'name': cols[0].text.strip(),
                    'name_en': cols[1].text.strip(),
                    'damage': cols[2].text.strip(),
                    'weight': cols[3].text.strip(),
                    'price': cols[4].text.strip(),
                    'properties': cols[5].text.strip()
                }
                weapons.append(weapon)

        print(f"✅ {len(weapons)} armes récupérées")

        # Sauvegarder
        output_file = self.output_dir / "weapons.json"
        with open(output_file, 'w', encoding='utf-8') as f:
            json.dump(weapons, f, ensure_ascii=False, indent=2)

        print(f"💾 Sauvegardé dans {output_file}")
        return weapons

    def scrape_armor(self):
        """Récupère toutes les armures"""
        print("\n🛡️  Récupération des armures...")
        url = self.base_url + "armures/"
        soup = self.get_page(url)

        if not soup:
            return []

        armors = []
        tables = soup.find_all('table')

        if not tables:
            print("❌ Tableau d'armures non trouvé")
            return []

        # Premier tableau = armures
        table = tables[0]
        rows = table.find_all('tr')[1:]  # Skip header

        for row in rows:
            cols = row.find_all('td')
            if len(cols) >= 7:
                armor = {
                    'name': cols[0].text.strip(),
                    'name_en': cols[1].text.strip(),
                    'ac': cols[2].text.strip(),
                    'strength': cols[3].text.strip(),
                    'stealth': cols[4].text.strip(),
                    'weight': cols[5].text.strip(),
                    'price': cols[6].text.strip()
                }
                armors.append(armor)

        print(f"✅ {len(armors)} armures récupérées")

        # Sauvegarder
        output_file = self.output_dir / "armor.json"
        with open(output_file, 'w', encoding='utf-8') as f:
            json.dump(armors, f, ensure_ascii=False, indent=2)

        print(f"💾 Sauvegardé dans {output_file}")
        return armors

    def scrape_gear(self):
        """Récupère l'équipement d'aventurier"""
        print("\n🎒 Récupération de l'équipement d'aventurier...")
        url = self.base_url + "materiel/"
        soup = self.get_page(url)

        if not soup:
            return []

        gear = []
        tables = soup.find_all('table')

        if not tables:
            print("❌ Tableau d'équipement non trouvé")
            return []

        # Premier tableau = équipement
        table = tables[0]
        rows = table.find_all('tr')[1:]  # Skip header

        for row in rows:
            cols = row.find_all('td')
            if len(cols) >= 4:
                item = {
                    'name': cols[0].text.strip(),
                    'name_en': cols[1].text.strip(),
                    'price': cols[2].text.strip(),
                    'weight': cols[3].text.strip()
                }
                gear.append(item)

        print(f"✅ {len(gear)} objets d'équipement récupérés")

        # Sauvegarder
        output_file = self.output_dir / "gear.json"
        with open(output_file, 'w', encoding='utf-8') as f:
            json.dump(gear, f, ensure_ascii=False, indent=2)

        print(f"💾 Sauvegardé dans {output_file}")
        return gear

    def scrape_all(self):
        """Récupère tout l'équipement"""
        print("=" * 70)
        print("⚔️  SCRAPER ÉQUIPEMENT D&D 5e")
        print("=" * 70)

        self.scrape_weapons()
        self.scrape_armor()
        self.scrape_gear()

        print("\n" + "=" * 70)
        print("✅ SCRAPING TERMINÉ")
        print("=" * 70)
        print(f"📁 Fichiers sauvegardés dans : {self.output_dir.absolute()}")


def main():
    scraper = EquipmentScraper()
    scraper.scrape_all()


if __name__ == "__main__":
    main()
