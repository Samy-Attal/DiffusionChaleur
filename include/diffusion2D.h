#ifndef DIFFUSION2D

#define dy 0.001

#define file_source "./initialisation/source.txt"
#define file_syst "./initialisation/syst.txt"

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

/**
 * @brief creer une matrice 3D
 * 
 * @param x 
 * @param y 
 * @param t temps de simulation
 * @return float*** 
 */
float*** creerEspace(int x, int y, long t);

/**
 * @brief creer un systeme de dimension 2 (surface)
 * 
 * @param namesyst fichier systeme
 * @param namesrc fichier source de chaleur
 * @return syst 
 */
syst initSys2D(char* namesyst, char* namesrc);

/**
 * @brief calcul diffusion de la chaleur sur une surface et stock les resultats dans une matrice 3D
 * 
 * @param s 
 * @return float*** 
 */
float*** calculChaleur2D(syst s);

/**
 * @brief ecriture de fichiers resultats "imageXX.txt"
 * 
 * @param s 
 */
void writeFiles(syst s);


#endif