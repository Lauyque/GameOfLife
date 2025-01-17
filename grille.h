// Prototype de la grille qui permet de créer la liste avec la taille en YxZ
typedef struct Grille Grille;

typedef struct GrilleChaine GrilleChaine;
typedef struct GrilleTest GrilleTest;

struct Grille
{
    int tailleX;
    int tailleY;
    int** listePointeursLignes; // "int**"" car c'est le pointeur vers la liste contenant d'autres pointeurs vers les lignes
};


// Test de structure pour une chaine doublement chainé
struct GrilleChaine
{
    size_t taille;
    struct GrilleTest *dernier;
    struct GrilleTest *premier;
};

struct GrilleTest
{
    struct GrilleTest *precedent;
    struct GrilleTest *suivant;
    int tailleX;
    int tailleY;
    int** listePointeursLignes;
};

// Prototypes des fonctions
GrilleChaine* creerGrilleChaine(int largeur, int hauteur);
void libererGrilleChaine(GrilleChaine *grille);
void afficherGrilleChaine(GrilleChaine *grilleChaine);
void mettreAJourGrilleChaine(GrilleChaine *grille);