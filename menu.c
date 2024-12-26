#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>
#define SDL_MAIN_HANDLED
#include <C:\SDL2\SDL2-devel-2.30.10-VC\SDL2-2.30.10\include\SDL.h>

// PROTOTYPES
void lancementMenu();

// Fonction qui affiche le menu
void lancementMenu(){
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        exit(1);
    }

    // Création de la fenêtre
    SDL_Window *win = SDL_CreateWindow("Game of Life", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    // Création du renderer
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == NULL) {
        SDL_DestroyWindow(win);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
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

        // Dessiner une grille (exemple simple)
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        for (int i = 0; i < 640; i += 20) {
            SDL_RenderDrawLine(ren, i, 0, i, 480);
        }
        for (int j = 0; j < 480; j += 20) {
            SDL_RenderDrawLine(ren, 0, j, 640, j);
        }

        // Afficher le rendu
        SDL_RenderPresent(ren);
    }


    // Libération des ressources
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    exit(0);
}
