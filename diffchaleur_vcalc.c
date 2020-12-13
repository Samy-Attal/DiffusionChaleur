/*PROJET DIFFUSION DE LA CHALEUR 
 *******************************************
		CAS MONO DIMENSIONNEL*/

/* Problemes actuels :
	 ne fonctionne pas pour des simulations >= 1 seconde, erreur de segmentation dans calculChaleur probablement 
*/

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
	//int typeSrc; // si 1 dim = 1 / si 2 dim = 2 non pris en compte dans le cas mono dimensionnel, source = ponctuelle
	int posSrc; // position de la source (si ponctuelle)
	double valTemp;	//valeur de temperature de la source

}source;

typedef struct {
	double resX;
	int nbEchantillons;
	//int taille;
	double initTemp;
	materiau objet;
	source src;
}syst;

/*fonction de bases*/
double readDouble(){
    double var;
    if(scanf("%lf",&var) != 1){
        printf("erreur lors de la saisie\n");
    }
    return var;
}

int readInt(){
    int var;
    if(scanf("%d",&var) != 1){
        printf("erreur lors de la saisie\n");
    }
    return var;
}

char readChar(){
    char var;
    if(scanf("%c",&var) != 1){
        printf("erreur lors de la saisie\n");
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

/*fonctions de gestions du systeme*/

materiau* initMatiere(char* name){
	int i; double mk, mc, mrho; char* mnom; 
	materiau mater[3];
	FILE* file = fopen(name,"r");
	if(file == NULL){
		printf("erreur lors de l'ouverture du fichier\n");
		exit(1);
	}
	for(i=0;i<3;i++){
		fscanf(file,"%s",mnom);
		fscanf(file,"%lf %lf %lf", &mk, &mc, &mrho);
		mater[i].nom = mnom;
		mater[i].K = mk; mater[i].C = mc; mater[i].rho = mrho;  
	}
	fclose(file);
	return mater;
}

source initSource(int echantillons){
	source src;
	printf("entrer la temperature de la source : ");
	src.valTemp = readDouble();
	printf("entrer la position de la source sur %d echantillons : ", echantillons);
	src.posSrc = readInt();
	return src;
}

syst initSys(int echantillons, double resol_x){
	syst s; char* mater_name;
	s.resX = resol_x;
	s.nbEchantillons = echantillons;
	materiau* mater = initMatiere("materiaux.txt"); 
	printf("choisir le materiau : ");
	scanf("%s",mater_name); // a tester
	if(mater_name == "cuivre") {
		s.objet = mater[0];
	}
	else if(mater_name == "aluminium") {
		s.objet = mater[1];
	}
	else if(mater_name == "air") {
		s.objet = mater[2];
	} else {
		printf("materiau non reconnu\n");
		exit(2); // remplacer par do while ?
	}
	s.src = initSource(echantillons);
	
	// initialisation de la temperature initiale du système 
	printf("entrer la valeur initiale du systeme : ");
	s.initTemp = readDouble(); 
	return s;
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


int main() {
	syst fil = initSys(nb_X, dx);
	writeFile("resultats.txt", calculChaleur(fil, fil.nbEchantillons, temp_simu), fil.nbEchantillons, temp_simu);
	return 0;	
}