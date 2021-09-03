#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
#include "SDL.h"
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>
#include "jatek.h"
#include "jatek_utan.h"


void menuspec(SDL_Renderer *rmenu,GombHelyzet GOMB);
Uint32 idozit(Uint32 ms, void *param);
int jatek_indit(int szelesseg, int magassag, int oldal, int db, int ido1,SDL_Surface* feliratok[]);
void beallspec(GombHelyzet gombok, SDL_Renderer* rbeall,SDL_Color szin );
void beallitasok(GombHelyzet GOMB, SDL_Surface* feliratok[]);
void ranglista(GombHelyzet GOMB);
void szamSurfaceLetrehoz(SDL_Surface *tomb[]);
int balklikk(int mousex,int mousey,PalyaTul jellemzok,int oldal,int db,int megjelolt_aknak,int ido1,int hatralevo_ido,SDL_Renderer *renderer,SDL_Window *window,SDL_Surface *feliratok[],bool* vege);



#endif // MENU_H_INCLUDED
