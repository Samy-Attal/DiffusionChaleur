/*PROJET DIFFUSION DE LA CHALEUR 01/12/2020
			VERSION DU 08/12/2020 
update 09/12 : correction des erreurs de syntaxe  
update 10/12 : ajout de la fonction writeFile
update 11/12 : ajout des fonctions de gestion de systeme 
			   modification de la fonction de calcul (adapté au système)

		-> TO DO : retirer les dependances par define dans les fonctions 
				   -> utiliser des variables  en parametres d'entrées
************************************************************************ */
			/*PROBLEME MONO DIMENSIONNEL*/

#include <stdio.h>
#include <stdlib.h>

/* definition des constantes*/
#define dx 0.001 //= 1mm 
#define dt 1 //= 1µs


// a remplacer peut etre par des variables
#define nb_X 10
#define TposSrc 333 // = 60°C
#define Tsyst 293 // = 20°C
#define temp_simu 10*dt // 10 µs

/***************************/
/*definition des structures*/

typedef struct {
	char* nom;
	double K, C, rho, alpha;
}materiau;

typedef struct {
	int typeSrc; // si 1 dim = 1 / si 2 dim = 2
	double* valTemp;	//valeur de temperature de la source pr chaque x
}source;

typedef struct {
	double resX;
	int nb_echantillons;
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
int writeFile(char* name, syst s){  
    int i,j;
    double** mat = calculChaleur(s, s.nb_echantillons);
    FILE* file = fopen(name, "w");
    if(file == NULL){
        printf("erreur lors de l'ouverture du fichier\n");
        return 0;
    }
    for (i = 0; i < nb_X ; i++) {
        for(j=0;j<temp_simu+1;j++){
            fprintf(file, "%lf", mat[i][j]);
        }
        fprintf(file, "\n");
    }
	//suppmat
    fclose(file);
    return 1;
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
	source src; int i, pos; double tempSrc;
	src.valTemp = malloc(echantillons*sizeof(double));
	printf("indiquer le type de source : ");
	src.typeSrc = readInt();
	printf("entrer la temperature de la source : ");
	tempSrc = readDouble();
	if(!src.typeSrc){
		printf("entrer la position de la source sur %d echantillons : ", echantillons);
		pos = readInt();
		for(i=0;i<echantillons;i++){
			if(i != pos){
				src.valTemp[i] = 0;
			} else {
				src.valTemp[i] = tempSrc;
			}
		}
	} else {
		printf("entrer manuellement les valeurs de la sources pour %d echantillons :\n", echantillons);
		for(i=0;i<echantillons;i++){
			src.valTemp[i] = readDouble();
		}
	}
	return src;
}

syst initSys(int echantillons, double resol_x){
	syst s; char* mater_name;
	s.resX = resol_x;
	s.nb_echantillons = echantillons;
	//s.taille = ?? taille utile ??
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
double** calculChaleur(syst s, int echantillons) {
	double** res = creerMat(echantillons, temp_simu + 1);
	double alpha = s.objet.alpha;	

	int t, x;

	// initialisation de la premiere colonne X
	for (x = 0; x < echantillons; x++)
		res[x][0] = x * s.resX;

	// initialisation de la temperature du systeme + source
	for (x = 0; x < echantillons; x++) {
		if (s.src.valTemp[x]) {
			res[x][1] = s.src.valTemp[x];
		}
		else {
			res[x][1] = s.initTemp;
		}
	}
	// effets de bords T(a,t) = Ta et T(b,t) = Tb pour tout t>0, conditions limites de la plaque
	for (t = 1; t < temp_simu - 1; t++) {
		res[0][t] = s.initTemp;
		res[echantillons- 1][t] = s.initTemp;
	}

	for (t = 0; t < temp_simu - 1; t++) { // t < temp_simu -2 ?
		for (x = 1; x < echantillons - 1; x++) {
			res[x][t + 2] = res[x][t + 1] + dt * alpha * ((res[x - 1][t + 1] - (2 * res[x][t + 1]) + res[x + 1][t + 1]) / dx * dx) + s.src.valTemp[x];
		}
	}
	return res;
}

int main() {
	syst fil = initSys(nb_X, dx);
	writeFile("fildecuivre.txt", fil);

	return 0;	
}