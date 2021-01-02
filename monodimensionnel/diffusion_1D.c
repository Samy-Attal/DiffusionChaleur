#include <stdio.h>
#include <stdlib.h>

#define dx 0.001 //= 1mm 
#define dt 0.000001 //= 1µs

/*structure definissant un materiau
avec son nom et ses parametres K, C, rho et alpha*/
typedef struct {
	char nom[10];
	float K, C, rho, alpha;
}materiau;

/*structure definissant une source de chaleur ponctuelle
avec sa position (dans un espace 1D) et sa temperature en ce point */
typedef struct {
	int posSrc; 
	float valTemp;	
}source;

/*structure definissant le système etudié
comprenant sa resolution en x, son nombre d'echantillons (sa taille est nbEchantillons x resX, sa temperature initiale, 
son materiau ainsi que sa source */
typedef struct {
	float resX;
	int nbEchantillons;
	float initTemp;
	materiau objet;
	source src;
}syst;


/*fonction permettant de recuperer par l'utilisateur un nombre réel de type float */
float readFloat(){
    float var;
    if(scanf("%f",&var) != 1){
        printf("erreur lors de la saisie\n");
		exit(1);
	}
    return var;
}

/*fonction permettant de recuperer par l'utilisateur un nombre entier */
int readInt(){
    int var;
    if(scanf("%d",&var) != 1){
        printf("erreur lors de la saisie\n");
		exit(1);
    }
    return var;
}

/*fonction de creation de matrice 2D de dimension nb_lignes nb_colonnes*/
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

void freeMat(float*** mat, int dimX) {
    int i;
    for (i = 0; i < dimX; i++) {
        free(*mat[i]);
    }
    free(*mat);
	*mat = NULL;
}

/*fonction de comptage du nombre de lignes dans un fichier */
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

/*fonction de creation et initialisation d'un tableau contenant les donnés de materiaux depuis un fichier name */
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

/*fonction d'initialisation d'une source */
source initSource(int echantillons){
	source src;
	printf("entrer la temperature de la source : ");
	src.valTemp = readFloat();
	printf("entrer la position de la source sur %d echantillons : ", echantillons);
	src.posSrc = readInt();
	return src;
}

/*fonction de creation et d'initialisation d'une structure syst */
syst initSys(int echantillons, float resol_x){
	syst s; int i, choix = 0;
	s.resX = resol_x;
	s.nbEchantillons = echantillons;
	materiau* mater = initMatiere("materiaux.txt"); 
	do {
		if(choix == 0) 
			printf("choisir le materiau :\n");
		else
 			printf("materiau non reconnu, choisissez entre :\n");
		
		for(i=0;i<cptLignes("materiaux.txt");i++)
			printf("%d : %s\n", i, mater[i].nom);

		choix = readInt();
	}while(choix > i-1 || choix < 0);
	s.objet = mater[choix];
	
	printf("entrer la temperature initiale du systeme : ");
	s.initTemp = readFloat(); 
	printf("\nConfiguration de la source\n");
	
	do{
		s.src = initSource(echantillons);
		if(s.src.posSrc < 0 || s.src.posSrc > echantillons){
			printf("position de la source non valide\n");
		}
	}while(s.src.posSrc < 0 || s.src.posSrc > echantillons);
	return s;
}

/*fonction d'ecriture des resultats de calcul de chaleur dans un fichier name*/
void writeCalc(char* name, float** calcul, int echantillons, float tps){
	unsigned long t_micro = tps/dt;
	printf("ecriture du fichier %s\n", name);
	FILE* file = fopen(name, "w");
	int i, j;
	for(i=0;i<echantillons;i++){
		for(j=0;j<t_micro+1;j= j+t_micro/1000){ // augmentation de l'incrementation pr diminuer le nombre de reultats
			fprintf(file, "%.0f ",calcul[i][j]);
		}
		fprintf(file, "\n");
	}
	fclose(file);
}

/*fonction de clacul de diffusion de chaleur d'un systeme */
float** calculChaleur(syst s, int echantillons, float tps) {
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
	// effets de bords T(a,t) = Ta et T(b,t) = Tb pour tout t>0, conditions limites de la plaque
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
	return res;
}

/*fonction d'ecriture des caracteristiques systeme dans un fichier name*/
void writeCarac(char* name, syst s){
    FILE* file = fopen(name, "w");
    fprintf(file, "%s\n%d\n%1.f\n%1.f\n%d", s.objet.nom, s.nbEchantillons, s.initTemp, s.src.valTemp,  s.src.posSrc);
    fclose(file);
}

int main(){
    int taille; float t_simu;
	FILE* carac;
	printf("creation du systeme\n");
    printf("choisissez la taille du fil en mm : "); taille = readInt();
    printf("choisissez le temps de simulation en s : "); t_simu = readFloat();
    syst fil = initSys(taille, dx);
    float** calcul = calculChaleur(fil, taille, t_simu);
    writeCalc("resultats.txt", calcul, taille, t_simu);
    writeCarac("caracteristiques.txt", fil);
	carac = fopen("caracteristiques.txt", "a");
	fprintf(carac, "\n%f", t_simu);
	fclose(carac);
	printf("resultats prets\n");
	
    return 0;
}