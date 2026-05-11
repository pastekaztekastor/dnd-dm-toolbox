#!/usr/bin/env python3
"""
Scraper v2 pour aidedd.org - Adapté à la nouvelle structure du site (2025)
Site : https://www.aidedd.org/
Licence : Contenu libre d'accès (OGL 1.0a)
"""

import requests
from bs4 import BeautifulSoup
import json
import time
import re
from pathlib import Path
import argparse

class AideddScraperV2:
    def __init__(self, output_dir="scraped_data"):
        self.base_url = "https://www.aidedd.org/"
        self.output_dir = Path(output_dir)
        self.output_dir.mkdir(exist_ok=True)
        self.session = requests.Session()
        self.session.headers.update({
            'User-Agent': 'Mozilla/5.0 (DnD Toolbox Educational Scraper)'
        })

    def get_page(self, url, delay=1):
        """Récupère une page web avec un délai pour être respectueux"""
        time.sleep(delay)
        try:
            response = self.session.get(url, timeout=30)
            response.raise_for_status()
            response.encoding = 'utf-8'
            return BeautifulSoup(response.text, 'html.parser')
        except Exception as e:
            print(f"❌ Erreur lors du chargement de {url}: {e}")
            return None

    def scrape_spell_list(self):
        """Récupère la liste de tous les sorts depuis la nouvelle structure"""
        print("\n🔮 Récupération de la liste des sorts...")
        url = self.base_url + "dnd-filters/sorts.php"
        soup = self.get_page(url)

        if not soup:
            return []

        spells = []
        table = soup.find('table', {'class': 'liste'})

        if not table:
            print("❌ Tableau de sorts non trouvé")
            return []

        rows = table.find_all('tr')[1:]  # Skip header

        for row in rows:
            cols = row.find_all('td')
            if len(cols) >= 12:
                link = cols[1].find('a')
                if link:
                    # Extraire les informations
                    name_vf = link.text.strip()
                    url = link['href']
                    name_vo = cols[3].text.strip()
                    level = cols[4].text.strip()
                    school = cols[5].text.strip()
                    casting_time = cols[6].text.strip()
                    range_val = cols[7].text.strip()
                    components = cols[8].text.strip()
                    concentration = 'Concentration' in cols[9].text
                    ritual = 'Rituel' in cols[10].text or 'oui' in cols[10].text.lower()
                    description_short = cols[11].text.strip()
                    source = cols[12].text.strip()

                    spell = {
                        'name': name_vf,
                        'name_en': name_vo,
                        'url': url if url.startswith('http') else self.base_url + url.lstrip('/'),
                        'level': level,
                        'school': school,
                        'casting_time': casting_time,
                        'range': range_val,
                        'components': components,
                        'concentration': concentration,
                        'ritual': ritual,
                        'description_short': description_short,
                        'source': source
                    }
                    spells.append(spell)

        print(f"✅ {len(spells)} sorts trouvés dans la liste")
        return spells

    def scrape_spell_detail(self, spell_info):
        """Récupère les détails complets d'un sort"""
        print(f"  📖 {spell_info['name']}...", end='', flush=True)
        soup = self.get_page(spell_info['url'], delay=0.5)

        if not soup:
            print(" ❌")
            return None

        details = spell_info.copy()

        # Description complète
        description_div = soup.find('div', {'class': 'description'})
        if description_div:
            # Nettoyer la description
            desc_text = description_div.get_text(separator='\n', strip=True)

            # Séparer la description principale des niveaux supérieurs
            if 'Aux niveaux supérieurs' in desc_text or 'niveaux supérieurs' in desc_text:
                parts = re.split(r'Aux niveaux supérieurs[:\s\.]*', desc_text, flags=re.IGNORECASE)
                if len(parts) > 1:
                    details['description'] = parts[0].strip()
                    details['at_higher_levels'] = parts[1].strip()
                else:
                    details['description'] = desc_text
            else:
                details['description'] = desc_text

        # Classes qui peuvent lancer le sort
        class_divs = soup.find_all('div', {'class': 'classe'})
        if class_divs:
            details['classes'] = [div.text.strip() for div in class_divs]

        print(" ✓")
        return details

    def scrape_all_spells(self):
        """Récupère tous les sorts avec leurs détails"""
        spell_list = self.scrape_spell_list()

        if not spell_list:
            print("❌ Aucun sort trouvé")
            return []

        print(f"\n📚 Récupération des détails de {len(spell_list)} sorts...")
        print("Cela peut prendre ~5-10 minutes (délai respectueux de 0.5s entre chaque requête)")

        detailed_spells = []

        for i, spell_info in enumerate(spell_list, 1):
            print(f"[{i}/{len(spell_list)}]", end=" ")
            details = self.scrape_spell_detail(spell_info)
            if details:
                detailed_spells.append(details)

            # Pause plus longue tous les 50 sorts
            if i % 50 == 0:
                print(f"\n  ⏸️  Pause de 5 secondes (respectueux du serveur)...")
                time.sleep(5)

        print(f"\n✅ {len(detailed_spells)} sorts récupérés avec succès")

        # Sauvegarder
        output_file = self.output_dir / "spells.json"
        with open(output_file, 'w', encoding='utf-8') as f:
            json.dump(detailed_spells, f, ensure_ascii=False, indent=2)

        print(f"💾 Sauvegardé dans {output_file}")
        return detailed_spells

    def scrape_monster_list(self):
        """Récupère la liste de tous les monstres"""
        print("\n👹 Récupération de la liste des monstres...")
        url = self.base_url + "dnd-filters/monstres.php"
        soup = self.get_page(url)

        if not soup:
            return []

        monsters = []
        table = soup.find('table', {'class': 'liste'})

        if not table:
            print("❌ Tableau de monstres non trouvé")
            return []

        rows = table.find_all('tr')[1:]  # Skip header

        for row in rows:
            cols = row.find_all('td')
            if len(cols) >= 3:
                link = cols[1].find('a')
                if link:
                    monster = {
                        'name': link.text.strip(),
                        'url': link['href'] if link['href'].startswith('http') else self.base_url + link['href'].lstrip('/'),
                        'type': cols[2].text.strip() if len(cols) > 2 else '',
                        'cr': cols[3].text.strip() if len(cols) > 3 else ''
                    }
                    monsters.append(monster)

        print(f"✅ {len(monsters)} monstres trouvés")
        return monsters

    def scrape_all_monsters(self):
        """Récupère tous les monstres (liste seulement pour le moment)"""
        monster_list = self.scrape_monster_list()

        if not monster_list:
            print("❌ Aucun monstre trouvé")
            return []

        output_file = self.output_dir / "monsters.json"
        with open(output_file, 'w', encoding='utf-8') as f:
            json.dump(monster_list, f, ensure_ascii=False, indent=2)

        print(f"💾 Sauvegardé dans {output_file}")
        print("ℹ️  Note: Détails des monstres non implémentés (parsing complexe)")
        return monster_list

    def scrape_magic_items_list(self):
        """Récupère la liste des objets magiques"""
        print("\n✨ Récupération des objets magiques...")
        url = self.base_url + "dnd-filters/objets-magiques.php"
        soup = self.get_page(url)

        if not soup:
            return []

        items = []
        table = soup.find('table', {'class': 'liste'})

        if not table:
            print("❌ Tableau d'objets magiques non trouvé")
            return []

        rows = table.find_all('tr')[1:]  # Skip header

        for row in rows:
            cols = row.find_all('td')
            if len(cols) >= 2:
                link = cols[1].find('a')
                if link:
                    item = {
                        'name': link.text.strip(),
                        'url': link['href'] if link['href'].startswith('http') else self.base_url + link['href'].lstrip('/'),
                        'type': cols[2].text.strip() if len(cols) > 2 else '',
                        'rarity': cols[3].text.strip() if len(cols) > 3 else ''
                    }
                    items.append(item)

        print(f"✅ {len(items)} objets magiques trouvés")

        output_file = self.output_dir / "magic_items.json"
        with open(output_file, 'w', encoding='utf-8') as f:
            json.dump(items, f, ensure_ascii=False, indent=2)

        print(f"💾 Sauvegardé dans {output_file}")
        return items

    def scrape_all(self):
        """Récupère toutes les données"""
        print("=" * 70)
        print("🎲 SCRAPER AIDEDD.ORG V2 - D&D 5e (2025)")
        print("=" * 70)

        # Sorts (complet avec détails)
        self.scrape_all_spells()

        # Monstres (liste uniquement)
        self.scrape_all_monsters()

        # Objets magiques (liste uniquement)
        self.scrape_magic_items_list()

        print("\n" + "=" * 70)
        print("✅ SCRAPING TERMINÉ")
        print("=" * 70)
        print(f"📁 Fichiers sauvegardés dans : {self.output_dir.absolute()}")


def main():
    parser = argparse.ArgumentParser(
        description="Scraper v2 pour aidedd.org - Récupère les données D&D 5e"
    )
    parser.add_argument(
        '--output', '-o',
        default='scraped_data',
        help='Répertoire de sortie (défaut: scraped_data)'
    )
    parser.add_argument(
        '--spells-only',
        action='store_true',
        help='Récupérer uniquement les sorts'
    )
    parser.add_argument(
        '--monsters-only',
        action='store_true',
        help='Récupérer uniquement les monstres'
    )
    parser.add_argument(
        '--items-only',
        action='store_true',
        help='Récupérer uniquement les objets magiques'
    )
    parser.add_argument(
        '--quick',
        action='store_true',
        help='Mode rapide: liste uniquement sans détails'
    )

    args = parser.parse_args()

    scraper = AideddScraperV2(output_dir=args.output)

    if args.spells_only:
        if args.quick:
            spell_list = scraper.scrape_spell_list()
            output_file = scraper.output_dir / "spells.json"
            with open(output_file, 'w', encoding='utf-8') as f:
                json.dump(spell_list, f, ensure_ascii=False, indent=2)
            print(f"💾 Liste sauvegardée dans {output_file}")
        else:
            scraper.scrape_all_spells()
    elif args.monsters_only:
        scraper.scrape_all_monsters()
    elif args.items_only:
        scraper.scrape_magic_items_list()
    else:
        scraper.scrape_all()


if __name__ == "__main__":
    main()
