/* TEST DE LA FONCTION CALCUL CHALEUR */


#include <stdio.h>
#include <stdlib.h>

#define dx 0.001 //= 1mm 
#define dt 0.000001 //= 1µs

#define src_temp 350
#define sys_temp 280
#define nb_X 10
#define tempsmicro 100000
#define temp_simu tempsmicro*dt // en s

typedef struct {
	char* nom;
	double K, C, rho, alpha;
}materiau;

typedef struct {
	int typeSrc; // si 1 dim = 1 / si 2 dim = 2
	int posSrc; // position de la source (si ponctuelle)
	double valTemp;	//valeur de temperature de la source pr chaque x

}source;

typedef struct {
	double resX;
	int nbEchantillons;
	//int taille;
	double initTemp;
	materiau objet;
	source src;
}syst;


double** creerMat(int nb_lignes, int nb_colonnes) {
	double** mat = malloc(nb_lignes * sizeof(double*));
	int i = 0;
	while (i < nb_lignes) {
		mat[i] = calloc(nb_colonnes, sizeof(double)); 
		i++;
	}
	return mat;
}


void writeCalc(char* name, double** calcul, int echantillons, double tps){
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

void printTab(double* tab, int taille) {
    int i;
    for(i=0;i<taille;i++){
        printf("%lf ", tab[i]);
    }
}

void printMat(double** mat, int lignes, int colonnes){
    int i, j;
    for(i=0;i<lignes;i++){
        for(j=0;j<colonnes;j++){
            printf("%.1lf ", mat[i][j]);
        }
        printf("\n");
    }
}

void print3Val(double** mat, int c1, int c2, int c3){
	int i;
	for(i=0;i<nb_X;i++){
		printf("%.3lf %.3lf %.3lf\n", mat[i][c1], mat[i][c2], mat[i][c3]);
	}
}

int main(){
    materiau cuivre;
    cuivre.K = 390; cuivre.C = 385; cuivre.rho = 8930;
    cuivre.alpha = cuivre.K / (cuivre.C * cuivre.rho);

    source mono;
    mono.typeSrc = 0;
	mono.posSrc = 4;
	mono.valTemp = src_temp;

    syst fil; 
    fil.objet = cuivre;
    fil.nbEchantillons = nb_X;
    fil.initTemp = sys_temp;
    fil.resX = dx;
    fil.src = mono;

    double** calcul = calculChaleur(fil, fil.nbEchantillons, temp_simu);
    //printMat(calcul, fil.nbEchantillons, (temp_simu/dt)+1); //attention ne pas decommenter si temps de simulation 
	//print3Val(calcul, 1, tempsmicro/2, tempsmicro); // affiche les valeurs de temperature au debut, milieu et fin de simulation
	writeCalc("resultats.txt", calcul, fil.nbEchantillons, temp_simu);
	exit(0);
	return 0;
}



