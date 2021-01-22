phrase = ['Veuillez choisir les resultats que voulez afficher', newline,'1. monodimensionnel ou 2. bidimensionnel : '];

choix = input(phrase);

if choix == 1
    diffusion_1D();
elseif choix == 2
    diffusion_2D();
else
    disp('demande non reconnue');
end    