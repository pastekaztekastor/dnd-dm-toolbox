-- race_dd_5_5_seed.sql — Races D&D 5e PHB (VF)
-- Converti depuis Postgres ($$..$$ → '..'' + id explicites)

-- =============================================================================
-- HUMAIN
-- =============================================================================
INSERT INTO races (id, nom, alias, description, aide_joueur,
    bonus_forces, bonus_dexterite, bonus_constitution,
    bonus_intelligence, bonus_sagesse, bonus_charisme,
    age_adulte, age_max,
    taille_min, taille_max, taille_base, taille_facteur, dee_de_mod,
    poids_base, poids_facteur, vitesse_base, liste_langues, source)
VALUES ('humain', 'Humain', 'humain',
    'Dans l''histoire de bien des mondes, les humains forment la plus jeune des races communes et sont arrivés bien tardivement sur la scène mondiale comparés aux nains, aux elfes et aux dragons. Peut-être est-ce à cause de leur espérance de vie réduite qu''ils tentent d''accomplir tant de choses lors de leur courte vie. Quelles que soient leurs motivations, les humains sont des innovateurs qui rencontrent un franc succès et deviennent les pionniers de leur monde.',
    'Idéal pour les joueurs qui veulent une race polyvalente sans spécialisation. Le +1 à toutes les caractéristiques s''adapte à n''importe quelle classe.',
    1, 1, 1, 1, 1, 1, 18, 100,
    150, 190, 145, 1, '2D10', 60, 4, 30, 'Commun + une langue au choix', 'Player''s Handbook');

INSERT INTO race_presentations (id, race_id, ordre, titre, texte) VALUES
    ('humain-p1', 'humain', 1, 'Humain',
    'Dans l''histoire de bien des mondes, les humains forment la plus jeune des races communes et sont arrivés bien tardivement sur la scène mondiale comparés aux nains, aux elfes et aux dragons. Peut-être est-ce à cause de leur espérance de vie réduite qu''ils tentent d''accomplir tant de choses lors de leur courte vie. Ou peut-être pensent-ils avoir quelque chose à prouver aux races plus anciennes et bâtissent donc leurs puissants empires sur des fondations faites de commerce et de conquêtes. Quelles que soient leurs motivations, les humains sont des innovateurs qui rencontrent un franc succès et deviennent les pionniers de leur monde.'),
    ('humain-p2', 'humain', 2, 'Un large spectre',
    'Avec leur penchant pour la conquête et les migrations, les humains présentent une diversité physique bien supérieure à celle des autres races les plus courantes. Il n''y a pas d''humain typique. Un humain peut mesurer de 1,50 mètre à un peu plus de 1,80 mètre et pèse entre 60 et 125 kilos. Sa carnation va du très pâle au presque noir et ses cheveux (crépus, bouclés ou raides) vont du noir de jais au blond.

De nombreux humains possèdent un peu de sang étranger, révélant des traces de lignée elfique, orque ou autre. Les humains atteignent l''âge adulte un peu avant la vingtaine et survivent rarement jusqu''à leur premier siècle.'),
    ('humain-p3', 'humain', 3, 'De la variété en toutes choses',
    'De toutes les races communes, les humains sont les plus adaptables et les plus ambitieux. Ils ont des goûts, une éthique et des coutumes très variés, en fonction du pays où ils se sont installés. Et quand ils s''installent, c''est pour longtemps : ils construisent des villes conçues pour résister au passage du temps et de grands royaumes capables de survivre pendant des siècles.'),
    ('humain-p4', 'humain', 4, 'Des institutions durables',
    'Là où un seul elfe ou nain prend la responsabilité de protéger un lieu ou un secret, les humains fondent des ordres sacrés et des institutions pour remplir la même fonction. Les humains rêvent d''immortalité, mais, à l''exception des rares individus qui tentent d''échapper aux griffes de la mort en devenant des morts-vivants ou des dieux, ils y parviennent en s''assurant que l''on se souviendra d''eux après leur mort.'),
    ('humain-p5', 'humain', 5, 'Des parangons d''ambition',
    'Les humains qui partent en quête d''aventure font partie des membres les plus téméraires et ambitieux d''une race déjà ambitieuse et téméraire. Ils cherchent à se couvrir de gloire devant leurs concitoyens en amassant puissance, richesses et célébrité. Plus que tout autre peuple, les humains ont tendance à se faire les champions de grandes causes plutôt que de territoires ou de groupes.'),
    ('humain-p6', 'humain', 6, 'Le deuxième meilleur ami de tout le monde',
    'Les humains se mêlent aux autres races aussi facilement qu''à la leur et s''entendent bien avec tout le monde, bien qu''ils soient rarement proches d''autrui.

**Les nains.** « Ils sont robustes, fidèles en amitié et respectent leur parole. En revanche, leur amour de l''or causera leur perte. »

**Les elfes.** « Il vaut mieux éviter de se promener dans les bois des elfes. Ils n''aiment pas les intrus et vous avez autant de chances de vous faire ensorceler que de vous faire cribler de flèches. Mais si un elfe arrive à surmonter son maudit orgueil racial et à vous considérer comme un égal, vous pourrez apprendre beaucoup de lui. »

**Les halfelins.** « C''est dur de faire mieux qu''un repas chez des halfelins, à condition de pas s''ouvrir le crâne sur leur plafond. Si les halfelins avaient ne serait-ce qu''un peu d''ambition, on pourrait vraiment faire quelque chose d''eux. »'),
    ('humain-p7', 'humain', 7, 'Noms et ethnies des humains',
    'Comme la race humaine possède bien plus de cultures différentes que les autres, les humains n''ont pas de noms typiques. Certains parents donnent des noms étrangers à leurs enfants, par exemple tirés du nain ou de l''elfique, mais la plupart baptisent leurs enfants en fonction de leur culture ou des traditions baptismales de leurs ancêtres.');

INSERT INTO race_noms (id, race_id, ordre, titre, texte) VALUES
    ('humain-n1',  'humain', 1,  'Calishites (masculins)', 'Aseir, Bardeid, Haseid, Khemed, Mehmen, Sudeiman, Zasheir'),
    ('humain-n2',  'humain', 2,  'Calishites (féminins)', 'Atala, Ceidil, Hama, Jasmal, Meilil, Seipora, Yasheira, Zasheida'),
    ('humain-n3',  'humain', 3,  'Chondathiens (masculins)', 'Darvin, Dorn, Evendur, Gorstag, Grim, Helm, Malark, Morn, Randal, Stedd'),
    ('humain-n4',  'humain', 4,  'Chondathiens (féminins)', 'Arveene, Esvele, Jhessail, Kerri, Lureene, Miri, Rowan, Shandri, Tessèle'),
    ('humain-n5',  'humain', 5,  'Damariens (masculins)', 'Bor, Fodel, Glar, Grigor, Igan, Ivor, Kosef, Mival, Orel, Pavel, Sergor'),
    ('humain-n6',  'humain', 6,  'Damariens (féminins)', 'Alèthre, Kara, Katernine, Mara, Natali, Olma, Tana, Zora'),
    ('humain-n7',  'humain', 7,  'Illuskiens (masculins)', 'Andre, Blath, Bran, Frath, Geth, Landre, Luth, Malcre, Stor, Taman, Urth'),
    ('humain-n8',  'humain', 8,  'Illuskiens (féminins)', 'Amafrey, Bèthe, Cefrey, Kethra, Mara, Olga, Silifrey, Westra'),
    ('humain-n9',  'humain', 9,  'Turamis (masculins)', 'Anton, Diero, Marcon, Pieron, Rimardo, Roméro, Salazar, Umbéro'),
    ('humain-n10', 'humain', 10, 'Turamis (féminins)', 'Balama, Dona, Faila, Jalana, Luisa, Marta, Quara, Sélise, Vonda');

INSERT INTO race_traits (id, race_id, ordre, titre, texte) VALUES
    ('humain-t1', 'humain', 1, 'Augmentation de caractéristiques', 'La valeur de toutes vos caractéristiques augmente de 1.'),
    ('humain-t2', 'humain', 2, 'Âge', 'Les humains deviennent adultes à la fin de l''adolescence et vivent moins d''un siècle.'),
    ('humain-t3', 'humain', 3, 'Taille', 'Les humains ont des tailles et des carrures très variées. Ils peuvent faire 1,50 mètre comme plus de 1,80 mètre. Vous êtes considéré comme une créature de taille Moyenne.'),
    ('humain-t4', 'humain', 4, 'Vitesse', 'Votre vitesse de base au sol est de 9 mètres.'),
    ('humain-t5', 'humain', 5, 'Langues', 'Vous pouvez parler, lire et écrire le commun et une langue supplémentaire de votre choix. Les humains apprennent généralement la langue des peuples qu''ils fréquentent.');

-- =============================================================================
-- ELFE
-- =============================================================================
INSERT INTO races (id, nom, alias, description, aide_joueur,
    bonus_dexterite, age_adulte, age_max,
    taille_min, taille_max, taille_base, taille_facteur, dee_de_mod,
    poids_base, poids_facteur, vitesse_base, liste_langues, source)
VALUES ('elfe', 'Elfe', 'elfe',
    'Les elfes forment un peuple magique à la grâce surnaturelle. Ils vivent en des lieux à la beauté éthérée, au cœur d''antiques forêts ou dans des tours d''argent luisant d''un éclat féerique. Les elfes aiment la nature et la magie, les arts et le talent artistique, la musique et la poésie, et toutes les bonnes choses de la vie.',
    'Idéal pour les joueurs qui veulent un personnage agile et résistant aux charmes. Choisissez votre sous-race selon votre style de jeu : haut-elfe pour la magie, sylvestre pour la mobilité, drow pour une ambiance sombre.',
    2, 100, 750, 150, 180, 145, 1, '2D8', 50, 1, 30, 'Commun, Elfique', 'Player''s Handbook');

INSERT INTO race_presentations (id, race_id, ordre, titre, texte) VALUES
    ('elfe-p1', 'elfe', 1, 'Elfe',
    'Les elfes forment un peuple magique à la grâce surnaturelle. Ils résident en des lieux à la beauté éthérée, au cœur d''antiques forêts ou dans des tours d''argent luisant d''un éclat féerique, là où une douce musique flotte dans les airs et une brise parfumée enchante les narines. Les elfes aiment la nature et la magie, les arts et le talent artistique, la musique et la poésie, et toutes les bonnes choses de la vie.'),
    ('elfe-p2', 'elfe', 2, 'Minces et gracieux',
    'Les elfes sont dotés d''une grâce surnaturelle et de traits élégants qui les rendent incroyablement séduisants aux yeux des humains et de nombreuses autres races. Ils sont un peu plus grands qu''un humain, allant de moins de 1,50 mètre à 1,80 mètre et sont plus minces qu''eux, pesant dans les 50 à 75 kilos.

La carnation elfique couvre le même spectre que celle des humains, en lui ajoutant des teintes de cuivre, de bronze, ainsi que des cheveux verts ou bleus tirant parfois presque sur le blanc, et des yeux semblables à de l''or ou de l''argent liquides. Les elfes aiment les habits élégants aux couleurs vives et apprécient les jolis bijoux simples.'),
    ('elfe-p3', 'elfe', 3, 'Une perspective intemporelle',
    'Les elfes peuvent vivre plus de 700 ans, ce qui leur donne une vision très globale des événements. Ils sont plus souvent amusés que passionnés et plus facilement curieux qu''avides. Ils restent à l''écart et impassibles devant les événements mineurs. En revanche, quand ils poursuivent un objectif, ils restent inlassablement concentrés sur leur tâche.

Face au danger, les elfes sont comme les branches d''un arbrisseau : souples et flexibles. Ils comptent sur la diplomatie et les compromis pour résoudre leurs différends avant de recourir à la violence.'),
    ('elfe-p4', 'elfe', 4, 'Des royaumes forestiers cachés',
    'La plupart des elfes résident dans de petits villages forestiers dissimulés parmi les arbres. Ils chassent du gibier, partent cueillir de la nourriture et cultivent des légumes, leurs talents et leur magie leur permettant de s''alimenter sans avoir besoin d''abattre les arbres ni de labourer la terre.'),
    ('elfe-p5', 'elfe', 5, 'L''exploration et l''aventure',
    'Les elfes partent à l''aventure quand ils éprouvent une insatiable soif de voyages. Comme ils ont une espérance de vie très longue, ils peuvent passer des siècles à explorer et découvrir le monde. Un elfe aime à développer ses talents d''épéiste et sa puissance magique, ce qu''il peut faire aisément en menant une vie d''aventurier.'),
    ('elfe-p6', 'elfe', 6, 'Hautains mais courtois',
    'Les elfes peuvent se montrer hautains, mais ils sont généralement courtois, même envers ceux qui déçoivent leurs attentes si exigeantes.

**Les halfelins.** « Les halfelins aiment les plaisirs simples et c''est une qualité qu''il ne faut pas mépriser. Ce sont de bonnes gens, qui prennent soin les uns des autres, s''occupent de leur jardin et ont prouvé qu''ils étaient bien plus résistants qu''ils n''en ont l''air quand le besoin s''en fait sentir. »

**Les humains.** « Toute cette hâte, ces ambitions et ce besoin d''accomplir quelque chose avant de mourir au terme de leur brève existence... Les entreprises humaines semblent parfois si futiles. Mais quand vous étudiez ce qu''ils ont accompli, vous devez leur reconnaître une certaine valeur. »

**Les nains.** « Les nains sont des lourdauds maladroits et moroses, ceci dit, ils compensent en courage tout ce qui leur manque en humour, sophistication et bonnes manières. »'),
    ('elfe-p7', 'elfe', 7, 'Les noms des elfes',
    'Un elfe est considéré comme un enfant jusqu''à ce qu''il se déclare adulte, ce qui arrive parfois un peu après son premier siècle. L''elfe se choisit un nom d''adulte quand il se proclame adulte. Il y a peu de distinction entre les noms féminins et masculins. De plus, tout elfe porte un nom de famille qui se compose habituellement d''une combinaison de mots elfiques.');

INSERT INTO race_noms (id, race_id, ordre, titre, texte) VALUES
    ('elfe-n1', 'elfe', 1, 'Noms d''enfant', 'Ara, Bryn, Del, Éryn, Faen, Innil, Lael, Mella, Naill, Naéris, Phann, Rael, Rinn, Sai, Syllin, Thia, Vall'),
    ('elfe-n2', 'elfe', 2, 'Noms masculins', 'Adran, Aélar, Aramil, Arannis, Aust, Beiro, Berrian, Carric, Énialis, Erdan, Erevan, Galinndan, Hadaraï, Heian, Himo, Immeral, Ivellios, Laucian, Mindartis, Paelias, Péren, Quarion, Riardon, Rolen, Soveliss, Thamior, Tharivol, Théren, Varis'),
    ('elfe-n3', 'elfe', 3, 'Noms féminins', 'Adrie, Althaéa, Anastrianne, Andraste, Antinua, Béthrynne, Birel, Caélynn, Drusilia, Enna, Félosial, Iélénia, Jelenneth, Keyleth, Leshanna, Lia, Meriele, Mialee, Naivara, Quélenna, Quillathe, Sariel, Shanairra, Shava, Silaqui, Theirastre, Thia, Vadania, Valanthe, Xanaphie'),
    ('elfe-n4', 'elfe', 4, 'Noms de famille', 'Amakiir (Fleurdegemme), Amastacie (Fleurstellaire), Galanodel (Murmurelunaire), Holimion (Roséedediamant), Ilphelkiir (Boutondegemme), Liadon (Feuilleargent), Méliamne (Pieddechêne), Naïlo (Brisenocturne), Siannodel (Ruisseaulunaire), Xiloscient (Pétald''or)');

INSERT INTO race_traits (id, race_id, ordre, titre, texte) VALUES
    ('elfe-t1', 'elfe', 1, 'Augmentation de caractéristiques', 'Votre valeur de Dextérité augmente de 2.'),
    ('elfe-t2', 'elfe', 2, 'Âge', 'Bien que les elfes atteignent leur maturité physique environ au même âge que les humains, ils considèrent qu''être un adulte dépasse ce simple aspect et prend en compte l''expérience de la vie. Un elfe se déclare généralement adulte vers 100 ans et choisit à ce moment son nom d''adulte. Les elfes peuvent vivre jusqu''à 750 ans.'),
    ('elfe-t3', 'elfe', 3, 'Taille', 'Les elfes mesurent généralement entre 1,50 et 1,80 mètre et sont minces. Ils sont de taille Moyenne.'),
    ('elfe-t4', 'elfe', 4, 'Vitesse', 'Votre vitesse de base au sol est de 9 mètres.'),
    ('elfe-t5', 'elfe', 5, 'Vision dans le noir', 'Dans un rayon de 18 mètres, vous pouvez voir dans une zone de lumière faible comme s''il s''agissait d''une lumière vive et dans l''obscurité comme s''il s''agissait d''une lumière faible. Par contre, vous ne distinguez pas les couleurs dans l''obscurité, seulement des nuances de gris.'),
    ('elfe-t6', 'elfe', 6, 'Sens aiguisés', 'Vous maîtrisez la compétence Perception.'),
    ('elfe-t7', 'elfe', 7, 'Ascendance féerique', 'Vous êtes avantagé sur les jets de sauvegarde contre l''effet charmé et vous ne pouvez être contraint à dormir par la magie.'),
    ('elfe-t8', 'elfe', 8, 'Transe', 'Les elfes n''ont pas besoin de dormir. À la place, ils passent 4 heures par jour dans un état de méditation profonde, tout en restant semi-conscients. En vous reposant ainsi, vous bénéficiez des mêmes avantages conférés par 8 heures de sommeil qu''un humain.'),
    ('elfe-t9', 'elfe', 9, 'Langues', 'Vous pouvez parler, lire et écrire le commun et l''elfique. Le langage des elfes est fluide, avec des intonations subtiles et une grammaire élaborée. La littérature des elfes est riche et variée.');

-- Sous-race : Haut-elfe
INSERT INTO races (id, nom, alias, race_parent_id, bonus_intelligence, source)
VALUES ('haut-elfe', 'Haut-elfe', 'haut-elfe', 'elfe', 1, 'Player''s Handbook');

INSERT INTO race_presentations (id, race_id, ordre, titre, texte) VALUES
    ('haut-elfe-p1', 'haut-elfe', 1, 'Haut-elfe',
    'Les hauts-elfes ont l''esprit vif et maîtrisent au moins les bases de la magie. Dans de nombreux mondes de D&D, on trouve deux types de hauts-elfes : les premiers sont hautains et secrets, convaincus d''être supérieurs aux autres races. Les seconds sont plus nombreux et plus amicaux, et on les trouve souvent parmi les humains et les autres races.');

INSERT INTO race_traits (id, race_id, ordre, titre, texte) VALUES
    ('haut-elfe-t1', 'haut-elfe', 1, 'Augmentation de caractéristique', 'Votre valeur d''Intelligence augmente de 1.'),
    ('haut-elfe-t2', 'haut-elfe', 2, 'Entraînement aux armes elfiques', 'Vous maîtrisez les épées longues, les épées courtes, les arcs longs et les arcs courts.'),
    ('haut-elfe-t3', 'haut-elfe', 3, 'Sort mineur', 'Vous connaissez un sort mineur de votre choix parmi la liste de sorts de magicien. Votre caractéristique d''incantation pour ce sort mineur est l''Intelligence.'),
    ('haut-elfe-t4', 'haut-elfe', 4, 'Langue supplémentaire', 'Vous pouvez parler, lire et écrire une langue supplémentaire de votre choix.');

-- Sous-race : Elfe sylvestre
INSERT INTO races (id, nom, alias, race_parent_id, bonus_sagesse, source)
VALUES ('elfe-sylvestre', 'Elfe sylvestre', 'elfe-sylvestre', 'elfe', 1, 'Player''s Handbook');

INSERT INTO race_presentations (id, race_id, ordre, titre, texte) VALUES
    ('elfe-sylvestre-p1', 'elfe-sylvestre', 1, 'Elfe sylvestre',
    'En tant qu''elfe des bois, vous avez des sens aiguisés et une excellente intuition. De plus, vos pieds agiles vous emportent rapidement et sans bruit au travers de votre forêt natale. Sur Féerune, les elfes sylvestres forment un peuple isolationniste qui n''accorde aucune confiance aux membres des races autres qu''elfiques.');

INSERT INTO race_traits (id, race_id, ordre, titre, texte) VALUES
    ('elfe-sylvestre-t1', 'elfe-sylvestre', 1, 'Augmentation de caractéristique', 'Votre valeur de Sagesse augmente de 1.'),
    ('elfe-sylvestre-t2', 'elfe-sylvestre', 2, 'Entraînement aux armes elfiques', 'Vous maîtrisez les épées longues, les épées courtes, les arcs longs et les arcs courts.'),
    ('elfe-sylvestre-t3', 'elfe-sylvestre', 3, 'Foulée légère', 'Votre vitesse de base passe à 10,5 mètres.'),
    ('elfe-sylvestre-t4', 'elfe-sylvestre', 4, 'Cachette naturelle', 'Vous pouvez vous cacher même quand la visibilité n''est que légèrement réduite du fait des frondaisons, d''une forte pluie, de chutes de neige, d''une nappe de brume ou tout autre phénomène naturel.');

-- Sous-race : Elfe noir (Drow)
INSERT INTO races (id, nom, alias, race_parent_id, bonus_charisme, source)
VALUES ('drow', 'Elfe noir (Drow)', 'drow', 'elfe', 1, 'Player''s Handbook');

INSERT INTO race_presentations (id, race_id, ordre, titre, texte) VALUES
    ('drow-p1', 'drow', 1, 'Elfe noir (Drow)',
    'Comme tout drow, vous êtes imprégné par la magie d''Outreterre, ce royaume souterrain fait de merveilles et d''horreurs presque inconnues à la surface. Les ombres restent votre domaine et vos aptitudes innées vous permettent d''invoquer la lumière comme les ténèbres. Les vôtres affichent généralement une chevelure blanche et une peau grise, d''un ton plus ou moins profond.');

INSERT INTO race_traits (id, race_id, ordre, titre, texte) VALUES
    ('drow-t1', 'drow', 1, 'Augmentation de caractéristique', 'Votre valeur de Charisme augmente de 1.'),
    ('drow-t2', 'drow', 2, 'Vision dans le noir supérieure', 'Votre vision dans le noir fonctionne dans un rayon de 36 mètres.'),
    ('drow-t3', 'drow', 3, 'Sensibilité au soleil', 'Vous êtes désavantagé lors des jets d''attaque et des tests de Sagesse (Perception) basés sur la vue lorsque vous, votre cible ou ce que vous tentez de percevoir se trouve sous la lumière directe du soleil.'),
    ('drow-t4', 'drow', 4, 'Magie drow', 'Vous connaissez le sort mineur *lumières dansantes*. Quand vous atteignez le niveau 3, vous pouvez aussi lancer *lueurs féeriques* une fois par repos long. Quand vous atteignez le niveau 5, vous pouvez également lancer *ténèbres* une fois par repos long. Votre caractéristique incantatoire est le Charisme.'),
    ('drow-t5', 'drow', 5, 'Formation martiale drow', 'Vous maîtrisez les rapières, les épées courtes et les arbalètes de poing.');

-- =============================================================================
-- NAIN
-- =============================================================================
INSERT INTO races (id, nom, alias, description, aide_joueur,
    bonus_constitution, age_adulte, age_max,
    taille_min, taille_max, taille_base, taille_facteur, dee_de_mod,
    poids_base, poids_facteur, vitesse_base, liste_langues, source)
VALUES ('nain', 'Nain', 'nain',
    'Des royaumes riches d''une antique splendeur, des salles taillées dans les racines des montagnes, l''écho des pioches et des marteaux dans les profondeurs des mines et au cœur des forges aux flammes crépitantes, un engagement envers le clan et les traditions, une haine farouche envers les gobelins et les orcs... voici les traits communs à tous les nains.',
    'Idéal pour les joueurs qui veulent un personnage résistant et tenace. Le +2 en Constitution s''adapte à la plupart des classes de mêlée. Le nain des collines convient bien au clerc, le nain des montagnes au guerrier.',
    2, 50, 350, 120, 150, 115, 1, '2D8', 70, 1, 25, 'Commun, Nain', 'Player''s Handbook');

INSERT INTO race_presentations (id, race_id, ordre, titre, texte) VALUES
    ('nain-p1', 'nain', 1, 'Nain',
    'Des royaumes riches d''une antique splendeur, des salles taillées dans les racines des montagnes, l''écho des pioches et des marteaux dans les profondeurs des mines et au cœur des forges aux flammes crépitantes, un engagement envers le clan et les traditions, une haine farouche envers les gobelins et les orcs... voici les traits communs à tous les nains.'),
    ('nain-p2', 'nain', 2, 'Petit et costaud',
    'Les nains sont des êtres courageux et résistants, connus pour leurs talents de guerriers, de mineurs et de travailleurs de la pierre et du métal. Ils mesurent moins d''un mètre cinquante, mais ils ont une large carrure et un corps compact et pèsent donc aussi lourd qu''un humain d''une soixantaine de centimètres de plus qu''eux.

La pigmentation du nain va du brun sombre à des carnations plus pâles, parfois teintées de rouge, mais la plupart du temps, un nain a la peau beige ou brune. Les hommes accordent une grande valeur à leur barbe et en prennent grand soin.'),
    ('nain-p3', 'nain', 3, 'Bonne mémoire et forte rancœur',
    'Un nain peut vivre dans les 400 ans, les plus âgés se souviennent donc souvent d''un monde bien différent de l''actuel. Les nains sont aussi solides et résistants que les montagnes qu''ils aiment. Ils respectent les traditions de leur clan, leurs arbres généalogiques remontant jusqu''à la fondation de leurs plus anciennes places fortes.

Un nain est loyal et déterminé, fidèle à sa parole et résolu dans ses actes, parfois au point de se comporter avec obstination.'),
    ('nain-p4', 'nain', 4, 'Les clans et les royaumes',
    'Les royaumes nains s''étendent profondément sous les montagnes, là où leurs habitants extraient des gemmes et des métaux précieux et forgent de véritables merveilles. Ils aiment la beauté, l''aspect artistique des métaux précieux et des beaux bijoux. Le clan représente l''unité principale de la société naine. Se voir privé de clan, voilà la pire chose qui puisse arriver à un nain.'),
    ('nain-p5', 'nain', 5, 'Lents à faire confiance',
    '« La différence entre une connaissance et un ami, c''est juste un petit siècle. » Ce dicton nain montre bien à quel point il est difficile pour un membre des races à l''espérance de vie réduite de gagner la confiance d''un nain.

**Les elfes.** « La sagesse conseille de ne pas dépendre des elfes. Parce qu''on ne sait jamais ce qu''ils vont faire. Mais quand la montagne se met à vomir des orcs ou des gobelins, c''est bien d''avoir un elfe pour couvrir vos arrières. »

**Les halfelins.** « Ils sont de bonne compagnie, ça, c''est sûr. Mais trouvez-moi ne serait-ce qu''un seul héros halfelin ! »

**Les humains.** « Vous prenez un peu de temps pour apprendre à connaître un humain... et le voilà déjà sur son lit de mort. Regardez-les faire ! Dès qu''ils mettent quelque chose en tête, ils l''obtiennent. Ce genre de dévotion, c''est admirable. »');

INSERT INTO race_noms (id, race_id, ordre, titre, texte) VALUES
    ('nain-n1', 'nain', 1, 'Noms masculins', 'Adrik, Albérich, Baern, Barendd, Brottor, Bruenor, Dain, Darrak, Delg, Eberk, Einkil, Fargrim, Flint, Gardain, Harbek, Kildrak, Morgran, Orsik, Oskar, Rangrim, Rurik, Taklinn, Thoradin, Thorin, Tordek, Traubon, Travok, Ulfgar, Veit, Vondal'),
    ('nain-n2', 'nain', 2, 'Noms féminins', 'Ambre, Artin, Audhild, Bardryn, Dagnal, Diesa, Eldeth, Falkrunn, Finellen, Gunnloda, Gurdis, Helja, Hlin, Kathra, Kristryd, Ilde, Liftrasa, Mardred, Riswynn, Sannl, Torbera, Torgga, Vistra'),
    ('nain-n3', 'nain', 3, 'Noms de clan', 'Balderk, Barbegelée, Dankil, Forgefeu, Fortenclume, Gorunn, Holderhek, Loderr, Lutgehr, Marteaudeguerre, Poing de Fer, Rumnaheim, Strakeln, Torunn, Ungart');

INSERT INTO race_traits (id, race_id, ordre, titre, texte) VALUES
    ('nain-t1',  'nain', 1,  'Bonus de caractéristiques', 'Votre valeur de Constitution augmente de 2.'),
    ('nain-t2',  'nain', 2,  'Âge', 'Les nains deviennent matures au même rythme que les humains, mais on considère qu''ils sont encore jeunes jusqu''à l''âge de 50 ans. Ils vivent en moyenne 350 ans.'),
    ('nain-t3',  'nain', 3,  'Taille', 'Les nains mesurent entre 1,20 et 1,50 mètre et pèsent en moyenne 70 kilos. Ils sont de taille Moyenne.'),
    ('nain-t4',  'nain', 4,  'Vitesse', 'Votre vitesse de base au sol est de 7,50 mètres. Vous pouvez porter une armure lourde sans que cela réduise votre vitesse.'),
    ('nain-t5',  'nain', 5,  'Vision dans le noir', 'Dans un rayon de 18 mètres, vous pouvez voir dans une zone de lumière faible comme s''il s''agissait d''une lumière vive et dans l''obscurité comme s''il s''agissait d''une lumière faible. Par contre, vous ne distinguez pas les couleurs dans l''obscurité, seulement des nuances de gris.'),
    ('nain-t6',  'nain', 6,  'Résistance naine', 'Vous êtes avantagé sur vos jets de sauvegarde contre le poison et vous possédez une résistance contre les dégâts de poison.'),
    ('nain-t7',  'nain', 7,  'Entraînement aux armes naines', 'Vous maîtrisez les haches d''armes, les hachettes, les marteaux légers et les marteaux de guerre.'),
    ('nain-t8',  'nain', 8,  'Maîtrise des outils', 'Vous maîtrisez les outils d''artisan de votre choix parmi cette liste : outils de forgeron, matériel de brasseur, outils de maçon.'),
    ('nain-t9',  'nain', 9,  'Connaissance de la pierre', 'Quand vous faites un test d''Intelligence (Histoire) relatif au travail de la pierre, vous ajoutez le double de votre bonus de maîtrise au résultat du test.'),
    ('nain-t10', 'nain', 10, 'Langues', 'Vous pouvez parler, lire et écrire le commun et le nain. Le nain est une langue aux consonnes dures et aux sons gutturaux.');

-- Sous-race : Nain des collines
INSERT INTO races (id, nom, alias, race_parent_id, bonus_sagesse, source)
VALUES ('nain-des-collines', 'Nain des collines', 'nain-des-collines', 'nain', 1, 'Player''s Handbook');

INSERT INTO race_presentations (id, race_id, ordre, titre, texte) VALUES
    ('nain-des-collines-p1', 'nain-des-collines', 1, 'Nain des collines',
    'En tant que nain des collines, vous avez des sens aiguisés, une excellente intuition et une résistance remarquable. Les nains d''or des puissants royaumes du sud de Féerune sont des nains des collines.');

INSERT INTO race_traits (id, race_id, ordre, titre, texte) VALUES
    ('nain-des-collines-t1', 'nain-des-collines', 1, 'Augmentation de caractéristique', 'Votre valeur de Sagesse augmente de 1.'),
    ('nain-des-collines-t2', 'nain-des-collines', 2, 'Ténacité naine', 'Votre nombre maximum de points de vie augmente de 1. Il augmente de nouveau de 1 chaque fois que vous gagnez un niveau.');

-- Sous-race : Nain des montagnes
INSERT INTO races (id, nom, alias, race_parent_id, bonus_forces, source)
VALUES ('nain-des-montagnes', 'Nain des montagnes', 'nain-des-montagnes', 'nain', 2, 'Player''s Handbook');

INSERT INTO race_presentations (id, race_id, ordre, titre, texte) VALUES
    ('nain-des-montagnes-p1', 'nain-des-montagnes', 1, 'Nain des montagnes',
    'En tant que nain des montagnes, vous êtes fort et robuste, habitué à mener une vie difficile dans des zones accidentées. Vous êtes probablement de grande taille (pour un nain) et avez une carnation assez pâle.');

INSERT INTO race_traits (id, race_id, ordre, titre, texte) VALUES
    ('nain-des-montagnes-t1', 'nain-des-montagnes', 1, 'Augmentation de caractéristique', 'Votre valeur de Force augmente de 2.'),
    ('nain-des-montagnes-t2', 'nain-des-montagnes', 2, 'Formation au port des armures naines', 'Vous maîtrisez les armures légères et intermédiaires.');

-- =============================================================================
-- GNOME
-- =============================================================================
INSERT INTO races (id, nom, alias, description, aide_joueur,
    bonus_intelligence, age_adulte, age_max,
    taille_min, taille_max, taille_base, taille_facteur, dee_de_mod,
    poids_base, poids_facteur, vitesse_base, liste_langues, source)
VALUES ('gnome', 'Gnome', 'gnome',
    'Un bourdonnement trahissant une activité débordante accompagne en permanence les écheveaux d''habitations souterraines et les quartiers où les gnomes s''installent en communautés très soudées. Les gnomes se réjouissent de la vie et apprécient chaque minute passée à inventer quelque chose, à explorer, chercher, créer et jouer.',
    'Idéal pour les joueurs qui veulent un personnage intelligent et résistant à la magie. Le gnome des roches convient bien à l''artisan ou à l''alchimiste, le gnome des forêts à l''illusionniste discret.',
    2, 40, 500, 90, 120, 85, 1, '2D8', 18, 1, 25, 'Commun, Gnome', 'Player''s Handbook');

INSERT INTO race_presentations (id, race_id, ordre, titre, texte) VALUES
    ('gnome-p1', 'gnome', 1, 'Gnome',
    'Un bourdonnement trahissant une activité débordante accompagne en permanence les écheveaux d''habitations souterraines et les quartiers où les gnomes s''installent en communautés très soudées. D''autres sons couvrent ponctuellement ce bourdonnement perpétuel : le crissement d''une broyeuse, une petite explosion, un cri de surprise ou de triomphe et surtout, surtout, des éclats de rire. Les gnomes se réjouissent de la vie et apprécient chaque minute passée à inventer quelque chose, à explorer, chercher, créer et jouer.'),
    ('gnome-p2', 'gnome', 2, 'Des gens pleins de vie',
    'L''énergie et le plaisir de vivre des gnomes se reflètent sur chaque centimètre carré de leur petit corps. Les gnomes mesurent à peine un mètre et pèsent dans les 20 à 25 kilos. Leur visage bronzé ou olivâtre s''orne généralement d''un large sourire et leurs yeux vifs brillent d''excitation. Leurs cheveux blonds ont tendance à partir en tous sens, comme pour symboliser l''insatiable intérêt qu''ils portent à tout ce qui les entoure.'),
    ('gnome-p3', 'gnome', 3, 'Un dévouement enthousiaste',
    'Pour les gnomes, le simple fait de vivre est une chose merveilleuse et ils veillent à tirer un maximum de joie de leurs trois à cinq siècles d''existence. Les gnomes aiment les plaisanteries en tout genre, en particulier les jeux de mots et les farces, mais ils sont tout à fait capables de se concentrer sur une tâche sérieuse. Nombre de gnomes sont des ingénieurs, des alchimistes, des bricoleurs et des inventeurs de talent.'),
    ('gnome-p4', 'gnome', 4, 'Des terriers lumineux',
    'Les gnomes aiment s''installer dans les collines boisées. Ils vivent sous terre, mais sortent plus souvent que les nains et profitent du monde naturel qui s''étend à la surface. Leurs demeures sont dissimulées de par leur conception habile, mais aussi à l''aide d''illusions basiques.'),
    ('gnome-p5', 'gnome', 5, 'À la découverte du monde',
    'Curieux et impulsifs, les gnomes partent parfois à l''aventure pour découvrir le monde ou par amour de l''exploration. Quelles que soient leurs motivations, les gnomes qui optent pour une vie d''aventurier y prennent autant de plaisir que dans toute autre activité, parfois au grand dam de leurs camarades d''aventure.');

INSERT INTO race_noms (id, race_id, ordre, titre, texte) VALUES
    ('gnome-n1', 'gnome', 1, 'Noms masculins', 'Alston, Alvyn, Boddynock, Brocc, Burgell, Dimble, Eldon, Erky, Fonkin, Frug, Gerbo, Gimble, Glim, Jebeddo, Kellen, Namfoodle, Orryn, Roondar, Seebo, Sindri, Warryn, Wrenn, Zook'),
    ('gnome-n2', 'gnome', 2, 'Noms féminins', 'Bimpnottine, Breena, Caramip, Carline, Donella, Duvamil, Ella, Ellyjobell, Ellywick, Lilli, Loopmottine, Lorilla, Mardnab, Nissa, Nyx, Oda, Orla, Roywyn, Shamil, Tana, Waywocket, Zanna'),
    ('gnome-n3', 'gnome', 3, 'Noms de clan', 'Beren, Daergel, Folkor, Garrick, Nackle, Murnig, Ningel, Raulnor, Scheppen, Timbers, Turen'),
    ('gnome-n4', 'gnome', 4, 'Surnoms', 'Brillegemme, Canardboiteux, Cendrefoyer, Clapebière, Doubleboucle, Fnipper, Furet, Ku, Manteau, Nim, Piquepâte, Pock, Qu''unechaussure');

INSERT INTO race_traits (id, race_id, ordre, titre, texte) VALUES
    ('gnome-t1', 'gnome', 1, 'Augmentation de caractéristiques', 'Votre valeur d''Intelligence augmente de 2.'),
    ('gnome-t2', 'gnome', 2, 'Âge', 'Les gnomes grandissent au même rythme que les humains. On estime généralement qu''ils deviennent adultes vers 40 ans. Ils peuvent vivre de 350 à presque 500 ans.'),
    ('gnome-t3', 'gnome', 3, 'Taille', 'Les gnomes mesurent entre 90 et 120 centimètres et pèsent en moyenne une vingtaine de kilos. Ils sont de taille Petite.'),
    ('gnome-t4', 'gnome', 4, 'Vitesse', 'Votre vitesse de base au sol est de 7,50 mètres.'),
    ('gnome-t5', 'gnome', 5, 'Vision dans le noir', 'Dans un rayon de 18 mètres, vous pouvez voir dans une zone de lumière faible comme s''il s''agissait d''une lumière vive et dans l''obscurité comme s''il s''agissait d''une lumière faible.'),
    ('gnome-t6', 'gnome', 6, 'Ruse gnome', 'Vous êtes avantagé pour tous les jets de sauvegarde d''Intelligence, de Sagesse et de Charisme contre la magie.'),
    ('gnome-t7', 'gnome', 7, 'Langues', 'Vous pouvez parler, lire et écrire le commun et le gnome. La langue gnome utilise l''alphabet nain.');

-- Sous-race : Gnome des forêts
INSERT INTO races (id, nom, alias, race_parent_id, bonus_dexterite, source)
VALUES ('gnome-des-forets', 'Gnome des forêts', 'gnome-des-forets', 'gnome', 1, 'Player''s Handbook');

INSERT INTO race_presentations (id, race_id, ordre, titre, texte) VALUES
    ('gnome-des-forets-p1', 'gnome-des-forets', 1, 'Gnome des forêts',
    'Les gnomes des forêts ont un don naturel pour les illusions et disposent d''une rapidité et d''une discrétion naturelles. Ils se rassemblent en communautés secrètes au cœur des forêts et recourent aux illusions et aux supercheries pour se cacher. Ils se lient d''amitié avec de petits animaux des bois qui leur transmettent des informations sur les éventuels dangers rôdant sur leurs terres.');

INSERT INTO race_traits (id, race_id, ordre, titre, texte) VALUES
    ('gnome-des-forets-t1', 'gnome-des-forets', 1, 'Augmentation de caractéristiques', 'Votre valeur de Dextérité augmente de 1.'),
    ('gnome-des-forets-t2', 'gnome-des-forets', 2, 'Illusionniste-né', 'Vous connaissez le sort mineur *illusion mineure* et utilisez l''Intelligence comme caractéristique d''incantation pour le lancer.'),
    ('gnome-des-forets-t3', 'gnome-des-forets', 3, 'Communication avec les petits animaux', 'À l''aide de bruits et de gestes, vous arrivez à transmettre des concepts simples à des bêtes de taille Petite ou inférieure.');

-- Sous-race : Gnome des roches
INSERT INTO races (id, nom, alias, race_parent_id, bonus_constitution, source)
VALUES ('gnome-des-roches', 'Gnome des roches', 'gnome-des-roches', 'gnome', 1, 'Player''s Handbook');

INSERT INTO race_presentations (id, race_id, ordre, titre, texte) VALUES
    ('gnome-des-roches-p1', 'gnome-des-roches', 1, 'Gnome des roches',
    'Les gnomes des roches sont naturellement plus inventifs et résistants que les autres gnomes. Dans les mondes de D&D, la majorité des gnomes appartiennent à cette catégorie, y compris les gnomes bricoleurs de *Dragonlance*.');

INSERT INTO race_traits (id, race_id, ordre, titre, texte) VALUES
    ('gnome-des-roches-t1', 'gnome-des-roches', 1, 'Augmentation de caractéristiques', 'Votre valeur de Constitution augmente de 1.'),
    ('gnome-des-roches-t2', 'gnome-des-roches', 2, 'Connaissances en ingénierie', 'Quand vous faites un test d''Intelligence (Histoire) relatif aux objets magiques, alchimiques ou technologiques, vous ajoutez le double de votre bonus de maîtrise au résultat du test.'),
    ('gnome-des-roches-t3', 'gnome-des-roches', 3, 'Bricoleur', 'Vous maîtrisez les outils d''artisans (outils de bricoleur). Grâce à ces outils, vous pouvez passer 1 heure et dépenser 10 po de matériaux pour construire un mécanisme de taille Très Petite (CA 5, 1 pv). Vous pouvez avoir jusqu''à trois mécanismes fonctionnant en même temps.');

-- =============================================================================
-- DEMI-ELFE
-- =============================================================================
INSERT INTO races (id, nom, alias, description, aide_joueur,
    bonus_charisme, age_adulte, age_max,
    taille_min, taille_max, taille_base, taille_facteur, dee_de_mod,
    poids_base, poids_facteur, vitesse_base, liste_langues, source)
VALUES ('demi-elfe', 'Demi-elfe', 'demi-elfe',
    'Issus de deux mondes sans vraiment appartenir à aucun, les demi-elfes combinent ce que certains disent être les meilleures qualités de leurs parents humains et elfes : la curiosité, l''inventivité et l''ambition des humains, tempérées par le sens du raffinement, l''amour de la nature et les goûts artistiques des elfes.',
    'Idéal pour les joueurs qui veulent un personnage sociable et polyvalent. Le +2 en Charisme et +1 dans deux caractéristiques libres offre une grande flexibilité. Excellent pour le barde, le paladin ou le sorcier.',
    2, 20, 180, 150, 180, 145, 1, '2D8', 55, 2, 30, 'Commun, Elfique + une langue au choix', 'Player''s Handbook');

INSERT INTO race_presentations (id, race_id, ordre, titre, texte) VALUES
    ('demi-elfe-p1', 'demi-elfe', 1, 'Demi-elfe',
    'Issus de deux mondes sans vraiment appartenir à aucun, les demi-elfes combinent ce que certains disent être les meilleures qualités de leurs parents humains et elfes : la curiosité, l''inventivité et l''ambition des humains, tempérées par le sens du raffinement, l''amour de la nature et les goûts artistiques des elfes. De nombreux demi-elfes se révèlent incapables de s''intégrer pleinement dans la société d''un de leurs parents et optent pour une vie de vagabondages solitaires ou rejoignent d''autres marginaux et exilés pour mener une vie d''aventures.'),
    ('demi-elfe-p2', 'demi-elfe', 2, 'Issus de deux mondes',
    'Aux yeux des humains, les demi-elfes ressemblent à des elfes et aux yeux des elfes, ils ressemblent à des humains. Ils ont la même stature que leurs parents, mais ne sont ni aussi minces que les elfes, ni aussi large de carrure que les humains. Ils mesurent un peu moins de 1,50 mètre à 1,80 mètre et pèsent 50 à 90 kilos. Ils ont tendance à avoir les yeux de leur ascendant elfique.'),
    ('demi-elfe-p3', 'demi-elfe', 3, 'Diplomates ou vagabonds',
    'Les demi-elfes n''ont pas de terres, même s''ils sont les bienvenus dans les villes humaines et tolérés dans les forêts elfiques. Certains demi-elfes préfèrent éviter toute compagnie et sillonnent les étendues sauvages comme trappeurs ou aventuriers. D''autres, au contraire, se jettent à corps perdu dans le flot de la société et mettent leur charisme et leurs compétences sociales à profit pour devenir d''excellents diplomates ou de parfaits escrocs.'),
    ('demi-elfe-p4', 'demi-elfe', 4, 'D''excellents ambassadeurs',
    'Beaucoup de demi-elfes apprennent dès leur plus jeune âge à s''entendre avec tout le monde, à détourner l''hostilité d''autrui et à trouver un terrain d''entente. En tant que race, ils possèdent la grâce des elfes sans leur attitude hautaine et l''énergie des humains sans leur côté rustre et grossier.');

INSERT INTO race_noms (id, race_id, ordre, titre, texte) VALUES
    ('demi-elfe-n1', 'demi-elfe', 1, 'Noms',
    'Les demi-elfes utilisent les conventions baptismales des elfes ou des humains. Les demi-elfes nés parmi les humains reçoivent souvent un nom elfique tandis que ceux qui naissent chez les elfes héritent d''un nom humain.');

INSERT INTO race_traits (id, race_id, ordre, titre, texte) VALUES
    ('demi-elfe-t1', 'demi-elfe', 1, 'Augmentation de caractéristiques', 'Votre valeur de Charisme augmente de 2. La valeur de deux caractéristiques de votre choix augmente de 1.'),
    ('demi-elfe-t2', 'demi-elfe', 2, 'Âge', 'Les demi-elfes grandissent au même rythme que les humains et deviennent matures vers 20 ans. Par contre, ils vivent bien plus longtemps que les humains et il n''est pas rare qu''ils dépassent 180 ans.'),
    ('demi-elfe-t3', 'demi-elfe', 3, 'Taille', 'Les demi-elfes sont de taille similaire aux humains. Ils font entre 150 et 180 centimètres et sont de taille Moyenne.'),
    ('demi-elfe-t4', 'demi-elfe', 4, 'Vitesse', 'Votre vitesse de base au sol est de 9 mètres.'),
    ('demi-elfe-t5', 'demi-elfe', 5, 'Vision dans le noir', 'Dans un rayon de 18 mètres, vous pouvez voir dans une zone de lumière faible comme s''il s''agissait d''une lumière vive et dans l''obscurité comme s''il s''agissait d''une lumière faible.'),
    ('demi-elfe-t6', 'demi-elfe', 6, 'Ascendance féerique', 'Vous êtes avantagé sur les jets de sauvegarde contre l''effet charmé et vous ne pouvez être contraint à dormir par la magie.'),
    ('demi-elfe-t7', 'demi-elfe', 7, 'Polyvalence', 'Vous gagnez la maîtrise de deux compétences de votre choix.'),
    ('demi-elfe-t8', 'demi-elfe', 8, 'Langues', 'Vous pouvez parler, lire et écrire le commun, l''elfique et une autre langue de votre choix.');

-- =============================================================================
-- DEMI-ORC
-- =============================================================================
INSERT INTO races (id, nom, alias, description, aide_joueur,
    bonus_forces, bonus_constitution, age_adulte, age_max,
    taille_min, taille_max, taille_base, taille_facteur, dee_de_mod,
    poids_base, poids_facteur, vitesse_base, liste_langues, source)
VALUES ('demi-orc', 'Demi-orc', 'demi-orc',
    'Qu''elles soient unies sous les ordres d''un puissant occultiste ou qu''elles se soient battues pendant des années jusqu''à arriver à un statu quo, il arrive que des communautés orques et humaines forment une alliance. Et quand ces alliances sont scellées par un mariage, il naît des demi-orcs. Certains s''élèvent au rang de puissant chef. D''autres sillonnent le monde afin de prouver leur valeur.',
    'Idéal pour les joueurs qui veulent un personnage offensif et résistant. Le +2 en Force et +1 en Constitution en font l''un des meilleurs combattants de corps à corps. Les capacités Acharnement et Sauvagerie sont particulièrement utiles au barbare ou au guerrier.',
    2, 1, 14, 75, 150, 215, 145, 2, '2D10', 90, 4, 30, 'Commun, Orc', 'Player''s Handbook');

INSERT INTO race_presentations (id, race_id, ordre, titre, texte) VALUES
    ('demi-orc-p1', 'demi-orc', 1, 'Demi-orc',
    'Qu''elles soient unies sous les ordres d''un puissant occultiste ou qu''elles se soient battues pendant des années jusqu''à arriver à un statu quo, il arrive que des communautés orques et humaines forment une alliance. Et quand ces alliances sont scellées par un mariage, il naît des demi-orcs. Certains s''élèvent au rang de puissant chef. D''autres sillonnent le monde afin de prouver leur valeur. Nombre d''entre ceux-là optent pour une carrière d''aventurier. Ils atteignent la grandeur par leurs tours de force.'),
    ('demi-orc-p2', 'demi-orc', 2, 'Puissant et balafré',
    'Les demi-orcs mêlent des traits humains et orcs, leur aspect s''avérant très variable. Ils partagent le plus souvent une peau grisâtre et des dents proéminentes. Ils mesurent entre 1,50 mètre et 2,15 mètres et pèsent généralement entre 90 et 125 kilos.

Les orcs tirent orgueil des cicatrices reçues au combat et considèrent les scarifications comme des ornements. Tout demi-orc qui a vécu un temps auprès des orcs porte des cicatrices ; qu''elles soient des marques d''orgueil ou d''humiliation, elles racontent ses exploits et ses blessures.'),
    ('demi-orc-p3', 'demi-orc', 3, 'La marque de Gruumsh',
    'C''est Gruumsh, le dieu borgne de la guerre et de la fureur, qui créa jadis les premiers orcs, sachant que même ceux qui s''en sont détournés jouissent de la puissance et de l''endurance qu''il leur a léguées. Certains entendent les murmures de Gruumsh dans leurs rêves, des murmures qui les incitent à libérer la rage qui bouillonne en eux.

En dehors de la rage de Gruumsh, les demi-orcs ressentent des émotions très puissantes. La rage ne se contente pas d''accélérer leur pouls, elle embrase tout leur corps. Une insulte brûle comme de l''acide et la tristesse sape leurs forces. Mais leur rire résonne clair et fort et les plaisirs simples comme les festins, la lutte, les beuveries, la musique des tambours et les folles danses emplissent leur cœur de joie.');

INSERT INTO race_noms (id, race_id, ordre, titre, texte) VALUES
    ('demi-orc-n1', 'demi-orc', 1, 'Noms masculins', 'Dench, Feng, Gell, Henk, Holg, Imsh, Keth, Krusk, Mhurren, Ront, Shump, Thokk'),
    ('demi-orc-n2', 'demi-orc', 2, 'Noms féminins', 'Baggi, Émen, Engong, Kansif, Myev, Neega, Ovak, Ownka, Shautha, Sutha, Vola, Volen, Yevelda');

INSERT INTO race_traits (id, race_id, ordre, titre, texte) VALUES
    ('demi-orc-t1', 'demi-orc', 1, 'Augmentation de caractéristiques', 'Votre valeur de Force augmente de 2 et votre valeur de Constitution de 1.'),
    ('demi-orc-t2', 'demi-orc', 2, 'Âge', 'Les demi-orcs deviennent matures un peu plus vite que les humains. Ils sont considérés comme adultes vers 14 ans. Par contre, ils vieillissent plus vite et vivent rarement plus de 75 ans.'),
    ('demi-orc-t3', 'demi-orc', 3, 'Taille', 'Les demi-orcs sont plus larges et plus corpulents que les humains. Ils sont de taille Moyenne.'),
    ('demi-orc-t4', 'demi-orc', 4, 'Vitesse', 'Votre vitesse de base au sol est de 9 mètres.'),
    ('demi-orc-t5', 'demi-orc', 5, 'Vision dans le noir', 'Dans un rayon de 18 mètres, vous pouvez voir dans une zone de lumière faible comme s''il s''agissait d''une lumière vive et dans l''obscurité comme s''il s''agissait d''une lumière faible.'),
    ('demi-orc-t6', 'demi-orc', 6, 'Menaçant', 'Vous gagnez la maîtrise de la compétence Intimidation.'),
    ('demi-orc-t7', 'demi-orc', 7, 'Acharnement', 'Quand vous vous retrouvez à 0 point de vie mais que vous n''êtes pas tué sur le coup, vous pouvez choisir d''être réduit à 1 point de vie à la place. Vous ne pouvez pas réutiliser cette aptitude tant que vous n''avez pas terminé un repos long.'),
    ('demi-orc-t8', 'demi-orc', 8, 'Sauvagerie', 'Quand vous réussissez un coup critique avec une attaque de corps à corps, vous pouvez relancer l''un des dés de dégâts de l''arme et ajouter son résultat aux dégâts supplémentaires du coup critique.'),
    ('demi-orc-t9', 'demi-orc', 9, 'Langues', 'Vous pouvez parler, lire et écrire le commun et l''orc. La langue orc a une sonorité dure et certains sons évoquent des grincements. Les orcs n''ont pas d''alphabet propre et utilisent donc l''alphabet nain.');

-- =============================================================================
-- DRAKÉIDE
-- =============================================================================
INSERT INTO races (id, nom, alias, description, aide_joueur,
    bonus_forces, bonus_charisme, age_adulte, age_max,
    taille_min, taille_max, taille_base, taille_facteur, dee_de_mod,
    poids_base, poids_facteur, vitesse_base, liste_langues, source)
VALUES ('drakeide', 'Drakéide', 'drakeide',
    'Nés d''un dragon, comme leur nom le proclame, les drakéides parcourent fièrement un monde qui les accueille avec une incompréhension mêlée de crainte. Façonnés par les dieux draconiques ou par les dragons eux-mêmes, les drakéides sont éclos d''œufs de dragon afin de former une race unique, combinant le meilleur des dragons et des humanoïdes.',
    'Idéal pour les joueurs qui veulent un personnage imposant avec une arme de souffle unique. Le +2 en Force et +1 en Charisme convient bien au paladin ou au guerrier. Le choix d''ascendance draconique personnalise le type de dégâts du souffle.',
    2, 1, 15, 80, 180, 210, 175, 1, '2D8', 100, 4, 30, 'Commun, Draconique', 'Player''s Handbook');

INSERT INTO race_presentations (id, race_id, ordre, titre, texte) VALUES
    ('drakeide-p1', 'drakeide', 1, 'Drakéide',
    'Nés d''un dragon, comme leur nom le proclame, les drakéides parcourent fièrement un monde qui les accueille avec une incompréhension mêlée de crainte. Façonnés par les dieux draconiques ou par les dragons eux-mêmes, les drakéides sont éclos d''œufs de dragon afin de former une race unique, combinant le meilleur des dragons et des humanoïdes. Certains sont de fidèles serviteurs des véritables dragons, d''autres forment la piétaille de grandes armées et d''autres encore errent sans vocation.'),
    ('drakeide-p2', 'drakeide', 2, 'De fiers parents des dragons',
    'Les drakéides ressemblent fort à des dragons dressés sur leurs pattes arrière dans une attitude humanoïde, bien qu''ils soient dépourvus d''ailes et de queue. Leurs écailles, petites et fines, sont généralement couleur de bronze ou d''airain, avec des teintes allant de l''écarlate à l''or en passant par le rouille et le vert cuivré. Ils sont grands et solidement bâtis, mesurant souvent dans les 2 mètres pour 150 kilos ou plus.'),
    ('drakeide-p3', 'drakeide', 3, 'Des clans autonomes',
    'Pour un drakéide le clan revêt plus d''importance que la vie elle-même. Chaque individu voue sa vie à son clan et lui accorde plus de respect et de dévotion qu''à toute autre chose, plus qu''aux dieux eux-mêmes. La conduite de chaque drakéide reflète l''honneur de son clan et un individu qui déshonore son clan risque l''exil.

Toute la race est animée d''une volonté continue de s''améliorer. Un drakéide apprécie par-dessus tout les talents et l''excellence. Il déteste échouer et fournit des efforts acharnés avant de s''avouer vaincu. L''objectif de toute sa vie consiste à atteindre la maîtrise parfaite d''un talent particulier.');

INSERT INTO race_noms (id, race_id, ordre, titre, texte) VALUES
    ('drakeide-n1', 'drakeide', 1, 'Noms masculins', 'Arjhan, Balasar, Bharash, Donaar, Ghesh, Heskan, Kriv, Medrash, Méhen, Nadarr, Pandjed, Patrin, Rhogar, Shamash, Shedinn, Tarhun, Torinn'),
    ('drakeide-n2', 'drakeide', 2, 'Noms féminins', 'Akra, Biri, Daar, Farideh, Harann, Havilar, Jheri, Kava, Korinn, Mishann, Nala, Perra, Raiann, Sora, Surina, Thava, Uadjit'),
    ('drakeide-n3', 'drakeide', 3, 'Noms d''enfant', 'Grimpeur, Mordeur-de-Bouclier, Perce-Oreille, Dévot, Sauteur, Zélé'),
    ('drakeide-n4', 'drakeide', 4, 'Noms de clan', 'Clethtinthiallor, Daardendrian, Delmirev, Drachedandion, Fenkenkabradon, Kepeshkmolik, Kerrhylon, Kimbatuul, Linxakasendalor, Myastan, Nemmonis, Norixius, Ophinshtalajiir, Prexijandilin, Shestendeliath, Turnuroth, Verthisathurgiesh, Yarjerit');

INSERT INTO race_traits (id, race_id, ordre, titre, texte) VALUES
    ('drakeide-t1', 'drakeide', 1, 'Augmentation de caractéristiques', 'Votre valeur de Force augmente de 2 et votre valeur de Charisme de 1.'),
    ('drakeide-t2', 'drakeide', 2, 'Âge', 'Les drakéides grandissent vite. Ils atteignent une taille et un niveau de développement équivalent à un humain de 10 ans en seulement 3 ans. On les considère adultes vers 15 ans et ils vivent environ 80 ans.'),
    ('drakeide-t3', 'drakeide', 3, 'Taille', 'Les drakéides sont plus grands et lourds que les humains. Ils dépassent facilement 1,80 mètre et pèsent en moyenne 125 kilos. Ils sont de taille Moyenne.'),
    ('drakeide-t4', 'drakeide', 4, 'Vitesse', 'Votre vitesse de base au sol est de 9 mètres.'),
    ('drakeide-t5', 'drakeide', 5, 'Ascendance draconique', 'Vous avez des ancêtres draconiques. Choisissez un type de dragon parmi : Airain (Feu), Argent (Froid), Blanc (Froid), Bleu (Foudre), Bronze (Foudre), Cuivre (Acide), Noir (Acide), Or (Feu), Rouge (Feu), Vert (Poison). Votre choix détermine votre souffle et votre résistance aux dégâts.'),
    ('drakeide-t6', 'drakeide', 6, 'Souffle', 'Vous pouvez utiliser votre action pour exhaler une vague d''énergie destructrice déterminée par votre ascendance draconique. Le DD du jet de sauvegarde est égal à 8 + votre modificateur de Constitution + votre bonus de maîtrise. Dégâts : 2d6 (3d6 au niv. 6, 4d6 au niv. 11, 5d6 au niv. 16). Utilisable une fois par repos court ou long.'),
    ('drakeide-t7', 'drakeide', 7, 'Résistance aux dégâts', 'Vous possédez une résistance au type de dégâts associé à votre ascendance draconique.'),
    ('drakeide-t8', 'drakeide', 8, 'Langues', 'Vous pouvez parler, lire et écrire le commun et le draconique. Le draconique est considéré comme une des langues les plus anciennes et est souvent utilisé pour étudier la magie.');

-- =============================================================================
-- TIEFFELIN
-- =============================================================================
INSERT INTO races (id, nom, alias, description, aide_joueur,
    bonus_charisme, bonus_intelligence, age_adulte, age_max,
    taille_min, taille_max, taille_base, taille_facteur, dee_de_mod,
    poids_base, poids_facteur, vitesse_base, liste_langues, source)
VALUES ('tieffelin', 'Tieffelin', 'tieffelin',
    'Être accueilli par des regards appuyés et des murmures, subir des insultes et des démonstrations de violence dans la rue, lire la méfiance et la peur dans les yeux de chacun, voilà le lot des tieffelins. Et pour ne rien arranger, ils savent tous qu''ils souffrent de ce comportement à cause d''un pacte conclu il y a des générations de cela, grâce auquel Asmodée a imprégné leur lignée de son essence.',
    'Idéal pour les joueurs qui veulent un personnage charismatique avec des sorts innés. Le +2 en Charisme et +1 en Intelligence en font d''excellents sorciers ou bardes. Leur vision dans le noir et leur résistance au feu sont de vrais atouts.',
    2, 1, 18, 120, 150, 190, 145, 1, '2D10', 55, 4, 30, 'Commun, Infernal', 'Player''s Handbook');

INSERT INTO race_presentations (id, race_id, ordre, titre, texte) VALUES
    ('tieffelin-p1', 'tieffelin', 1, 'Tieffelin',
    'Être accueilli par des regards appuyés et des murmures, subir des insultes et des démonstrations de violence dans la rue, lire la méfiance et la peur dans les yeux de chacun, voilà le lot des tieffelins. Et pour ne rien arranger, ils savent tous qu''ils souffrent de ce comportement à cause d''un pacte conclu il y a des générations de cela, grâce auquel Asmodée, le seigneur suprême des Neuf Enfers, a imprégné leur lignée de son essence. Ils ne sont pas coupables de leur apparence ni de leur nature, pourtant elles découlent d''un péché antique pour lequel eux-mêmes, leurs enfants et les enfants de leurs enfants seront à jamais tenus responsables.'),
    ('tieffelin-p2', 'tieffelin', 2, 'Une lignée infernale',
    'Les tieffelins sont issus de lignées humaines et ont encore l''air humain, au sens large du terme, cependant, leur héritage infernal a laissé une marque visible sur leur physique. Ils sont dotés de larges cornes qui revêtent toutes sortes de formes : torsadées comme celles d''un bélier, hautes et droites comme celles d''une gazelle ou spiralées comme celles d''une antilope. Ils ont une queue épaisse d''un mètre vingt à un mètre cinquante de long. Leurs canines sont extrêmement pointues et leurs yeux sont unis, noirs, rouges, blancs, argentés ou dorés, sans pupilles ni sclère visibles.'),
    ('tieffelin-p3', 'tieffelin', 3, 'Autonomes et suspicieux',
    'Les tieffelins survivent en petites minorités, principalement dans les villes humaines où ils grandissent dans les quartiers les plus mal famés et deviennent escrocs, voleurs ou seigneurs du crime.

Comme les tieffelins n''ont pas de foyer, ils savent qu''ils doivent se forger leur propre place dans le monde et qu''il leur faudra être forts pour survivre. Ils ne font pas facilement confiance à ceux qui prétendent être leurs amis, mais quand leurs camarades prouvent qu''ils leur font confiance, les tieffelins apprennent à leur rendre la pareille.'),
    ('tieffelin-p4', 'tieffelin', 4, 'Méfiance suscitée',
    'Les gens mal avisés se méfient souvent des tieffelins, convaincus que leur héritage infernal déteint sur leur personnalité et leur moralité en plus de leur apparence. En réalité, cette lignée n''a pas d''influence sur le caractère du tieffelin. Les royaumes infernaux leur lèguent bien des pouvoirs magiques, mais chaque tieffelin trace sa propre route.');

INSERT INTO race_noms (id, race_id, ordre, titre, texte) VALUES
    ('tieffelin-n1', 'tieffelin', 1, 'Noms infernaux masculins', 'Akménos, Amnon, Barakas, Damakos, Êkemon, Iados, Kairon, Leucis, Melech, Mordaï, Morthos, Pelaïos, Skamos, Théraï'),
    ('tieffelin-n2', 'tieffelin', 2, 'Noms infernaux féminins', 'Akta, Anakis, Bryséis, Criella, Damaïa, Éa, Kallista, Lérissa, Makarie, Néméïa, Orianna, Phélaïa, Rieta'),
    ('tieffelin-n3', 'tieffelin', 3, 'Noms de « vertu »', 'Art, Chagrin, Chant, Charogne, Credo, Désespoir, Espoir, Excellence, Gloire, Hasard, Idéal, Lassitude, Musique, Nulle-Part, Ouverture, Poésie, Quête, Révérence, Témérité, Terreur, Tourment');

INSERT INTO race_traits (id, race_id, ordre, titre, texte) VALUES
    ('tieffelin-t1', 'tieffelin', 1, 'Augmentation de caractéristiques', 'Votre valeur de Charisme augmente de 2 et votre valeur d''Intelligence de 1.'),
    ('tieffelin-t2', 'tieffelin', 2, 'Âge', 'Les tieffelins vieillissent à une vitesse comparable aux humains, mais ils vivent quelques années supplémentaires.'),
    ('tieffelin-t3', 'tieffelin', 3, 'Taille', 'Les tieffelins sont de taille et de corpulence comparables aux humains. Ils sont de taille Moyenne.'),
    ('tieffelin-t4', 'tieffelin', 4, 'Vitesse', 'Votre vitesse de base au sol est de 9 mètres.'),
    ('tieffelin-t5', 'tieffelin', 5, 'Vision dans le noir', 'Dans un rayon de 18 mètres, vous pouvez voir dans une zone de lumière faible comme s''il s''agissait d''une lumière vive et dans l''obscurité comme s''il s''agissait d''une lumière faible.'),
    ('tieffelin-t6', 'tieffelin', 6, 'Résistance infernale', 'Vous êtes résistant aux dégâts de feu.'),
    ('tieffelin-t7', 'tieffelin', 7, 'Ascendance infernale', 'Vous connaissez le sort mineur *thaumaturgie*. Au niveau 3, vous pouvez lancer *représailles infernales* comme sort de niveau 2 une fois par repos long. Au niveau 5, vous pouvez lancer *ténèbres* une fois par repos long. Votre caractéristique d''incantation est le Charisme.'),
    ('tieffelin-t8', 'tieffelin', 8, 'Langues', 'Vous pouvez parler, lire et écrire le commun et l''infernal.');

-- =============================================================================
-- HALFELIN
-- =============================================================================
INSERT INTO races (id, nom, alias, description, aide_joueur,
    bonus_dexterite, age_adulte, age_max,
    taille_min, taille_max, taille_base, taille_facteur, dee_de_mod,
    poids_base, poids_facteur, vitesse_base, liste_langues, source)
VALUES ('halfelin', 'Halfelin', 'halfelin',
    'La plupart des halfelins n''ont qu''un objectif dans la vie : profiter du confort de leur foyer. Ils aiment vivre dans un endroit paisible, loin des monstres en maraude et du fracas des armées, en appréciant un feu ronflant dans la cheminée, un repas copieux, un bon verre et une conversation agréable. Certains halfelins passent leur vie dans une communauté agricole isolée, mais d''autres mènent une vie de nomades et voyagent constamment en groupe, attirés par la route et les grands horizons, impatients de découvrir de nouvelles terres et de nouvelles personnes.',
    'Idéal pour les joueurs qui souhaitent un personnage agile et chanceux, discret et difficile à intimider. Le bonus de +2 en Dextérité en fait un excellent roublard ou rôdeur.',
    2, 20, 150, 82, 96, 80, 1, '2D8', 18, 1, 25, 'Commun, Halfelin', 'Player''s Handbook');

INSERT INTO race_presentations (id, race_id, ordre, titre, texte) VALUES
    ('halfelin-p1', 'halfelin', 1, 'Halfelin',
    'La plupart des halfelins n''ont qu''un objectif dans la vie : profiter du confort de leur foyer. Ils aiment vivre dans un endroit paisible, loin des monstres en maraude et du fracas des armées, en appréciant un feu ronflant dans la cheminée, un repas copieux, un bon verre et une conversation agréable. Certains halfelins passent leur vie dans une communauté agricole isolée, mais d''autres mènent une vie de nomades et voyagent constamment en groupe, attirés par la route et les grands horizons, impatients de découvrir de nouvelles terres et de nouvelles personnes. Mais même ces nomades aiment la paix, la bonne chère, le confort et un chez-soi douillet, même s''il s''agit d''une roulotte qui cahote sur une route poussiéreuse ou d''un bateau flottant sur la rivière.'),
    ('halfelin-p2', 'halfelin', 2, 'Petits et pragmatiques',
    'Les halfelins à la taille si réduite parviennent à survivre dans un monde plein de dangers en évitant de se faire remarquer ou, à défaut, en évitant d''importuner autrui. Comme ils mesurent un peu moins d''un mètre, ils semblent relativement inoffensifs, cela fait donc des siècles qu''ils survivent dans l''ombre des empires, en marge des guerres et des querelles politiques. Ils ont tendance à l''embonpoint et pèsent entre 20 et 25 kilos.

La peau du halfelin va de l''olivâtre à l''albâtre avec un teint rougeaud tandis que ses cheveux sont plutôt bruns ou châtains et frisés. Les hommes portent souvent de longues rouflaquettes, mais rarement une barbe et encore moins une moustache.'),
    ('halfelin-p3', 'halfelin', 3, 'Aimables et curieux',
    'Les halfelins sont gens affables et enthousiastes. Ils accordent une grande importance à leur famille et leurs amis, ainsi qu''au confort de leur foyer, rêvant rarement d''or et de gloire. Même les aventuriers de cette race parcourent le monde par amitié, soif de voyages, curiosité ou dans l''intérêt de leur communauté. Ils aiment découvrir des nouveautés, même des choses simples comme un plat exotique ou un style de vêtement inconnu.

Les halfelins sont prompts à éprouver de la pitié et détestent voir souffrir un être vivant. Ils sont généreux et partagent volontiers ce qu''ils ont, même quand les temps sont durs.'),
    ('halfelin-p4', 'halfelin', 4, 'Se fondre dans la masse',
    'Les halfelins sont très doués pour s''intégrer au sein des communautés humaines, naines ou elfiques. Ils s''y rendent utiles et deviennent les bienvenus. Grâce à leur discrétion innée et leur nature modeste, ils évitent facilement d''attirer une attention malvenue.

Les halfelins travaillent volontiers avec autrui et se montrent loyaux envers leurs amis, qu''ils soient halfelins ou non. Ils sont capables de faire montre d''une férocité inattendue quand leurs amis, leur famille ou leur communauté sont en danger.'),
    ('halfelin-p5', 'halfelin', 5, 'Courtoisies pastorales',
    'La plupart des halfelins vivent dans de petites communautés paisibles composées de grandes fermes et de vergers bien entretenus. Il est bien rare qu''ils bâtissent leur propre royaume ou possèdent beaucoup de terres en dehors de leur douce campagne. Ils ne reconnaissent pas de noblesse ni de royauté halfeline et cherchent plutôt conseil auprès de leurs aînés.'),
    ('halfelin-p6', 'halfelin', 6, 'Affables et positifs',
    'Les halfelins s''efforcent de s''entendre avec tout le monde et détestent les généralités, en particulier les généralités négatives.

**Les elfes.** « Ils sont si beaux ! Leur visage, leur musique, leur grâce, tout ça... C''est comme s''ils sortaient tout droit d''un merveilleux rêve. »

**Les humains.** « Les humains nous ressemblent vraiment beaucoup, certains en tout cas. Si vous oubliez les châteaux et les forteresses et que vous allez parler aux fermiers et aux bergers, vous allez voir que ce sont de bonnes gens et qu''ils sont très sensés. »

**Les nains.** « Les nains sont des amis loyaux et vous pouvez leur faire confiance quand il s''agit de tenir parole. Mais ça ne leur ferait pas de mal de sourire un peu de temps en temps... »'),
    ('halfelin-p7', 'halfelin', 7, 'Saisir chaque opportunité',
    'Si un halfelin embrasse la vie d''aventurier, c''est généralement pour défendre sa communauté, aider ses amis ou explorer un monde immense et rempli de merveilles. Pour les halfelins, l''aventure n''est pas un métier, c''est une opportunité ou, parfois, une nécessité.'),
    ('halfelin-p8', 'halfelin', 8, 'Les noms des halfelins',
    'Un halfelin a un prénom, un nom de famille et parfois un surnom. Le nom de famille est souvent un surnom si tenace qu''il s''est transmis d''une génération à l''autre.');

INSERT INTO race_noms (id, race_id, ordre, titre, texte) VALUES
    ('halfelin-n1', 'halfelin', 1, 'Les noms masculins', 'Alton, Andre, Cade, Corrin, Eldon, Errich, Finnan, Garret, Lindal, Lyle, Merric, Milo, Osborn, Perrin, Reed, Roscoe, Wellby'),
    ('halfelin-n2', 'halfelin', 2, 'Les noms féminins', 'Andry, Bree, Callie, Cora, Euphémie, Jillian, Kithri, Lavinia, Lidda, Merla, Nedda, Paële, Portia, Seraphine, Shaena, Trym, Vani, Verna'),
    ('halfelin-n3', 'halfelin', 3, 'Noms de famille', 'Bontonneau, Buissons, Butepavé, Épine, Feuilledethé, Grandpré, Hautecolline, Récoltepinceaux, Roulecolline, Vertbouteille');

INSERT INTO race_traits (id, race_id, ordre, titre, texte) VALUES
    ('halfelin-t1', 'halfelin', 1, 'Augmentation de caractéristique', 'Votre valeur de Dextérité augmente de 2.'),
    ('halfelin-t2', 'halfelin', 2, 'Âge', 'Un halfelin atteint la maturité vers 20 ans et vit généralement 150 ans.'),
    ('halfelin-t3', 'halfelin', 3, 'Taille', 'D''une taille moyenne de 90 cm, les halfelins pèsent environ 20 kilos. Ils sont de taille Petite.'),
    ('halfelin-t4', 'halfelin', 4, 'Vitesse', 'Votre vitesse de base au sol est de 7,50 mètres.'),
    ('halfelin-t5', 'halfelin', 5, 'Chanceux', 'Quand vous faites un 1 avec le d20 d''un jet d''attaque, d''un test de caractéristique ou d''un jet de sauvegarde, vous pouvez relancer le dé. Mais vous devez utiliser le nouveau résultat du jet.'),
    ('halfelin-t6', 'halfelin', 6, 'Brave', 'Vous êtes avantagé sur les jets de sauvegarde contre la terreur.'),
    ('halfelin-t7', 'halfelin', 7, 'Agilité halfeline', 'Vous pouvez traverser n''importe quel espace occupé par une créature d''au moins une catégorie de taille de plus que vous.'),
    ('halfelin-t8', 'halfelin', 8, 'Langues', 'Vous pouvez parler, lire et écrire le commun et le halfelin. Bien que la langue des halfelins n''ait rien d''un secret, ils n''aiment pas l''apprendre aux autres. Presque tous les halfelins parlent le commun, ce qui leur permet de converser avec les gens qui habitent sur les mêmes territoires qu''eux.');

-- Sous-race : Pied-léger
INSERT INTO races (id, nom, alias, race_parent_id, bonus_charisme, source)
VALUES ('pied-leger', 'Pied-léger', 'pied-leger', 'halfelin', 1, 'Player''s Handbook');

INSERT INTO race_presentations (id, race_id, ordre, titre, texte) VALUES
    ('pied-leger-p1', 'pied-leger', 1, 'Pied-léger',
    'En tant que halfelin pied-léger, vous cacher est presque une seconde nature et vous n''hésitez pas à vous dissimuler derrière d''autres créatures pour échapper aux regards. Vous êtes d''un naturel plutôt affable et sympathique. Dans les *Forgotten Realms*, ce sont les pieds-légers qui se sont le plus répandus de par le monde et ils constituent donc la variante raciale la plus courante.');

INSERT INTO race_traits (id, race_id, ordre, titre, texte) VALUES
    ('pied-leger-t1', 'pied-leger', 1, 'Augmentation de caractéristique', 'Votre valeur de Charisme augmente de 1.'),
    ('pied-leger-t2', 'pied-leger', 2, 'Discrétion naturelle', 'Vous pouvez tenter de vous cacher même quand vous êtes seulement dissimulé par une créature d''au moins une catégorie de taille de plus que vous.');

-- Sous-race : Robuste
INSERT INTO races (id, nom, alias, race_parent_id, bonus_constitution, source)
VALUES ('robuste', 'Robuste', 'robuste', 'halfelin', 1, 'Player''s Handbook');

INSERT INTO race_presentations (id, race_id, ordre, titre, texte) VALUES
    ('robuste-p1', 'robuste', 1, 'Robuste',
    'En tant qu''halfelin robuste, vous êtes plus costaud que la moyenne et vous possédez une certaine résistance au poison. Certains disent que les robustes ont du sang nain dans les veines. Dans les *Forgotten Realms*, on appelle ces halfelins des cœurs vaillants et ils sont plus répandus au sud.');

INSERT INTO race_traits (id, race_id, ordre, titre, texte) VALUES
    ('robuste-t1', 'robuste', 1, 'Augmentation de caractéristique', 'Votre valeur de Constitution augmente de 1.'),
    ('robuste-t2', 'robuste', 2, 'Résilience des robustes', 'Vous êtes avantagé lors des jets de sauvegarde contre le poison et vous êtes résistant aux dégâts de poison.');
