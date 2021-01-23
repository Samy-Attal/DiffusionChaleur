Projet diffusion de la chaleur par Lounes ALLIOUI Katherine VALENCIA Samy ATTAL Zakaria BOUHAFS

Ce projet entre dans le cadre de l'UE Methodes numeriques en C,
ou nous concevons un logiciel permettant de simuler la diffusion de la chaleur dans un materiau.


Explication des fichiers :

* Fichiers d'entres / initialisation 

- materiaux.txt : est le fichier qui contient tous les parametres des differents materiaux. 
Vous pouvez egalement manuellement ajouter un(des) materiau(x) à(aux) ligne(s) suivante(s) sous cette forme :

nom_du_materiau K C rho


- syst.txt : est un fichier qui modelise les parametres du systeme et simulation en bidimensionnel, sous cette forme 

longueur(x) largeur(y) temps_de_simulation temperature_initiale


- source.txt : est un fichier qui modelise la source de chaleur, et etre utilise uniquement dans la simulation bidimensionnelle, 
il contient des informations sous cette forme :

position_en_x dim_x position_en_y dim_y temperature_de_la_source

Vous pouvez bien evidemment modifier la configuration de la source de chaleur en modifiant ces parametres.


* Fichiers de sortie

Monodimensionnel (dossier resultat1D)
- caracteristiques.txt : contient les propritetes de la simulation 1D sous cette forme :

    nom_du_materiau_choisi
    taille_en_mm_du_fil 
    temperature_initiale
    temperature_de_la_source_de_chaleur
    position_de_la_source_de_chaleur

- resultats.txt : contient les resultats de la simulation 1D, 
la premiere colonne contenant les segments du fil/barre et ensuite les valeurs des temperatures qui evoluent au fil du temps 
(chaque colonne etant un pas dans le temps).
Ce fichier pourra etre ensuite traite par un programme matlab afin de visualiser la diffusion de chaleur en monodimensionnel.

Bidimensionnel (dossier resultats2D)
- image00 a imageXX : resultats de la simulation 2D, chaque fichier texte "image" contient les valeurs de temperature en chaque point.
Chaque "image" est generée par pas de temps d'un dixieme de seconde.
Ces "images" 


* Fichiers sources (dossier src) et headers (dossier include)

- diffusion1D.c / diffusion1D.h : fichier c/h contenant les fonctions utilisés pour la partie monodimensionnel

- diffusion2D.c / diffusion2D.h : fichier c/h contentant les fonctions utlilisés pour la partie bidimensionnel

- main.c : programme c principal permettant de choisir entre les deux types de simulation.


* Programme matlab

- diffusion_1D.m : fonction destinant a afficher la simulation de propagation de chaleur en 1D. 
Une seule figure est generee avec le temps de simulation en abscisse.

- diffusion_2D.m : fonction destinant a afficher la simulation de propagation de chaleur en 2D.
Une video est generee et sauvgarger sous le nom de diffusion2D.avi

- main.m : programme principal permettant de choisir de traiter et afficher la simulation effectuee prealablement avec le programme c.