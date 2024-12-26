#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>

//Mes propres fichiers
#include "grille.c"
#include "caseVie.c"
#include "menu.c"


int main( int argc, char* args[] ){
    // Définition de utf-8 pour les accents
    setlocale(LC_ALL, "fr_FR.UTF-8");

    // Déclaration de la grille
    Grille grille = creationGrille();
    printf("Taille du tableau : %d x %d\n", grille.tailleX, grille.tailleY);
    //AfficherGrille(grille);

    grille.listePointeursLignes[2][1] = 1;
    grille.listePointeursLignes[2][2] = 1;
    grille.listePointeursLignes[3][1] = 1;
    grille.listePointeursLignes[1][1] = 1;
    grille.listePointeursLignes[4][1] = 1;

    //AfficherGrille(grille);
    int i = 0;
    while (i < 10)
    {
        VérifierCaseVivante(&grille);
        //AfficherGrille(grille);
        i++;
    }
    
    // Lancement du menu avec une verification de la réussite
    if (lancementMenu() != 0){
        fprintf(stderr, "Erreur lors du lancement du menu\n");
        exit(EXIT_FAILURE);
    }

    // Libération de la mémoire
    printf("Libération de la mémoire\n");
    libererGrille(&grille);

    // Fin du programme
    printf("Fin du programme\n");
    exit(0);
}