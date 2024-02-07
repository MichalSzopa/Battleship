#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// TODO move consts, enums, types and functions to separate files

#define SIZE 12
#define RESULTS_X 34
#define RESULTS_Y 3
#define PLAYER_X 16
#define PLAYER_Y 7
#define BOT_X 46
#define BOT_Y 7
#define COMMENTS_X 31
#define COMMENTS_Y 19
#define LEGEND_X 60
#define LEGEND_Y 10
#define DIALOG_X 5
#define DIALOG_Y 12
#define MENU_X 38
#define MENU_Y 10
#define MAX_SHIP_SIZE 6
#define MIN_SHIP_SIZE 2

enum State
{
	MENU = 0,
	PLAYER = 1,
	BOT = 2,
	HELP = 3
};

enum FieldType
{
	EMPTY = 0,
	SHIP_INTACT = 1,
	SHIP_HIT = 2,
	SHIP_SUNK = 3,
	NEARBY = 4,
	FRAME = 5,
	MISSED = 6,
	TEMPORARILY_EXCLUDED = 7
};

enum Colors
{
	BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	TURQUOISE = 3,
	RED = 4,
	PURPLE = 5,
	BROWN = 6,
	LIGHT_GREY = 7,
	DARK_GREY = 8,
	LIGHT_BLUE = 9,
	LIGHT_GREEN = 10,
	LIGHT_TURQUOISE = 11,
	LIGHT_RED = 12,
	LIGHT_PURPLE = 13,
	YELLOW = 14,
	WHITE = 15
};

typedef struct
{
	int X;
	int Y;
} Coordinates;

typedef struct
{
	int ShipSize;
	int Status;
} Field;

typedef struct
{
	int X;
	int Y;
	short Direction; // false - drawing goes down, true - drawing goes right
	short Ok;		 // true when finished drawing
} DrawingTable;

typedef struct
{
	short HitSuccessful;
	int X;
	int Y;
} Hit;

void ClearTable(DrawingTable table[])
{
	for (int i = 0; i < (SIZE - 2) * (SIZE - 2 - MIN_SHIP_SIZE + 1) * 2; i++)
	{
		table[i].X = 0;
		table[i].Y = 0;
		table[i].Direction = 0;
		table[i].Ok = 0;
	}
}

void GoToXY(int x, int y)
{
	// COORD c;
	// c.X = x - 1;
	// c.Y = y - 1;
	// SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void DrawShips(Field drawnTable[SIZE][SIZE], DrawingTable table[])
{
	// for (int i = 0; i < rozmiar - 2; i++)
	// {
	// 	for (int j = 0; j < rozmiar - 2; j++)
	// 	{
	// 		tr[i + 1][j + 1].status = puste;
	// 	}
	// }

	// short a;
	// short czyok;
	// int iter;
	// for (int i = statek_maks; i > statek_min - 1; i--) /* i odpowiada rozmiarowi skatku */
	// {
	// 	czysct(t);
	// 	iter = 0;
	// 	a = 0; /* false */
	// 	for (int x = 1; x < rozmiar - 1; x++)
	// 	{
	// 		for (int y = 1; y < rozmiar - 1 - i; y++)
	// 		{
	// 			czyok = 1; /* true */
	// 			for (int j = 0; j < i; j++)
	// 			{
	// 				if (tr[x][y + j].status != puste)
	// 				{
	// 					czyok = 0; /* false */
	// 					break;
	// 				}
	// 			}
	// 			if (czyok)
	// 			{
	// 				t[iter].x = x;
	// 				t[iter].y = y;
	// 				t[iter].a = a;
	// 				t[iter].ok = 1; /* true */
	// 				iter++;
	// 			}
	// 		}
	// 	}
	// 	a = 1; /* true */
	// 	for (int y = 1; y < rozmiar - 1; y++)
	// 	{
	// 		for (int x = 1; x < rozmiar - i; x++)
	// 		{
	// 			czyok = 1; /* true */
	// 			for (int j = 0; j < i; j++)
	// 			{
	// 				if (tr[x + j][y].status != puste)
	// 				{
	// 					czyok = 0; /* false */
	// 					break;
	// 				}
	// 			}
	// 			if (czyok)
	// 			{
	// 				t[iter].x = x;
	// 				t[iter].y = y;
	// 				t[iter].a = a;
	// 				t[iter].ok = 1; /* true */
	// 				iter++;
	// 			}
	// 		}
	// 	}
	// 	iter = 0;
	// 	while (t[iter].ok)
	// 	{
	// 		iter++;
	// 	}
	// 	iter = rand() % (iter - 1);
	// 	a = t[iter].a;
	// 	if (!a)
	// 	{
	// 		for (int j = 0; j < i; j++)
	// 		{
	// 			tr[t[iter].x][t[iter].y + j].status = nienaruszony;
	// 			tr[t[iter].x - 1][t[iter].y + j].status = obok;
	// 			tr[t[iter].x + 1][t[iter].y + j].status = obok;
	// 			tr[t[iter].x][t[iter].y + j].typ_statku = i;
	// 		}
	// 		for (int j = 0; j < 3; j++)
	// 		{
	// 			tr[t[iter].x - 1 + j][t[iter].y - 1].status = obok;
	// 			tr[t[iter].x - 1 + j][t[iter].y + i].status = obok;
	// 		}
	// 	}
	// 	else
	// 	{
	// 		for (int j = 0; j < i; j++)
	// 		{
	// 			tr[t[iter].x + j][t[iter].y].status = nienaruszony;
	// 			tr[t[iter].x + j][t[iter].y - 1].status = obok;
	// 			tr[t[iter].x + j][t[iter].y + 1].status = obok;
	// 			tr[t[iter].x + j][t[iter].y].typ_statku = i;
	// 		}
	// 		for (int j = 0; j < 3; j++)
	// 		{
	// 			tr[t[iter].x - 1][t[iter].y - 1 + j].status = obok;
	// 			tr[t[iter].x + i][t[iter].y - 1 + j].status = obok;
	// 		}
	// 	}
	// 	Sleep(5);
	// }
	// for (int i = 0; i < rozmiar; i++)
	// {
	// 	tr[i][0].status = ramka;
	// 	tr[i][rozmiar - 1].status = ramka;
	// }
	// for (int i = 0; i < rozmiar; i++)
	// {
	// 	tr[0][i].status = ramka;
	// 	tr[rozmiar - 1][i].status = ramka;
	// }
}

void PrepareBoard()
{
	// system("cls");

	// /*tabela wynikow */
	// gotoxy(wyniki_x, wyniki_y);
	// printf("%c", (char)201);
	// for (int i = 0; i < 5; i++)
	// 	printf("%c", (char)205);
	// printf("%c", (char)187);
	// gotoxy(wyniki_x, wyniki_y + 1);
	// printf("%c", (char)186);
	// gotoxy(wyniki_x + 3, wyniki_y + 1);
	// printf(":");
	// gotoxy(wyniki_x + 6, wyniki_y + 1);
	// printf("%c", (char)186);
	// gotoxy(wyniki_x, wyniki_y + 2);
	// printf("%c", (char)186);
	// gotoxy(wyniki_x + 3, wyniki_y + 2);
	// printf(":");
	// gotoxy(wyniki_x + 6, wyniki_y + 2);
	// printf("%c", (char)186);
	// gotoxy(wyniki_x, wyniki_y + 3);
	// printf("%c", (char)200);
	// for (int i = 0; i < 5; i++)
	// 	printf("%c", (char)205);
	// printf("%c", (char)188);

	// /*tablica gracza */
	// gotoxy(gracz_x + 2, gracz_y);
	// printf("0123456789");
	// gotoxy(gracz_x + 1, gracz_y + 1);
	// printf("%c", (char)201);
	// for (int i = 0; i < rozmiar - 2; i++)
	// 	printf("%c", (char)205);
	// printf("%c", (char)187);
	// for (int i = 0; i < rozmiar - 2; i++)
	// {
	// 	gotoxy(gracz_x + 1, gracz_y + 2 + i);
	// 	printf("%c", (char)186);
	// 	gotoxy(gracz_x + rozmiar, gracz_y + 2 + i);
	// 	printf("%c", (char)186);
	// }
	// gotoxy(gracz_x + 1, gracz_y + rozmiar);
	// printf("%c", (char)200);
	// for (int i = 0; i < rozmiar - 2; i++)
	// 	printf("%c", (char)205);
	// printf("%c", (char)188);
	// for (int i = 0; i < rozmiar - 2; i++)
	// {
	// 	gotoxy(gracz_x, gracz_y + 2 + i);
	// 	printf("%c", (char)(i + 'A'));
	// }

	// /*tablica przeciwnika */
	// gotoxy(bot_x + 2, bot_y);
	// printf("0123456789");
	// gotoxy(bot_x + 1, bot_y + 1);
	// printf("%c", (char)201);
	// for (int i = 0; i < rozmiar - 2; i++)
	// 	printf("%c", (char)205);
	// printf("%c", (char)187);
	// for (int i = 0; i < rozmiar - 2; i++)
	// {
	// 	gotoxy(bot_x + 1, bot_y + 2 + i);
	// 	printf("%c", (char)186);
	// 	gotoxy(bot_x + rozmiar, bot_y + 2 + i);
	// 	printf("%c", (char)186);
	// }
	// gotoxy(bot_x + 1, bot_y + rozmiar);
	// printf("%c", (char)200);
	// for (int i = 0; i < rozmiar - 2; i++)
	// 	printf("%c", (char)205);
	// printf("%c", (char)188);
	// for (int i = 0; i < rozmiar - 2; i++)
	// {
	// 	gotoxy(bot_x, bot_y + 2 + i);
	// 	printf("%c", (char)(i + 'A'));
	// }

	// /*tablica komentarzy */
	// gotoxy(komentarze_x, komentarze_y);
	// printf("%c", (char)201);
	// for (int i = 0; i < 11; i++)
	// 	printf("%c", (char)205);
	// printf("%c", (char)187);
	// for (int i = 0; i < 3; i++)
	// {
	// 	gotoxy(komentarze_x, komentarze_y + 1 + i);
	// 	printf("%c           %c", (char)186, (char)186);
	// }
	// gotoxy(komentarze_x, komentarze_y + 4);
	// printf("%c", (char)200);
	// for (int i = 0; i < 11; i++)
	// 	printf("%c", (char)205);
	// printf("%c", (char)188);

	// /*legenda */
	// gotoxy(legenda_x, legenda_y);
	// SetTextColor(jasnozielony);
	// printf("H");
	// SetTextColor(7);
	// printf(" - STATEK");
	// gotoxy(legenda_x, legenda_y + 1);
	// SetTextColor(jasnoszary);
	// printf("O");
	// printf(" - PUSTE/NIEODKRYTE");
	// gotoxy(legenda_x, legenda_y + 2);
	// SetTextColor(jasnoczerwony);
	// printf("X");
	// SetTextColor(7);
	// printf(" - TRAFIONY");
	// gotoxy(legenda_x, legenda_y + 3);
	// SetTextColor(jasnoniebieski);
	// printf("Z");
	// SetTextColor(7);
	// printf(" - ZATOPIONY");
	// gotoxy(legenda_x, legenda_y + 4);
	// SetTextColor(brazowy);
	// printf("&");
	// SetTextColor(7);
	// printf(" - PUDLO");

	// /* "okno" dialogowe ;) */
	// gotoxy(dialog_x, dialog_y);
	// printf("%c%c%c%c", (char)201, (char)205, (char)205, (char)187);
	// gotoxy(dialog_x, dialog_y + 1);
	// printf("%c  %c", (char)186, (char)186);
	// gotoxy(dialog_x, dialog_y + 2);
	// printf("%c%c%c%c", (char)200, (char)205, (char)205, (char)188);
}

void DrawPlayer(Field playerTable[SIZE][SIZE], short ifPlayer, short visibilityTable[SIZE - 2][SIZE - 2])
{
	// if (czyg)
	// {
	// 	for (int x = 1; x < rozmiar - 1; x++)
	// 	{
	// 		for (int y = 1; y < rozmiar - 1; y++)
	// 		{
	// 			gotoxy(x + gracz_x + 1, y + gracz_y + 1);
	// 			switch (tr[x][y].status)
	// 			{
	// 			case (0):
	// 			{
	// 				SetTextColor(jasnoszary);
	// 				printf("O");
	// 				break;
	// 			}
	// 			case (1):
	// 			{
	// 				SetTextColor(jasnozielony);
	// 				printf("H");
	// 				break;
	// 			}
	// 			case (2):
	// 			{
	// 				SetTextColor(jasnoczerwony);
	// 				printf("X");
	// 				break;
	// 			}
	// 			case (3):
	// 			{
	// 				SetTextColor(jasnoniebieski);
	// 				printf("Z");
	// 				break;
	// 			}
	// 			case (4):
	// 			{
	// 				SetTextColor(jasnoszary);
	// 				printf("O");
	// 				break;
	// 			}
	// 			case (6):
	// 			{
	// 				SetTextColor(brazowy);
	// 				printf("&");
	// 				break;
	// 			}
	// 			}
	// 		}
	// 	}
	// }
	// else
	// {
	// 	for (int x = 1; x < rozmiar - 1; x++)
	// 	{
	// 		for (int y = 1; y < rozmiar - 1; y++)
	// 		{
	// 			gotoxy(x + bot_x + 1, y + bot_y + 1);
	// 			if (tablica_widocznosci[x - 1][y - 1])
	// 			{
	// 				switch (tr[x][y].status)
	// 				{
	// 				case (5):
	// 					break;
	// 				case (0):
	// 				{
	// 					SetTextColor(jasnoszary);
	// 					printf("O");
	// 					break;
	// 				}
	// 				case (1):
	// 				{
	// 					SetTextColor(jasnozielony);
	// 					printf("H");
	// 					break;
	// 				}
	// 				case (2):
	// 				{
	// 					SetTextColor(jasnoczerwony);
	// 					printf("X");
	// 					break;
	// 				}
	// 				case (3):
	// 				{
	// 					SetTextColor(jasnoniebieski);
	// 					printf("Z");
	// 					break;
	// 					break;
	// 				}
	// 				case (6):
	// 				{
	// 					SetTextColor(brazowy);
	// 					printf("&");
	// 					break;
	// 				}
	// 				}
	// 			}
	// 			else
	// 			{
	// 				SetTextColor(jasnoszary);
	// 				printf("O");
	// 			}
	// 		}
	// 	}
	// }
}

void SetTextColor(int color)
{
	// WORD wColor;

	// HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	// CONSOLE_SCREEN_BUFFER_INFO csbi;
	// if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
	// {
	// 	wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
	// 	SetConsoleTextAttribute(hStdOut, wColor);
	// }
}

void ShowCursor(int showFlag)
{
	// HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	// CONSOLE_CURSOR_INFO cursorInfo;

	// GetConsoleCursorInfo(out, &cursorInfo);
	// cursorInfo.bVisible = showFlag;
	// SetConsoleCursorInfo(out, &cursorInfo);
}

void ClearDialog()
{
	// gotoxy(dialog_x, dialog_y);
	// printf("%c%c%c%c", (char)201, (char)205, (char)205, (char)187);
	// gotoxy(dialog_x, dialog_y + 1);
	// printf("%c  %c", (char)186, (char)186);
	// gotoxy(dialog_x, dialog_y + 2);
	// printf("%c%c%c%c", (char)200, (char)205, (char)205, (char)188);
}

void ClearComments()
{
	// gotoxy(komentarze_x + 1, komentarze_y + 1);
	// printf("           ");
	// gotoxy(komentarze_x + 1, komentarze_y + 2);
	// printf("           ");
	// gotoxy(komentarze_x + 1, komentarze_y + 3);
	// printf("           ");
	// return;
}

int HitsSum(int table[])
{
	// int suma = 0;
	// for (int i = statek_min - 2; i < statek_maks - 1; i++)
	// {
	// 	suma += tab[i];
	// }
	// return suma;
	return 0;
}

void PrepareFields(Coordinates fields[(SIZE - 2) * (SIZE - 2)])
{
	// for (int y = 1; y < rozmiar - 1; y++)
	// {
	// 	for (int x = 1; x < rozmiar - 1; x++)
	// 	{
	// 		pom[x][y].status = puste;
	// 	}
	// }
	// int opcja = rand() % 2;
	// for (int y = 1; y < rozmiar - 1; y++)
	// {
	// 	for (int x = 1; x < rozmiar - 1; x += 2)
	// 	{
	// 		pom[x + (y + opcja) % 2][y].status = tym_wykl;
	// 	}
	// }
	// for (int i = 0; i < rozmiar; i++)
	// {
	// 	pom[0][i].status = ramka;
	// 	pom[rozmiar - 1][i].status = ramka;
	// }
	// for (int i = 0; i < rozmiar; i++)
	// {
	// 	pom[i][0].status = ramka;
	// 	pom[i][rozmiar - 1].status = ramka;
	// }
}

int HandleMenu(int *currentState, Field playersTable[SIZE][SIZE], Field botsTable[SIZE][SIZE], Field helpTable[SIZE][SIZE], short visibilityTable[SIZE - 2][SIZE - 2])
{
	// char a;
	// pokaz_kursor(0);
	// system("cls");

	// gotoxy(menu_x + 2, menu_y);
	// printf("MENU");
	// gotoxy(menu_x, menu_y + 1);
	// printf("1) Graj");
	// gotoxy(menu_x, menu_y + 2);
	// printf("2) Pomoc");
	// gotoxy(menu_x, menu_y + 3);
	// printf("3) Wyjdz");

	// a = _getch();
	// if (a == '1')
	// {
	// 	tabdorysst tablica_mozliwosci[(rozmiar - 2) * (rozmiar - 2 - statek_min + 1) * 2];
	// 	rysuj_statki(tablica_gracza, tablica_mozliwosci);
	// 	rysuj_statki(tablica_bota, tablica_mozliwosci);
	// 	przygotuj_plansze();
	// 	gotoxy(wyniki_x + 1, wyniki_y + 1);
	// 	printf("00:00");
	// 	gotoxy(wyniki_x + 1, wyniki_y + 2);
	// 	printf("00:00");
	// 	rysuj_gracza(tablica_gracza, 1, tablica_widocznosci);
	// 	rysuj_gracza(tablica_bota, 0, tablica_widocznosci);
	// 	przygotuj_pola(tablica_pomocnicza);
	// 	*obecny = gracz;
	// }
	// else if (a == '2')
	// {
	// 	*obecny = pomoc;
	// 	return 0;
	// }
	// else if (a == '3')
	// 	return 1;

	return 0;
}

void HandlePlayersMove(int *currentState, Field table[SIZE][SIZE], short visibilityTable[SIZE - 2][SIZE - 2], int hits[5], int *sinks)
{
	// SetTextColor(jasnoszary);
	// char wspolrzedne[20] = {0};

	// ClearComments();
	// gotoxy(komentarze_x + 3, komentarze_y + 1);
	// printf("PROSZE");
	// gotoxy(komentarze_x + 3, komentarze_y + 2);
	// printf("WPISAC");
	// gotoxy(komentarze_x + 1, komentarze_y + 3);
	// printf("WSPOLRZEDNE");

	// pokaz_kursor(1);

	// char x = -1;
	// char y = -1;

	// while ((y > rozmiar - 2) || (y < 0) || (x > rozmiar - 2) || (x < 0))
	// {
	// 	ClearDialog();
	// 	gotoxy(dialog_x + 1, dialog_y + 1);

	// 	scanf("%s", &wspolrzedne);

	// 	x = (wspolrzedne[1] - '0');
	// 	y = toupper(wspolrzedne[0]) - 'A';
	// }

	// ClearComments();
	// pokaz_kursor(0);

	// tablica_widocznosci[x][y] = 1;
	// if ((tab[x + 1][y + 1].status == puste) || (tab[x + 1][y + 1].status == obok))
	// {
	// 	tab[x + 1][y + 1].status = pudlo;
	// 	gotoxy(komentarze_x + 4, komentarze_y + 2);
	// 	printf("PUDLO");
	// }
	// else
	// {
	// 	if (tab[x + 1][y + 1].status == nienaruszony)
	// 	{
	// 		gotoxy(komentarze_x + 2, komentarze_y + 2);
	// 		printf("TRAFIONY");
	// 		tab[x + 1][y + 1].status = trafiony;
	// 		trafienia[tab[x + 1][y + 1].typ_statku - 2]++;
	// 		if (trafienia[tab[x + 1][y + 1].typ_statku - 2] == tab[x + 1][y + 1].typ_statku)
	// 		{
	// 			gotoxy(komentarze_x + 2, komentarze_y + 3);
	// 			printf("ZATOPIONY");
	// 			(*zatopienia)++;
	// 			for (int i = 1; i < rozmiar - 1; i++)
	// 			{
	// 				for (int j = 1; j < rozmiar - 1; j++)
	// 				{
	// 					if (tab[i][j].typ_statku == tab[x + 1][y + 1].typ_statku)
	// 						tab[i][j].status = zatopiony;
	// 				}
	// 			}
	// 		}
	// 	}
	// }
	// gotoxy(wyniki_x + 1, wyniki_y + 1);
	// printf("%02d", *zatopienia);
	// gotoxy(wyniki_x + 1, wyniki_y + 2);
	// printf("%02d", HitsSum(trafienia));
	// rysuj_gracza(tab, 0, tablica_widocznosci);
	// Sleep(900);
	// ClearComments();
	// ClearDialog();

	// if ((*zatopienia) == 5)
	// {
	// 	gotoxy(komentarze_x + 1, komentarze_y + 2);
	// 	printf("ZWYCIESTWO!");
	// 	_getch();
	// 	*obecny = menu;
	// 	return;
	// }

	// *obecny = bot;
}

void HandleBotsMove(int *currentState, Field table[SIZE][SIZE], Field helpTable[SIZE][SIZE], int hits[], int *sinks, Hit *thereWasAHit, Hit *firstHit, short visibilityTable[SIZE - 2][SIZE - 2])
{
	// SetTextColor(jasnoszary);
	// if (bylo_trafienie->bylo_t)
	// {
	// 	trafienie pula[4] = {0};
	// 	if (bylo_trafienie->y != 0)
	// 	{
	// 		pula[0].x = bylo_trafienie->x;
	// 		pula[0].y = bylo_trafienie->y - 1;
	// 		pula[0].bylo_t = 2;
	// 	}
	// 	if (bylo_trafienie->x != rozmiar - 2)
	// 	{
	// 		pula[1].x = bylo_trafienie->x + 1;
	// 		pula[1].y = bylo_trafienie->y;
	// 		pula[1].bylo_t = 2;
	// 	}
	// 	if (bylo_trafienie->y != rozmiar - 2)
	// 	{
	// 		pula[2].x = bylo_trafienie->x;
	// 		pula[2].y = bylo_trafienie->y + 1;
	// 		pula[2].bylo_t = 2;
	// 	}
	// 	if (bylo_trafienie->x != 0)
	// 	{
	// 		pula[3].x = bylo_trafienie->x - 1;
	// 		pula[3].y = bylo_trafienie->y;
	// 		pula[3].bylo_t = 2;
	// 	}

	// 	for (int i = 0; i < 4; i++)
	// 	{
	// 		if (pula[i].bylo_t == 2)
	// 		{
	// 			if ((pom[pula[i].x + 1][pula[i].y + 1].status == puste) || (pom[pula[i].x + 1][pula[i].y + 1].status == tym_wykl))
	// 				pula[i].bylo_t = 1;
	// 		}
	// 	}

	// 	trafienie los[4];
	// 	int ile = 0;
	// 	for (int i = 0; i < 4; i++)
	// 	{
	// 		if (pula[i].bylo_t == 1)
	// 		{
	// 			los[ile] = pula[i];
	// 			ile++;
	// 		}
	// 	}
	// 	if (ile == 0)
	// 	{
	// 		*bylo_trafienie = *pierwsze_trafienie;
	// 		*obecny = bot;
	// 		return;
	// 	}

	// 	int x = 0, y = 0, wybor;

	// 	wybor = rand() % ile;
	// 	x = los[wybor].x;
	// 	y = los[wybor].y;

	// 	ClearComments();

	// 	gotoxy(komentarze_x + 1, komentarze_y + 2);
	// 	printf("STRZELAM W:");
	// 	gotoxy(komentarze_x + 5, komentarze_y + 3);
	// 	printf("%c%d", y + 'A', x);
	// 	Sleep(900);

	// 	ClearComments();

	// 	if ((tab[x + 1][y + 1].status == puste) || (tab[x + 1][y + 1].status == obok))
	// 	{
	// 		tab[x + 1][y + 1].status = pudlo;
	// 		pom[x + 1][y + 1].status = pudlo;
	// 		gotoxy(komentarze_x + 4, komentarze_y + 2);
	// 		printf("PUDLO");
	// 	}
	// 	else
	// 	{
	// 		if (tab[x + 1][y + 1].status == 1)
	// 		{
	// 			gotoxy(komentarze_x + 2, komentarze_y + 2);
	// 			printf("TRAFIONY");
	// 			tab[x + 1][y + 1].status = trafiony;
	// 			pom[x + 1][y + 1].status = trafiony;
	// 			if ((x != 0) && (y != 0))
	// 				pom[x][y].status = obok;
	// 			if ((x != 0) && (y != rozmiar - 2))
	// 				pom[x][y + 2].status = obok;
	// 			if ((x != rozmiar - 2) && (y != 0))
	// 				pom[x + 2][y].status = obok;
	// 			if ((x != rozmiar - 2) && (y != rozmiar - 2))
	// 				pom[x + 2][y + 2].status = obok;
	// 			bylo_trafienie->bylo_t = 1; /* true */
	// 			bylo_trafienie->x = x;
	// 			bylo_trafienie->y = y;
	// 			trafienia[tab[x + 1][y + 1].typ_statku - 2]++;
	// 			if (trafienia[tab[x + 1][y + 1].typ_statku - 2] == tab[x + 1][y + 1].typ_statku)
	// 			{
	// 				gotoxy(komentarze_x + 2, komentarze_y + 3);
	// 				printf("ZATOPIONY");
	// 				(*zatopienia)++;
	// 				bylo_trafienie->bylo_t = 0;
	// 				for (int i = 1; i < rozmiar - 1; i++)
	// 				{
	// 					for (int j = 1; j < rozmiar - 1; j++)
	// 					{
	// 						if (tab[i][j].typ_statku == tab[x + 1][y + 1].typ_statku)
	// 							tab[i][j].status = zatopiony;
	// 					}
	// 				}
	// 			}
	// 		}
	// 	}
	// 	gotoxy(wyniki_x + 4, wyniki_y + 1);
	// 	printf("%02d", *zatopienia);
	// 	gotoxy(wyniki_x + 4, wyniki_y + 2);
	// 	printf("%02d", HitsSum(trafienia));
	// 	rysuj_gracza(tab, 1, tablica_widocznosci);
	// 	Sleep(900);
	// 	ClearComments();
	// 	ClearDialog();

	// 	if ((*zatopienia) == 5)
	// 	{
	// 		gotoxy(komentarze_x + 3, komentarze_y + 2);
	// 		printf("PORAZKA");
	// 		_getch();
	// 		*obecny = menu;
	// 		return;
	// 	}
	// }

	// else
	// {
	// 	trafienie los[(rozmiar - 2) * (rozmiar - 2) / 2];
	// 	int i = 0;
	// 	for (int x = 0; x < rozmiar - 2; x++)
	// 	{
	// 		for (int y = 0; y < rozmiar - 2; y++)
	// 		{
	// 			if (pom[x + 1][y + 1].status == puste)
	// 			{
	// 				los[i].x = x;
	// 				los[i].y = y;
	// 				i++;
	// 			}
	// 		}
	// 	}
	// 	int x = 0, y = 0, wybor;

	// 	wybor = rand() % i;
	// 	x = los[wybor].x;
	// 	y = los[wybor].y;

	// 	ClearComments();

	// 	gotoxy(komentarze_x + 1, komentarze_y + 2);
	// 	printf("STRZELAM W:");
	// 	gotoxy(komentarze_x + 5, komentarze_y + 3);
	// 	printf("%c%d", y + 'A', x);
	// 	Sleep(900);

	// 	ClearComments();

	// 	if ((tab[x + 1][y + 1].status == puste) || (tab[x + 1][y + 1].status == obok))
	// 	{
	// 		tab[x + 1][y + 1].status = pudlo;
	// 		pom[x + 1][y + 1].status = pudlo;
	// 		gotoxy(komentarze_x + 4, komentarze_y + 2);
	// 		printf("PUDLO");
	// 		bylo_trafienie->bylo_t = 0; /* false */
	// 	}
	// 	else
	// 	{
	// 		if (tab[x + 1][y + 1].status == nienaruszony)
	// 		{
	// 			gotoxy(komentarze_x + 2, komentarze_y + 2);
	// 			printf("TRAFIONY");
	// 			tab[x + 1][y + 1].status = trafiony;
	// 			pom[x + 1][y + 1].status = trafiony;
	// 			if ((x != 0) && (y != 0))
	// 				pom[x][y].status = obok;
	// 			if ((x != 0) && (y != rozmiar - 2))
	// 				pom[x][y + 2].status = obok;
	// 			if ((x != rozmiar - 2) && (y != 0))
	// 				pom[x + 2][y].status = obok;
	// 			if ((x != rozmiar - 2) && (y != rozmiar - 2))
	// 				pom[x + 2][y + 2].status = obok;
	// 			bylo_trafienie->bylo_t = 1; /* true */
	// 			bylo_trafienie->x = x;
	// 			bylo_trafienie->y = y;
	// 			*pierwsze_trafienie = *bylo_trafienie;
	// 			trafienia[tab[x + 1][y + 1].typ_statku - 2]++;
	// 			if (trafienia[tab[x + 1][y + 1].typ_statku - 2] == tab[x + 1][y + 1].typ_statku)
	// 			{
	// 				gotoxy(komentarze_x + 2, komentarze_y + 3);
	// 				printf("ZATOPIONY");
	// 				(*zatopienia)++;
	// 				for (int i = 1; i < rozmiar - 1; i++)
	// 				{
	// 					for (int j = 1; j < rozmiar - 1; j++)
	// 					{
	// 						if (tab[i][j].typ_statku == tab[x + 1][y + 1].typ_statku)
	// 							tab[i][j].status = zatopiony;
	// 					}
	// 				}
	// 			}
	// 		}
	// 	}
	// 	gotoxy(wyniki_x + 4, wyniki_y + 1);
	// 	printf("%02d", *zatopienia);
	// 	gotoxy(wyniki_x + 4, wyniki_y + 2);
	// 	printf("%02d", HitsSum(trafienia));
	// 	rysuj_gracza(tab, 1, tablica_widocznosci);
	// 	Sleep(900);
	// 	ClearComments();
	// 	ClearDialog();

	// 	if ((*zatopienia) == 5)
	// 	{
	// 		gotoxy(komentarze_x + 3, komentarze_y + 2);
	// 		printf("PORAZKA");
	// 		_getch();
	// 		*obecny = menu;
	// 		return;
	// 	}
	// }
	// *obecny = gracz;
}

void HandleHelp(int *currentState)
{
	// przygotuj_plansze();

	// gotoxy(dialog_x - 2, dialog_y - 3);
	// printf("Okno");
	// gotoxy(dialog_x - 2, dialog_y - 2);
	// printf("dialogowe");

	// gotoxy(gracz_x + 3, gracz_y + 5);
	// printf("Plansza");
	// gotoxy(gracz_x + 4, gracz_y + 6);
	// printf("gracza");

	// gotoxy(bot_x + 3, bot_y + 5);
	// printf("Plansza");
	// gotoxy(bot_x + 4, bot_y + 6);
	// printf("bota");

	// gotoxy(wyniki_x - 8, wyniki_y);
	// printf("Tablica");
	// gotoxy(wyniki_x - 8, wyniki_y + 1);
	// printf("wynikow");
	// gotoxy(wyniki_x + 7, wyniki_y + 1);
	// printf("zatopienia");
	// gotoxy(wyniki_x + 7, wyniki_y + 2);
	// printf("trafienia");
	// gotoxy(wyniki_x - 2, wyniki_y + 4);
	// printf("gracz:bot");

	// gotoxy(komentarze_x + 1, komentarze_y + 1);
	// printf("Okno");
	// gotoxy(komentarze_x + 2, komentarze_y + 2);
	// printf("komentarzy");

	// gotoxy(legenda_x, legenda_y - 2);
	// printf("Legenda:");

	// _getch();
	// *obecny = menu;
}

Field PlayersBoard[SIZE][SIZE], BotsBoard[SIZE][SIZE], HelpBoard[SIZE][SIZE];
short VisibilityTable[SIZE - 2][SIZE - 2] = {0};
int PlayersHits[MAX_SHIP_SIZE - MIN_SHIP_SIZE + 1] = {0}, BotsHits[MAX_SHIP_SIZE - MIN_SHIP_SIZE + 1] = {0};
int PlayersSinks = 0, BotsSinks = 0;
Hit ThereWasAHit = {0, 0, 0}, FirstHit = {0, 0, 0};

int main()
{
	SetTextColor(LIGHT_GREY);
	srand(time(NULL));
	int CurrentState = HELP; // TODO make it MENU
	int GameFinished = 1;	 // TODO make it 0

	while (1)
	{
		switch (CurrentState)
		{
		case MENU:
			GameFinished = HandleMenu(&CurrentState, PlayersBoard, BotsBoard, HelpBoard, VisibilityTable);
			break;
		case PLAYER:
			HandlePlayersMove(&CurrentState, BotsBoard, VisibilityTable, PlayersHits, &PlayersSinks);
			break;
		case BOT:
			HandleBotsMove(&CurrentState, PlayersBoard, HelpBoard, BotsHits, &BotsSinks, &ThereWasAHit, &FirstHit, VisibilityTable);
			break;
		case HELP:
			HandleHelp(&CurrentState);
		}
		if (GameFinished)
			break;
	}

	return 0;
}
