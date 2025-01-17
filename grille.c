#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

//Mes propres librairies
#include "grille.h"
// // PROTOTYPE
// Grille creationGrille(const char *input);
// //void AfficherGrille(Grille grille);
// void libererGrille(Grille* grille);

// // Fonction pour créer la grille (la taille et la liste des pointeurs vers les futurs lignes de notre grille)
// Grille creationGrille(const char *input){
//     // Utilisation de la structure Grille
//     Grille grille;

    // Demande de la taille de la grille via un terminal
    //printf("Choisissez la taille du tableau : \n- Y : ");
    //scanf("%d",&grille.tailleX);
    //printf("- Z : ");
    //scanf("%d",&grille.tailleY);

    // lecture de "input" pour la taille de la grille
    // Variable pour sauvegarde la valeur de input
//     char inputSauv[100];
//     strcpy(inputSauv, input);
//     char *token = strtok(input, "x"); // On découpe la chaine de caractère avec un espace
//     grille.tailleX = atoi(token); // On convertit le premier élément en int
//     token = strtok(NULL, " "); // On passe au prochain élément
//     grille.tailleY = atoi(token); // On convertit le deuxième élément en int
//     printf("Taille du tableau : %d x %d\n", grille.tailleX, grille.tailleY);
//     // réassignation de la bonna valeur à la variable input
//     strcpy(input, inputSauv);


//     // Allocation de la mémoire pour la liste de pointeurs
//     grille.listePointeursLignes = (int**)malloc(grille.tailleX * sizeof(int*));
    
//     // Vérification de la bonne allocation de la mémoire
//     if (grille.listePointeursLignes == NULL) {
//         fprintf(stderr, "Erreur d'allocation de mémoire pour la liste des pointeurs vers les lignes\n");
//         exit(EXIT_FAILURE);
//     }

//     // Remplissage de la grille avec des 0
//     for (int i = 0; i < grille.tailleX; i++){
//         grille.listePointeursLignes[i] = (int*)malloc(grille.tailleY * sizeof(int));
//         if (grille.listePointeursLignes[i] == NULL) {
//             fprintf(stderr, "Erreur d'allocation de mémoire pour la ligne %d\n", i);
//             exit(EXIT_FAILURE);
//         }
//         for (int j = 0; j < grille.tailleY; j++){
//             grille.listePointeursLignes[i][j] = 0;
//         }
//     }


//     return grille;
// }

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
//void libererGrille(Grille* grille) {
    //if (!grille || !grille->listePointeursLignes) {
        //return;
    //}
    //for (int i = 0; i < grille->tailleX; i++) {
        //if (grille->listePointeursLignes[i] != NULL) {
            //free(grille->listePointeursLignes[i]);
            //grille->listePointeursLignes[i] = NULL;
            //printf("Mémoire libérée pour la ligne %d\n", i);
        //}
    //}
    //free(grille->listePointeursLignes);
    //grille->listePointeursLignes = NULL;
    //printf("Mémoire libérée pour la liste des pointeurs vers les lignes\n");
//}



// NOUVELLE STRUCT EN DOUBLE CHAINEE
GrilleChaine* creerGrilleChaine(int largeur, int hauteur){
    // Test de la nouvelle structure de grille
    Grille* grille = (Grille*)malloc(sizeof(Grille));
    if (!grille) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour la grille\n");
        exit(EXIT_FAILURE);
    }
    grille->tailleX = largeur;
    grille->tailleY = hauteur;
    //printf("Création de la grille : x = %d y = %d\n",grille->tailleX ,grille->tailleY);
    grille->listePointeursLignes = (int**)malloc(grille->tailleY * sizeof(int*));
    if (!grille->listePointeursLignes) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour les pointeurs de lignes\n");
        free(grille);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < grille->tailleY; i++) {
        grille->listePointeursLignes[i] = (int*)malloc(grille->tailleX * sizeof(int));
        if (!grille->listePointeursLignes[i]) {
            fprintf(stderr, "Erreur d'allocation de mémoire pour la ligne %d\n", i);
        }
        for (int j = 0; j < grille->tailleX; j++) {
            //printf("1er allocution : i = %d j = %d", i,j);
            grille->listePointeursLignes[i][j] = 0;
        }
    }

    GrilleChaine* grilleChaine = (GrilleChaine*)malloc(sizeof(GrilleChaine));
    // if (!grilleChaine) {
    //     fprintf(stderr, "Erreur d'allocation de mémoire pour la grille chaînée\n");
    //     for (int i = 0; i < grille->tailleY; i++) {
    //         free(grille->listePointeursLignes[i]);
    //     }
    //     free(grille->listePointeursLignes);
    //     free(grille);
    //     exit(EXIT_FAILURE);
    // }
    grilleChaine->taille = 1;
    grilleChaine->premier = grille;
    grilleChaine->dernier = grille;
    grille->precedent = NULL;
    grille->suivant = NULL;

    return grilleChaine;
}


void ajoutGrilleChaine(GrilleChaine *grille) {
    //printf("Tentative Nouvelle grille");
    setlocale(LC_ALL, "fr_FR.UTF-8");
    if (!grille || !grille->dernier) {
        fprintf(stderr, "La grille transferer est vide\n");
        return;
    }

    Grille *nouvelleGrille = (Grille*)malloc(sizeof(Grille));
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
            //printf("allocution : i = %d j = %d", i,j);

        }
    }

    nouvelleGrille->precedent = grille->dernier;
    nouvelleGrille->suivant = NULL;
    grille->dernier->suivant = nouvelleGrille;
    grille->dernier = nouvelleGrille;
    grille->taille++;

    //printf("Nouvelle grille de taille %d x %d ajoutée à la chaîne\n", nouvelleGrille->tailleX, nouvelleGrille->tailleY);
    //printf("Nombre de grilles dans la chaîne: %lld\n", grille->taille);
    //printf("Nouvelle grille ajoutée à la chaîne\n");
}

void libererGrilleChaine(GrilleChaine *grille){
    setlocale(LC_ALL, "fr_FR.UTF-8");
    if (!grille) {
        return;
    }

    while (grille->dernier != NULL) {
        Grille *current = grille->dernier;

        // Libérer la mémoire de la grille actuelle
        for (int i = 0; i < current->tailleY; i++) {
            free(current->listePointeursLignes[i]);
            //printf("Libération de la memoire pour la ligne %d du tableau %lld\n", i, grille->taille);
        }
        free(current->listePointeursLignes);
        grille->dernier = current->precedent;
        free(current);
        grille->taille -= 1;
    }

    grille->premier = NULL;
    grille->dernier = NULL;
    grille->taille = 0;
    free(grille);

    printf("Mémoire libérée pour toutes les grilles de la chaîne\n");
}


void afficherGrilleChaine(GrilleChaine *grilleChaine) {
    //printf("Affichage grille");
    setlocale(LC_ALL, "fr_FR.UTF-8");
    if (!grilleChaine || !grilleChaine->premier) {
        printf("La chaîne de grilles est vide.\n");
        return;
    }

    Grille *current = grilleChaine->premier;
    int grilleIndex = 0;

    while (current != NULL) {
        printf("Grille %d (taille %d x %d):\n", grilleIndex, current->tailleX, current->tailleY);
        for (int i = 0; i < current->tailleY; i++) {
            for (int j = 0; j < current->tailleX; j++) {
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