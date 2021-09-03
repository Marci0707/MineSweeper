#include "jatek_utan.h"
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "debugmalloc-impl.h"



/*
Lekezeli ha nyert a játékos. Megjelenít egy üdvözlő ablakot
*/

void gyozelemm()
{
    SDL_Window *wgyoz;
    SDL_Renderer *rgyoz;
    sdl_init("Gyozelem!", 400, 400, &wgyoz, &rgyoz);
    SDL_Color szin = {24, 2, 231};
    szKiir(40, szin, rgyoz, "Gratu, gyoztel!:)!",50, 50);

    szKiir(30, szin, rgyoz, " Tovabb ",165, 350);
    rectangleRGBA(rgyoz, 160,  345,  280,  390, 0,233, 240, 255);

    SDL_RenderPresent(rgyoz);

    SDL_Event ev;

    while ((SDL_WaitEvent(&ev) && ev.type != SDL_QUIT))
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
                if ((mousex > 160) && (mousex < 260) && (mousey > 345) && (mousey < 390) )
                {
                    SDL_DestroyWindow(wgyoz);
                    return;
                }
            }
        }
    }


}

/*
A játék végét jelentő ablak. Bekéri a játékos nevét és hozzáadja a ranglistához a pontszámával
*/
void vege(int pont)
{
    SDL_Window *wvege;
    SDL_Renderer *rvege;
    sdl_init("vege", 400, 400, &wvege, &rvege);
    SDL_Color szin = {24, 2, 231};
    szKiir(40, szin, rvege, "Jatek vege!",150, 50);

    szKiir(30, szin, rvege, " Vissza",165, 350);
    rectangleRGBA(rvege, 160,  345,  260,  390, 0,233, 240, 255);

    SDL_Color hatter = {0,0,0};
    SDL_Color szoveg = {244, 244, 244};

    TTF_Font *font = TTF_OpenFont("LiberationSerif-Regular.ttf", 32);
    if (font == NULL)
    {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    Eredmeny eredmenyek[11];

    FILE *f;
    f = fopen("rang.txt","r");
    if(f==NULL)
    {
        f = fopen("rang.txt","w");
        for(int i = 0; i < 20; i++)
        {
            if(i%2==0)
                fprintf(f,"nev\n");
            else
                fprintf(f,"0\n");
        }
    }
    fclose(f);
    beolvas(eredmenyek);

    char nev[51];
    SDL_Rect G = {100, 180, 200,  40};
    input_text(nev,51*sizeof(char), G,  hatter,  szoveg,  font, rvege);

    eredmenyek[10].pont = pont;
    strcpy(eredmenyek[10].nev, nev);
    kozvetlen_csere(eredmenyek, 11);
    fajlt_ir(eredmenyek);
    SDL_RenderPresent(rvege);

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
                if ((mousex > 160) && (mousex < 260) && (mousey > 345) && (mousey < 390) )
                {
                    SDL_DestroyWindow(wvege);
                    return;
                }
            }
        }
    }

}

/*
Beolvassa a ranglistát a fileból
*/
void beolvas(Eredmeny *eredmenyek)
{
    FILE* f = fopen("rang.txt", "r");
    for (int i = 0; i < 10; i++)
    {

        if(fgets(eredmenyek[i].nev, 51, f)!= NULL){
            int a = strlen(eredmenyek[i].nev);
            eredmenyek[i].nev[a-1] = '\0';
            char t[51];
            fgets(t, 51, f);
            eredmenyek[i].pont = atoi(t);
        }
        else{


            strcpy(eredmenyek[i].nev, "-");
            eredmenyek[i].pont = 0;
        }
    }
    fclose(f);
}

/*
Kiírja a ranglistát a fileba
*/
void fajlt_ir( Eredmeny *eredmenyek)
{

    FILE *f = fopen("rang.txt", "w");
    for(int i = 0; i < 20; i++)
    {
        if(i%2 == 0)
        {
            fprintf(f,"%s \n", eredmenyek[i/2].nev);
        }
        else
        {
            fprintf(f,"%d \n",eredmenyek[i/2].pont );
        }
    }
    fclose(f);
}

/*
megekeresi az új eredmény helyét a listában
*/
void kozvetlen_csere(Eredmeny *t, int db)
{
    for (int i = 0; i < db-1; ++i)
    {
        int maxIndex = i;            /* maximum keresése */
        for (int j = i+1; j < db; ++j)
            if (t[j].pont > t[maxIndex].pont)
                maxIndex = j;

        if (maxIndex != i)
        {
            Eredmeny temp = t[maxIndex];
            t[maxIndex] = t[i];       /* csere. */
            t[i] = temp;
        }
    }
}



