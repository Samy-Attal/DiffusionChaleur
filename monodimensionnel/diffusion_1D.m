clear all
clc

%Appel fichier
load('resultats.txt'); 

%Affichage resultat
size(resultats)
t=0:1e-6:1e-6*(size(resultats,2)-1);
t(1)
t(end)
size(t)
x=1:size(resultats,1);
figure(1)
imagesc(t,x,resultats(:,2:end))
title('1-D Diffusion chaleur');
xlabel('time samples (s)');
ylabel('Nb segments (mm)');
