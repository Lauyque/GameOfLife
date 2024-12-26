#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>
#define SDL_MAIN_HANDLED
#include <SDL.h> // Erreur normal puisqu'on rajoute le chemin vers la librairie SDL avec le makefile
#include <SDL_ttf.h>

// PROTOTYPES
int lancementMenu();

// Fonction qui affiche le menu
int lancementMenu(){
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    // Initialisation de SDL_ttf
    if (TTF_Init() == -1) {
        printf("TTF_Init Error: %s\n", TTF_GetError());
        SDL_Quit();
        TTF_Quit();
        SDL_Quit();
    }

    // Création de la fenêtre
    // Création de la fenêtre
    SDL_Window *menu = SDL_CreateWindow("Game of Life", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
    if (menu == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Création du renderer
    SDL_Renderer *ren = SDL_CreateRenderer(menu, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == NULL) {
        SDL_DestroyWindow(menu);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Boucle principale
    int running = 1;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Effacer l'écran
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        // Dessiner le titre
        SDL_Color white = {255, 255, 255, 255};
        TTF_Font *font = TTF_OpenFont("arial.ttf", 24);
        if (font == NULL) {
            printf("TTF_OpenFont Error: %s\n", TTF_GetError());
            running = 0;
            continue;
        }
        SDL_Surface *titleSurface = TTF_RenderText_Solid(font, "Game of Life Menu", white);
        SDL_Texture *titleTexture = SDL_CreateTextureFromSurface(ren, titleSurface);
        SDL_Rect titleRect = {200, 20, titleSurface->w, titleSurface->h};
        SDL_RenderCopy(ren, titleTexture, NULL, &titleRect);
        SDL_FreeSurface(titleSurface);
        SDL_DestroyTexture(titleTexture);

        // Dessiner les rectangles avec noms, images et informations
        for (int row = 0; row < 2; ++row) {
            for (int col = 0; col < 3; ++col) {
            SDL_Rect rect = {100 + col * 150, 100 + row * 150, 100, 100};
            SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
            SDL_RenderDrawRect(ren, &rect);

            // Dessiner le nom
            char name[20];
            sprintf(name, "Option %d", row * 3 + col + 1);
            SDL_Surface *nameSurface = TTF_RenderText_Solid(font, name, white);
            SDL_Texture *nameTexture = SDL_CreateTextureFromSurface(ren, nameSurface);
            SDL_Rect nameRect = {rect.x + 10, rect.y + 10, nameSurface->w, nameSurface->h};
            SDL_RenderCopy(ren, nameTexture, NULL, &nameRect);
            SDL_FreeSurface(nameSurface);
            SDL_DestroyTexture(nameTexture);

            // Dessiner l'image (placeholder)
            SDL_Rect imgRect = {rect.x + 10, rect.y + 40, 80, 40};
            SDL_SetRenderDrawColor(ren, 200, 200, 200, 255);
            SDL_RenderFillRect(ren, &imgRect);

            // Dessiner le petit 'i' pour information
            SDL_Surface *infoSurface = TTF_RenderText_Solid(font, "i", white);
            SDL_Texture *infoTexture = SDL_CreateTextureFromSurface(ren, infoSurface);
            SDL_Rect infoRect = {rect.x + 80, rect.y + 80, infoSurface->w, infoSurface->h};
            SDL_RenderCopy(ren, infoTexture, NULL, &infoRect);
            SDL_FreeSurface(infoSurface);
            SDL_DestroyTexture(infoTexture);
            }
        }

        TTF_CloseFont(font);

        // Afficher le rendu
        SDL_RenderPresent(ren);
    }


    // Libération des ressources
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(menu);
    SDL_Quit();

    return 0;
}