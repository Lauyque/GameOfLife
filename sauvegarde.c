#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>

void sauvegarder_grille(int** grille, int largeur, int hauteur, const char* nom_fichier) {
    FILE* fichier = fopen(nom_fichier, "w");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s\n", nom_fichier);
        return;
    }
    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; j++) {
            fprintf(fichier, "%d ", grille[i][j]);
        }
        fprintf(fichier, "\n");
    }
    fclose(fichier);
    printf("Grille sauvegardée dans %s\n", nom_fichier);
}

void charger_grille(int** grille, int largeur, int hauteur, const char* nom_fichier) {
    FILE* fichier = fopen(nom_fichier, "r");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s\n", nom_fichier);
        return;
    }
    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; j++) {
            fscanf(fichier, "%d", &grille[i][j]);
        }
    }
    fclose(fichier);
    printf("Grille chargée depuis %s\n", nom_fichier);
}
