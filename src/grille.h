#ifndef GRILLE_H
#define GRILLE_H

typedef struct Grille {
    int tailleX;
    int tailleY;
    int **listePointeursLignes;
    struct Grille *suivant;
    struct Grille *precedent;
} Grille;

typedef struct GrilleChaine {
    Grille *premier;
    Grille *dernier;
    size_t taille;
} GrilleChaine;

GrilleChaine* creerGrilleChaine(int largeur, int hauteur);
void ajoutGrilleChaine(GrilleChaine *grilleChaine);
void libererGrilleChaine(GrilleChaine *grilleChaine);
void afficherGrilleChaine(GrilleChaine *grilleChaine);
void vérifierCaseVivante(GrilleChaine *grilleChaine);

#endif // GRILLE_H