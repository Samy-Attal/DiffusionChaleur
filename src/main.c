#include <stdio.h>
#include <stdlib.h>
#include "../include/diffusion1D.h"
#include "../include/diffusion2D.h"

int main(){
    int choix, taille, t_simu;
    syst1 fil;

    printf("Simulation de la diffusion de chaleur\nVeuillez choisir entre :\n1. monodimensionnel\n2. bidimensionnel\n");
    choix = readInt();
    
    if(choix == 1){
        printf("choisir la taille du fil en mm : ");
        taille = readInt();
        fil = initSys1(taille, dx);
        printf("choisir le temps de simulation (en s) : ");
        t_simu = readFloat();
        writeCalc(file_resultat, calculChaleur(fil,taille,t_simu), taille, t_simu);
        writeCarac(file_carac,fil);
    }
    else if(choix == 2){
        writeFiles(initSys2D(file_syst, file_source));
    }

    printf("fin de la simulation\n");
    return 0;
}