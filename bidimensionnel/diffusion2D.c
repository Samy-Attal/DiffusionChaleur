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
    int x, y;
    unsigned long t_micro;
	float temp0;
	materiau obj;
	source src;

}syst;

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

float*** creerEspace(int x, int y, unsigned long t){
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

int readInt(){
    int var;
    if(scanf("%d",&var) != 1){
        printf("erreur lors de la saisie\n");
		exit(1);
    }
    return var;
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

source initSources(char* name){
    int i, nbsrc;
    source src;
    FILE* file = fopen(name, "r");
    if(file == NULL){
        printf("erreur lors de l'ouverture du fichier\n");
        exit(2);
    }
    if(fscanf(file, "%d %d %d %d %f", src.posx, src.posy, src.dimx, src.dimy, src.temp) != 5){
        printf("erreur : fichier %s non conforme\n", name);
        exit(3);
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

float*** calculChaleur(syst s) {
    float*** res = creerEspace(s.x, s.y, s.t_micro);
    int x, y, t;

    // initialisation a t = 0
    for(x=0;x<s.x;x++){
        for(y=0;y<s.y;y++){
            // position de la source
            if(x > s.src.posx && x < s.src.posx + s.src.dimx && y > s.src.posy && y < s.src.posy + s.src.dimy)
                res[0][x][y] = s.src.temp;
            else 
                res[0][x][y] = s.temp0;
        }
    }

    for(t=1;t<s.t_micro;t++){
        for(x=0;x<s.x;x++){
            for(y=0;y<s.y;y++){
                if(!x || x == s.x-1 || !y || y == s.y-1){
                    res[t][x][y] = s.temp0;
                } else {
                    //res[t][x][y] = s.obj.alpha * 
                }
            }
        }
    }
    return res;
}

void writeCalc(char* filename, float*** calcul, unsigned long temp, int x, int y){
    int i,j,t;
    FILE* file = fopen(filename, "w");
    if(file == NULL){
        printf("erreur lors de l'ouverture du fichier\n");
        exit(2);
    }
    for(t=0;t<temp;t++){ // augmenter incrementation pr diminuer ecriture inutile
        for(i=0;i<x;i++){
            for(j=0;j<y;j++){
                fprintf(file, "%.0f ", calcul[t][i][j]);
            }
            fprintf(file, "\n");
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

int main(){	


    return 0;
}