#ifndef JATEK_UTAN_H_INCLUDED
#define JATEK_UTAN_H_INCLUDED
#include "rajzol.h"
/*
eredm�ny rekord. n�v+pontsz�m
*/
typedef struct Eredmeny
{
    char nev[51];
    int pont;
} Eredmeny;

void gyozelemm();
void vege(int pont);
void beolvas(Eredmeny *eredmenyek);
void fajlt_ir( Eredmeny *eredmenyek);
void kozvetlen_csere(Eredmeny *t, int db);

#endif // JATEK_UTAN_H_INCLUDED
