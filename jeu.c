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
//typedef struct Grille grille;
int lancementJeu(SDL_Renderer *ren, TTF_Font *fontTitle, TTF_Font *font, SDL_Color color, const char *nom, Grille* grille);
void AfficherGrilleJeu(Grille* grille, SDL_Renderer *ren, TTF_Font *font, SDL_Color color);
SDL_Rect afficherTexte(SDL_Renderer *ren, TTF_Font *font, const char *texte, int posX, int posY, SDL_Color color);

// Jeu
int lancementJeu(SDL_Renderer *ren, TTF_Font *fontTitle, TTF_Font *font, SDL_Color color, const char *nom, Grille* grille){
    int runningJeu = 1;

    while(runningJeu == 1){
        // Effacer l'écran
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        // Afficher le titre
        SDL_Rect titleRect = afficherTexte(ren, fontTitle, nom, 10, 20, color);

        // Afficher la grille
        AfficherGrilleJeu(grille, ren, font, color);

        // Afficher le bouton "Quitter" en bas à droite
        SDL_Rect retourRect = afficherTexte(ren, font, "Retour", 500, 400, color);


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

        // Suppression de la grille
        libererGrille(grille);

        // Afficher le rendu
        SDL_RenderPresent(ren);
    }
    
    return 0;
}

// Fonction pour afficher la grille dans le jeu
void AfficherGrilleJeu(Grille* grille, SDL_Renderer *ren, TTF_Font *font, SDL_Color color){
    printf("Début du tableau\n");
    for (int i = 0; i < (*grille).tailleX; i++){
        for (int j = 0; j < (*grille).tailleY; j++){
            printf("| %d ", (*grille).listePointeursLignes[i][j]);
            if ((*grille).listePointeursLignes[i][j] == 1){
                SDL_Rect caseRect = afficherTexte(ren, font, "X", 10 + i * 20, 10 + j * 20, color);
            } else {
                SDL_Rect caseRect = afficherTexte(ren, font, "0", 10 + i * 20, 10 + j * 20, color);
            }
        }
        printf("|\n");
        for (int j = 0; j < (*grille).tailleY; j++){
            printf("----");
        }
        printf("-\n");
    }
    printf("Fin du tableau\n\n");
}