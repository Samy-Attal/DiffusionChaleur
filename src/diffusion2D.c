#include <stdio.h>
#include <stdlib.h>
#include "../include/diffusion1D.h"
#include "../include/diffusion2D.h"

float*** creerEspace(int x, int y, long t){
    int i;
    float*** esp;
    esp = malloc(t * sizeof(float**));
    for(i=0;i<t;i++){
        esp[i] = creerMat(x,y);
    }
    if(esp == NULL){
        printf("erreur d'allocation\n");
        exit(4);
    }
    return esp;
}

syst initSys2D(char* namesyst, char* namesrc){
    syst s; source src; materiau* m;
    float t_seconde;
    int choix = 0, nb, i;
    FILE* file = fopen(namesrc, "r");
    if(file == NULL){
        printf("erreur lors de l'ouverture du fichier\n");
        exit(2);
    }
    if(fscanf(file, "%d %d %d %d %f", &src.posx, &src.dimx, &src.posy, &src.dimy, &src.temp) != 5){
        printf("erreur : fichier %s non conforme\n", namesrc);
        exit(3);
    }
    fclose(file);
    file = fopen(namesyst, "r");
    if(file == NULL){
        printf("erreur lors de l'ouverture du fichier\n");
        exit(2);
    }
    if(fscanf(file, "%d %d %f %f", &s.x, &s.y, &t_seconde, &s.temp0) != 4){
        printf("erreur : fichier %s non conforme\n", namesyst);
        exit(3);
    }
    s.t_micro = t_seconde/dt;
    printf("choix du materiau :\n");
    m = initMatiere(file_materiaux);
    nb = cptLignes(file_materiaux);
    choix = 0;
    do {
        if(choix != 0){
            printf("materiau non reconnu, choisissez :\n");
        } else {
            printf("choisissez entre :\n");
        }
        for(i=0;i<nb;i++){
            printf("%d %s\n", i, m[i].nom);
        }
        choix = readInt();
    }while(choix < 0 || choix >= nb);
    s.obj = m[choix];   
    s.src = src;

    return s;
}

float*** calculChaleur2D(syst s) {
    float*** res = creerEspace(s.x, s.y, s.t_micro);
    int x, y, t;
    float alpha = s.obj.alpha;
    float dTx, dTy;

    printf("calcul en cours\n");
    // initialisation a t = 0
    for(x=0;x<s.x;x++){
        for(y=0;y<s.y;y++){
            // position de la source
            if(x >= s.src.posx && x < s.src.posx + s.src.dimx && y >= s.src.posy && y < s.src.posy + s.src.dimy)
                res[0][x][y] = s.src.temp;
            else 
                res[0][x][y] = s.temp0;
        }
    }
    for(t=1;t<s.t_micro;t++){
        for(x=0;x<s.x;x++){
            for(y=0;y<s.y;y++){
                if((x == 0 || x == s.x-1) || (y == 0 || y == s.y-1)){ // effets de bord - condition aux limites
                    res[t][x][y] = s.temp0;
                } else {
                    res[t][x][y] = res[t-1][x][y]; 
                    dTx = (res[t-1][x+1][y] + res[t-1][x-1][y] - 2*res[t-1][x][y]) / (dx * dx);
                    dTy = (res[t-1][x][y+1] + res[t-1][x][y-1] - 2*res[t-1][x][y]) / (dy * dy); 
                    res[t][x][y] += dt*(alpha * (dTx + dTy)); 
                }
            }
        }
    }
    printf("fin du calcul\n");
    return res;
}

void writeFiles(syst s){
    float*** calcul = calculChaleur2D(s);
    int i=0,j,k,f=0;
    FILE* file;
    int nbfiles = s.t_micro / 100000; //afin d'avoir 10 "images" pour 1 seconde
    char name[26] = "./resultats2D/image00.txt";
    while(f < nbfiles) {
        name[19] = (f / 10) + 48 ;  
        name[20] = (f % 10) + 48;
        file = fopen(name, "w"); 
        for(j=0;j<s.x;j++){
            for(k=0;k<s.y;k++)
                fprintf(file, "%.0f ", calcul[i][j][k]);
            fprintf(file, "\n");
        }
        i = i+nbfiles; 
    fclose(file);
    f++;
    }
}
