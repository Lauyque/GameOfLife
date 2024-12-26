#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define SDL_MAIN_HANDLED
#include <SDL.h> // Erreur normal puisqu'on rajoute le chemin vers la librairie SDL avec le makefile
#include <SDL_TTF.h>

//#include <getcwd()\dependancy\SDL2\SDL2-2.30.10\include\SDL.h>
//#include <dependancy\SDL2_TTF\SDL2_ttf-2.22.0\include\SDL_ttf.h>


// PROTOTYPES
int lancementMenu();
//void afficherPlusInformation();

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

    // Chemin relatif vers le fichier de police
    const char* fontPathArial = "assets/fonts/arial/arial.ttf";
    const char* fontPathVdub = "assets/fonts/v-dub/v_dub.ttf";

    // Variable avec les tailles de la fenêtre
    int windowWidth = 1000;
    int windowHeight = 600;

    // Création de la fenêtre
    SDL_Window *menu = SDL_CreateWindow("Game of Life", 100, 100, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
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

        // Déclaration des fonts
        SDL_Color white = {255, 255, 255, 255};
        TTF_Font *fontTitle = TTF_OpenFont(fontPathVdub, 24);
        TTF_Font *font = TTF_OpenFont(fontPathArial, 18);
        if (font == NULL || fontTitle == NULL) {
            printf("TTF_OpenFont Error: %s\n", TTF_GetError());
            running = 0;
            continue;
        }

        // Dessiner le titre
        SDL_Surface *titleSurface = TTF_RenderText_Solid(fontTitle, "Game of Life", white);
        SDL_Texture *titleTexture = SDL_CreateTextureFromSurface(ren, titleSurface);
        SDL_Rect titleRect = {(windowWidth - titleSurface->w) / 2, 20, titleSurface->w, titleSurface->h};
        SDL_RenderCopy(ren, titleTexture, NULL, &titleRect);
        SDL_FreeSurface(titleSurface);
        SDL_DestroyTexture(titleTexture);

        // Dessiner les rectangles avec noms, images et informations
        for (int row = 0; row < 2; ++row) {
            for (int col = 0; col < 3; ++col) {
            SDL_Rect rect = {100 + col * 250, 150 + row * 200, 200, 150};
            SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
            SDL_RenderDrawRect(ren, &rect);

            // Dessiner le nom
            char name[20];
            sprintf(name, "Option %d", row * 3 + col + 1);
            SDL_Surface *nameSurface = TTF_RenderText_Solid(font, name, white);
            SDL_Texture *nameTexture = SDL_CreateTextureFromSurface(ren, nameSurface);
            SDL_Rect nameRect = {rect.x + (rect.w - nameSurface->w) / 2, rect.y + 10, nameSurface->w, nameSurface->h};
            SDL_RenderCopy(ren, nameTexture, NULL, &nameRect);
            SDL_FreeSurface(nameSurface);
            SDL_DestroyTexture(nameTexture);

            // Dessiner l'image (placeholder)
            SDL_Rect imgRect = {rect.x + (rect.w - 80) / 2, rect.y + 40, 80, 40};
            SDL_SetRenderDrawColor(ren, 200, 200, 200, 255);
            SDL_RenderFillRect(ren, &imgRect);

            // Dessiner "plus d'information"
            const char* infoText = "plus d'information";
            SDL_Surface *infoSurface = TTF_RenderText_Solid(font, infoText, white);
            SDL_Texture *infoTexture = SDL_CreateTextureFromSurface(ren, infoSurface);
            SDL_Rect infoRect = {rect.x + (rect.w - infoSurface->w) / 2, rect.y + 90, infoSurface->w, infoSurface->h};
            SDL_RenderCopy(ren, infoTexture, NULL, &infoRect);
            SDL_FreeSurface(infoSurface);
            SDL_DestroyTexture(infoTexture);

            // Vérifier si on clique sur "plus d'information"
            int mouseX, mouseY;
            Uint32 buttons = SDL_GetMouseState(&mouseX, &mouseY);
            if ((buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) && 
                mouseX >= infoRect.x && mouseX <= infoRect.x + infoRect.w &&
                mouseY >= infoRect.y && mouseY <= infoRect.y + infoRect.h) {
                // Appeler la fonction pour afficher une autre page
                //afficherPlusInformation();
                printf("Afficher plus d'information\n");
                sleep(1);
            }
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