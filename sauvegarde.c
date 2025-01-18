#include <stdio.h>
#include "grille.h"

void sauvegarderGrilleChaine(GrilleChaine *grilleChaine, const char *nomFichier) {
    if (grilleChaine == NULL || grilleChaine->dernier == NULL) {
        fprintf(stderr, "Erreur : GrilleChaine vide.\n");
        return;
    }

    FILE *fichier = fopen(nomFichier, "w");
    if (!fichier) {
        perror("Erreur ouverture fichier");
        return;
    }

    fprintf(fichier, "%lu\n", grilleChaine->taille);

    Grille *actuelle = grilleChaine->premier;
    while (actuelle != NULL) {
        fprintf(fichier, "%d %d\n", actuelle->tailleX, actuelle->tailleY);
        for (int i = 0; i < actuelle->tailleY; i++) {
            for (int j = 0; j < actuelle->tailleX; j++) {
                fprintf(fichier, "%d ", actuelle->listePointeursLignes[i][j]);
            }
            fprintf(fichier, "\n");
        }
        actuelle = actuelle->suivant;
    }

    fclose(fichier);
    printf("Sauvegarde réussie dans %s\n", nomFichier);
}

GrilleChaine* chargerGrilleChaine(const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    if (!fichier) {
        perror("Erreur ouverture fichier");
        return NULL;
    }

    size_t taille;
    fscanf(fichier, "%lu", &taille);

    GrilleChaine *grilleChaine = (GrilleChaine *)malloc(sizeof(GrilleChaine));
    grilleChaine->taille = 0;
    grilleChaine->premier = grilleChaine->dernier = NULL;

    for (size_t t = 0; t < taille; t++) {
        int largeur, hauteur;
        fscanf(fichier, "%d %d", &largeur, &hauteur);

        Grille *nouvelleGrille = (Grille *)malloc(sizeof(Grille));
        nouvelleGrille->tailleX = largeur;
        nouvelleGrille->tailleY = hauteur;

        nouvelleGrille->listePointeursLignes = (int **)malloc(hauteur * sizeof(int *));
        for (int i = 0; i < hauteur; i++) {
            nouvelleGrille->listePointeursLignes[i] = (int *)malloc(largeur * sizeof(int));
            for (int j = 0; j < largeur; j++) {
                fscanf(fichier, "%d", &nouvelleGrille->listePointeursLignes[i][j]);
            }
        }

        nouvelleGrille->precedent = grilleChaine->dernier;
        nouvelleGrille->suivant = NULL;
        if (grilleChaine->dernier) {
            grilleChaine->dernier->suivant = nouvelleGrille;
        } else {
            grilleChaine->premier = nouvelleGrille;
        }
        grilleChaine->dernier = nouvelleGrille;
        grilleChaine->taille++;
    }

    fclose(fichier);
    printf("Chargement réussi depuis %s\n", nomFichier);
    return grilleChaine;
}
