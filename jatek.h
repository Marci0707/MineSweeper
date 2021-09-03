#ifndef JATEK_H_INCLUDED
#define JATEK_H_INCLUDED
#include <stdbool.h>
#include "SDL.h"
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>

/*
cellaállípot leíró enum
*/
typedef enum Allapot {fedett,feltart} Allapot;


/*
egy klikkelhető gomb leírása
*/
typedef struct GombHelyzet{
    int gombokKezdX;
    int gombokKezdY;
    int gombSzelesseg;
    int gombMagassag;
}GombHelyzet;

/*
cella modellezése
*/
typedef struct Cella
{
    int y; //grid y
    int x; //grid x
    bool akna; //akna-e
    Allapot allapot; //mi az állapota
    int szomszed; //hány szomszédja van
    bool megjelolt; //meg van a jelölve (bombaként)
} Cella;

/*
Pályaleíró objektum
*/
typedef struct PalyaTul{
    int szelesseg; //cellák száma vísszintesen
    int magassag; //pályák száma magasságban
    Cella **palya; //a 2D cell lista
}PalyaTul;

void feltolt(PalyaTul *jellemzok);
void akna_feltolt(int db, PalyaTul *jellemzok);
int szomszedok(int j, int i, PalyaTul * jellemzok);
void szomszedok_feltolt(PalyaTul *jellemzok);
void felfed(PalyaTul *jellemzok,int j, int  i);



#endif // JATEK_H_INCLUDED
