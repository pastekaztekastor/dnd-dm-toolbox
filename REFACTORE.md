# Refactoring de l'app pour remmetre au propre. 

Bon dans les problèmes principaux qui me pose problème pour le dev car c'est larchitecture meme du projet qui n'est pas bonne. 

- Chaque plugin doit avoir sa propre DB. 
- Si un plugin A à besoin des infos fournit dans un plugin B il demande des donnée au A qui les lui passe sur le pipeline de com interplugin. 
- Les plugin ont des cétégorie (ex : sort, race, classe) pour q'un plugin sache s'il existe le plugin qui manage la donné dont il à besoin. 

En gros : 
- les plugin embarque : Leur logique, leur rendu, leur data, leur datamanageur. 
- L'app (le moteur) gère : La com inter plugin, le chargement des DLL 
- Les plugin sont précompilé. 

## Compilation 

Je n'utilise pas Docker qui est pourtant la il faut vraiemnt que je l'utilise. Donc les compilation doivent toute passé par docker. 

## Organisation du git 

- sur main le moteur
- chaque plugin à sa branche 
- archi Trunk based

## Organisation de l'existant. 

On sépare bien chaque plugin. Pour le moment j'ai tout voulu centrer dans un gentionnaire de fiche de perso, sauf qu'il repose sur plein de chose tière. Donc on vas tout séparer. 

- Race.
- Classe
- Sort
- Stuf

