#ifndef funkcje_c
#define funkcje_c

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <dos.h>

#define rozmiar 12
#define wyniki_x 34 
#define wyniki_y 3
#define gracz_x 16
#define gracz_y 7
#define bot_x 46
#define bot_y 7
#define komentarze_x 31
#define komentarze_y 19
#define legenda_x 60
#define legenda_y 10
#define dialog_x 5
#define dialog_y 12
#define menu_x 38
#define menu_y 10
#define statek_maks 6
#define statek_min 2

enum stan { menu, gracz, bot, pomoc };
enum status { puste, nienaruszony, trafiony, zatopiony, obok, ramka, pudlo, tym_wykl }; /* tym_wykl- tymczasowo wykluczony, szachownica */
enum kolory { czarny, niebieski, zielony, turkusowy, czerwony, fioletowy, brazowy, jasnoszary, ciemnoszary, jasnoniebieski, jasnozielony, jasnoturkusowy, jasnoczerwony, jasnofioletowy, zolty, bialy };

typedef struct
{
	int x;
	int y;
} xy; /* do przechowywania pol z "szachownicy" */

typedef struct
{
	int typ_statku; /* 2,3,4,5,6 rozmiary */
	int status; /* 0-puste,1-nienaruszony statek,2-trafiony,3-zatopiony,4-obok statku, 5-ramka,6-pud³o,7-tym_wykl czyli tymczasowo wykluczony, "szachownica" */
} pole;

typedef struct  /*pole tablicy do generowania statkow */
{
	int x;
	int y;
	short a; /* false- w dol, true- w prawo */
	short ok; /* tablica zapelniana mozliwosciami, kiedy koniec to false */
} tabdorysst;

typedef struct
{
	short bylo_t;
	int x;
	int y;
} trafienie; /* struktura przechowujaca dane o trafieniu */


void czysct(tabdorysst t[]); /* do czyszczenia t */

void gotoxy(int x, int y); /* gotoxy z Pascala */

void rysuj_statki(pole tr[rozmiar][rozmiar], tabdorysst t[]); /* tr- tablica, w ktorej rysujemy */

void przygotuj_plansze(); /* rysuje ramki i oznaczenia wspolrzednych */

void rysuj_gracza(pole tr[rozmiar][rozmiar], short czyg, short tablica_widocznosciw[rozmiar - 2][rozmiar - 2]); /* czyg-czygracz, 1 jesli gracz 0 jesli komp */

void textcolor(int ForgC); /* textcolor z Pascala */

void pokaz_kursor(int showFlag); /* 1- kursor pokazany, 0- kursor schowany */

void czyscd(); /* czyszczenie okna dialogowego */

void czysck(); /* czyszczenie okna komentarzy */

int suma_trafien(int tab[]); /* zliczanie wszystkich trafien */

void przygotuj_pola(xy pola[(rozmiar - 2)*(rozmiar - 2)]); /* funkcja przygotowuje pola w ktore bedzie strzelal bot */

int handleMenu(int * obecny, pole tablica_gracza[rozmiar][rozmiar], pole tablica_bota[rozmiar][rozmiar], pole tablica_pomocnicza[rozmiar][rozmiar], short tablica_widocznosci[rozmiar - 2][rozmiar - 2]);

void handleGracz(int * obecny, pole tab[rozmiar][rozmiar], short tablica_widocznosci[rozmiar - 2][rozmiar - 2], int trafienia[5], int * zatopienia);

void handleBot(int * obecny, pole tab[rozmiar][rozmiar], pole pom[rozmiar][rozmiar], int trafienia[], int * zatopienia, trafienie * bylo_trafienie, trafienie * pierwsze_trafienie, short tablica_widocznosci[rozmiar - 2][rozmiar - 2]);

void handlePomoc(int * obecny);


#endif /* funkcje_c */

