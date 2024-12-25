#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

//Mes propres librairies
#include "grille.h"

// PROTOTYPE
Grille CreationGrille();
void AfficherGrille(Grille grille);
void LibererGrille(Grille grille);


int main(){

    Grille grille = CreationGrille();
    printf("Taille du tableau : %d x %d\n", grille.tailleX, grille.tailleY);
    AfficherGrille(grille);

    printf("Fin du programme\n");

    exit(0);
}

// Fonction pour créer la grille (la taille et la liste des pointeurs vers les futurs lignes de notre grille)
Grille CreationGrille(){
    // Utilisation de la structure Grille
    Grille grille;
    printf("Choisissez la taille du tableau : \n - Y : ");
    scanf("%d",&grille.tailleX);
    printf(" - Z : ");
    scanf("%d",&grille.tailleY);

    // Allocution de la mémoire pour la liste de pointeurs
    grille.listePointeursLignes = (int*)malloc(grille.tailleX * grille.tailleY * sizeof(int));
    
    // Vérification de la bonne allocution de la mémoire
    if (grille.listePointeursLignes == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }
    return grille;
}

//Fonction pour afficher la grille
void AfficherGrille(Grille grille){
    for (int i = 0; i < grille.tailleX; i++){
        for (int j = 0; j < grille.tailleY; j++){
            printf("%d ", grille.listePointeursLignes[i * grille.tailleY + j]);
        }
        printf("\n");
    }
}

// Fonction pour libérer la mémoire de la grille
void LibererGrille(Grille grille){
    free(grille.listePointeursLignes);
}