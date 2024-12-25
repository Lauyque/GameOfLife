# GameOfLife
Jeu de la Vie

## Description
Le Jeu de la Vie est un automate cellulaire imaginé par le mathématicien britannique John Horton Conway en 1970. C'est un jeu à zéro joueur, ce qui signifie que son évolution est déterminée par son état initial, sans nécessiter d'intervention supplémentaire.

## Développement
Ce projet implémente le Jeu de la Vie en C, avec une grille de jeu représentée par une structure de données personnalisée.

## Structure du Projet
- `grille.h` : Contient les définitions et prototypes pour la gestion de la grille.
- `grille.c` : Contient les implémentations des fonctions pour manipuler la grille.
- `main.c` : Point d'entrée du programme, contenant la logique principale du jeu.

## Fichiers Importants

### grille.h
```cpp
// Prototype de la grille qui permet de créer la liste avec la taille en YxZ
typedef struct Grille Grille;

struct Grille
{
    int tailleX;
    int tailleY;
    int* listePointeursLignes;
};