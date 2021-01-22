function diffusion_1D
%Appel fichier
resultats = load('./resultat1D/resultats.txt'); 

%Affichage resultat
size(resultats);
t=0:1e-3:1e-3*(size(resultats,2)-1);
x=1:size(resultats,1);

figure(1)
imagesc(t,x,resultats(:,2:end))
colorbar
title('Diffusion 1D');
xlabel('time samples (s)');
ylabel('Nb segments (mm)');
end 