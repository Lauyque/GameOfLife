#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

//Mes propres fichiers
#include "grille.c"


int main(){
    // Définition de utf-8 pour les accents
    setlocale(LC_ALL, "fr_FR.UTF-8");

    // Déclaration de la grille
    Grille grille = CreationGrille();
    printf("Taille du tableau : %d x %d\n", grille.tailleX, grille.tailleY);
    AfficherGrille(grille);

    grille.listePointeursLignes[1][1] = 1;

    AfficherGrille(grille);

    // Libération de la mémoire
    printf("Libération de la mémoire\n");
    LibererGrille(&grille);

    // Fin du programme
    printf("Fin du programme\n");
    exit(0);
}