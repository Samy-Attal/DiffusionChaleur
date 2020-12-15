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
	char nom[10];
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
            printf("attention fichier non conforme : ligne en trop dans la liste de materiaux, nom du materiau ou parametre manquant\n");
            return mater;
        }
        mater[i].alpha = mater[i].K / (mater[i].C * mater[i].rho);
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
	s.initTemp = readDouble(); 
	printf("\nConfiguration de la source\n");
	s.src = initSource(echantillons);

	return s;
}

int main(){
    syst fil = initSys(nb_X, dx);

	return 0;
}