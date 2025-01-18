#ifndef GRILLE_H
#define GRILLE_H

typedef struct GrilleChaine GrilleChaine;
typedef struct Grille Grille;

struct GrilleChaine {
    size_t taille;
    struct Grille *dernier;
    struct Grille *premier;
};

struct Grille {
    struct Grille *precedent;
    struct Grille *suivant;
    int tailleX;
    int tailleY;
    int **listePointeursLignes;
};

GrilleChaine* creerGrilleChaine(int largeur, int hauteur);
void libererGrilleChaine(GrilleChaine *grille);
void afficherGrilleChaine(GrilleChaine *grilleChaine);
void ajoutGrilleChaine(GrilleChaine *grille);

#endif // GRILLE_H
