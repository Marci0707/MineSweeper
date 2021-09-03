#include "menu.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "rajzol.h"
#include <time.h>
#include "debugmalloc-impl.h"

/*
dinamikus memória felszabadítás a cellák után
param1: 2D grid a cellákból
param2: a pálya magassága
*/
void felszabadit(Cella** palya,int magassag)
{
    for(int y = 0; y < magassag; y ++)
        free(palya[y]);
    free(palya);

}

/*
A menüt megejelnítő függvény
param1: menüablak
param2: A Gombokat leíró objektum
*/
void menuspec(SDL_Renderer* rmenu,GombHelyzet gombok)
{


    SDL_SetRenderDrawColor(rmenu, 40, 240, 4, 255);

    rectangleRGBA(rmenu, gombok.gombokKezdX, gombok.gombokKezdY + gombok.gombMagassag*0, gombok.gombokKezdX+gombok.gombSzelesseg, gombok.gombokKezdY + gombok.gombMagassag*1, 255, 0, 0, 255);
    rectangleRGBA(rmenu, gombok.gombokKezdX, gombok.gombokKezdY + gombok.gombMagassag*2, gombok.gombokKezdX+gombok.gombSzelesseg, gombok.gombokKezdY + gombok.gombMagassag*3, 0, 0, 255, 255);

    SDL_Color feher = {255, 255, 255};

    szKiir(30, feher, rmenu, "Aknakereso jatek - menu",50, 50);
    szKiir(20, feher, rmenu, "inditas", gombok.gombokKezdX+100, gombok.gombokKezdY+gombok.gombMagassag*0+5);
    szKiir(20, feher, rmenu, "ranglista", gombok.gombokKezdX+100, gombok.gombokKezdY+gombok.gombMagassag*2+5);
}
/* ez a fuggveny hivodik meg az idozito altal.
 * betesz a feldolgozando esemenyek koze (push) egy felhasznaloi esemenyt */
Uint32 idozit(Uint32 ms, void *param)
{
    SDL_Event ev;
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    return ms;   /* ujabb varakozas */
}

/*
kiírja a hátralevő időt
param1: hátralevő idő
param2: a renderer
param3: a pályát leíró objektum
param4: cellaoldal mérete
*/
void idokiir(int hatralevo_ido,SDL_Renderer *renderer,PalyaTul jellemzok,int oldal)
{
    char t[7];
    itoa( hatralevo_ido,t, 10);

    SDL_Color szin = {200, 200, 30};
    szKiir(20, szin, renderer, " hatralevo ido :",10, jellemzok.magassag * oldal + 10 );
    boxRGBA(renderer, 150,  jellemzok.magassag * oldal + 10,  200,  jellemzok.magassag * oldal + 30, 0, 0, 0, 255);
    szKiir(20, szin, renderer, t ,150, jellemzok.magassag * oldal + 10 );
}

/*
kiértékeli a játékot,számít egy pontszámot, kiírja a pontszámot és felszabadítja a memóriát.
param1: mennyi aknát jelölt meg
param2: pályaleíró objektum
param3: rendelkezésre állt idő
param4: hátralevő idő
param5: renderer
param6: window
*/

int kiertekel(int megjelolt_aknak,PalyaTul jellemzok,int ido1,int hatralevo_ido,SDL_Renderer *renderer, SDL_Window *window)
{
    SDL_RenderPresent(renderer);
    Sleep(2000);
    int pontszam = megjelolt_aknak * jellemzok.szelesseg*jellemzok.magassag / (ido1 - hatralevo_ido);

    SDL_DestroyWindow(window);
    felszabadit(jellemzok.palya,jellemzok.magassag);
    return pontszam;
}

/*
feltárja a pályát a játék végén
param1: pályaleíró obj
param2: az aknák szomszédjait elmondó feliratok 0-8
param3: renderer
param4: cellaoldal mérete
param6: megejelölt aknák száma
param7: hátralevő idő
param8: összes idő
param9: window
*/
int  feltaras(PalyaTul jellemzok,SDL_Surface *feliratok[],SDL_Renderer *renderer,int oldal,int megjelolt_aknak,int hatralevo_ido,int ido1,SDL_Window *window){
    for(int j = 0; j < jellemzok.magassag; j++)
    {
        for(int i = 0; i < jellemzok.szelesseg; i++)
        {
            felfed(&jellemzok,j,i);
            rajzolas(jellemzok, oldal, renderer,feliratok);
        }
    }
    return kiertekel(megjelolt_aknak,jellemzok,ido1,hatralevo_ido,renderer,window);
}


/*
elindítja a játékot
param1: pályaszélesség
param2: pályamagasság
param3: cellaoldal
param4: hány db akna van
param5: rendelkezésre álló idő
param6: feliratok amiket ki kell írni
*/
int jatek_indit(int szelesseg, int magassag, int oldal, int db, int ido1,SDL_Surface *feliratok[])
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    sdl_init("aknakereso jatek", szelesseg * oldal +1, magassag * oldal +1+50, &window, &renderer);

    Cella ** palya;
    palya = (Cella**) malloc(magassag * sizeof(Cella*));
    for (int y = 0; y < magassag; ++y)
        palya[y] = (Cella*) malloc(szelesseg * sizeof(Cella));


    PalyaTul jellemzok = {szelesseg, magassag, palya};
    feltolt(&jellemzok);
    srand(time(0));
    akna_feltolt(db, &jellemzok);
    szomszedok_feltolt(&jellemzok);

    SDL_RenderPresent(renderer);
    rajzolas(jellemzok, oldal, renderer,feliratok);

    int hatralevo_ido = ido1;
    int megjelolt_aknak = 0;
    int pontszam = 0;
    SDL_Event ev;

    bool stop = false;
    while (!stop)
    {
        SDL_WaitEvent(&ev);

        if(ev.type == SDL_QUIT || (ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_CLOSE)){
            felszabadit(palya,magassag);
            SDL_Quit();
        }
        switch (ev.type)
        {
        case SDL_USEREVENT:
            hatralevo_ido -= 1;
            idokiir(hatralevo_ido,renderer,jellemzok,oldal);

            if(hatralevo_ido == 0)
               {
                    pontszam = feltaras(jellemzok,feliratok,renderer,oldal,megjelolt_aknak,hatralevo_ido,ido1,window);
                    return pontszam;
               }

            SDL_RenderPresent(renderer);
            break;


        case SDL_MOUSEBUTTONDOWN:
                if(ev.button.button == SDL_BUTTON_LEFT)
                {
                    int mousex, mousey;
                    SDL_GetMouseState(&mousex, &mousey);
                    for(int j = 0; j < magassag; j++)
                    {
                        for(int i = 0; i < szelesseg; i++)
                        {
                            Cella a = palya[j][i];
                            if ((mousex > a.x*oldal) && (mousex < a.x*oldal+oldal) && (mousey > a.y*oldal) && (mousey < a.y*oldal+oldal) && palya[j][i].allapot != feltart){
                                felfed(&jellemzok,j,i);
                                rajzolas(jellemzok, oldal, renderer,feliratok);
                                if (a.akna){
                                    pontszam = kiertekel(megjelolt_aknak,jellemzok,ido1,hatralevo_ido,renderer,window);
                                    return pontszam;
                                }
                            }
                        }
                    }
                }
                if(ev.button.button == SDL_BUTTON_RIGHT){
                    int mousex, mousey;
                    SDL_GetMouseState(&mousex, &mousey);
                    for(int j = 0; j < magassag; j++){
                        for(int i = 0; i <  szelesseg; i++){
                            Cella a = palya[j][i];
                            if ((mousex > a.x*oldal) && (mousex < a.x*oldal+oldal) && (mousey > a.y*oldal) && (mousey < a.y*oldal+oldal)){
                                if(palya[j][i].akna && palya[j][i].megjelolt == false)
                                    megjelolt_aknak ++;

                                palya[j][i].megjelolt = true;
                                if(megjelolt_aknak == db){
                                    for(int j = 0; j < magassag; j++)
                                    {
                                        for(int i = 0; i < szelesseg; i++)
                                        {
                                            felfed(&jellemzok,j,i);
                                            rajzolas(jellemzok, oldal, renderer,feliratok);
                                        }
                                    }
                                    pontszam = kiertekel(megjelolt_aknak,jellemzok,ido1,hatralevo_ido,renderer,window);
                                    gyozelemm();
                                    return pontszam;
                                }
                                rajzolas(jellemzok, oldal, renderer,feliratok);
                            }
                        }
                    }
                }
        }
        SDL_RenderPresent(renderer);
    }
    return pontszam;
}
/*
kiírja a ranglistát miután beolvasta
*/
void ranglista(GombHelyzet gombok)
{


    SDL_Window *wrang;
    SDL_Renderer *rrang;
    sdl_init("ranglista", 1200, 700, &wrang, &rrang);

    int gomb = 50;

    SDL_Color szin = {244, 53, 0};
    szKiir(40, szin, rrang, "Aknakereso jatek ",500, 30);
    szKiir(40, szin, rrang, " - ranglista",50, 70);
    szKiir(30, szin, rrang, " Vissza",555, 555);
    rectangleRGBA(rrang, 500,  550,  700,  600, 240,70, 20, 255);
    szKiir(30, szin, rrang, "gyoztes:  ",30, gomb *3);

    Eredmeny eredmenyek[11];
    beolvas( eredmenyek );

    for(int i = 0; i < 10; i++)
    {
        szKiir(20, szin, rrang,eredmenyek[i].nev,150, i * 40 + 160);

        char t[51];
        itoa(eredmenyek[i].pont, t, 10);
        szKiir(20, szin, rrang,t,1040, i * 40 + 160);
    }
    SDL_RenderPresent(rrang);

    SDL_Event ev;
    while (SDL_WaitEvent(&ev) && ev.type != SDL_QUIT)
    {
        if((ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_CLOSE))
        {
            SDL_Quit();
        }

        if (ev.type== SDL_MOUSEBUTTONDOWN)
        {
            if(ev.button.button == SDL_BUTTON_LEFT)
            {
                int mousex, mousey;
                SDL_GetMouseState(&mousex, &mousey);

                if ((mousex > 500) && (mousex < 700) && (mousey > 550) && (mousey < 600) )
                {
                    SDL_HideWindow(wrang);
                    return;
                }
            }
        }
    }
}


/*
létrehozza a számokat a pályához 0-8-ig
*/
void szamSurfaceLetrehoz(SDL_Surface *tomb[])
{
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("LiberationSerif-Regular.ttf", 32);
    if (!font)
    {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }
    SDL_Color fekete = {0, 0, 0}, feher = {255, 255, 255};

    SDL_Surface *felirat;


    for(int i = 0; i <=8 ;i++)
    {
        char c[2];
        itoa(i, c, 10);
        felirat = TTF_RenderUTF8_Shaded(font, c, fekete, feher);
        tomb[i] = felirat;
    }
}

