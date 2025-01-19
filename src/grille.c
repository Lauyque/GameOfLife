#include <stdio.h>
#include <stdlib.h>

//Mes propres librairies
#include "grille.h"

// Création de la grille
GrilleChaine* creerGrilleChaine(int largeur, int hauteur){
    // Test de la nouvelle structure de grille
    //printf("Création d'une nouvelle grille\n");
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
            for (int j = 0; j < i; j++) {
                free(grille->listePointeursLignes[j]);
            }
            free(grille->listePointeursLignes);
            free(grille);
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < grille->tailleX; j++) {
            //printf("1er allocution : i = %d j = %d", i,j);
            grille->listePointeursLignes[i][j] = 0;
        }
    }

    GrilleChaine* grilleChaine = (GrilleChaine*)malloc(sizeof(GrilleChaine));
    if (!grilleChaine) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour la grille chaînée\n");
        for (int i = 0; i < grille->tailleY; i++) {
            free(grille->listePointeursLignes[i]);
        }
        free(grille->listePointeursLignes);
        free(grille);
        exit(EXIT_FAILURE);
    }
    grilleChaine->taille = 1;
    grilleChaine->premier = grille;
    grilleChaine->dernier = grille;
    grille->precedent = NULL;
    grille->suivant = NULL;

    return grilleChaine;
}

// Ajout d'une grille
void ajoutGrilleChaine(GrilleChaine *grille) {
    //printf("Tentative Nouvelle grille");
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

// Suppression de toutes les grilles de la chaine
void libererGrilleChaine(GrilleChaine *grillechaine) {
    if (!grillechaine) {
        return;
    }
    Grille *grille = grillechaine->dernier;
    while (grille != NULL) {
        if (!grille || !grille->listePointeursLignes) {
            return;
        }
        for (int i = 0; i < grille->tailleY; i++) {
            if (grille->listePointeursLignes[i] != NULL) {
                free(grille->listePointeursLignes[i]);
                grille->listePointeursLignes[i] = NULL;
                //printf("Mémoire libérée pour la ligne %d\n", i);
            }
        }
        free(grille->listePointeursLignes);
        grille->listePointeursLignes = NULL;
        Grille *precedent = grille->precedent;
        free(grille);
        grille = precedent;
    }

    free(grillechaine);

    printf("Mémoire libérée pour toutes les grilles de la chaîne\n");
}

// Afficher la grille dans le terminal
void afficherGrilleChaine(GrilleChaine *grilleChaine) {
    //printf("Affichage grille");
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

// Calcul de la prochaine grille
void vérifierCaseVivante(GrilleChaine* grille){

    // Création de la nouvelle grille
    ajoutGrilleChaine(grille);
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
}