#ifndef CHOIXGRILLE_H
#define CHOIXGRILLE_H

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>

GrilleChaine* lancementChoixGrille(SDL_Renderer *ren, TTF_Font *fontTitle, TTF_Font *font, SDL_Color color);
void lectureTextInput(SDL_Renderer *ren, TTF_Font *font, SDL_Color color, SDL_Rect inputRect, char *inputText, int maxLength);
SDL_Rect afficherTexte(SDL_Renderer *ren, TTF_Font *font, const char *texte, int posX, int posY, SDL_Color color);

#endif // CHOIXGRILLE_H