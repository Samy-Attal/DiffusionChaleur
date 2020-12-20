#include <stdio.h>
#include <stdlib.h>

#define dx 0.001
#define dt 0.000001

typedef struct {
	char nom[10];
	float K, C, rho, alpha;
}materiau;

typedef struct {
    int pos, dimx, dimy;
    float temp;
}source;

typedef struct {
    int x, y, t_micro;
	float tempInit;
	materiau m;
	source src;

}syst;


float** creerMat(int nb_lignes, int nb_colonnes) {
	    int i; float** mat;
    if((mat = (float**)malloc(nb_lignes * sizeof(float*))) == NULL) {
		fprintf(stderr, "erreur d'allocation\n");
		return 0;
    }
    for(i=0;i<nb_lignes;i++) {
        if((mat[i] = (float*)malloc(nb_colonnes * sizeof(float))) == NULL){
			fprintf(stderr, "erreur d'allocation\n");
            return 0;
        } 
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

int main(){	


    return 0;
}