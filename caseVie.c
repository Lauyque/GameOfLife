#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

//Mes propres librairies
//#include "grille.h"

// PROTOTYPE
void VérifierCaseVivante(Grille* grille);

void VérifierCaseVivante(Grille* grille){
    // Créer une grille temporaire pour stocker les nouvelles valeurs
    int** nouvelleGrille = (int**)malloc((*grille).tailleX * sizeof(int*));
    if (nouvelleGrille == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour la nouvelle grille\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < (*grille).tailleX; i++) {
        nouvelleGrille[i] = (int*)malloc((*grille).tailleY * sizeof(int));
        if (nouvelleGrille[i] == NULL) {
            fprintf(stderr, "Erreur d'allocation de mémoire pour la nouvelle grille\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < (*grille).tailleX; i++){
        for (int j = 0; j < (*grille).tailleY; j++){

            int nbVoisins = 0;
            //printf("La case X= %d, Y= %d est vivante\n", i, j);

            // Vérification des cases autour
            for (int k = i-1; k <= i+1; k++){
                for (int l = j-1; l <= j+1; l++){
                    if (k >= 0 && k < (*grille).tailleX && l >= 0 && l < (*grille).tailleY){
                        if ((*grille).listePointeursLignes[k][l] == 1){
                            //printf("*La case X= %d, Y= %d est vivante\n", k, l);
                            if (k != i || l != j){
                                nbVoisins++;
                            }
                        }
                    }
                }
            }
            //printf("Nombre de voisins : %d\n", nbVoisins);
            // Appliquer les règles du jeu de la vie et stocker le résultat dans la grille temporaire
            if ((nbVoisins == 2 || nbVoisins == 3) && (*grille).listePointeursLignes[i][j] == 1){
                nouvelleGrille[i][j] = 1;
            } else if (nbVoisins == 3){
                nouvelleGrille[i][j] = 1;
            } else {
                nouvelleGrille[i][j] = 0;
            }
        }
    }

    // Copier la grille temporaire dans la grille principale
    for (int i = 0; i < (*grille).tailleX; i++){
        for (int j = 0; j < (*grille).tailleY; j++){
            (*grille).listePointeursLignes[i][j] = nouvelleGrille[i][j];
        }
    }

    // Libérer la mémoire de la grille temporaire
    for (int i = 0; i < (*grille).tailleX; i++) {
        free(nouvelleGrille[i]);
    }
    free(nouvelleGrille);
}

// Différentes possibilités pour la fonction VérifierCaseVivante
// 1 - une cellule vivante ayant exactement 2 ou 3 voisins vivants survit à la génération suivante ;
// 2 - une cellule vivante ayant de 4 à 8 cellules voisines vivantes meurt d'étouffement à la génération suivante ;
// 3 - une cellule vivante ayant zéro ou une cellule voisine vivante meurt d'isolement à la génération suivante ;
// 4 - sur une case vide ayant exactement 3 voisins vivants, une cellule naîtra à la génération suivante