#include "rajzol.h"
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <string.h>
/*
sdl inicializásála
param1: ablaknév
param2: szélesség
param3: magasság
param4: window
param5: renderer
*/
void sdl_init(char const *felirat,int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer)
{


    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow(felirat, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szeles, magas, 0);
    if (window == NULL)
    {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL)
    {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(renderer);

    *pwindow = window;
    *prenderer = renderer;
}



/*
ráír a cellára egy számot, ahány szomszádja

param1: a pálya, 2D cella grid
param2: cella x poz
param3: cella y poz
param4: cellaoldal nagysága
param5: renderer
param6: ráírandó szám
param7: a számokat tartalmazó feliratlista
*/
void rair( Cella ** palya, int y, int x, int oldal, SDL_Renderer **renderer, int szam, SDL_Surface *feliratok[])
{
    SDL_Texture *felirat_t;
    SDL_Rect hova = { 0, 0, 0, 0 };

    Cella a = palya[y][x];
    hova.x = a.x*oldal;
    hova.y = a.y*oldal;
    hova.w = oldal;
    hova.h = oldal;

    felirat_t = SDL_CreateTextureFromSurface(*renderer, feliratok[szam]);
    SDL_RenderCopy(*renderer, felirat_t, NULL, &hova);
    SDL_DestroyTexture(felirat_t);

}

/*
kiír szöveget valahova a képernyőre
param1: szöveg mérete
param2: színe
param3: renderer
param4: a kiírandó szöveg
param5: szöveg poz x
param6: szöveg poz y
*/
void szKiir(int meret, SDL_Color szin, SDL_Renderer* renderer, char* str, int x, int y)
{
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("LiberationSerif-Regular.ttf", meret);
    if (!font)
    {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }
    SDL_Surface *felirat;
    SDL_Texture *felirat_t;
    SDL_Rect hova = { 0, 0, 0, 0 };

    felirat = TTF_RenderUTF8_Blended(font,str,szin);
    felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
    hova.x = x;
    hova.y = y;
    hova.w = felirat->w;
    hova.h = felirat->h;
    SDL_RenderCopy(renderer, felirat_t, NULL, &hova);
    SDL_FreeSurface(felirat);
    SDL_DestroyTexture(felirat_t);

}

/*
kirajzolja a pályaképet
param1: pályaleíró objektum
param2: cellaoldal nagységa
param3: renderer
param4: a számokat reprezentáló sdl surface lista, 9 db 0-8
*/
void rajzolas(PalyaTul jellemzok, int oldal, SDL_Renderer *renderer,SDL_Surface *feliratok[])
{
    int szelesseg = jellemzok.szelesseg;
    int magassag = jellemzok.magassag;


    for(int j = 0; j < magassag; j++)
    {
        for(int i = 0; i < szelesseg; i++)
        {

            Cella a =jellemzok. palya[j][i];

            if(a.allapot == feltart)
            {
                if(a.akna == true)
                {
                    boxRGBA(renderer,  a.x*oldal,  a.y*oldal,  a.x*oldal+oldal,  a.y*oldal+oldal,200,56,0,255);
                }
                else
                {
                    rair(jellemzok.palya, j, i, oldal, &renderer,a.szomszed,feliratok);
                }
            }
            else
            {
                if(a.megjelolt == true)
                {
                    boxRGBA(renderer,a.x*oldal,a.y*oldal,a.x*oldal+oldal,a.y*oldal+oldal,0,0,0,255);
                }
                else
                    boxRGBA(renderer,a.x*oldal,a.y*oldal,a.x*oldal+oldal,a.y*oldal+oldal,255,134,100,255);
            }
        }
    }

    for(int x =0 ; x< szelesseg*oldal+1 ; x+=oldal)
    {
        lineRGBA(renderer, x, 0, x, magassag*oldal+1, 0, 0, 0, 255);
    }
    for(int y =0 ; y< magassag*oldal+1 ; y+=oldal)
    {
        lineRGBA(renderer, 0, y, szelesseg*oldal+1, y, 0, 0, 0, 255);
    }

}


/*
bekér egy szöveget
param1: a szöveg pointere
param2: maxhossz
paarm3: a tégalap ahova ír
paarm3: a tégalap ahova ír
param4: színe a háttérnek
param5: színe a szövegnek
param6: a font
param7: renderer
*/
bool input_text(char *dest, size_t hossz, SDL_Rect teglalap, SDL_Color hatter, SDL_Color szoveg, TTF_Font *font, SDL_Renderer *renderer) {
    /* Ez tartalmazza az aktualis szerkesztest */
    char composition[SDL_TEXTEDITINGEVENT_TEXT_SIZE];
    composition[0] = '\0';
    /* Ezt a kirajzolas kozben hasznaljuk */
    char textandcomposition[hossz + SDL_TEXTEDITINGEVENT_TEXT_SIZE + 1];
    /* Max hasznalhato szelesseg */
    int maxw = teglalap.w - 2;
    int maxh = teglalap.h - 2;

    dest[0] = '\0';

    bool enter = false;
    bool kilep = false;

    SDL_StartTextInput();
    while (!kilep && !enter) {
        /* doboz kirajzolasa */
        boxRGBA(renderer, teglalap.x, teglalap.y, teglalap.x + teglalap.w - 1, teglalap.y + teglalap.h - 1, hatter.r, hatter.g, hatter.b, 255);
        rectangleRGBA(renderer, teglalap.x, teglalap.y, teglalap.x + teglalap.w - 1, teglalap.y + teglalap.h - 1, szoveg.r, szoveg.g, szoveg.b, 255);
        /* szoveg kirajzolasa */
        int w;
        strcpy(textandcomposition, dest);
        strcat(textandcomposition, composition);
        if (textandcomposition[0] != '\0') {
            SDL_Surface *felirat = TTF_RenderUTF8_Blended(font, textandcomposition, szoveg);
            SDL_Texture *felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
            SDL_Rect cel = { teglalap.x, teglalap.y, felirat->w < maxw ? felirat->w : maxw, felirat->h < maxh ? felirat->h : maxh };
            SDL_RenderCopy(renderer, felirat_t, NULL, &cel);
            SDL_FreeSurface(felirat);
            SDL_DestroyTexture(felirat_t);
            w = cel.w;
        } else {
            w = 0;
        }
        /* kurzor kirajzolasa */
        if (w < maxw) {
            vlineRGBA(renderer, teglalap.x + w + 2, teglalap.y + 2, teglalap.y + teglalap.h - 3, szoveg.r, szoveg.g, szoveg.b, 192);
        }
        /* megjeleniti a képernyon az eddig rajzoltakat */
        SDL_RenderPresent(renderer);

        SDL_Event event;
        SDL_WaitEvent(&event);
        switch (event.type) {
            /* Kulonleges karakter */
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    int textlen = strlen(dest);
                    do {
                        if (textlen == 0) {
                            break;
                        }
                        if ((dest[textlen-1] & 0x80) == 0x00)   {
                            /* Egy bajt */
                            dest[textlen-1] = 0x00;
                            break;
                        }
                        if ((dest[textlen-1] & 0xC0) == 0x80) {
                            /* Bajt, egy tobb-bajtos szekvenciabol */
                            dest[textlen-1] = 0x00;
                            textlen--;
                        }
                        if ((dest[textlen-1] & 0xC0) == 0xC0) {
                            /* Egy tobb-bajtos szekvencia elso bajtja */
                            dest[textlen-1] = 0x00;
                            break;
                        }
                    } while(true);
                }
                if (event.key.keysym.sym == SDLK_RETURN) {
                    enter = true;
                }
                break;

            /* A feldolgozott szoveg bemenete */
            case SDL_TEXTINPUT:
                if (strlen(dest) + strlen(event.text.text) < hossz) {
                    strcat(dest, event.text.text);
                }

                /* Az eddigi szerkesztes torolheto */
                composition[0] = '\0';
                break;

            /* Szoveg szerkesztese */
            case SDL_TEXTEDITING:
                strcpy(composition, event.edit.text);
                break;

            case SDL_QUIT:
                /* visszatesszuk a sorba ezt az eventet, mert
                 * sok mindent nem tudunk vele kezdeni */
                SDL_PushEvent(&event);
                kilep = true;
                break;
        }
    }

    /* igaz jelzi a helyes beolvasast; = ha enter miatt allt meg a ciklus */
    SDL_StopTextInput();
    return enter;
}
