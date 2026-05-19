# Explication du TEXTFORMATEUR

l'idée c'est que le texte de tout les champs de text puisseêtre fomater pour faire du MD + alias
Ca doit pouvoir prendre en charge le markdown donc : 
- les titre 
- liste à puce
- italique 
- gras 
- tableau
- maths
- alias.

## Les alias.

Chaque élément en DB possède un alias. si j'ecrit ```@troll``` je dois avoir un lien clicable qui m'affiche la page du montre troll et ma l'affic. Pour que ça soit possible toutes les table aves un alias doivent avoire DEUX méthodes 

- ```OnOverFlow()```
- ```OnClick()``` 

## Roadmap 

- TODO : Créer la classe/l'élément TEXTFORMATEUR .cpp et .h
- TODO : Faire les DEUX méthode. 