#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

//Mes propres librairies
#include "grille.h"

// PROTOTYPE
Grille creationGrille();
void AfficherGrille(Grille grille);
void libererGrille(Grille* grille);

// Fonction pour créer la grille (la taille et la liste des pointeurs vers les futurs lignes de notre grille)
Grille creationGrille(){
    // Utilisation de la structure Grille
    Grille grille;
    printf("Choisissez la taille du tableau : \n- Y : ");
    scanf("%d",&grille.tailleX);
    printf("- Z : ");
    scanf("%d",&grille.tailleY);

    // Allocution de la mémoire pour la liste de pointeurs
    grille.listePointeursLignes = (int**)malloc(grille.tailleX * sizeof(int*));
    
    // Vérification de la bonne allocution de la mémoire
    if (grille.listePointeursLignes == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour la liste des pointeurs vers les lignes\n");
        exit(EXIT_FAILURE);
    }

    // Remplossage de la grille avec des 0
    for (int i = 0; i < grille.tailleX; i++){
        grille.listePointeursLignes[i] = (int*)malloc(grille.tailleY * sizeof(int));
        if (grille.listePointeursLignes[i] == NULL) {
            fprintf(stderr, "Erreur d'allocation de mémoire pour la ligne %d\n", i);
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < grille.tailleY; j++){
            grille.listePointeursLignes[i][j] = 0;
        }
    }


    return grille;
}

//Fonction pour afficher la grille
void AfficherGrille(Grille grille){
    printf("Début du tableau\n");
    for (int i = 0; i < grille.tailleX; i++){
        for (int j = 0; j < grille.tailleY; j++){
            printf("| %d ", grille.listePointeursLignes[i][j]);
        }
        printf("|\n");
        for (int j = 0; j < grille.tailleY; j++){
            printf("----");
        }
        printf("-\n");
    }
    printf("Fin du tableau\n\n");
}


// Fonction pour libérer la mémoire de la grille
void libererGrille(Grille* grille) {
    if (!grille || !grille->listePointeursLignes) {
        return;
    }
    for (int i = 0; i < grille->tailleX; i++) {
        if (grille->listePointeursLignes[i] != NULL) {
            free(grille->listePointeursLignes[i]);
            grille->listePointeursLignes[i] = NULL;
            printf("Mémoire libérée pour la ligne %d\n", i);
        }
    }
    free(grille->listePointeursLignes);
    grille->listePointeursLignes = NULL;
    printf("Mémoire libérée pour la liste des pointeurs vers les lignes\n");
}