#!/usr/bin/env python3
"""Teste la structure d'une page de détail de sort"""

import requests
from bs4 import BeautifulSoup

url = "https://www.aidedd.org/dnd/sorts.php?vf=boule-de-feu"

session = requests.Session()
session.headers.update({
    'User-Agent': 'Mozilla/5.0 (DnD Toolbox Educational Scraper)'
})

response = session.get(url, timeout=30)
response.encoding = 'utf-8'
soup = BeautifulSoup(response.text, 'html.parser')

print("=" * 80)
print("STRUCTURE DE LA PAGE DE DÉTAIL")
print("=" * 80)

# Chercher le contenu principal
print("\n1. Divs avec ID 'pagecontent':")
pagecontent = soup.find('div', {'id': 'pagecontent'})
if pagecontent:
    print("✓ Trouvé")
    # Afficher les sous-éléments
    for child in pagecontent.find_all(['div', 'p', 'strong'], recursive=False, limit=10):
        print(f"  - {child.name}: classes={child.get('class', [])}, texte={child.text[:50]}...")
else:
    print("✗ Non trouvé")

print("\n2. Div 'fiche':")
fiche = soup.find('div', {'class': 'fiche'})
if fiche:
    print("✓ Trouvé")
else:
    print("✗ Non trouvé")

print("\n3. Div 'description':")
description_div = soup.find('div', {'class': 'description'})
if description_div:
    print("✓ Trouvé")
    print(f"  Contenu: {description_div.text[:200]}...")
else:
    print("✗ Non trouvé")

print("\n4. Paragraphes (p):")
all_p = soup.find_all('p')
print(f"  Nombre de <p>: {len(all_p)}")
if all_p:
    for i, p in enumerate(all_p[:5]):
        text = p.text.strip()[:60]
        print(f"  P{i+1}: {text}...")

print("\n5. Recherche du texte 'Classes':")
classes_elem = soup.find(string=lambda text: text and 'Classes' in text and 'lanceur' in text.lower())
if classes_elem:
    print(f"✓ Trouvé: '{classes_elem.strip()[:80]}'")
    # Récupérer l'élément parent
    parent = classes_elem.parent
    print(f"  Parent: {parent.name}, classes={parent.get('class', [])}")
    # Essayer de trouver les classes suivantes
    next_elem = parent.find_next()
    if next_elem:
        print(f"  Suivant: {next_elem.name}, texte='{next_elem.text.strip()[:80]}'")
else:
    print("✗ Non trouvé")

print("\n6. Structure complète du body:")
body = soup.find('body')
if body:
    for child in body.children:
        if hasattr(child, 'name') and child.name:
            classes = child.get('class', [])
            id_attr = child.get('id', '')
            print(f"  - {child.name}: id='{id_attr}', classes={classes}")

print("\n7. Tous les divs principaux:")
main_divs = soup.find_all('div', recursive=False)
for div in main_divs[:10]:
    classes = div.get('class', [])
    id_attr = div.get('id', '')
    print(f"  - id='{id_attr}', classes={classes}")

print("\n8. Texte complet (premiers 500 caractères):")
text = soup.get_text(separator='\n', strip=True)
print(text[:500])
