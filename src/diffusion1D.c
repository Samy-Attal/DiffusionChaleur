#include <stdio.h>
#include <stdlib.h>
#include "../include/diffusion1D.h"

float readFloat(){
    float var;
    if(scanf("%f",&var) != 1){
        printf("erreur lors de la saisie\n");
		exit(1);
	}
    return var;
}

int readInt(){
    int var;
    if(scanf("%d",&var) != 1){
        printf("erreur lors de la saisie\n");
		exit(1);
    }
    return var;
}

float** creerMat(int nb_lignes, int nb_colonnes) {
	int i; float** mat;
    mat = malloc(nb_lignes * sizeof(float*));
    for(i=0;i<nb_lignes;i++) {
        mat[i] = malloc(nb_colonnes * sizeof(float));
    }
	if(mat == NULL){
		printf("erreur d'allocation\n");
		exit(4);
	}
    return mat;
}

int cptLignes(char *name){
    int nb = 1; char c;
    FILE* file = fopen(name, "r");
    if(file == NULL){
        printf("erreur lors de l'ouverture du fichier\n");
		exit(2);
    }
    while((c = fgetc(file)) != EOF){ 
        if(c == '\n') {
            nb++;
        }
    }
    fclose(file);
    return nb;
}

materiau* initMatiere(char* name){
	int i, nblignes = cptLignes(name);
	materiau *mater = malloc(nblignes*sizeof(materiau));
	FILE* file = fopen(name,"r");
	if(file == NULL){
		printf("erreur lors de l'ouverture du fichier\n");
		exit(2);
	}
    
    for(i=0;i<nblignes;i++) {
		if(fscanf(file,"%s %f %f %f", mater[i].nom, &mater[i].K, &mater[i].C, &mater[i].rho) != 4){
            printf("attention fichier non conforme : ligne en trop dans la liste de materiaux, nom du materiau ou parametre manquant\n");
			exit(3);
        }
        mater[i].alpha = mater[i].K / (mater[i].C * mater[i].rho);
    }
	fclose(file);
	return mater;
}

source1 initSource1(int echantillons){
	source1 src;
	printf("entrer la temperature de la source : ");
	src.valTemp = readFloat();
	printf("entrer la position de la source sur %d echantillons : ", echantillons);
	src.posSrc = readInt();
	return src;
}

syst1 initSys1(int echantillons, float resol_x){
	syst1 s; int i, choix = 0;
	s.resX = resol_x;
	s.nbEchantillons = echantillons;
	materiau* mater = initMatiere(file_materiaux); 
	printf("Configuration du systeme monodimensionnel\n");
	do {
		if(choix == 0) 
			printf("choisir le materiau :\n");
		else
 			printf("materiau non reconnu, choisissez entre :\n");
		
		for(i=0;i<cptLignes(file_materiaux);i++)
			printf("%d : %s\n", i, mater[i].nom);

		choix = readInt();
	}while(choix > i-1 || choix < 0);
	s.objet = mater[choix];
	
	printf("entrer la temperature initiale du systeme (en K) : ");
	s.initTemp = readFloat(); 

	printf("configuation de la source :\n");
	do{
		s.src = initSource1(echantillons);
		if(s.src.posSrc < 0 || s.src.posSrc > echantillons){
			printf("position de la source non valide\n");
		}
	}while(s.src.posSrc < 0 || s.src.posSrc > echantillons);
	return s;
}

void writeCalc(char* name, float** calcul, int echantillons, float tps){
	unsigned long t_micro = tps/dt;
	printf("ecriture du fichier %s\n", name);
	FILE* file = fopen(name, "w");
	int i, j;
	for(i=0;i<echantillons;i++){
		for(j=0;j<t_micro+1;j= j+1000){ // augmentation de l'incrementation pour diminuer le nombre de reultats
			fprintf(file, "%.0f ",calcul[i][j]);
		}
		fprintf(file, "\n");
	}
	fclose(file);
}

float** calculChaleur(syst1 s, int echantillons, float tps) {
	unsigned long t_micro= tps/dt;
    printf("calcul en cours\n");
    float** res = creerMat(echantillons, t_micro + 1);
	float* source = calloc(echantillons,sizeof(echantillons));
	float alpha = s.objet.alpha;	
	float dT;
	int t, x;
	source[s.src.posSrc] = s.src.valTemp;

	// initialisation de la premiere colonne X
	for (x = 0; x < echantillons; x++)
		res[x][0] = x * s.resX *1000; //x1000 pour facilite la lecture en mm 

	// pour t = 0
	for (x = 0; x < echantillons; x++) {
		if (x == s.src.posSrc) {
			res[x][1] = s.src.valTemp;
		}
		else {
			res[x][1] = s.initTemp;
		}
	}
	// effets de bords T(a,t) = Ta et T(b,t) = Tb pour tout t>0, conditions limites du fil
	for (t = 1; t < t_micro + 1 ; t++) {
		res[0][t] = s.initTemp;
		res[echantillons- 1][t] = s.initTemp;
	}
	for (t = 0; t < t_micro - 1; t++) { 
		for (x = 1; x < echantillons - 1; x++) {
			res[x][t+2] = source[x];
			res[x][t+2] += (res[x][t+1]/dt); 
			dT = (res[x-1][t+1] - (2 * res[x][t+1]) + res[x+1][t+1]) / (dx * dx);
			res[x][t+2] +=  alpha * dT + s.src.valTemp;
			res[x][t+2] *= dt;
  		}
	}
	printf("calcul termine\n");
	return res;
}

void writeCarac(char* name, syst1 s){
    FILE* file = fopen(name, "w");
    fprintf(file, "%s\n%d\n%1.f\n%1.f\n%d", s.objet.nom, s.nbEchantillons, s.initTemp, s.src.valTemp,  s.src.posSrc);
    fclose(file);
}