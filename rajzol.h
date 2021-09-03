#ifndef RAJZOL_H_INCLUDED
#define RAJZOL_H_INCLUDED
#include "jatek.h"
#include <SDL_ttf.h>
void sdl_init(char const *felirat, int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer);
void rajzolas(PalyaTul jellemzok, int oldal, SDL_Renderer *renderer,SDL_Surface *feliratok[]);
void szKiir(int meret, SDL_Color szin, SDL_Renderer* renderer, char* str, int x, int y);
bool input_text(char *dest, size_t hossz, SDL_Rect teglalap, SDL_Color hatter, SDL_Color szoveg, TTF_Font *font, SDL_Renderer *renderer) ;



#endif // RAJZOL_H_INCLUDED
