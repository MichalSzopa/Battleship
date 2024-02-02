#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <dos.h>
#include "funkcje.h"


pole tablica_gracza[rozmiar][rozmiar], tablica_bota[rozmiar][rozmiar], tablica_pomocnicza[rozmiar][rozmiar]; /* plansza 10X10, ale zewnętrzne będą potrzebne */
short tablica_widocznosci[rozmiar - 2][rozmiar - 2] = { 0 }; /* false-nieodkryte, true- odkryte */
int trafieniag[statek_maks - statek_min + 1] = { 0 }, trafieniak[statek_maks - statek_min + 1] = { 0 }; /* w tych tablicach jest zapisywana ilosc trafien w konkretny typ statku dla obu graczy */
int  zatopieniag = 0, zatopieniak = 0; /* zatopienia obu graczy */
trafienie bylo_trafienie = { 0,0,0 }, pierwsze_trafienie = { 0,0,0 }; /* zapamietywanie trafien */


int main()
{

	textcolor(jasnoszary);
	srand(time(NULL));
	int obecny = menu;
	int czykoniec = 0; /* false */


	while (1)
	{
		switch (obecny)
		{
		case menu:
			czykoniec = handleMenu(&obecny, tablica_gracza, tablica_bota, tablica_pomocnicza, tablica_widocznosci);
			break;
		case gracz:
			handleGracz(&obecny, tablica_bota, tablica_widocznosci, trafieniag, &zatopieniag);
			break;
		case bot:
			handleBot(&obecny, tablica_gracza, tablica_pomocnicza, trafieniak, &zatopieniak, &bylo_trafienie, &pierwsze_trafienie, tablica_widocznosci);
			break;
		case pomoc:
			handlePomoc(&obecny);
		}
		if (czykoniec)
			break;
	}

	return 0;
}
