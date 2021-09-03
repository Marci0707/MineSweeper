
#include "SDL.h"
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include "rajzol.h"
#include "menu.h"
#include "jatek_utan.h"
#include "debugmalloc-impl.h"



/*
elindítja a játékot, sdl inicializálás után megjeleníti a kezdő ablakot a menüpontokkal.
Ennek a függvénynek a végére akkor érkezünk el, ha kilépnek a programból, A menüpontok eseményeit itt kezeli le
*/
int main(int argc, char *argv[])
{
    debugmalloc_log_file("memlog.txt");
    SDL_Window *wmenu;
    SDL_Renderer *rmenu;




    /*
        int gombokKezdX;
        int gombokKezdY;
        int gombSzelesseg;
        int gombMagassag;

    */
    GombHelyzet gombok = { 50,150,300,30 };

    int szeles = 400;
    int magas = 400;
    sdl_init("menu",szeles,magas, &wmenu, &rmenu);


    menuspec(rmenu, gombok);

    SDL_RenderPresent(rmenu);

    SDL_Surface* feliratok[9];

    szamSurfaceLetrehoz(feliratok);

    SDL_TimerID id = SDL_AddTimer( 1000, idozit, NULL);

    int pont;
    SDL_Event ev;
    while ( (SDL_WaitEvent(&ev) && ev.type != SDL_QUIT))
    {
        if(ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_CLOSE)
        {
            SDL_Quit();

        }
        if (ev.type== SDL_MOUSEBUTTONDOWN)
        {
            int mousex, mousey;
            SDL_GetMouseState(&mousex, &mousey);

            if ((mousex > gombok.gombokKezdX) && (mousey > gombok.gombokKezdY)
                && (mousex < gombok.gombokKezdX+gombok.gombSzelesseg)
                && (mousey < gombok.gombokKezdY + gombok.gombMagassag ))   //Jatek
            {
                SDL_HideWindow(wmenu);
                pont = jatek_indit(10, 10, 30, 10, 300,feliratok);
                vege(pont);
                SDL_ShowWindow(wmenu);

            }

             else if ((mousex > gombok.gombokKezdX) &&
                     (mousey > gombok.gombokKezdY+gombok.gombMagassag*2) &&
                     (mousex < gombok.gombokKezdX+gombok.gombSzelesseg) && (mousey < gombok.gombokKezdY + gombok.gombMagassag*3))   //Jatek
            {
                SDL_HideWindow(wmenu);
                ranglista(gombok);
                SDL_ShowWindow(wmenu);
            }
        }
        debugmalloc_dump();
        SDL_RenderPresent(rmenu);
    }
    for(int i = 0; i <=8 ; i++)
        SDL_FreeSurface(feliratok[i]);

    /* idozito torlese */
    SDL_RemoveTimer(id);


    SDL_Quit();

    return 0;
}
