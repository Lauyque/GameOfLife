#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define SDL_MAIN_HANDLED
#include <SDL.h> // Erreur normal puisqu'on rajoute le chemin vers la librairie SDL avec le makefile
#include <SDL_ttf.h> // Erreur normal puisqu'on rajoute le chemin vers la librairie SDL avec le makefile
#include <SDL_image.h> // Erreur normal puisqu'on rajoute le chemin vers la librairie SDL avec le makefile
#include <SDL_mixer.h> // Erreur normal puisqu'on rajoute le chemin vers la librairie SDL avec le makefile

// Mes propres fichiers
#include "grille.h"
#include "menu.h"
#include "sauvegarde.h"
#include "choixGrille.h"
#include "jeu.h"

// Fonction qui affiche le menu
int lancementMenu()
{
    // Rediriger les erreurs vers un fichier
    freopen("error_menu_log.txt", "w", stderr);

    setlocale(LC_ALL, "fr_FR.UTF-8");
    // Variable pour la boucle principale
    int runningMenu = 1;
    // Variable pour verifier si la fenetre est minimisé
    int windowMinimized = 0;

    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    // Initialisation de SDL_ttf
    if (TTF_Init() == -1)
    {
        fprintf(stderr, "TTF_Init Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }
     // Initialisation de SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
    {
        fprintf(stderr, "Mix_OpenAudio Error: %s\n", Mix_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Charger la musique
    Mix_Music *music = Mix_LoadMUS("../assets/musics/Music1.mp3");
    if (!music)
    {
        fprintf(stderr, "Mix_LoadMUS Error: %s\n", Mix_GetError());
        //Mix_CloseAudio();
        Mix_FreeMusic(music);
        music = NULL;
        //TTF_Quit();
        //SDL_Quit();
        return 1;
    }

    // Jouer la musique
    if (Mix_PlayMusic(music, -1) == -1)
    {
        fprintf(stderr, "Mix_PlayMusic Error: %s\n", Mix_GetError());
        Mix_FreeMusic(music);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Récupérer la taille de l'écran
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    int largeurEcran = DM.w;
    int hauteurEcran = DM.h;

    // Chemin relatif vers le fichier de police
    const char *fontPathArial = "../assets/fonts/arial/arial.ttf";
    const char *fontPathVdub = "../assets/fonts/v-dub/v_dub.ttf";

    // Nom des options
    const char *nomOptions[6] = {
        "Glider",
        "2",
        "3",
        "Aléatoire",
        "SandBox",
        "Personnalisé"};

    // Description des options
    const char *descriptionOptions[6] = {
        "Le 'Glider' est le nom donner pour le plus petit model de cases de vie qui avance à l'infini",
        "coucou2",
        "coucou3",
        "Permet de commencer avec une grille aléatoire",
        "Permet de créer sa propre grille",
        "Permet d'importer un fichier personnalisé"
    };

    // Chemins des images
    const char *imagePaths[6] = {
        "../assets/images/Glider.bmp",
        "../assets/images/Glider.bmp",
        "../assets/images/Glider.bmp",
        "../assets/images/Aleatoire.bmp",
        "../assets/images/Sandbox.bmp",
        "../assets/images/Personnalise.bmp"};


    // Création de la fenêtre
    SDL_Window *menu = SDL_CreateWindow("Game of Life", 0, 0, largeurEcran, hauteurEcran, SDL_WINDOW_FULLSCREEN);
    if (menu == NULL)
    {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Définir l'icône de la fenêtre
    SDL_Surface *iconSurface = SDL_LoadBMP("../assets/images/Glider.bmp");
    if (iconSurface != NULL)
    {
        SDL_SetWindowIcon(menu, iconSurface);
        SDL_FreeSurface(iconSurface);
    }
    else
    {
        fprintf(stderr, "SDL_LoadBMP Error: %s\n", SDL_GetError());
    }

    // Création du renderer
    SDL_Renderer *ren = SDL_CreateRenderer(menu, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == NULL)
    {
        SDL_DestroyWindow(menu);
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Déclaration des fonts
    //SDL_Color black = {0, 0, 0, 255};
    SDL_Color white = {255, 255, 255, 255};
    TTF_Font *fontTitle = TTF_OpenFont(fontPathVdub, 24);
    TTF_Font *font = TTF_OpenFont(fontPathArial, 18);
    if (font == NULL || fontTitle == NULL)
    {
        printf("TTF_OpenFont Error: %s\n", TTF_GetError());
        runningMenu = 0;
    }


    // Afficher une image de fond avec sdl2 image
    SDL_Surface *background = IMG_Load("../assets/images/BG.png");
    if (background == NULL)
    {
        fprintf(stderr, "IMG_Load Error: %s\n", IMG_GetError());
        runningMenu = 0;
    }
    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(ren, background); // Créer une texture à partir de l'image
    if (backgroundTexture == NULL)
    {
        fprintf(stderr, "SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
        printf("- Impossible d'afficher l'image: %s\n", SDL_GetError());
        SDL_FreeSurface(background);
        runningMenu = 0;
    }



    // Boucle principale
    while (runningMenu)
    {
        // Affichage du bouton quitter
        // Je le place ici pour que je puisse appeller son rect dans la boucle des events juste après
        // Définir le rectangle du bouton quitter
        SDL_Rect quitterRect = {largeurEcran / 2 - 50, hauteurEcran - 50, 100, 50}; // Centré en bas

        // Gestion des événements de la fenêtre
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) 
            {
                runningMenu = 0;
            } 
            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int mouseX = e.button.x;
                int mouseY = e.button.y;

                if (mouseX >= quitterRect.x && mouseX <= quitterRect.x + quitterRect.w &&
                    mouseY >= quitterRect.y && mouseY <= quitterRect.y + quitterRect.h)
                {
                    runningMenu = 0;
                }
            } else if (e.type == SDL_WINDOWEVENT) {
                if (e.window.event == SDL_WINDOWEVENT_MINIMIZED) {
                    windowMinimized = 1;
                    printf("Fenetre minimisée\n");
                } else if (e.window.event == SDL_WINDOWEVENT_RESTORED) {
                    windowMinimized = 0;
                    printf("Fenetre restaurée\n");
                } else if (e.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
                    windowMinimized = 1;
                    printf("Fenetre perdue de focus\n");
                } else if (e.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
                    windowMinimized = 0;
                    printf("Fenetre regagnée de focus\n");
                }
            } else if (e.type == SDL_QUIT) {
                runningMenu = 0;
            }
        }
        if (windowMinimized == 0){
            // Effacer l'écran
            SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
            SDL_RenderClear(ren);

            // Appliquer la texture de fond
            SDL_RenderCopy(ren, backgroundTexture, NULL, NULL); // Copier la texture sur le renderer

            // Remplir le rectangle du bouton quitter avec une couleur
            // Rajouter 2 triangle sur les 2 cotés du rectangle
            // Triangle gauche
            SDL_Point triangleGauche[4] = {{quitterRect.x, quitterRect.y}, {quitterRect.x, quitterRect.y + 50}, {quitterRect.x -30, quitterRect.y + 50}, {quitterRect.x, quitterRect.y}};
            SDL_Point triangleDroite[4] = {{quitterRect.x + quitterRect.w, quitterRect.y}, {quitterRect.x + quitterRect.w, quitterRect.y + 50}, {(quitterRect.x + quitterRect.w) +30, quitterRect.y + 50}, {quitterRect.x + quitterRect.w, quitterRect.y}};
            SDL_SetRenderDrawColor(ren, 255, 0, 0, 255); // Rouge pour toutes les formes du quitter
            SDL_RenderDrawLines(ren, triangleGauche, 4);
            SDL_RenderDrawLines(ren, triangleDroite, 4);
            SDL_RenderFillRect(ren, &quitterRect);

            // Remplir le triangle gauche
            for (int y = triangleGauche[0].y; y <= triangleGauche[1].y; ++y) {
                int startX = triangleGauche[0].x;
                int endX = startX - (y - triangleGauche[0].y) * 30 / 50;
                SDL_RenderDrawLine(ren, startX, y, endX, y);
            }

            // Remplir le triangle droite
            for (int y = triangleDroite[0].y; y <= triangleDroite[1].y; ++y) {
                int startX = triangleDroite[0].x;
                int endX = startX + (y - triangleDroite[0].y) * 30 / 50;
                SDL_RenderDrawLine(ren, startX, y, endX, y);
            }

            // Afficher le texte "Quitter" centré dans le rectangle
            afficherTexte(ren, font, "Quitter", quitterRect.x + (quitterRect.w / 2) - 26, quitterRect.y + (quitterRect.h / 2) - 10, white);
            quitterRect.w += 20; // Permet d'agrandir la zone de clic en largeur
            quitterRect.h += 10; // Permet d'agrandir la zone de clic en hauteur
            quitterRect.x -= 10; // Permet de commencer la zone de clic plus à gauche
            quitterRect.y -= 5; // Permet de commencer la zone de clic plus en haut

            // Afficher le titre
            // Afficher le titre au debut de la fenetre pour recuperer la taille du texte
            SDL_Rect titleTextRect = afficherTexte(ren, fontTitle, "Game Of Life", ((largeurEcran/2)-(100)), 10,  white);
            SDL_Rect titleRect = {largeurEcran / 2 - 400, 0, 800, 55};
            // triangles pour le titre
            SDL_Point triangleGaucheTitle[4] = {{titleRect.x, titleRect.y}, {titleRect.x, titleRect.y + titleRect.h - 1}, {titleRect.x - 30, titleRect.y}, {titleRect.x, titleRect.y}};
            SDL_Point triangleDroiteTitle[4] = {{titleRect.x + titleRect.w, titleRect.y}, {titleRect.x + titleRect.w, titleRect.y + titleRect.h - 1}, {(titleRect.x + titleRect.w) + 30, titleRect.y}, {titleRect.x + titleRect.w, titleRect.y}};

            SDL_SetRenderDrawColor(ren, 0, 0, 0, 255); // Rouge pour toutes les formes du titre
            SDL_RenderDrawLines(ren, triangleGaucheTitle, 4);
            SDL_RenderDrawLines(ren, triangleDroiteTitle, 4);
            SDL_RenderFillRect(ren, &titleRect);

            // Remplir le triangle gauche
            for (int y = triangleGaucheTitle[0].y; y <= triangleGaucheTitle[1].y; ++y) {
                int startX = triangleGaucheTitle[0].x;
                int endX = startX - (triangleGaucheTitle[1].y - y) * 30 / 50;
                SDL_RenderDrawLine(ren, endX, y, startX, y);
            }

            // Remplir le triangle droite
            for (int y = triangleDroiteTitle[0].y; y <= triangleDroiteTitle[1].y; ++y) {
                int startX = triangleDroiteTitle[0].x;
                int endX = startX + (triangleDroiteTitle[1].y - y) * 30 / 50;
                SDL_RenderDrawLine(ren, startX, y, endX, y);
            }

            // Centrer le texte du titre par rapport à titleRect
            int titleTextWidth = titleTextRect.w;
            int titleTextHeight = titleTextRect.h;
            int titleTextX = titleRect.x + (titleRect.w - titleTextWidth) / 2;
            int titleTextY = titleRect.y + (titleRect.h - titleTextHeight) / 2;
            // Afficher le texte du titre centré
            titleTextRect = afficherTexte(ren, fontTitle, "Game Of Life", titleTextX, titleTextY, white);


            // Calculer les marges pour centrer les rectangles
            int marginX = (largeurEcran - (3 * 200 + 2 * 50)) / 2;
            int marginY = (hauteurEcran - (2 * 150 + 1 * 50)) / 2;

            // Afficher les rectangles
            for (int row = 0; row < 2; ++row)
            {
                for (int col = 0; col < 3; ++col)
                {
                    SDL_Rect rect = {marginX + col * (200 + 50), marginY + row * (150 + 50), 200, 150};
                    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255); // Blanc
                    SDL_RenderDrawRect(ren, &rect);

                    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255); // Noir
                    SDL_RenderFillRect(ren, &rect); // Remplir les rectangles

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
                        //printf("Afficher plus d'information pour l'option %s\n", nomOptions[row * 3 + col]);
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
                        //printf("Lancer pour l'option %s\n", nomOptions[row * 3 + col]);
                        // Si le choix est personnalisé, pas besoin de choisir une grille
                        if (strcmp(nomOptions[row * 3 + col], "Personnalisé") == 0)
                        {
                            //GrilleChaine* chargerGrilleChaine(*nomFichier);

                            // Minimiser la fenêtre SDL
                            SDL_MinimizeWindow(menu);

                            // Utiliser la fonction pour obtenir le chemin du fichier sélectionné
                            char* nomFichier = ouvrirExplorateurFichiers();

                            // Restaurer la fenêtre SDL
                            SDL_RestoreWindow(menu);
                            if (nomFichier) {
                                GrilleChaine* grille = chargerGrilleChaine(nomFichier);
                                //afficherGrilleChaine(grille);
                                free(nomFichier);

                                // Arrêter la musique actuelle
                                if (Mix_PlayingMusic()) {
                                    Mix_HaltMusic();
                                }
                                if (music) {
                                    Mix_FreeMusic(music);
                                    music = NULL;
                                }

                                if (grille) {
                                    if (lancementJeu(ren, font, font, white, "Partie Personnalisé", grille) != 0) {
                                        fprintf(stderr, "Erreur lors du lancement du jeu chargé\n");
                                        runningMenu = 0;
                                    }
                                    libererGrilleChaine(grille);

                                    // Relancer la musique
                                    music = Mix_LoadMUS("assets/musics/Music1.mp3");
                                    if (!music) {
                                        printf("Mix_LoadMUS Error: %s\n", Mix_GetError());
                                    } else {
                                        if (Mix_PlayMusic(music, -1) == -1) {
                                            printf("Mix_PlayMusic Error: %s\n", Mix_GetError());
                                        }
                                    }
                                }
                            } else {
                                printf("Aucun fichier sélectionné.\n");
                            }

                        // Si c'est pas une partie personnalisée
                        } else {

                            // Pour toutes les autres options, on doit choisir une grille
                            GrilleChaine* grille = lancementChoixGrille(ren, fontTitle, font, white);

                            // Vérification de la grille
                            if (grille->dernier->tailleX != 0 || grille->dernier->tailleY != 0)
                            {
                                // Arrêter la musique actuelle
                                Mix_HaltMusic();
                                Mix_FreeMusic(music);
                                
                                //printf("Lancement du jeu pour l'option %s\n", nomOptions[row * 3 + col]);
                                // Condition si le choix est "aleatoire"
                                if (strcmp(nomOptions[row * 3 + col], nomOptions[3]) == 0) { // Lancement de l'option aléatoire
                                    srand(time(NULL));
                                    for (int i = 0; i < grille->dernier->tailleY; i++) {
                                        for (int j = 0; j < grille->dernier->tailleX; j++) {
                                            grille->dernier->listePointeursLignes[i][j] = rand() % 2;
                                            //printf("Alléatoire pour la case : x = %d y = %d\n", j,i);
                                        }
                                    }
                                } else if (strcmp(nomOptions[row * 3 + col], nomOptions[4]) == 0) { // Lancement du sandbox
                                    //void lancementSandBox(SDL_Renderer *ren, TTF_Font *fontTitle, TTF_Font *font, SDL_Color color, Grille *grille); // A faire si on a le temps
                                } else if (grille->dernier->tailleX > 3 && grille->dernier->tailleY > 3) {
                                    grille->dernier->listePointeursLignes[2][1] = 1;
                                    grille->dernier->listePointeursLignes[3][2] = 1;
                                    grille->dernier->listePointeursLignes[1][3] = 1;
                                    grille->dernier->listePointeursLignes[2][3] = 1;
                                    grille->dernier->listePointeursLignes[3][3] = 1;
                                } else {
                                    fprintf(stderr, "Erreur: taille de la grille insuffisante\n");
                                    runningMenu = 0;
                                }
                                if (lancementJeu(ren, fontTitle, font, white, nomOptions[row * 3 + col], grille) != 0)
                                {
                                    fprintf(stderr, "Erreur lors du lancement du jeu\n");
                                    runningMenu = 0;
                                }
                                //libererGrilleChaine(grille);
                                // Relancer la musique
                                music = Mix_LoadMUS("assets/musics/Music1.mp3");
                                if (!music) {
                                    printf("Mix_LoadMUS Error: %s\n", Mix_GetError());
                                } else {
                                    Mix_PlayMusic(music, -1);
                                }
                            } else {
                                libererGrilleChaine(grille);
                                printf("Erreur lors du choix de la grille\n");
                            }
                        }
                        
                    }
                }
            }

            // Afficher le rendu
            SDL_RenderPresent(ren);
        }
    }

    // Libération de la mémoire
    SDL_DestroyTexture(backgroundTexture);
    SDL_FreeSurface(background);
    IMG_Quit();
    // si la musique tourne
    if (Mix_PlayingMusic()) {
        Mix_FreeMusic(music);
        Mix_CloseAudio();
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
    if (ren == NULL || font == NULL || texte == NULL)
    {
        printf("Erreur: paramètre manquant\n");
        return (SDL_Rect){0, 0, 0, 0};
    }

    // Dessiner le texte
    SDL_Surface *textSurface = TTF_RenderUTF8_Solid(font, texte, color);
    if (textSurface == NULL) {
        // Normal puisque le texte est vide
        //printf("TTF_RenderText_Solid Error: %s\n", TTF_GetError());
        return (SDL_Rect){0, 0, 0, 0};
    }
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(ren, textSurface);
    if (textTexture == NULL) {
        printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
        printf("- Impossible d'afficher le texte : %s\n", texte);
        SDL_FreeSurface(textSurface);
        return (SDL_Rect){0, 0, 0, 0};
    }

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
    SDL_Surface *nameSurface = TTF_RenderUTF8_Solid(font, nom, color);
    if (nameSurface == NULL) {
        printf("TTF_RenderText_Solid Error: %s\n", TTF_GetError());
        return (SDL_Rect){0, 0, 0, 0};
    }
    SDL_Texture *nameTexture = SDL_CreateTextureFromSurface(ren, nameSurface);
    if (nameTexture == NULL) {
        printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
        printf("- Impossible d'afficher le nom\n");
        SDL_FreeSurface(nameSurface);
        return (SDL_Rect){0, 0, 0, 0};
    }

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
    if (imgTexture == NULL) {
        printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
        printf("- Impossible d'afficher l'image'\n");
        SDL_FreeSurface(imgSurface);
        return rect;
    }

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
        afficherTexte(ren, fontTitle, nom, 10, 20, color);

        // Afficher l'image
        //prochainement

        // Afficher la description
        afficherTexte(ren, font, description, 300, 200, color);

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