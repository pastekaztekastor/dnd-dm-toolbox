#!/usr/bin/env python3
"""Teste la structure des colonnes du tableau de sorts"""

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

table = soup.find('table', {'class': 'liste'})

if table:
    rows = table.find_all('tr')

    # Header
    print("=" * 80)
    print("HEADER (première ligne)")
    print("=" * 80)
    headers = rows[0].find_all(['th', 'td'])
    for i, h in enumerate(headers):
        text = h.text.strip()
        classes = h.get('class', [])
        print(f"Colonne {i}: '{text}' (classes: {classes})")

    # Première ligne de données
    print("\n" + "=" * 80)
    print("PREMIÈRE LIGNE DE DONNÉES")
    print("=" * 80)
    if len(rows) > 1:
        cols = rows[1].find_all('td')
        for i, col in enumerate(cols):
            text = col.text.strip()[:50]
            classes = col.get('class', [])
            link = col.find('a')
            print(f"\nColonne {i}: (classes: {classes})")
            print(f"  Texte: '{text}'")
            if link:
                print(f"  Lien: {link.get('href', 'N/A')}")

    # Deuxième ligne pour comparaison
    print("\n" + "=" * 80)
    print("DEUXIÈME LIGNE DE DONNÉES (pour comparaison)")
    print("=" * 80)
    if len(rows) > 2:
        cols = rows[2].find_all('td')
        for i, col in enumerate(cols):
            text = col.text.strip()[:50]
            link = col.find('a')
            lien_info = f" | Lien: {link.get('href', '')}" if link else ""
            print(f"Col {i}: '{text}'{lien_info}")
