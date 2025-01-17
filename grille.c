#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

//Mes propres librairies
#include "grille.h"

// PROTOTYPE
Grille creationGrille(const char *input);
//void AfficherGrille(Grille grille);
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


    // Allocation de la mémoire pour la liste de pointeurs
    grille.listePointeursLignes = (int**)malloc(grille.tailleX * sizeof(int*));
    
    // Vérification de la bonne allocation de la mémoire
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
//void AfficherGrille(Grille grille){
//    printf("Début du tableau\n");
//    for (int i = 0; i < grille.tailleX; i++){
//        for (int j = 0; j < grille.tailleY; j++){
//            printf("| %d ", grille.listePointeursLignes[i][j]);
//        }
//        printf("|\n");
//        for (int j = 0; j < grille.tailleY; j++){
//            printf("----");
//        }
//        printf("-\n");
//    }
//    printf("Fin du tableau\n\n");
//}


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



// NOUVELLE STRUCT
GrilleChaine* creerGrilleChaine(int largeur, int hauteur){
    // Test de la nouvelle structure de grille
    GrilleTest* grilleTest = (GrilleTest*)malloc(sizeof(GrilleTest));
    grilleTest->tailleX = largeur;
    grilleTest->tailleY = hauteur;
    grilleTest->listePointeursLignes = (int**)malloc(grilleTest->tailleY * sizeof(int*));
    for (int i = 0; i < grilleTest->tailleY; i++) {
        grilleTest->listePointeursLignes[i] = (int*)malloc(grilleTest->tailleX * sizeof(int));
        for (int j = 0; j < grilleTest->tailleX; j++) {
            grilleTest->listePointeursLignes[i][j] = 0;
        }
    }

    GrilleChaine* grilleChaine = (GrilleChaine*)malloc(sizeof(GrilleChaine));
    grilleChaine->taille = 1;
    grilleChaine->premier = grilleTest;
    grilleChaine->dernier = grilleTest;
    grilleTest->precedent = NULL;
    grilleTest->suivant = NULL;

    return grilleChaine;
}


void mettreAJourGrilleChaine(GrilleChaine *grille) {
    if (!grille || !grille->dernier) {
        return;
    }

    GrilleTest *nouvelleGrille = (GrilleTest*)malloc(sizeof(GrilleTest));
    if (!nouvelleGrille) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour la nouvelle grille\n");
        exit(EXIT_FAILURE);
    }

    nouvelleGrille->tailleX = grille->dernier->tailleX;
    nouvelleGrille->tailleY = grille->dernier->tailleY;
    nouvelleGrille->listePointeursLignes = (int**)malloc(nouvelleGrille->tailleY * sizeof(int*));
    if (!nouvelleGrille->listePointeursLignes) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour les lignes de la nouvelle grille\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nouvelleGrille->tailleY; i++) {
        nouvelleGrille->listePointeursLignes[i] = (int*)malloc(nouvelleGrille->tailleX * sizeof(int));
        if (!nouvelleGrille->listePointeursLignes[i]) {
            fprintf(stderr, "Erreur d'allocation de mémoire pour la ligne %d de la nouvelle grille\n", i);
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < nouvelleGrille->tailleX; j++) {
            // Mettre à jour les valeurs de la nouvelle grille ici
            nouvelleGrille->listePointeursLignes[i][j] = grille->dernier->listePointeursLignes[i][j]; // Exemple de copie des valeurs
        }
    }

    nouvelleGrille->precedent = grille->dernier;
    nouvelleGrille->suivant = NULL;
    grille->dernier->suivant = nouvelleGrille;
    grille->dernier = nouvelleGrille;
    grille->taille++;

    printf("Nouvelle grille de taille %d x %d ajoutée à la chaîne\n", nouvelleGrille->tailleX, nouvelleGrille->tailleY);
    printf("Nombre de grilles dans la chaîne: %d\n", grille->taille);
    printf("Nouvelle grille ajoutée à la chaîne\n");
}

void libererGrilleChaine(GrilleChaine *grille){
    if (!grille) {
        return;
    }

    GrilleTest *current = grille->premier;
    while (current != NULL) {
        GrilleTest *next = current->suivant;

        // Libérer la mémoire de la grille actuelle
        for (int i = 0; i < current->tailleY; i++) {
            free(current->listePointeursLignes[i]);
            printf("Libération de la memoire pour la ligne %d du tableau %d\n", i, grille->taille);
        }
        free(current->listePointeursLignes);

        // Passer à la grille suivante
        current = next;
    }

    grille->premier = NULL;
    grille->dernier = NULL;
    grille->taille = 0;

    printf("Mémoire libérée pour toutes les grilles de la chaîne\n");
}


void afficherGrilleChaine(GrilleChaine *grilleChaine) {
    if (!grilleChaine || !grilleChaine->premier) {
        printf("La chaîne de grilles est vide.\n");
        return;
    }

    GrilleTest *current = grilleChaine->premier;
    int grilleIndex = 0;

    while (current != NULL) {
        printf("Grille %d (taille %d x %d):\n", grilleIndex, current->tailleX, current->tailleY);
        for (int i = 0; i < current->tailleX; i++) {
            for (int j = 0; j < current->tailleY; j++) {
                printf("| %d ", current->listePointeursLignes[i][j]);
            }
            printf("|\n");
            for (int j = 0; j < current->tailleY; j++) {
                printf("----");
            }
            printf("-\n");
        }
        printf("Fin de la grille %d\n\n", grilleIndex);
        current = current->suivant;
        grilleIndex++;
    }
}