#ifndef JEU_H
#define JEU_H

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>

// PROTOTYPES
int lancementJeu(SDL_Renderer *ren, TTF_Font *fontTitle, TTF_Font *font, SDL_Color color, const char *nom, GrilleChaine* grille);
SDL_Rect* afficherGrilleJeu(GrilleChaine* grille, SDL_Renderer *ren, TTF_Font *font, SDL_Color color,const char *nom,int *caseX,int *caseY, int* mouseX, int* mouseY);
SDL_Rect afficherTexte(SDL_Renderer *ren, TTF_Font *font, const char *texte, int posX, int posY, SDL_Color color);
int pause(SDL_Renderer *ren, TTF_Font *fontTitle, TTF_Font *font, SDL_Color color, SDL_Rect pauseRect ,SDL_Rect quitterRect);

#endif