#include <stdio.h>
#include <stdlib.h>

#define dx 0.001
#define dt 0.000001

#define file_materiaux "materiaux.txt"
#define file_sources "sources.txt"
#define file_syst "syst.txt"

typedef struct {
	char nom[10];
	float K, C, rho, alpha;
}materiau;

typedef struct {
    int posx, posy, dimx, dimy;
    float temp;
}source;

typedef struct {
    int x, y, t_micro;
	float temp0;
	materiau obj;
	source* src;

}syst;


float** creerMat(int nb_lignes, int nb_colonnes) {
	    int i; float** mat;
    if(mat = (float**)malloc(nb_lignes * sizeof(float*)) == NULL) {
		fprintf(stderr, "erreur d'allocation\n");
		return 0;
    }
    for(i=0;i<nb_lignes;i++) {
        if(mat[i] = (float*)malloc(nb_colonnes * sizeof(float)) == NULL){
			fprintf(stderr, "erreur d'allocation\n");
            return 0;
        } 
    }
    return mat;
}

float*** creerEspace(int x, int y, int t){
    int i;
    float*** esp;
    if(esp = (float***)malloc(t * sizeof(float**)) == NULL){
        fprintf(stderr, "erreur d'allocation\n");
        return 0;
    }
    for(i=0;i<t;i++){
        esp[i] = creerMat(x,y);
    }
    return esp;
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
	materiau* mater = malloc(nblignes*sizeof(materiau));
	FILE* file = fopen(name,"r");
	if(file == NULL){
		printf("erreur lors de l'ouverture du fichier\n");
		exit(2);
	}
    for(i=0;i<nblignes;i++) {
		if(fscanf(file,"%s %f %f %f", mater[i].nom, &mater[i].K, &mater[i].C, &mater[i].rho) != 4){
            printf("erreur : fichier %s non conforme\n", name);
			exit(3);
        }
        mater[i].alpha = mater[i].K / (mater[i].C * mater[i].rho);
    }
	fclose(file);
	return mater;
}

source* initSources(char* name){
    int i, nbsrc;
    source* src;
    FILE* file = fopen(name, "r");
    if(file == NULL){
        printf("erreur lors de l'ouverture du fichier\n");
        exit(2);
    }
    nbsrc = cptLignes(name);
    src = malloc(nbsrc*sizeof(source));

    for(i=0;i<nbsrc;i++){
        if(fscanf(file, "%d %d %d %d %f", src[i].posx, src[i].posy, src[i].dimx, src[i].dimy, src->temp) != 5){
            printf("erreur : fichier %s non conforme\n", name);
            exit(3);
        }
    }
    return src;
}

syst initSys(char* name){
    syst s; materiau* m; int nb, choix, i;
    FILE* file = fopen(name, "r");
    if(file == NULL){
        printf("erreur lors de l'ouverture du fichier\n");
        exit(2);
    }
    if(fscanf(file, "%d %d\n%d\n%f", s.x, s.y, s.t_micro, s.temp0) != 4){
        printf("erreur : fichier %s non conforme\n", name);
        exit(3);
    }
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
    s.src = initSources(file_sources);
    return s;
}



int main(){	


    return 0;
}