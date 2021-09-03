#include "jatek.h"
#include <string.h>
#include <stdlib.h>
#include "debugmalloc-impl.h"


/*
A PályaTul(ajdonságok) objektum cellákkal való feltöltése
param1: A pályaleíró objektum
*/

void feltolt(PalyaTul* jellemzok)
{
    int szelesseg = jellemzok->szelesseg;
    int magassag = jellemzok->magassag;

    for(int j = 0; j < magassag; j ++)
    {
        for( int i = 0; i < szelesseg; i++)
        {
            int szomszedok = 0;
            Cella a = {j,i,false,fedett,szomszedok,false};
            jellemzok->palya[j][i] = a;
        }
    }
}

/*
A pályát a paraméterben kapott számú aknával feltölti
param1: aknák száma
param2: a pályaleíró objektum
*/
void akna_feltolt(int db, PalyaTul* jellemzok)
{
    while(db > 0)
    {
        int a;
        a = rand()%jellemzok->szelesseg;
        int b;
        b = rand()%jellemzok->magassag;

        if(jellemzok->palya[b][a].akna == false)
        {
            jellemzok->palya[b][a].akna = true;
            db--;
        }
    }
}


/*
Megszámolja egy cella bomba szomszédjait. Ha olyann mezőra hívták, ami maga is bomba, akkor -1et,
különben a bombaszomszédok számát adja vissza
param1: cella y poz
param2: cella x poz
param3: pályaleíró objektum
*/
int szomszedok(int y, int x, PalyaTul* jellemzok)
{
    if(jellemzok->palya[y][x].akna)
        return -1;
    else
    {
        int sz = 0;
        for(int i =  -1; i <= 1; i++)
        {
            for(int j = -1; j<= 1; j++)
            {
                if(x+i >= 0 && x+i < jellemzok->szelesseg && y+j>=0 && y+j < jellemzok->magassag)
                {
                    if(jellemzok->palya[y+j][x+i].akna)
                        sz++;
                }
            }
        }
        return sz;
    }
}


/*
Végigmegy a pályán és minden mezőnek beállítja a szomszédjait
param1: a pályaleíró objektum

*/
void szomszedok_feltolt(PalyaTul* jellemzok)
{
    for(int j = 0; j < jellemzok->magassag; j++)
    {
        for (int i = 0; i < jellemzok->szelesseg; i++)
        {
            jellemzok->palya[j][i].szomszed = szomszedok(j,i,jellemzok);
        }
    }
}


/*
Felfed egy cellát a játékosnak. Ha 0 a szomszédja az aknának, akkor rekúrzívan tovább
hívja a szomszédokra a feltárást
param1: pályaleíró objektum
param2: függőleges cella poz
param3: vízszintes cella poz
*/
void felfed(PalyaTul* jellemzok, int j, int i)
{
    jellemzok->palya[j][i].allapot = feltart;


    if(jellemzok->palya[j][i].akna)
    {

        for(int x = 0; x < jellemzok->szelesseg; x++)
        {
            for(int y = 0; y <jellemzok->magassag; y++)
            {
                jellemzok->palya[y][x].allapot = feltart;
            }
        }
    }
    else if(jellemzok->palya[j][i].szomszed == 0){

        for(int x = -1; x <= 1; x++)
        {
            for(int y = -1; y <=1; y++)
            {
                if(x+i >= 0 && x+i < jellemzok->szelesseg && y+j>=0 && y+j < jellemzok->magassag)
                {

                    if(jellemzok->palya[j+y][i+x].allapot != feltart)
                        felfed(jellemzok,j+y,i+x);
                    /*if(jellemzok->palya[j+y][i+x].allapot != feltart )
                        jellemzok->palya[j+y][i+x].allapot = feltart;*/
                }
            }
        }
    }
}


