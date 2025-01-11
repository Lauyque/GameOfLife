#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>

#define SDL_MAIN_HANDLED
#include <SDL.h> // Erreur normal puisqu'on rajoute le chemin vers la librairie SDL avec le makefile
#include <SDL_ttf.h> // Erreur normal puisqu'on rajoute le chemin vers la librairie SDL avec le makefile
#include <SDL_image.h> // Erreur normal puisqu'on rajoute le chemin vers la librairie SDL avec le makefile

// Mes propres fichiers
//#include "grille.h"

// PROTOTYPES
int lancementJeu(SDL_Renderer *ren, TTF_Font *fontTitle, TTF_Font *font, SDL_Color color, const char *nom, Grille* grille);
void AfficherGrilleJeu(Grille* grille, SDL_Renderer *ren, TTF_Font *font, SDL_Color color);
SDL_Rect afficherTexte(SDL_Renderer *ren, TTF_Font *font, const char *texte, int posX, int posY, SDL_Color color);

// Jeu
int lancementJeu(SDL_Renderer *ren, TTF_Font *fontTitle, TTF_Font *font, SDL_Color color, const char *nom, Grille* grille){
    int runningJeu = 1;

    // Vérification de la bonne allocution de la mémoire
    if ((*grille).listePointeursLignes == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour la liste des pointeurs vers les lignes\n");
        exit(EXIT_FAILURE);
    }

    while(runningJeu == 1){
        // Effacer l'écran
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        // Récuperer les dimention de l'écran
        SDL_DisplayMode DM;
        SDL_GetCurrentDisplayMode(0, &DM);
        int largeurEcran = DM.w;
        int hauteurEcran = DM.h;

        // Afficher le titre au centre en haut
        SDL_Rect titleRect = afficherTexte(ren, fontTitle, nom, largeurEcran / 2 - 100, 20, color);

        // Faire un rectangle au centre de l'écran pour la grille adapter a la taille de la grille
        SDL_Rect grilleRect = {largeurEcran / 2 - (*grille).tailleX * 10, hauteurEcran / 2 - (*grille).tailleY * 10, (*grille).tailleX * 20, (*grille).tailleY * 20};
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255); // Couleur blanche
        SDL_RenderDrawRect(ren, &grilleRect); // Dessiner le rectangle

        // Afficher la grille dans le rectangle
        AfficherGrilleJeu(grille, ren, font, color);

        // Gestion du temps entre les tours
        // Si on veut mettre un temps d'attente entre chaque tour
        // On peut utiliser la fonction SDL_GetTicks() qui retourne le nombre de millisecondes écoulées depuis l'initialisation de la SDL
        if (SDL_GetTicks() % 100 == 0){
            VérifierCaseVivante(grille);
        }

        // Afficher le bouton "Quitter" en bas au centre
        SDL_Rect retourRect = afficherTexte(ren, font, "Quitter", largeurEcran / 2 - 50, hauteurEcran - 50, color);


        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                runningJeu = 0;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
            {
                int mouseX = e.button.x;
                int mouseY = e.button.y;
                if (mouseX >= retourRect.x && mouseX <= retourRect.x + retourRect.w &&
                    mouseY >= retourRect.y && mouseY <= retourRect.y + retourRect.h)
                {
                    runningJeu = 0;
                }
            }
        }

        // Afficher le rendu
        SDL_RenderPresent(ren);
    }
    
    // Suppression de la grille
    libererGrille(grille);

    return 0;
}

// Fonction pour afficher la grille dans le jeu
void AfficherGrilleJeu(Grille* grille, SDL_Renderer *ren, TTF_Font *font, SDL_Color color){

    // Vérification de la bonne allocution de la mémoire
    if ((*grille).listePointeursLignes == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour la liste des pointeurs vers les lignes\n");
        exit(EXIT_FAILURE);
    }

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    int largeurEcran = DM.w;
    int hauteurEcran = DM.h;

    SDL_Rect grilleRect = {largeurEcran / 2 - (*grille).tailleX * 10, hauteurEcran / 2 - (*grille).tailleY * 10, (*grille).tailleX * 20, (*grille).tailleY * 20};

    for (int i = 0; i < (*grille).tailleX; i++){
        for (int j = 0; j < (*grille).tailleY; j++){
            if ((*grille).listePointeursLignes[i][j] == 0 || (*grille).listePointeursLignes[i][j] == 1){
                if ((*grille).listePointeursLignes[i][j] == 1){
                    // Afficher un carré plein
                    SDL_Rect caseRect = {grilleRect.x + i * 20, grilleRect.y + j * 20, 20, 20};
                    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255); // Couleur blanche
                    SDL_RenderFillRect(ren, &caseRect);
                } else {
                    // Afficher un carré gris vide
                    SDL_Rect caseRect = {grilleRect.x + i * 20, grilleRect.y + j * 20, 20, 20};
                    SDL_SetRenderDrawColor(ren, 128, 128, 128, 255); // Couleur grise
                    SDL_RenderFillRect(ren, &caseRect);
                }
            } else {
                fprintf(stderr, "Erreur: valeur de la case incorrecte\n");
            }
        }
    }
}