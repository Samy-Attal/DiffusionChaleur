#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char nom[10];
	double K, C, rho, alpha;
}materiau;


//compte le nombres de lignes d'un fichier texte
int cptLignes(char *name){
    int nb = 1; char c;
    FILE* file = fopen(name, "r");
    if(file == NULL){
        printf("erreur lors de l'ouverture du fichier\n");
		exit(1);
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
		exit(1);
	}
    
    for(i=0;i<nblignes;i++) {
		if(fscanf(file,"%s %lf %lf %lf", mater[i].nom, &mater[i].K, &mater[i].C, &mater[i].rho) != 4){
            printf("attention fichier non conforme : ligne en trop, nom du materiau ou parametre manquant\n");
            return mater;
        }
        mater[i].alpha = mater[i].K / (mater[i].C * mater[i].rho);
    }
	fclose(file);
	return mater;
}

int main(){
    int i ; 
    materiau* m = initMatiere("materiaux.txt");
    for(i=0;i<cptLignes("materiaux.txt");i++){
        printf("%s\n", m[i].nom);
    }
    return 0;
}