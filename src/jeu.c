#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define SDL_MAIN_HANDLED
#include <SDL.h> // Erreur normal puisqu'on rajoute le chemin vers la librairie SDL avec le makefile
#include <SDL_ttf.h> // Erreur normal puisqu'on rajoute le chemin vers la librairie SDL avec le makefile
#include <SDL_image.h> // Erreur normal puisqu'on rajoute le chemin vers la librairie SDL avec le makefile
#include <SDL_mixer.h> // Erreur normal puisqu'on rajoute le chemin vers la librairie SDL avec le makefile

// Mes propres fichiers
#include "grille.h"
//#include "sauvegarde.h"
#include "sauvegarde.h"

// PROTOTYPES
int lancementJeu(SDL_Renderer *ren, TTF_Font *fontTitle, TTF_Font *font, SDL_Color color, const char *nom, GrilleChaine* grille);
void afficherGrilleJeu(GrilleChaine* grille, SDL_Renderer *ren, const char *nom,int *caseX,int *caseY, int* mouseX, int* mouseY);
SDL_Rect afficherTexte(SDL_Renderer *ren, TTF_Font *font, const char *texte, int posX, int posY, SDL_Color color);
int pause(SDL_Renderer *ren, TTF_Font *fontTitle, SDL_Color color, SDL_Rect pauseRect ,SDL_Rect quitterRect);

// Jeu
int lancementJeu(SDL_Renderer *ren, TTF_Font *fontTitle, TTF_Font *font, SDL_Color color, const char *nom, GrilleChaine* grille){
    setlocale(LC_ALL, "fr_FR.UTF-8");
    int runningJeu = 1;
    int tour = 1;
    int DELAY = 200; // Délai en millisecondes entre chaque tour
    int DELAY_MAX = 400; // Délai maximum en millisecondes entre chaque tour
    int DELAY_VISIBLE = 2; // Délai en millisecondes pour afficher les changements
    bool caseCoche = false; // Permet de passer les tours automatiquements

    // Initialisation de SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
    {
        printf("Mix_OpenAudio Error: %s\n", Mix_GetError());
        //TTF_Quit();
        //SDL_Quit();
        return 1;
    }

    // Charger la musique
    Mix_Music *music = Mix_LoadMUS("../assets/musics/Music_Jeu.mp3");
    if (!music)
    {
        printf("Mix_LoadMUS Error: %s\n", Mix_GetError());
        Mix_FreeMusic(music);
        music = NULL;
        //Mix_CloseAudio();
        //TTF_Quit();
        //SDL_Quit();
        //return 1;
    } else {
        // Jouer la musique
        if (Mix_PlayMusic(music, -1) == -1)
        {
            printf("Mix_PlayMusic Error: %s\n", Mix_GetError());
            Mix_FreeMusic(music);
            Mix_CloseAudio();
            music = NULL;
            //TTF_Quit();
            //SDL_Quit();
            //return 1;
        }
    }

    // Vérification de la bonne allocution de la mémoire
    if (grille->dernier->listePointeursLignes == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour la liste des pointeurs vers les lignes\n");
        exit(EXIT_FAILURE);
    }

    while(runningJeu == 1){
        // Effacer l'écran
        SDL_SetRenderDrawColor(ren, 100, 100, 100, 255);
        SDL_RenderClear(ren);

        // Récuperer les dimention de l'écran
        SDL_DisplayMode DM;
        SDL_GetCurrentDisplayMode(0, &DM);
        int largeurEcran = DM.w;
        int hauteurEcran = DM.h;

        // Vérification des cases
        int caseX = -1;
        int caseY = -1;

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
        SDL_Rect grilleRect = {largeurEcran / 2 - grille->dernier->tailleX * 10 + 1, hauteurEcran / 2 - grille->dernier->tailleY * 10 + 1, grille->dernier->tailleX * 20 + 0, grille->dernier->tailleY * 20 + 0}; // +1 pour afficher un contour
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255); // Couleur blanche
        SDL_RenderDrawRect(ren, &grilleRect); // Dessiner le rectangle

        // Afficher la grille dans le rectangle
        int temp1 = 0;
        int temp2 = 0;
        afficherGrilleJeu(grille, ren, nom, &caseX, &caseY, &temp1, &temp2);

        // Condition pour ne pas avancer de tour si l'option "SandBox a été choisie"
        if (strcmp(nom, "SandBox") != 0){
            // Gestion du temps entre les tours
            // Si on veut mettre un temps d'attente entre chaque tour
            // On peut utiliser la fonction SDL_GetTicks() qui retourne le nombre de millisecondes écoulées depuis l'initialisation de la SDL
            if (caseCoche){
                // Vérifier si la grille suivante n'a pas déjà été calculée
                if (grille->dernier->suivant != NULL){
                    grille->dernier = grille->dernier->suivant;
                } else {
                    vérifierCaseVivante(grille);
                    // Augmenter le nombre de tours
                    tour++;
                }
            }
        }

        // Menu à droite
        SDL_Rect menuDroiteRect = {largeurEcran - 200, 0, 200, hauteurEcran};
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255); // Couleur noire
        SDL_RenderFillRect(ren, &menuDroiteRect); // Dessiner le rectangle

        // Titre menu à droite
        SDL_Rect menuDroiteTextRect = afficherTexte(ren, fontTitle, "Menu", largeurEcran - 150, 10, color);

        // transformer un int en char
        char tourText[100];
        sprintf(tourText, "Tour : %d", tour); // Utiliser %d pour les entiers
        // Obtenir les dimensions du texte
        SDL_Rect tourTextRect = afficherTexte(ren, font, tourText, largeurEcran - (menuDroiteRect.w - 20), menuDroiteTextRect.y +100, color);
        // rectangle à la toute droite de l'écran pour afficher le nombre de tours
        SDL_Rect tourRect = {tourTextRect.x - 10, tourTextRect.y - 5, tourTextRect.w + 20, tourTextRect.h * 3};
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255); // Couleur blanche
        SDL_RenderDrawRect(ren, &tourRect); // Dessiner le rectangle

        // Faire la case à cocher et les boutons pour passer les tours manuellement.
        // Bouton retour arrière
        SDL_Rect boutonRetourArriere = {largeurEcran - (menuDroiteRect.w - 20), tourRect.y + 40, 25, 20}; // Taille du bouton
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderDrawRect(ren, &boutonRetourArriere);
        //SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        //SDL_RenderFillRect(ren, &boutonRetourArriere);
        afficherTexte(ren, font, "<", boutonRetourArriere.x + 8, boutonRetourArriere.y, color); // Afficher le texte du bouton
        // Bouton pour avancer
        SDL_Rect boutonAvancer = {largeurEcran - (menuDroiteRect.w - 60), tourRect.y + 40, 25, 20}; // Taille du bouton
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderDrawRect(ren, &boutonAvancer);
        //SDL_SetRenderDrawColor(ren, 0, 0, 0, 255); 
        //SDL_RenderFillRect(ren, &boutonAvancer);
        afficherTexte(ren, font, ">", boutonAvancer.x + 8, boutonAvancer.y, color); // Afficher le texte du bouton

        // Case à cocher
        SDL_Rect checkboxRect = {largeurEcran - (menuDroiteRect.w - 10), tourRect.y + 80, 20, 20}; // Taille de la case à cocher
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255); // Couleur blanche pour le contour
        SDL_RenderDrawRect(ren, &checkboxRect);

        // Si la case est cochée, dessiner un carré à l'intérieur
        if (caseCoche) {
            SDL_Rect innerRect = {checkboxRect.x + 5, checkboxRect.y + 5, 10, 10};
            SDL_SetRenderDrawColor(ren, 255, 255, 255, 255); // Couleur blanche pour l'intérieur
            SDL_RenderFillRect(ren, &innerRect);
        }

        // Afficher le texte à droite de la case à cocher
        afficherTexte(ren, font, "Tours Automatiques", checkboxRect.x + 25, tourRect.y + 80, color);

        // Afficher la vitesse en haut à droite sous le nombre de tour
        SDL_Rect vitesseTextRect = afficherTexte(ren, font, "Vitesse :", largeurEcran - (menuDroiteRect.w - 20), tourRect.y + 120, color);
        // Transformer un int en char
        char delayText[10];
        sprintf(delayText, "%d", DELAY_VISIBLE);
        SDL_Rect delayRect = afficherTexte(ren, font, delayText,largeurEcran - (vitesseTextRect.w + 40), vitesseTextRect.y, color);
        // Rectangle pour tout le compartiment de la vitesse
        SDL_Rect vitesseRect = {vitesseTextRect.x - 10, vitesseTextRect.y - 5, vitesseTextRect.w + delayRect.w + 25, vitesseTextRect.h + 10};
        SDL_RenderDrawRect(ren, &vitesseRect); // Dessiner le rectangle
        // bouton pour augmenter la vitesse sous l'affichage de la vitesse
        SDL_Rect boutonPlusTextRect = afficherTexte(ren, font, "+ 1", largeurEcran - (menuDroiteRect.w - 20), vitesseRect.y + 50, color);
        SDL_Rect boutonPlusRect = {boutonPlusTextRect.x - 10, boutonPlusTextRect.y - 5, boutonPlusTextRect.w + 20, boutonPlusTextRect.h + 10};
        SDL_SetRenderDrawColor(ren, 100, 100, 100, 255);
        SDL_RenderDrawRect(ren, &boutonPlusRect); // Dessiner le rectangle
        // bouton pour diminuer la vitesse à droite du bouton pour augmenter la vitesse
        SDL_Rect boutonMoinsTextRect = afficherTexte(ren, font, "- 1", vitesseRect.x + vitesseRect.w - (boutonPlusRect.w - 15), boutonPlusTextRect.y, color);
        SDL_Rect boutonMoinsRect = {boutonMoinsTextRect.x  - 10, boutonMoinsTextRect.y - 5, boutonMoinsTextRect.w + 20, boutonMoinsTextRect.h + 10};
        SDL_RenderDrawRect(ren, &boutonMoinsRect); // Dessiner le rectangle


        // Mettre un bouton pause à droite
        SDL_Rect pauseRect = afficherTexte(ren, font, "Pause", menuDroiteRect.x + 20, hauteurEcran - 50, color);
        // Rectangle pour le bouton pause
        SDL_Rect pauseTextRect = {pauseRect.x - 10, pauseRect.y - 5, pauseRect.w + 20, pauseRect.h + 10};
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderDrawRect(ren, &pauseTextRect); // Dessiner le rectangle

        // Bouton "Sauvegarder"
        SDL_Rect saveRect = {menuDroiteRect.x, pauseRect.y -60, 100, 50}; // Position en haut à gauche
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderFillRect(ren, &saveRect);
        afficherTexte(ren, font, "Sauvegarder", saveRect.x + 10, saveRect.y + 10, color);


        // Affichage du bouton quitter
        // Je le place ici pour que je puisse appeller son rect dans la boucle des events juste après
        // Définir le rectangle du bouton quitter
        SDL_Rect quitterRect = {largeurEcran / 2 - 50, hauteurEcran - 50, 100, 50}; // Centré en bas
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
        afficherTexte(ren, font, "Quitter", quitterRect.x + (quitterRect.w / 2) - 26, quitterRect.y + (quitterRect.h / 2) - 10, color);
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

                // Gestion du bouton "Sauvegarder"
                if (mouseX >= saveRect.x && mouseX <= saveRect.x + saveRect.w &&
                    mouseY >= saveRect.y && mouseY <= saveRect.y + saveRect.h) 
                {
                    sauvegarderGrilleChaine(grille, "sauvegarde.txt");
                }

                // QUITTER
                if (mouseX >= quitterRect.x && mouseX <= quitterRect.x + quitterRect.w &&
                    mouseY >= quitterRect.y && mouseY <= quitterRect.y + quitterRect.h) {
                    runningJeu = 0;
                } else if (mouseX >= boutonPlusRect.x && mouseX <= boutonPlusRect.x + boutonPlusRect.w && // Augmentation de la vitesse
                    mouseY >= boutonPlusRect.y && mouseY <= boutonPlusRect.y + boutonPlusRect.h)
                {
                    if (DELAY != 0) {
                        DELAY -= 100;
                        DELAY_VISIBLE += 1;
                    }
                } else if (mouseX >= boutonMoinsRect.x && mouseX <= boutonMoinsRect.x + boutonMoinsRect.w && // Diminution de la vitesse
                    mouseY >= boutonMoinsRect.y && mouseY <= boutonMoinsRect.y + boutonMoinsRect.h)
                {
                    if (DELAY != DELAY_MAX) {
                        DELAY += 100;
                        DELAY_VISIBLE -= 1;
                    }
                }

                // Aller en arrière
                else if (mouseX >= boutonRetourArriere.x && mouseX < boutonRetourArriere.x + boutonRetourArriere.w &&
                         mouseY >= boutonRetourArriere.y && mouseY < boutonRetourArriere.y + boutonRetourArriere.h) {
                        if (grille->dernier != grille->premier) {
                            grille->dernier = grille->dernier->precedent;
                            tour--;
                        }
                }
                // Aller en avant
                else if (mouseX >= boutonAvancer.x && mouseX < boutonAvancer.x + boutonAvancer.w &&
                         mouseY >= boutonAvancer.y && mouseY < boutonAvancer.y + boutonAvancer.h) {

                        vérifierCaseVivante(grille);
                        tour++;
                }

                // Case à cocher
                else if (mouseX >= checkboxRect.x && mouseX < checkboxRect.x + checkboxRect.w &&
                         mouseY >= checkboxRect.y && mouseY < checkboxRect.y + checkboxRect.h) {
                
                        if (caseCoche){
                            caseCoche = false;
                        } else {
                            caseCoche = true;
                        }
                }

                // PAUSE
                else if (mouseX >= pauseRect.x && mouseX <= pauseRect.x + pauseRect.w &&
                    mouseY >= pauseRect.y && mouseY <= pauseRect.y + pauseRect.h) {
                    
                    runningJeu = pause(ren, fontTitle, color, pauseRect, quitterRect);
                }

                // Clique dans la grille
                else if (mouseX >= grilleRect.x && mouseX < grilleRect.x + grilleRect.w &&
                        mouseY >= grilleRect.y && mouseY < grilleRect.y + grilleRect.h) {
                    //printf("Un clique dans la grille\n");
                    afficherGrilleJeu(grille, ren, nom, &caseX, &caseY, &mouseX, &mouseY);
                }
            }
        }

        // Afficher le rendu
        SDL_RenderPresent(ren);

        // Attendre un délai fixe avant le prochain tour
        SDL_Delay(DELAY);
    }
    
    // Suppression de la grille
    libererGrilleChaine(grille);

    Mix_CloseAudio();
    Mix_FreeMusic(music);

    return 0;
}

// Fonction pour afficher la grille dans le jeu
void afficherGrilleJeu(GrilleChaine* grille, SDL_Renderer *ren,const char *nom, int *caseX, int *caseY, int* mouseX, int* mouseY){
    // Vérification de la bonne allocution de la mémoire
    if (grille->dernier->listePointeursLignes == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour la liste des pointeurs vers les lignes\n");
        exit(EXIT_FAILURE);
    }

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    int largeurEcran = DM.w;
    int hauteurEcran = DM.h;

    SDL_Rect grilleRect = {largeurEcran / 2 - grille->dernier->tailleX * 10, hauteurEcran / 2 - grille->dernier->tailleY * 10, grille->dernier->tailleX * 20, grille->dernier->tailleY * 20};

    for (int i = 0; i < grille->dernier->tailleY; i++){
        for (int j = 0; j < grille->dernier->tailleX; j++){
            SDL_Rect caseRect = {grilleRect.x + j * 20, grilleRect.y + i * 20, 20, 20};
            if (grille->dernier->listePointeursLignes[i][j] == 1){
                // Afficher un carré plein
                SDL_SetRenderDrawColor(ren, 255, 255, 255, 255); // Couleur blanche
                SDL_RenderFillRect(ren, &caseRect);
            } else if (grille->dernier->listePointeursLignes[i][j] == 0){
                // Afficher un carré gris vide
                SDL_SetRenderDrawColor(ren, 128, 128, 128, 255); // Couleur grise
                SDL_RenderFillRect(ren, &caseRect);
            } else {
                fprintf(stderr, "Erreur: valeur de la case incorrecte\n");
            }
            // Dessiner le contour du carré
            SDL_SetRenderDrawColor(ren, 0, 0, 0, 255); // Couleur noir
            SDL_RenderDrawRect(ren, &caseRect);

            // Dectecter un clique
            if (strcmp(nom, "SandBox") == 0){
                //printf("Vérification de clique\n");
                // Vérifier si le clic est dans cette case
                if (*mouseX >= caseRect.x && *mouseX < caseRect.x + caseRect.w &&
                    *mouseY >= caseRect.y && *mouseY < caseRect.y + caseRect.h) {
                    //printf("- Clique verifié\n");
                    *caseX = j;
                    *caseY = i;
                    if (*caseX != -1 && *caseY != -1) {
                        grille->dernier->listePointeursLignes[i][j] = 1;
                        //printf("- Case cliquée: (%d, %d)\n", *caseX, *caseY);
                    }
                }
            }
        }
    }
}


// Fonction qui affiche la pause
int pause(SDL_Renderer *ren, TTF_Font *fontTitle, SDL_Color color, SDL_Rect pauseRect ,SDL_Rect quitterRect){
    int paused = 1;
    int runningJeu = 1;

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    int largeurEcran = DM.w;
    int hauteurEcran = DM.h;

    while (paused){
        // Griser l'écran avec une opacity réduite
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 200);
        //SDL_RenderClear(ren);

        // Afficher en grand le message de pause
        afficherTexte(ren, fontTitle, "PAUSE", largeurEcran / 2 - 50, hauteurEcran / 2 - 50, color);

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
                if (mouseX >= quitterRect.x && mouseX <= quitterRect.x + quitterRect.w &&
                    mouseY >= quitterRect.y && mouseY <= quitterRect.y + quitterRect.h) {
                    runningJeu = 0;
                    paused = 0;
                }
                // PAUSE
                else if (mouseX >= pauseRect.x && mouseX <= pauseRect.x + pauseRect.w &&
                    mouseY >= pauseRect.y && mouseY <= pauseRect.y + pauseRect.h)
                {
                    printf("Reprise\n");
                    paused = 0;
                }
            }
        }
    }
    return runningJeu;
}