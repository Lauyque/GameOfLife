#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

//Mes propres librairies
#include "grille.h"

// PROTOTYPE
Grille creationGrille(const char *input);
void AfficherGrille(Grille grille);
void libererGrille(Grille* grille);

// Fonction pour créer la grille (la taille et la liste des pointeurs vers les futurs lignes de notre grille)
Grille creationGrille(const char *input){
    // Utilisation de la structure Grille
    Grille grille;

    // Demande de la taille de la grille via un terminal
    //printf("Choisissez la taille du tableau : \n- Y : ");
    //scanf("%d",&grille.tailleX);
    //printf("- Z : ");
    //scanf("%d",&grille.tailleY);

    // lecture de "input" pour la taille de la grille
    // Variable pour sauvegarde la valeur de input
    char inputSauv[100];
    strcpy(inputSauv, input);
    char *token = strtok(input, "x"); // On découpe la chaine de caractère avec un espace
    grille.tailleX = atoi(token); // On convertit le premier élément en int
    token = strtok(NULL, " "); // On passe au prochain élément
    grille.tailleY = atoi(token); // On convertit le deuxième élément en int
    printf("Taille du tableau : %d x %d\n", grille.tailleX, grille.tailleY);
    // réassignation de la bonna valeur à la variable input
    strcpy(input, inputSauv);


    // Allocution de la mémoire pour la liste de pointeurs
    grille.listePointeursLignes = (int**)malloc(grille.tailleX * sizeof(int*));
    
    // Vérification de la bonne allocution de la mémoire
    if (grille.listePointeursLignes == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour la liste des pointeurs vers les lignes\n");
        exit(EXIT_FAILURE);
    }

    // Remplissage de la grille avec des 0
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