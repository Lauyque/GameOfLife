#ifndef MENU_H
#define MENU_H

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>

int lancementMenu();
SDL_Rect afficherTexte(SDL_Renderer *ren, TTF_Font *font, const char *texte, int posX, int posY, SDL_Color color);
SDL_Rect afficherImage(SDL_Renderer *ren, const char *imagePath, SDL_Rect rect);
SDL_Rect afficherNom(SDL_Renderer *ren, TTF_Font *font, SDL_Rect rect, const char *nom, SDL_Color color);
int lancementPlusInformation(SDL_Renderer *ren, TTF_Font *fontTitle, TTF_Font *font, SDL_Color color, const char *nom, const char *description);

#endif