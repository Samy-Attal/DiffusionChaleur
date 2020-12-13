#include <stdio.h>
#include <stdlib.h>

#define dx 0.001 //= 1mm 
#define dt 0.000001 //= 1µs

#define src_temp 350
#define sys_temp 280
#define nb_X 10
#define tempsmicro 100000 // ne sera pas dans le code final
#define temp_simu tempsmicro*dt // en s

typedef struct {
	char* nom;
	double K, C, rho, alpha;
}materiau;

typedef struct {
	int posSrc; // position de la source (si ponctuelle)
	double valTemp;	//valeur de temperature de la source

}source;

typedef struct {
	double resX;
	int nbEchantillons;
	double initTemp;
	materiau objet;
	source src;
}syst;

/*fonction de bases*/
double readDouble(){
    double var;
    if(scanf("%lf",&var) != 1){
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

char readChar(){
    char var;
    if(scanf("%c",&var) != 1){
        printf("erreur lors de la saisie\n");
		exit(1);
	}
    return var;
}

/*fonction de gestion matricielle*/
double** creerMat(int nb_lignes, int nb_colonnes) {
	double** mat = malloc(nb_lignes * sizeof(double*));
	int i = 0;
	while (i < nb_lignes) {
		mat[i] = malloc(nb_colonnes * sizeof(double)); // a remplacer peut etre par calloc 
		i++;
	}
	return mat;
}

void suppMat(double*** mat, int nb_lignes){
    int i;
    for(i=0;i<nb_lignes;i++){
        free(*mat[i]);
	}
    free(*mat);
	**mat = NULL; //a revoir
}

/*fonctions de gestion de fichiers*/
void writeFile(char* name, double** calcul, int echantillons, double tps){
	unsigned long t_micro = tps/dt;
	printf("echantillons : %d t micro : %ld\n", echantillons, t_micro);
	FILE* file = fopen(name, "w");
	int i, j;
	for(i=0;i<echantillons;i++){
		for(j=0;j<t_micro+1;j++){
			fprintf(file, "%lf ",calcul[i][j]);
		}
		fprintf(file, "\n");
	}
	fclose(file);
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

/*fonctions de gestions du systeme*/
materiau* initMatiere(char* name){
	int i, nblignes = cptLignes(name);
	materiau *mater = malloc(nblignes*sizeof(materiau));
	FILE* file = fopen(name,"r");
	if(file == NULL){
		printf("erreur lors de l'ouverture du fichier\n");
		exit(2);
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

/* fonctions de calculs */
double** calculChaleur(syst s, int echantillons, double tps) {
	unsigned long t_micro= tps/dt;
    //printf("temps simu = %ld micro s\n", t_micro);
    double** res = creerMat(echantillons, t_micro + 1);
	double alpha = s.objet.alpha;	
	double dT;
	int t, x;

	// initialisation de la premiere colonne X
	for (x = 0; x < echantillons; x++)
		res[x][0] = x * s.resX *1000; //x1000 pour facilite la lecture en mm 

	// initialisation de la temperature du systeme + source
	for (x = 0; x < echantillons; x++) {
		if (x == s.src.posSrc) {
			res[x][1] = s.src.valTemp;
		}
		else {
			res[x][1] = s.initTemp;
		}
	}
	// effets de bords T(a,t) = Ta et T(b,t) = Tb pour tout t>0, conditions limites de la plaque
	for (t = 1; t < t_micro + 1 ; t++) {
		res[0][t] = s.initTemp;
		res[echantillons- 1][t] = s.initTemp;
	}
	for (t = 0; t < t_micro - 1; t++) { 
		for (x = 1; x < echantillons - 1; x++) {
			if(x == s.src.posSrc) {
				res[x][t+2] = s.src.valTemp;
			} else {
				res[x][t+2] = (res[x][t+1]/dt); 
				dT = (res[x-1][t+1] - (2 * res[x][t+1]) + res[x+1][t+1]) / (dx * dx);
				res[x][t+2] +=  alpha * dT + s.src.valTemp;
				res[x][t+2] *= dt;
			}
  		}
	}
	return res;
}

int main(){


	return 0;
}