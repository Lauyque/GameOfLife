#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define SDL_MAIN_HANDLED
#include <SDL.h> // Erreur normal puisqu'on rajoute le chemin vers la librairie SDL avec le makefile
#include <SDL_ttf.h>

// #include <getcwd()\dependancy\SDL2\SDL2-2.30.10\include\SDL.h>
// #include <dependancy\SDL2_TTF\SDL2_ttf-2.22.0\include\SDL_ttf.h>

// PROTOTYPES
int lancementMenu();
SDL_Rect afficherTexte(SDL_Renderer *ren, TTF_Font *font, const char *texte, int posX, int posY, SDL_Color color);
SDL_Rect afficherPlusInformation(SDL_Renderer *ren, TTF_Font *font, SDL_Rect rect, SDL_Color color, char *Text);
SDL_Rect afficherImage(SDL_Renderer *ren, const char *imagePath, SDL_Rect rect);
SDL_Rect afficherNom(SDL_Renderer *ren, TTF_Font *font, SDL_Rect rect, const char *nom, SDL_Color color);

int lancementPlusInformation(SDL_Renderer *ren, TTF_Font *fontTitle, TTF_Font *font, SDL_Color color, const char *nom, const char *description);

// Fonction qui affiche le menu
int lancementMenu()
{
    // Variable pour la boucle principale
    int runningMenu = 1;

    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    // Initialisation de SDL_ttf
    if (TTF_Init() == -1)
    {
        printf("TTF_Init Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Récupérer la taille de l'écran
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    int largeurEcran = DM.w;
    int hauteurEcran = DM.h;

    // Chemin relatif vers le fichier de police
    const char *fontPathArial = "assets/fonts/arial/arial.ttf";
    const char *fontPathVdub = "assets/fonts/v-dub/v_dub.ttf";

    // Nom des options
    const char *nomOptions[6] = {
        "Glider",
        "2",
        "3",
        "4",
        "5",
        "6"};

    // Description des options
    const char *descriptionOptions[6] = {
        "Le 'Glider' est le nom donner pour le plus petit model de cases de vie qui avance à l'infini",
        "coucou2",
        "coucou3",
        "coucou4",
        "coucou5",
        "coucou6",
    };

    // Chemins des images
    const char *imagePaths[6] = {
        "assets/images/Glider.bmp",
        "assets/images/Glider.bmp",
        "assets/images/Glider.bmp",
        "assets/images/Glider.bmp",
        "assets/images/Glider.bmp",
        "assets/images/Glider.bmp"};


    // Création de la fenêtre
    SDL_Window *menu = SDL_CreateWindow("Game of Life", 0, 0, largeurEcran, hauteurEcran, SDL_WINDOW_FULLSCREEN);
    if (menu == NULL)
    {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Définir l'icône de la fenêtre
    SDL_Surface *iconSurface = SDL_LoadBMP("assets/images/Glider.bmp");
    if (iconSurface != NULL)
    {
        SDL_SetWindowIcon(menu, iconSurface);
        SDL_FreeSurface(iconSurface);
    }
    else
    {
        printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    }

    // Création du renderer
    SDL_Renderer *ren = SDL_CreateRenderer(menu, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == NULL)
    {
        SDL_DestroyWindow(menu);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Déclaration des fonts
    SDL_Color black = {255, 255, 255, 255};
    SDL_Color white = {255, 255, 255, 255};
    TTF_Font *fontTitle = TTF_OpenFont(fontPathVdub, 24);
    TTF_Font *font = TTF_OpenFont(fontPathArial, 18);
    if (font == NULL || fontTitle == NULL)
    {
        printf("TTF_OpenFont Error: %s\n", TTF_GetError());
        runningMenu = 0;
    }

    // Boucle principale
    while (runningMenu)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                runningMenu = 0;
            }
        }

        // Effacer l'écran
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        // Afficher le titre
        SDL_Rect titleRect = afficherTexte(ren, fontTitle, "Game Of Life", ((largeurEcran/2)-(100)), 10,  white);

        // Calculer les marges pour centrer les rectangles
        int marginX = (largeurEcran - (3 * 200 + 2 * 50)) / 2;
        int marginY = (hauteurEcran - (2 * 150 + 1 * 50)) / 2;

        // Afficher les rectangles
        for (int row = 0; row < 2; ++row)
        {
            for (int col = 0; col < 3; ++col)
            {
                SDL_Rect rect = {marginX + col * (200 + 50), marginY + row * (150 + 50), 200, 150};
                SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
                SDL_RenderDrawRect(ren, &rect);

                // Dessiner le nom
                SDL_Rect nameRect = afficherNom(ren, font, rect, nomOptions[row * 3 + col], white);
                // Dessiner l'image
                SDL_Rect imgRect = afficherImage(ren, imagePaths[row * 3 + col], rect);
                // Dessiner "plus d'information"
                SDL_Rect infoRect = afficherTexte(ren, font, "Plus d'information", rect.x + (rect.w/4+10) / 2, rect.y + 120, white);

                // Vérifier si on clique sur "plus d'information"
                int mouseX, mouseY;
                Uint32 buttons = SDL_GetMouseState(&mouseX, &mouseY);
                if ((buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) &&
                    mouseX >= infoRect.x && mouseX <= infoRect.x + infoRect.w &&
                    mouseY >= infoRect.y && mouseY <= infoRect.y + infoRect.h)
                {
                    printf("Afficher plus d'information pour l'option %s\n", nomOptions[row * 3 + col]);
                    if (lancementPlusInformation(ren, fontTitle, font, white, nomOptions[row * 3 + col], descriptionOptions[row * 3 + col]) != 0)
                    {
                        fprintf(stderr, "Erreur lors du lancement de plus d'information\n");
                        runningMenu = 0;
                    }
                }
                // Vérifier si on clique sur le nom ou l'image
                if ((buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) &&
                    ((mouseX >= nameRect.x && mouseX <= nameRect.x + nameRect.w &&
                      mouseY >= nameRect.y && mouseY <= nameRect.y + nameRect.h) ||
                     (mouseX >= imgRect.x && mouseX <= imgRect.x + imgRect.w &&
                      mouseY >= imgRect.y && mouseY <= imgRect.y + imgRect.h)))
                {
                    printf("Lancer pour l'option %s\n", nomOptions[row * 3 + col]);
                    sleep(1);
                }
            }
        }

        // Afficher le rendu
        SDL_RenderPresent(ren);
    }

    TTF_CloseFont(font);
    TTF_CloseFont(fontTitle);
    TTF_Quit();
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(menu);
    SDL_Quit();

    return 0;
}

// fonction qui affiche le texte
SDL_Rect afficherTexte(SDL_Renderer *ren, TTF_Font *font, const char *texte, int posX, int posY, SDL_Color color)
{
    // Dessiner le texte
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, texte, color);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(ren, textSurface);

    SDL_Rect textRect = {posX, posY, textSurface->w, textSurface->h};
    SDL_RenderCopy(ren, textTexture, NULL, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    return textRect;
}

// fonction qui affiche le nom
SDL_Rect afficherNom(SDL_Renderer *ren, TTF_Font *font, SDL_Rect rect, const char *nom, SDL_Color color)
{
    // Dessiner le nom
    SDL_Surface *nameSurface = TTF_RenderText_Solid(font, nom, color);
    SDL_Texture *nameTexture = SDL_CreateTextureFromSurface(ren, nameSurface);
    SDL_Rect nameRect = {rect.x + (rect.w - nameSurface->w) / 2, rect.y + 10, nameSurface->w, nameSurface->h};
    SDL_RenderCopy(ren, nameTexture, NULL, &nameRect);
    SDL_FreeSurface(nameSurface);
    SDL_DestroyTexture(nameTexture);

    return nameRect;
}

// fonction qui affiche l'image
SDL_Rect afficherImage(SDL_Renderer *ren, const char *imagePath, SDL_Rect rect)
{
    // Charger l'image
    SDL_Surface *imgSurface = SDL_LoadBMP(imagePath);
    if (imgSurface == NULL)
    {
        printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
        return rect;
    }
    SDL_Texture *imgTexture = SDL_CreateTextureFromSurface(ren, imgSurface);

    // Redimensionner l'image si elle est trop grande
    int maxWidth = rect.w - 20;
    int maxHeight = rect.h - 60;
    int imgWidth = imgSurface->w;
    int imgHeight = imgSurface->h;

    if (imgWidth > maxWidth)
    {
        imgHeight = (imgHeight * maxWidth) / imgWidth;
        imgWidth = maxWidth;
    }
    if (imgHeight > maxHeight)
    {
        imgWidth = (imgWidth * maxHeight) / imgHeight;
        imgHeight = maxHeight;
    }

    SDL_Rect imgRect = {rect.x + (rect.w - imgWidth) / 2, rect.y + 30, imgWidth, imgHeight};
    SDL_RenderCopy(ren, imgTexture, NULL, &imgRect);
    SDL_FreeSurface(imgSurface);
    SDL_DestroyTexture(imgTexture);

    return imgRect;
}

// AUTRES PAGES
int lancementPlusInformation(SDL_Renderer *ren, TTF_Font *fontTitle, TTF_Font *font, SDL_Color color, const char *nom, const char *description)
{
    // Variable pour la boucle principale
    int runningPlusInformation = 1;

    while(runningPlusInformation == 1){

                // Effacer l'écran
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        // Afficher le titre
        SDL_Rect titleRect = afficherTexte(ren, fontTitle, nom, 10, 20, color);

        // Afficher l'image
        //prochainement

        // Afficher la description
        SDL_Rect descriptionRect = afficherTexte(ren, font, description, 300, 200, color);

        // Afficher le bouton "Retour"
        SDL_Rect retourRect = afficherTexte(ren, font, "Retour", 500, 400, color);
        SDL_RenderDrawRect(ren, &retourRect);



        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                runningPlusInformation = 0;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
            {
                int mouseX = e.button.x;
                int mouseY = e.button.y;
                if (mouseX >= retourRect.x && mouseX <= retourRect.x + retourRect.w &&
                    mouseY >= retourRect.y && mouseY <= retourRect.y + retourRect.h)
                {
                    runningPlusInformation = 0;
                }
            }
        }

        // Afficher le rendu
        SDL_RenderPresent(ren);
    }
    
    return 0;
}
