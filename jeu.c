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
void afficherGrilleJeu(Grille* grille, SDL_Renderer *ren, TTF_Font *font, SDL_Color color);
SDL_Rect afficherTexte(SDL_Renderer *ren, TTF_Font *font, const char *texte, int posX, int posY, SDL_Color color);

// Jeu
int lancementJeu(SDL_Renderer *ren, TTF_Font *fontTitle, TTF_Font *font, SDL_Color color, const char *nom, Grille* grille){
    int runningJeu = 1;
    int tour = 1;
    const int DELAY = 200; // Délai en millisecondes entre chaque tour

    SDL_Rect tourTextRect; // Initialisation de la taille du texte pour le premier tour
    //tourTextRect.w = 0;
    //tourTextRect.h = 0;
    SDL_Rect retourRect; // Initialisation de la taille du texte pour le bouton "Quitter"

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
        
        // Afficher le titre
        // Afficher le titre au debut de la fenetre pour recuperer la taille du texte
        SDL_Rect titleTextRect = afficherTexte(ren, fontTitle, nom, ((largeurEcran/2)-(400)), 10,  color);
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
        titleTextRect = afficherTexte(ren, fontTitle, nom, titleTextX, titleTextY, color);




        // Faire un rectangle au centre de l'écran pour la grille adapter a la taille de la grille
        SDL_Rect grilleRect = {largeurEcran / 2 - (*grille).tailleX * 10, hauteurEcran / 2 - (*grille).tailleY * 10, (*grille).tailleX * 20, (*grille).tailleY * 20};
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255); // Couleur blanche
        SDL_RenderDrawRect(ren, &grilleRect); // Dessiner le rectangle

        // Afficher la grille dans le rectangle
        afficherGrilleJeu(grille, ren, font, color);

        // transformer un int en char
        char tourText[100];
        sprintf(tourText, "Tour : %d", tour); // Utiliser %d pour les entiers

        // Obtenir les dimensions du texte
        SDL_Rect tourTextRect = afficherTexte(ren, font, tourText, largeurEcran - 90, 25, color);

        // rectangle à la toute droite de l'écran pour afficher le nombre de tours
        SDL_Rect tourRect = {largeurEcran - 100, 20, tourTextRect.w + 20, tourTextRect.h + 10};
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255); // Couleur blanche
        SDL_RenderDrawRect(ren, &tourRect); // Dessiner le rectangle

        printf("width : %d, height : %d\n", tourTextRect.w, tourTextRect.h);

        // Gestion du temps entre les tours
        // Si on veut mettre un temps d'attente entre chaque tour
        // On peut utiliser la fonction SDL_GetTicks() qui retourne le nombre de millisecondes écoulées depuis l'initialisation de la SDL
        VérifierCaseVivante(grille);

        // Augmenter le nombre de tour
        tour++;

        // Mettre un bouton pause à droite
        SDL_Rect pauseRect = afficherTexte(ren, font, "Pause", largeurEcran - 100, hauteurEcran - 50, color);


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

                // QUITTER
                if (mouseX >= quitterRect.x && mouseX <= quitterRect.x + quitterRect.w &&
                    mouseY >= quitterRect.y && mouseY <= quitterRect.y + quitterRect.h) {
                    runningJeu = 0;
                }
                // PAUSE
                else if (mouseX >= pauseRect.x && mouseX <= pauseRect.x + pauseRect.w &&
                    mouseY >= pauseRect.y && mouseY <= pauseRect.y + pauseRect.h)
                {
                    printf("Pause\n");
                    int paused = 1;
                    while (paused){
                        // Griser l'écran
                        SDL_SetRenderDrawColor(ren, 128, 128, 128, 255);

                        // Afficher en grand le message de pause
                        SDL_Rect pauseMessageRect = afficherTexte(ren, fontTitle, "PAUSE", largeurEcran / 2 - 50, hauteurEcran / 2 - 50, color);

                        // Appliquer les modifications
                        SDL_RenderPresent(ren);

                        SDL_Event e;
                        while (SDL_PollEvent(&e))
                        {
                            if (e.type == SDL_QUIT)
                            {
                                runningJeu = 0;
                                paused = 0;
                            }
                            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
                            {
                                int mouseX = e.button.x;
                                int mouseY = e.button.y;

                                // QUITTER
                                if (mouseX >= retourRect.x && mouseX <= retourRect.x + retourRect.w &&
                                    mouseY >= retourRect.y && mouseY <= retourRect.y + retourRect.h)
                                {
                                    runningJeu = 0;
                                    paused = 0;
                                }

                                // PAUSE
                                if (mouseX >= pauseRect.x && mouseX <= pauseRect.x + pauseRect.w &&
                                    mouseY >= pauseRect.y && mouseY <= pauseRect.y + pauseRect.h)
                                {
                                    printf("Reprise\n");
                                    paused = 0;
                                }
                            }
                        }
                    }
                }
            }
        }

        // Afficher le rendu
        SDL_RenderPresent(ren);

        // Attendre un délai fixe avant le prochain tour
        SDL_Delay(DELAY);
    }
    
    // Suppression de la grille
    libererGrille(grille);

    return 0;
}

// Fonction pour afficher la grille dans le jeu
void afficherGrilleJeu(Grille* grille, SDL_Renderer *ren, TTF_Font *font, SDL_Color color){

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