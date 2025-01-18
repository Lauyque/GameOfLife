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

GrilleChaine* lancementChoixGrille(SDL_Renderer *ren, TTF_Font *fontTitle, TTF_Font *font, SDL_Color color, const char *nom);
void lectureTextInput(SDL_Renderer *ren, TTF_Font *font, SDL_Color color, SDL_Rect inputRect, char *inputText, int maxLength);

SDL_Rect afficherTexte(SDL_Renderer *ren, TTF_Font *font, const char *texte, int posX, int posY, SDL_Color color);

// Fonction qui affiche le choix de la taille de la grille
GrilleChaine* lancementChoixGrille(SDL_Renderer *ren, TTF_Font *fontTitle, TTF_Font *font, SDL_Color color, const char *nom) {
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

                        // Transformer un char en int
                        //char inputSauv[100];
                        //strcpy(inputSauv, inputText);
                        //printf("Transformation\n");
                        char *token = strtok(inputText, "x"); // On découpe la chaine de caractère avec un espace
                        int largeur = atoi(token); // On convertit le premier élément en int
                        token = strtok(NULL, " "); // On passe au prochain élément
                        int hauteur = atoi(token); // On convertit le deuxième élément en int
                        //printf("Fin transformation\n");
                        // réassignation de la bonna valeur à la variable input
                        //strcpy(inputText, inputSauv);

                        GrilleChaine* grille = creerGrilleChaine(largeur, hauteur);
                        //printf("Dimensions : x = %d, y = %d", grille->dernier->tailleX, grille->dernier->tailleY);
                        //printf("grille ok\n");
                        runningChoixGrille = 0;
                        //libererGrilleChaine(grille);
                        return grille;
                } else if (mouseX >= bouton1Rect.x && mouseX <= bouton1Rect.x + bouton1Rect.w && // Quand on clique sur le bouton "10*10"
                           mouseY >= bouton1Rect.y && mouseY <= bouton1Rect.y + bouton1Rect.h) {
                    inputText[0] = '1';
                    inputText[1] = '0';
                    inputText[2] = 'x';
                    inputText[3] = '1';
                    inputText[4] = '0';
                    //Grille grille = creationGrille(inputText);
                    //libererGrilleChaine(grille);
                } else if (mouseX >= bouton2Rect.x && mouseX <= bouton2Rect.x + bouton2Rect.w && // Quand on clique sur le bouton "15*15"
                           mouseY >= bouton2Rect.y && mouseY <= bouton2Rect.y + bouton2Rect.h) {
                    inputText[0] = '2';
                    inputText[1] = '0';
                    inputText[2] = 'x';
                    inputText[3] = '2';
                    inputText[4] = '0';
                    //Grille grille = creationGrille(inputText);
                    //libererGrilleChaine(grille);
                } else if (mouseX >= bouton3Rect.x && mouseX <= bouton3Rect.x + bouton3Rect.w && // Quand on clique sur le bouton "25*25"
                           mouseY >= bouton3Rect.y && mouseY <= bouton3Rect.y + bouton3Rect.h) {
                    inputText[0] = '2';
                    inputText[1] = '5';
                    inputText[2] = 'x';
                    inputText[3] = '2';
                    inputText[4] = '5';
                    //Grille grille = creationGrille(inputText);
                    //libererGrilleChaine(grille);
                } else if (mouseX >= bouton4Rect.x && mouseX <= bouton4Rect.x + bouton4Rect.w && // Quand on clique sur le bouton "50*50"
                           mouseY >= bouton4Rect.y && mouseY <= bouton4Rect.y + bouton4Rect.h) {
                    inputText[0] = '8';
                    inputText[1] = '0';
                    inputText[2] = 'x';
                    inputText[3] = '4';
                    inputText[4] = '0';
                    //Grille grille = creationGrille(inputText);
                    //libererGrilleChaine(grille);
                } else if (mouseX >= bouton5Rect.x && mouseX <= bouton5Rect.x + bouton5Rect.w && // Quand on clique sur le bouton "20*20"
                           mouseY >= bouton5Rect.y && mouseY <= bouton5Rect.y + bouton5Rect.h) {
                    inputText[0] = '1';
                    inputText[1] = '5';
                    inputText[2] = 'x';
                    inputText[3] = '1';
                    inputText[4] = '5';
                    //Grille grille = creationGrille(inputText);
                    //libererGrilleChaine(grille);
                } else if (mouseX >= bouton6Rect.x && mouseX <= bouton6Rect.x + bouton6Rect.w && // Quand on clique sur le bouton "25*25"
                           mouseY >= bouton6Rect.y && mouseY <= bouton6Rect.y + bouton6Rect.h) {
                    inputText[0] = '5';
                    inputText[1] = '0';
                    inputText[2] = 'x';
                    inputText[3] = '5';
                    inputText[4] = '0';
                    //Grille grille = creationGrille(inputText);
                    //libererGrilleChaine(grille);
                }
            }

        }

        // Afficher le rendu
        SDL_RenderPresent(ren);
    }

    // Si une erreur survient ou si l'utilisateur annule le choix de la grille
    GrilleChaine* grille = creerGrilleChaine(0, 0);
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