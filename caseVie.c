#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

//Mes propres librairies
//#include "grille.h"

// PROTOTYPE
void vérifierCaseVivante(GrilleChaine* grille);

void vérifierCaseVivante(GrilleChaine* grille){

    // Création de la nouvelle grille
    ajoutGrilleChaine(grille);
    // Créer une grille temporaire pour stocker les nouvelles valeurs
    //int** nouvelleGrille = (int**)malloc(grille->dernier->tailleX * sizeof(int*));
    //if (nouvelleGrille == NULL) {
        //fprintf(stderr, "Erreur d'allocation de mémoire pour la nouvelle grille\n");
        //exit(EXIT_FAILURE);
    //}
    //for (int i = 0; i < grille->dernier->tailleX; i++) {
        //nouvelleGrille[i] = (int*)malloc(grille->dernier->tailleY * sizeof(int));
        //if (nouvelleGrille[i] == NULL) {
            //fprintf(stderr, "Erreur d'allocation de mémoire pour la nouvelle grille\n");
            //exit(EXIT_FAILURE);
        //}
    //}
    //afficherGrilleChaine(grille);

    if (grille->dernier->precedent != NULL) {
        for (int i = 0; i < grille->dernier->precedent->tailleY; i++){
            for (int j = 0; j < grille->dernier->precedent->tailleX; j++){

                // Permet de connaitre le nombre de case vivante autour de nous
                int nbVoisins = 0;

                // Vérification des cases autours en passant pas la dernière grille puis la precedente ce qui nous mène à la dernière grille calculé
                for (int k = i-1; k <= i+1; k++){
                    for (int l = j-1; l <= j+1; l++){
                        if (k >= 0 && k < grille->dernier->precedent->tailleY && l >= 0 && l < grille->dernier->precedent->tailleX){
                            if (grille->dernier->precedent->listePointeursLignes[k][l] == 1){
                                //printf("*La case X= %d, Y= %d est vivante\n", k, l);
                                if (k != i || l != j){
                                    nbVoisins++;
                                    //printf("- +1 pour %d, %d\n", k,l);
                                }
                            }
                        }
                    }
                }
                // Appliquer les règles du jeu de la vie et stocker le résultat dans la nouvelle grille (donc la dernière)
                if ((nbVoisins == 2 || nbVoisins == 3) && grille->dernier->precedent->listePointeursLignes[i][j] == 1){
                    grille->dernier->listePointeursLignes[i][j] = 1;
                } else if (nbVoisins == 3){
                    grille->dernier->listePointeursLignes[i][j] = 1;
                } else {
                    grille->dernier->listePointeursLignes[i][j] = 0;
                }
            }
        }
    }



    // Copier la grille temporaire dans la grille principale
    //for (int i = 0; i < grille->dernier->precedent->tailleX; i++){
        //for (int j = 0; j < grille->dernier->precedent->tailleY; j++){
            //grille->dernier->listePointeursLignes[i][j] = nouvelleGrille[i][j];
        //}
    //}

    // Libérer la mémoire de la grille temporaire
    //for (int i = 0; i < grille->dernier->tailleX; i++) {
        //free(nouvelleGrille[i]);
    //}
    //free(nouvelleGrille);
}

// Différentes possibilités pour la fonction VérifierCaseVivante
// 1 - une cellule vivante ayant exactement 2 ou 3 voisins vivants survit à la génération suivante ;
// 2 - une cellule vivante ayant de 4 à 8 cellules voisines vivantes meurt d'étouffement à la génération suivante ;
// 3 - une cellule vivante ayant zéro ou une cellule voisine vivante meurt d'isolement à la génération suivante ;
// 4 - sur une case vide ayant exactement 3 voisins vivants, une cellule naîtra à la génération suivante