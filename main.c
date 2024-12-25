#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

//Mes propres librairies
#include "grille.h"

// PROTOTYPE
Grille choixTailleGrille();


int main(){

    Grille grille = choixTailleGrille();
    printf("Taille du tableau : %d x %d\n", grille.tailleX, grille.tailleY);

    printf("Fin du programme\n");

    exit(0);
}

// Fonction pour choisir la taille du tableau
Grille choixTailleGrille(){
    // Utilisation de la structure Grille
    Grille grille;
    printf("Choisissez la taille du tableau : \n - Y : ");
    scanf("%d",&grille.tailleX);
    printf(" - Z : ");
    scanf("%d",&grille.tailleY);
    return grille;
}