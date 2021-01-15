#ifndef DIFFUSION1D

#define dx 0.001 //= 1mm 
#define dt 0.000001 //= 1µs
#define file_resultat "./resultat1D/resultats.txt"
#define file_materiaux "./initialisation/materiaux.txt"
#define file_carac "./resultat1D/caracteristiques.txt"

typedef struct {
	char nom[10];
	float K, C, rho, alpha;
}materiau;

typedef struct {
	int posSrc; 
	float valTemp;	
}source1;

typedef struct {
	float resX;
	int nbEchantillons;
	float initTemp;
	materiau objet;
	source1 src;
}syst1;

/** 
 * @return lit une valeur de type float rentree au clavier 
 */
float readFloat();

/**
 * @return lit une valeur de type int rentree au clavier 
 */
int readInt();

/**
 * @brief creation de matrice
 * 
 * @param nb_lignes 
 * @param nb_colonnes 
 * @return float** 
 */
float** creerMat(int nb_lignes, int nb_colonnes);

/**
 * @brief compte le nombre de ligne dans un fichier texte
 * 
 * @param name 
 * @return int 
 */
int cptLignes(char *name);

/**
 * @brief creer un tableau de type materiau a partir d'un fichier texte
 * 
 * @param name 
 * @return materiau* 
 */
materiau* initMatiere(char* name);

/**
 * @brief creer une source ponctuelle
 * 
 * @param echantillons taille du systeme
 * @return source1 
 */
source1 initSource1(int echantillons);

/**
 * @brief creer un systeme de dimension 1 avec source ponctuelle et materiau
 * 
 * @param echantillons 
 * @param resol_x 
 * @return syst1 
 */
syst1 initSys1(int echantillons, float resol_x);

/**
 * @brief ecrit un fichier texte contenant les resultats
 * 
 * @param name nom du fichier
 * @param calcul matrice de calcul
 * @param echantillons taille du systeme
 * @param tps temps de simulation
 */
void writeCalc(char* name, float** calcul, int echantillons, float tps);

/**
 * @brief calcul et stock les resultats dans une matrice
 * 
 * @param s 
 * @param echantillons taille du systeme
 * @param tps temps de simulation
 * @return float** 
 */
float** calculChaleur(syst1 s, int echantillons, float tps);

/**
 * @brief ecrit dans un fichier texte les caracteristiques de la simulation
 * 
 * @param name nom du fichier
 * @param s 
 */
void writeCarac(char* name, syst1 s);

#endif 