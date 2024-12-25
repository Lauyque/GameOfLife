#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

// PROTOTYPE
void VérifierCaseVivante(Grille* grille);
void chasseur(Grille* grille, int x, int y, int* nbVoisins);

void VérifierCaseVivante(Grille* grille){
    for (int i = 0; i < (*grille).tailleX; i++){
        for (int j = 0; j < (*grille).tailleY; j++){
            if ((*grille).listePointeursLignes[i][j] == 1){
                //printf("La case X= %d, Y= %d est vivante\n", i, j);

                // Vérification des cases autour
                int nbVoisins = 0;
                int x, y = 0;
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
                        y = l;
                    }
                    x = k;
                }
                //printf("Nombre de voisins : %d\n", nbVoisins);
                chasseur(grille, x, y, &nbVoisins);
            }
        }
    }
}

void chasseur(Grille* grille, int x, int y, int* nbVoisins){
    printf("Le chasseur a rendu sont verdic pour la case X= %d, Y= %d\n", x, y);
    if (*nbVoisins == 2 || *nbVoisins == 3){
        printf("- La case X= %d, Y= %d est vivante\n", x, y);
    }
    else if (*nbVoisins >= 4 && *nbVoisins <= 8){
        printf("- La case X= %d, Y= %d est morte\n", x, y);
    }
    else if (*nbVoisins == 0 || *nbVoisins == 1){
        printf("- La case X= %d, Y= %d est morte\n", x, y);
    }
    else if (*nbVoisins == 3){
        printf("- La case X= %d, Y= %d est née\n", x, y);
    }
    else{
        printf("Erreur\n");
    }
}



// Différentes possibilités pour la fonction VérifierCaseVivante
// 1 - une cellule vivante ayant exactement 2 ou 3 voisins vivants survit à la génération suivante ;
// 2 - une cellule vivante ayant de 4 à 8 cellules voisines vivantes meurt d'étouffement à la génération suivante ;
// 3 - une cellule vivante ayant zéro ou une cellule voisine vivante meurt d'isolement à la génération suivante ;
// 4 - sur une case vide ayant exactement 3 voisins vivants, une cellule naîtra à la génération suivante