// Prototype de la grille qui permet de créer la liste avec la taille en YxZ
typedef struct Grille Grille;

struct Grille
{
    int tailleX;
    int tailleY;
    int** listePointeursLignes; // "int**"" car c'est le pointeur vers la liste contenant d'autres pointeurs vers les lignes
};