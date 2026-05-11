#!/usr/bin/env python3
"""
Scraper pour aidedd.org - Récupère toutes les données D&D 5e en français
Site : https://www.aidedd.org/adj/
Licence : Contenu libre d'accès (OGL 1.0a)
"""

import requests
from bs4 import BeautifulSoup
import json
import time
import re
from urllib.parse import urljoin, urlparse
import argparse
from pathlib import Path

class AideddScraper:
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
        """Récupère la liste de tous les sorts"""
        print("\n🔮 Récupération de la liste des sorts...")
        url = urljoin(self.base_url, "dnd-filters/sorts.php")
        soup = self.get_page(url)

        if not soup:
            return []

        spells = []
        # Chercher le tableau des sorts
        table = soup.find('table', {'class': 'tabsort'})
        if table:
            for row in table.find_all('tr')[1:]:  # Skip header
                cols = row.find_all('td')
                if len(cols) >= 4:
                    link = cols[0].find('a')
                    if link:
                        spell = {
                            'name': link.text.strip(),
                            'url': urljoin(self.base_url, link['href']),
                            'level': cols[1].text.strip(),
                            'school': cols[2].text.strip(),
                            'ritual': 'Oui' in cols[3].text if len(cols) > 3 else False
                        }
                        spells.append(spell)

        print(f"✅ {len(spells)} sorts trouvés")
        return spells

    def scrape_spell_detail(self, spell_info):
        """Récupère les détails d'un sort"""
        print(f"  📖 {spell_info['name']}...")
        soup = self.get_page(spell_info['url'], delay=0.5)

        if not soup:
            return None

        details = spell_info.copy()

        # Extraire les informations du sort
        content = soup.find('div', {'class': 'fiche'})
        if content:
            # Temps d'incantation
            casting_time = content.find('strong', string=re.compile('Temps d.incantation'))
            if casting_time:
                details['casting_time'] = casting_time.find_next('span').text.strip()

            # Portée
            range_elem = content.find('strong', string=re.compile('Portée'))
            if range_elem:
                details['range'] = range_elem.find_next('span').text.strip()

            # Composantes
            components = content.find('strong', string=re.compile('Composantes'))
            if components:
                comp_text = components.find_next('span').text.strip()
                details['components'] = comp_text
                details['has_verbal'] = 'V' in comp_text
                details['has_somatic'] = 'S' in comp_text
                details['has_material'] = 'M' in comp_text
                # Extraire matériau si présent
                if 'M' in comp_text and '(' in comp_text:
                    material = re.search(r'\((.*?)\)', comp_text)
                    if material:
                        details['material'] = material.group(1)

            # Durée
            duration = content.find('strong', string=re.compile('Durée'))
            if duration:
                dur_text = duration.find_next('span').text.strip()
                details['duration'] = dur_text
                details['concentration'] = 'concentration' in dur_text.lower()

            # Description
            desc_div = content.find('div', {'class': 'description'})
            if desc_div:
                # Nettoyer le texte
                description = desc_div.get_text(separator='\n', strip=True)
                details['description'] = description

                # Chercher "Aux niveaux supérieurs"
                if 'niveaux supérieurs' in description.lower():
                    parts = re.split(r'Aux niveaux supérieurs[:\s]*', description, flags=re.IGNORECASE)
                    if len(parts) > 1:
                        details['description'] = parts[0].strip()
                        details['at_higher_levels'] = parts[1].strip()

            # Classes
            classes_elem = content.find('strong', string=re.compile('Classes'))
            if classes_elem:
                classes_text = classes_elem.find_next('span').text.strip()
                details['classes'] = [c.strip() for c in classes_text.split(',')]

        return details

    def scrape_all_spells(self):
        """Récupère tous les sorts avec leurs détails"""
        spell_list = self.scrape_spell_list()

        if not spell_list:
            print("❌ Aucun sort trouvé")
            return []

        print(f"\n📚 Récupération des détails de {len(spell_list)} sorts...")
        detailed_spells = []

        for i, spell_info in enumerate(spell_list, 1):
            print(f"[{i}/{len(spell_list)}]", end=" ")
            details = self.scrape_spell_detail(spell_info)
            if details:
                detailed_spells.append(details)

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
        url = urljoin(self.base_url, "dnd-filters/monstres.php")
        soup = self.get_page(url)

        if not soup:
            return []

        monsters = []
        table = soup.find('table', {'class': 'tabsort'})
        if table:
            for row in table.find_all('tr')[1:]:
                cols = row.find_all('td')
                if len(cols) >= 3:
                    link = cols[0].find('a')
                    if link:
                        monster = {
                            'name': link.text.strip(),
                            'url': urljoin(self.base_url, link['href']),
                            'type': cols[1].text.strip() if len(cols) > 1 else '',
                            'cr': cols[2].text.strip() if len(cols) > 2 else ''
                        }
                        monsters.append(monster)

        print(f"✅ {len(monsters)} monstres trouvés")
        return monsters

    def scrape_monster_detail(self, monster_info):
        """Récupère les détails d'un monstre"""
        print(f"  👾 {monster_info['name']}...")
        soup = self.get_page(monster_info['url'], delay=0.5)

        if not soup:
            return None

        details = monster_info.copy()

        content = soup.find('div', {'class': 'fiche'})
        if content:
            # Stats de base
            stats = {}
            for strong in content.find_all('strong'):
                label = strong.text.strip().rstrip(':')
                value_elem = strong.find_next_sibling()
                if value_elem:
                    stats[label] = value_elem.text.strip() if hasattr(value_elem, 'text') else str(value_elem).strip()

            details['stats'] = stats

            # Description
            desc_div = content.find('div', {'class': 'description'})
            if desc_div:
                details['description'] = desc_div.get_text(separator='\n', strip=True)

        return details

    def scrape_all_monsters(self):
        """Récupère tous les monstres"""
        monster_list = self.scrape_monster_list()

        if not monster_list:
            print("❌ Aucun monstre trouvé")
            return []

        print(f"\n📚 Récupération des détails de {len(monster_list)} monstres...")
        detailed_monsters = []

        for i, monster_info in enumerate(monster_list, 1):
            print(f"[{i}/{len(monster_list)}]", end=" ")
            details = self.scrape_monster_detail(monster_info)
            if details:
                detailed_monsters.append(details)

        print(f"\n✅ {len(detailed_monsters)} monstres récupérés")

        output_file = self.output_dir / "monsters.json"
        with open(output_file, 'w', encoding='utf-8') as f:
            json.dump(detailed_monsters, f, ensure_ascii=False, indent=2)

        print(f"💾 Sauvegardé dans {output_file}")
        return detailed_monsters

    def scrape_magic_items(self):
        """Récupère tous les objets magiques"""
        print("\n✨ Récupération des objets magiques...")
        url = urljoin(self.base_url, "dnd-filters/objets-magiques.php")
        soup = self.get_page(url)

        if not soup:
            return []

        items = []
        table = soup.find('table', {'class': 'tabsort'})
        if table:
            for row in table.find_all('tr')[1:]:
                cols = row.find_all('td')
                if len(cols) >= 2:
                    link = cols[0].find('a')
                    if link:
                        item = {
                            'name': link.text.strip(),
                            'url': urljoin(self.base_url, link['href']),
                            'rarity': cols[1].text.strip() if len(cols) > 1 else ''
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
        print("=" * 60)
        print("🎲 SCRAPER AIDEDD.ORG - D&D 5e")
        print("=" * 60)

        # Sorts
        self.scrape_all_spells()

        # Monstres
        self.scrape_all_monsters()

        # Objets magiques
        self.scrape_magic_items()

        print("\n" + "=" * 60)
        print("✅ SCRAPING TERMINÉ")
        print("=" * 60)
        print(f"📁 Fichiers sauvegardés dans : {self.output_dir.absolute()}")


def main():
    parser = argparse.ArgumentParser(
        description="Scraper pour aidedd.org - Récupère les données D&D 5e"
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

    args = parser.parse_args()

    scraper = AideddScraper(output_dir=args.output)

    if args.spells_only:
        scraper.scrape_all_spells()
    elif args.monsters_only:
        scraper.scrape_all_monsters()
    elif args.items_only:
        scraper.scrape_magic_items()
    else:
        scraper.scrape_all()


if __name__ == "__main__":
    main()
