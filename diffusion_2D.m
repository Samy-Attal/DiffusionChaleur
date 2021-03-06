function diffusion_2D

rep='resultats2D';

% lecture format data
fileID = fopen(fullfile('.','initialisation','syst.txt'),'rt');
metaData = fscanf(fileID,'%d'); 
dimx = metaData(1); % 1ere dim materiau
dimy = metaData(2); % 2eme dim materiau
Delta_t = metaData(3); % dur�e temporelle simu
Tinit = metaData(4); % temperature initiale

filesrc = fopen(fullfile('.','initialisation','source.txt'),'rt');
metaSrc = fscanf(filesrc,'%d');
Tempsrc = metaSrc(5);


% init film avi
v = VideoWriter('diffusion2D.avi');
v.FrameRate=10;
open(v);


% lecture fichiers de donnees
fichtxt=dir(fullfile('.',rep,'image*.txt'));

%nbfich=size(fichtxt,1);
nbfich = Delta_t * 10;

for ifich=1:nbfich % boucles sur les pas de temps
    
     % recup fichier txt du temp ifich
    nomfich=fichtxt(ifich).name;
    
     % Ouverture du fichier
    fileID = fopen(fullfile('.',rep,nomfich),'rt');        

    % lecture
    A=fscanf(fileID,'%d'); % fichier txt-> 1 vecteur
    Data=reshape(A,dimx,dimy).'; % reformatage des donnees 
    
    figure(1)
    imagesc(Data)
    xlabel('[mm]')
    ylabel('[mm]')
    title('diffusion T�K 2D')
    colorbar
    caxis([Tinit Tempsrc]);
    drawnow
    pause(0.1)
    
    F(ifich) = getframe;
    writeVideo(v,F(ifich));
end
close(v) % fin ecriture .AVI


% sauvegarde dans fichier de donnees matlab
%save(fullfile('.',rep,'datamovie.mat'),'F');


% %%%%%% recup donnees fichier matlab et lecture du film dans matlab
% rep='images';
% load(fullfile('.',rep,'datamovie.mat'),'F');
% movie(F,1,1) %relecture de F 
end
