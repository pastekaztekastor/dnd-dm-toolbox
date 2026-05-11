#!/usr/bin/env python3
"""Test rapide pour voir la structure HTML du site AideDD"""

import requests
from bs4 import BeautifulSoup

url = "https://www.aidedd.org/dnd-filters/sorts.php"

session = requests.Session()
session.headers.update({
    'User-Agent': 'Mozilla/5.0 (DnD Toolbox Educational Scraper)'
})

response = session.get(url, timeout=30)
response.encoding = 'utf-8'
soup = BeautifulSoup(response.text, 'html.parser')

print("=" * 70)
print("RECHERCHE DE TABLEAUX")
print("=" * 70)

# Chercher tous les tableaux
tables = soup.find_all('table')
print(f"\nNombre de tableaux trouvés: {len(tables)}")

for i, table in enumerate(tables):
    classes = table.get('class', [])
    id_attr = table.get('id', '')
    print(f"\nTableau {i+1}:")
    print(f"  Classes: {classes}")
    print(f"  ID: {id_attr}")

    # Compter les lignes
    rows = table.find_all('tr')
    print(f"  Nombre de lignes: {len(rows)}")

    # Afficher la première ligne (header)
    if rows:
        headers = rows[0].find_all(['th', 'td'])
        print(f"  En-têtes: {[h.text.strip() for h in headers[:5]]}")

    # Afficher la première ligne de données
    if len(rows) > 1:
        first_row = rows[1].find_all('td')
        print(f"  Première ligne: {[td.text.strip()[:30] for td in first_row[:3]]}")

print("\n" + "=" * 70)
print("RECHERCHE DE LIENS DE SORTS")
print("=" * 70)

# Chercher les liens vers les sorts
spell_links = soup.find_all('a', href=lambda x: x and 'sorts.php?vf=' in x)
print(f"\nNombre de liens de sorts trouvés: {len(spell_links)}")

if spell_links:
    print("\nPremiers 5 sorts:")
    for link in spell_links[:5]:
        print(f"  - {link.text.strip()}: {link['href']}")

print("\n" + "=" * 70)
print("STRUCTURE DIV/SECTION")
print("=" * 70)

# Chercher d'autres conteneurs possibles
divs_with_class = soup.find_all('div', class_=True)
print(f"\nNombre de divs avec classes: {len(divs_with_class)}")

# Afficher les classes uniques
unique_classes = set()
for div in divs_with_class:
    unique_classes.update(div.get('class', []))

print(f"Classes uniques (premières 20): {list(unique_classes)[:20]}")
