#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>

#define SDL_MAIN_HANDLED
#include <SDL.h> // Erreur normal puisqu'on rajoute le chemin vers la librairie SDL avec le makefile
#include <SDL_ttf.h> // Erreur normal puisqu'on rajoute le chemin vers la librairie SDL avec le makefile
#include <SDL_image.h> // Erreur normal puisqu'on rajoute le chemin vers la librairie SDL avec le makefile
#include <SDL_mixer.h> // Erreur normal puisqu'on rajoute le chemin vers la librairie SDL avec le makefile

// #include <getcwd()\dependancy\SDL2\SDL2-2.30.10\include\SDL.h>
// #include <dependancy\SDL2_TTF\SDL2_ttf-2.22.0\include\SDL_ttf.h>

// Mes propres fichiers
#include "grille.c"
#include "caseVie.c"
#include "jeu.c"

// PROTOTYPES
int lancementMenu();
SDL_Rect afficherTexte(SDL_Renderer *ren, TTF_Font *font, const char *texte, int posX, int posY, SDL_Color color);
SDL_Rect afficherImage(SDL_Renderer *ren, const char *imagePath, SDL_Rect rect);
SDL_Rect afficherNom(SDL_Renderer *ren, TTF_Font *font, SDL_Rect rect, const char *nom, SDL_Color color);

int lancementPlusInformation(SDL_Renderer *ren, TTF_Font *fontTitle, TTF_Font *font, SDL_Color color, const char *nom, const char *description);
Grille lancementChoixGrille(SDL_Renderer *ren, TTF_Font *fontTitle, TTF_Font *font, SDL_Color color, const char *nom);
void lectureTextInput(SDL_Renderer *ren, TTF_Font *font, SDL_Color color, SDL_Rect inputRect, char *inputText, int maxLength);

// IDEE d'avancement.
// -> Faire une liste de tout les pointeurs de toutes nos varaibles et mettre en parametre de nos fonctions
//    pour pouvoir les modifier directement et avoir accès à toutes les variables.
// -> Ajouter une librairie de video ou gif pour mettre des animations dans le menu
// -> Ajouter une librairie pour des sons


// Fonction qui affiche le menu
int lancementMenu()
{
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
        printf("TTF_Init Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }
     // Initialisation de SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
    {
        printf("Mix_OpenAudio Error: %s\n", Mix_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Charger la musique
    Mix_Music *music = Mix_LoadMUS("assets/musics/Music1.mp3");
    if (!music)
    {
        printf("Mix_LoadMUS Error: %s\n", Mix_GetError());
        //Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Jouer la musique
    if (Mix_PlayMusic(music, -1) == -1)
    {
        printf("Mix_PlayMusic Error: %s\n", Mix_GetError());
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
    const char *fontPathArial = "assets/fonts/arial/arial.ttf";
    const char *fontPathVdub = "assets/fonts/v-dub/v_dub.ttf";

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
        "assets/images/Glider.bmp",
        "assets/images/Glider.bmp",
        "assets/images/Glider.bmp",
        "assets/images/Aleatoire.bmp",
        "assets/images/Sandbox.bmp",
        "assets/images/Personnalise.bmp"};


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
    SDL_Color black = {0, 0, 0, 255};
    SDL_Color white = {255, 255, 255, 255};
    TTF_Font *fontTitle = TTF_OpenFont(fontPathVdub, 24);
    TTF_Font *font = TTF_OpenFont(fontPathArial, 18);
    if (font == NULL || fontTitle == NULL)
    {
        printf("TTF_OpenFont Error: %s\n", TTF_GetError());
        runningMenu = 0;
    }


    // Afficher une image de fond avec sdl2 image
    SDL_Surface *background = IMG_Load("assets/images/Proposition_Fond-Ecran-Menu2.png");
    if (background == NULL)
    {
        printf("IMG_Load Error: %s\n", IMG_GetError());
        runningMenu = 0;
    }
    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(ren, background); // Créer une texture à partir de l'image
    if (backgroundTexture == NULL)
    {
        printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
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
            if (e.type == SDL_QUIT) {
                    runningMenu = 0;
            } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
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
            SDL_Rect quitterTextRect = afficherTexte(ren, font, "Quitter", quitterRect.x + (quitterRect.w / 2) - 26, quitterRect.y + (quitterRect.h / 2) - 10, white);
            quitterRect.w += 20; // Permet d'agrandir la zone de clic en largeur
            quitterRect.h += 10; // Permet d'agrandir la zone de clic en hauteur
            quitterRect.x -= 10; // Permet de commencer la zone de clic plus à gauche
            quitterRect.y -= 5; // Permet de commencer la zone de clic plus en haut
            //SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
            //SDL_RenderDrawRect(ren, &quitterRect);

            SDL_Event e;
            while (SDL_PollEvent(&e))
            {
                if (e.type == SDL_QUIT)
                {
                    runningMenu = 0;
                } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
                {
                    int mouseX = e.button.x;
                    int mouseY = e.button.y;
                    if (mouseX >= quitterRect.x && mouseX <= quitterRect.x + quitterRect.w &&
                        mouseY >= quitterRect.y && mouseY <= quitterRect.y + quitterRect.h)
                    {
                        runningMenu = 0;
                    }
                }
            }

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
                            //Grille grille = lancementSave(ren, fontTitle, font, white, nomOptions[row * 3 + col]); // Lancement de la partie sauvegardée
                            //if (lancementJeu(ren, fontTitle, font, white, nomOptions[row * 3 + col], &grille) != 0)
                            //{
                                //runningMenu = 0;
                            //}
                            //libererGrille(&grille);
                            // Sortir de la de la gestion du clic
                            break;
                        }

                        // Pour toutes les autres options, on doit choisir une grille
                        Grille grille = lancementChoixGrille(ren, fontTitle, font, white, nomOptions[row * 3 + col]);

                        // Vérification de la grille
                        if (grille.tailleX != 0 || grille.tailleY != 0)
                        {
                            //printf("Lancement du jeu pour l'option %s\n", nomOptions[row * 3 + col]);
                            // Condition si le choix est "aleatoire"
                            if (strcmp(nomOptions[row * 3 + col], nomOptions[3]) == 0)
                            {
                                srand(time(NULL));
                                for (int i = 0; i < grille.tailleX; i++)
                                {
                                    for (int j = 0; j < grille.tailleY; j++)
                                    {
                                        grille.listePointeursLignes[i][j] = rand() % 2;
                                    }
                                }
                            } else if (strcmp(nomOptions[row * 3 + col], nomOptions[4]) == 0) { // Lancement du sandbox
                                //void lancementSandBox(SDL_Renderer *ren, TTF_Font *fontTitle, TTF_Font *font, SDL_Color color, Grille *grille); // A faire si on a le temps
                            } else if (grille.tailleX > 3 && grille.tailleY > 3) {
                                grille.listePointeursLignes[2][1] = 1;
                                grille.listePointeursLignes[3][2] = 1;
                                grille.listePointeursLignes[1][3] = 1;
                                grille.listePointeursLignes[2][3] = 1;
                                grille.listePointeursLignes[3][3] = 1;
                            } else {
                                fprintf(stderr, "Erreur: taille de la grille insuffisante\n");
                                runningMenu = 0;
                            }
                            if (lancementJeu(ren, fontTitle, font, white, nomOptions[row * 3 + col], &grille) != 0)
                            {
                                fprintf(stderr, "Erreur lors du lancement du jeu\n");
                                runningMenu = 0;
                            }
                            libererGrille(&grille);
                        } else {
                            libererGrille(&grille);
                            printf("Erreur lors du choix de la grille\n");
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
    IMG_Quit();
    Mix_CloseAudio();
    Mix_FreeMusic(music);
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
        printf("- Impossible d'afficher le texte\n");
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


// Fonction qui affiche le choix de la taille de la grille
Grille lancementChoixGrille(SDL_Renderer *ren, TTF_Font *fontTitle, TTF_Font *font, SDL_Color color, const char *nom) {
    int runningChoixGrille = 1;
    char inputText[100] = "";

    // Récupérer la taille de l'écran
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    int largeurEcran = DM.w;
    int hauteurEcran = DM.h;

    while (runningChoixGrille == 1) {
        // Effacer l'écran
        SDL_SetRenderDrawColor(ren, 100, 100, 100, 255);
        SDL_RenderClear(ren);

        // Afficher le titre
        // Afficher le titre au debut de la fenetre pour recuperer la taille du texte
        SDL_Rect titleTextRect = afficherTexte(ren, fontTitle, "Choisissez la taille de votre grille", ((largeurEcran/2)-(400)), 10,  color);
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
        titleTextRect = afficherTexte(ren, fontTitle, "Choisissez la taille de votre grille", titleTextX, titleTextY, color);

        // Zone de saisie de texte
        SDL_Rect saisieRect = {(largeurEcran /2) - 250, 250, 500, 30};
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255); // Couleur noir
        //SDL_RenderDrawRect(ren, &saisieRect); // Dessiner le rectangle
        SDL_RenderFillRect(ren, &saisieRect); // Dessiner le rectangle
        //SDL_SetRenderDrawColor(ren, 255, 255, 255, 255); // Couleur blanche
        afficherTexte(ren, font, inputText, saisieRect.x + 5, saisieRect.y + 5, color);

        // Afficher les boutons avec 4 choix prédéfinis
        // Premier affichage du text pour avoir ces dimenssions
        SDL_Rect bouton1TextRect = afficherTexte(ren, font, "10*10", (largeurEcran /2) - 250, 350, color); // à gauche
        SDL_Rect bouton2TextRect = afficherTexte(ren, font, "20*20", (largeurEcran /2) - 250, 350, color);
        SDL_Rect bouton3TextRect = afficherTexte(ren, font, "25*25", (largeurEcran /2) - 250, 450, color); // à droite
        SDL_Rect bouton4TextRect = afficherTexte(ren, font, "80*40", (largeurEcran /2) - 250, 450, color);
        SDL_Rect bouton5TextRect = afficherTexte(ren, font, "15*15", (largeurEcran /2) - 250, 350, color); // au centre
        SDL_Rect bouton6TextRect = afficherTexte(ren, font, "50*50", (largeurEcran /2) - 250, 450, color);
        // Création des rectangles
        SDL_Rect bouton1Rect = {bouton1TextRect.x, bouton1TextRect.y, bouton1TextRect.w + 40, bouton1TextRect.h + 20};
        SDL_Rect bouton2Rect = {(largeurEcran /2) + saisieRect.w/2 - (bouton2TextRect.w + 40), bouton2TextRect.y, bouton2TextRect.w + 40, bouton2TextRect.h + 20};
        SDL_Rect bouton3Rect = {bouton3TextRect.x, bouton3TextRect.y, bouton3TextRect.w + 40, bouton3TextRect.h + 20};
        SDL_Rect bouton4Rect = {(largeurEcran /2) + saisieRect.w/2 - (bouton4TextRect.w + 40), bouton4TextRect.y, bouton4TextRect.w + 40, bouton4TextRect.h + 20};
        SDL_Rect bouton5Rect = {(largeurEcran /2) - (bouton5TextRect.w/2 + 20), bouton5TextRect.y, bouton5TextRect.w + 40, bouton5TextRect.h + 20};
        SDL_Rect bouton6Rect = {(largeurEcran /2) - (bouton6TextRect.w/2 + 20), bouton6TextRect.y, bouton6TextRect.w + 40, bouton6TextRect.h + 20};
        // Remplir les rectangles
        SDL_RenderFillRect(ren, &bouton1Rect);
        SDL_RenderFillRect(ren, &bouton2Rect);
        SDL_RenderFillRect(ren, &bouton3Rect);
        SDL_RenderFillRect(ren, &bouton4Rect);
        SDL_RenderFillRect(ren, &bouton5Rect);
        SDL_RenderFillRect(ren, &bouton6Rect);
        // Afficher les textes
        bouton1TextRect = afficherTexte(ren, font, "10*10", bouton1Rect.x + ((bouton1Rect.w/2)-bouton1TextRect.w/2), bouton1Rect.y + ((bouton1Rect.h/2)-(bouton1TextRect.h/2)), color);
        bouton2TextRect = afficherTexte(ren, font, "20*20", bouton2Rect.x + ((bouton2Rect.w/2)-bouton2TextRect.w/2), bouton2Rect.y + ((bouton2Rect.h/2)-(bouton2TextRect.h/2)), color);
        bouton3TextRect = afficherTexte(ren, font, "25*25", bouton3Rect.x + ((bouton3Rect.w/2)-bouton3TextRect.w/2), bouton3Rect.y + ((bouton3Rect.h/2)-(bouton3TextRect.h/2)), color);
        bouton4TextRect = afficherTexte(ren, font, "80*40", bouton4Rect.x + ((bouton4Rect.w/2)-bouton4TextRect.w/2), bouton4Rect.y + ((bouton4Rect.h/2)-(bouton4TextRect.h/2)), color);
        bouton5TextRect = afficherTexte(ren, font, "15*15", bouton5Rect.x + ((bouton5Rect.w/2)-bouton5TextRect.w/2), bouton5Rect.y + ((bouton5Rect.h/2)-(bouton5TextRect.h/2)), color);
        bouton6TextRect = afficherTexte(ren, font, "50*50", bouton6Rect.x + ((bouton6Rect.w/2)-bouton6TextRect.w/2), bouton6Rect.y + ((bouton6Rect.h/2)-(bouton6TextRect.h/2)), color);

        // Afficher le bouton "Jouer"
        SDL_Rect jouerTextRect = afficherTexte(ren, font, "Jouer", (largeurEcran /2) - 200, 550, color);
        SDL_Rect jouerRect = {jouerTextRect.x, jouerTextRect.y, 400, 40};
        SDL_SetRenderDrawColor(ren, 0, 255, 0, 255); // Vert
        SDL_RenderFillRect(ren, &jouerRect);
        jouerTextRect = afficherTexte(ren, font, "Jouer", (largeurEcran /2) - (jouerTextRect.w/2), (jouerTextRect.y) + (jouerTextRect.h/2), color);

        SDL_Rect retourTextRect = afficherTexte(ren, font, "Retour", (largeurEcran /2) - 200, 600, color);
        SDL_Rect retourRect = {retourTextRect.x, retourTextRect.y, 400, 40};
        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255); // Rouge
        SDL_RenderFillRect(ren, &retourRect);
        retourTextRect = afficherTexte(ren, font, "Retour", (largeurEcran /2) - (retourTextRect.w/2), (retourTextRect.y) + (retourTextRect.h/2), color);


        // Affichage du bouton quitter
        // Je le place ici pour que je puisse appeller son rect dans la boucle des events juste après
        // Définir le rectangle du bouton quitter
        SDL_Rect quitterRect = {largeurEcran / 2 - 50, hauteurEcran - 50, 100, 50}; // Centré en bas
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
        SDL_Rect quitterTextRect = afficherTexte(ren, font, "Quitter", quitterRect.x + (quitterRect.w / 2) - 26, quitterRect.y + (quitterRect.h / 2) - 10, color);
        quitterRect.w += 20; // Permet d'agrandir la zone de clic en largeur
        quitterRect.h += 10; // Permet d'agrandir la zone de clic en hauteur
        quitterRect.x -= 10; // Permet de commencer la zone de clic plus à gauche
        quitterRect.y -= 5; // Permet de commencer la zone de clic plus en haut
        //SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        //SDL_RenderDrawRect(ren, &quitterRect);

        

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                runningChoixGrille = 0;
            } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int mouseX = e.button.x;
                int mouseY = e.button.y;
                if (mouseX >= quitterRect.x && mouseX <= quitterRect.x + quitterRect.w &&
                    mouseY >= quitterRect.y && mouseY <= quitterRect.y + quitterRect.h) {
                    runningChoixGrille = 0;
                } else if (mouseX >= retourRect.x && mouseX <= retourRect.x + retourRect.w && // Quand on clique sur le bouton "Retour"
                           mouseY >= retourRect.y && mouseY <= retourRect.y + retourRect.h) {
                    runningChoixGrille = 0;
                } else if (mouseX >= saisieRect.x && mouseX <= saisieRect.x + saisieRect.w && // Quand on clique sur la zone de saisie
                           mouseY >= saisieRect.y && mouseY <= saisieRect.y + saisieRect.h) {
                    lectureTextInput(ren, font, color, saisieRect, inputText, sizeof(inputText));
                } else if ((mouseX >= jouerRect.x && mouseX <= jouerRect.x + jouerRect.w && // Quand on clique sur le bouton "Jouer"
                           mouseY >= jouerRect.y && mouseY <= jouerRect.y + jouerRect.h) ||
                           (mouseX >= jouerTextRect.x && mouseX <= jouerTextRect.x + jouerTextRect.w &&
                           mouseY >= jouerTextRect.y && mouseY <= jouerTextRect.y + jouerTextRect.h)) {
                        Grille grille = creationGrille(inputText);
                        runningChoixGrille = 0;
                        //libererGrille(&grille);
                        return grille;
                } else if (mouseX >= bouton1Rect.x && mouseX <= bouton1Rect.x + bouton1Rect.w && // Quand on clique sur le bouton "10*10"
                           mouseY >= bouton1Rect.y && mouseY <= bouton1Rect.y + bouton1Rect.h) {
                    inputText[0] = '1';
                    inputText[1] = '0';
                    inputText[2] = 'x';
                    inputText[3] = '1';
                    inputText[4] = '0';
                    //Grille grille = creationGrille(inputText);
                    //libererGrille(&grille);
                } else if (mouseX >= bouton2Rect.x && mouseX <= bouton2Rect.x + bouton2Rect.w && // Quand on clique sur le bouton "15*15"
                           mouseY >= bouton2Rect.y && mouseY <= bouton2Rect.y + bouton2Rect.h) {
                    inputText[0] = '2';
                    inputText[1] = '0';
                    inputText[2] = 'x';
                    inputText[3] = '2';
                    inputText[4] = '0';
                    //Grille grille = creationGrille(inputText);
                    //libererGrille(&grille);
                } else if (mouseX >= bouton3Rect.x && mouseX <= bouton3Rect.x + bouton3Rect.w && // Quand on clique sur le bouton "25*25"
                           mouseY >= bouton3Rect.y && mouseY <= bouton3Rect.y + bouton3Rect.h) {
                    inputText[0] = '2';
                    inputText[1] = '5';
                    inputText[2] = 'x';
                    inputText[3] = '2';
                    inputText[4] = '5';
                    //Grille grille = creationGrille(inputText);
                    //libererGrille(&grille);
                } else if (mouseX >= bouton4Rect.x && mouseX <= bouton4Rect.x + bouton4Rect.w && // Quand on clique sur le bouton "50*50"
                           mouseY >= bouton4Rect.y && mouseY <= bouton4Rect.y + bouton4Rect.h) {
                    inputText[0] = '8';
                    inputText[1] = '0';
                    inputText[2] = 'x';
                    inputText[3] = '4';
                    inputText[4] = '0';
                    //Grille grille = creationGrille(inputText);
                    //libererGrille(&grille);
                } else if (mouseX >= bouton5Rect.x && mouseX <= bouton5Rect.x + bouton5Rect.w && // Quand on clique sur le bouton "20*20"
                           mouseY >= bouton5Rect.y && mouseY <= bouton5Rect.y + bouton5Rect.h) {
                    inputText[0] = '1';
                    inputText[1] = '5';
                    inputText[2] = 'x';
                    inputText[3] = '1';
                    inputText[4] = '5';
                    //Grille grille = creationGrille(inputText);
                    //libererGrille(&grille);
                } else if (mouseX >= bouton6Rect.x && mouseX <= bouton6Rect.x + bouton6Rect.w && // Quand on clique sur le bouton "25*25"
                           mouseY >= bouton6Rect.y && mouseY <= bouton6Rect.y + bouton6Rect.h) {
                    inputText[0] = '5';
                    inputText[1] = '0';
                    inputText[2] = 'x';
                    inputText[3] = '5';
                    inputText[4] = '0';
                    //Grille grille = creationGrille(inputText);
                    //libererGrille(&grille);
                }
            }

        }

        // Afficher le rendu
        SDL_RenderPresent(ren);
    }

    // Si une erreur survient
    Grille grille = {0, 0, NULL};
    return grille;
}

void lectureTextInput(SDL_Renderer *ren, TTF_Font *font, SDL_Color color, SDL_Rect inputRect, char *inputText, int maxLength) {
    SDL_StartTextInput();
    int runningInput = 1;
    while (runningInput) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                runningInput = 0;
            } else if (e.type == SDL_TEXTINPUT) { // quand on tape un caractère
                if (strlen(inputText) < (size_t)(maxLength - 1)) {
                    strcat(inputText, e.text.text);
                }
            } else if (e.type == SDL_KEYDOWN) { // quand on appuie sur la touche backspace ou entrée
                if (e.key.keysym.sym == SDLK_BACKSPACE && strlen(inputText) > 0) {
                    inputText[strlen(inputText) - 1] = '\0';
                } else if (e.key.keysym.sym == SDLK_RETURN) {
                    runningInput = 0;
                }
            } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) { // quand on clique en dehors de la zone de saisie
                int mouseX = e.button.x;
                int mouseY = e.button.y;
                if (mouseX < inputRect.x || mouseX > inputRect.x + inputRect.w ||
                    mouseY < inputRect.y || mouseY > inputRect.y + inputRect.h) {
                    runningInput = 0;
                }
            }
        }

        // Effacer toutes la zone d'affichage de l'input
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderFillRect(ren, &inputRect);

        // On redesinne le rectangle
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        // Afficher la zone de saisie
        SDL_RenderDrawRect(ren, &inputRect);
        afficherTexte(ren, font, inputText, inputRect.x + 5, inputRect.y + 5, color);

        // Afficher le rendu
        SDL_RenderPresent(ren);
    }
    SDL_StopTextInput();
    // Ensure the screen is updated after text input is handled
    SDL_RenderPresent(ren);
}