#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>
#include <locale.h>
#include <termios.h>
#include <ctype.h>

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

#define RED L"\033[31m"
#define GREEN L"\033[32m"
#define YELLOW L"\033[33m"
#define BLUE L"\033[34m"
#define MAGENTA L"\033[35m"
#define CYAN L"\033[36m"
#define WHITE L"\033[37m"
#define RESET L"\033[0m"

#define TOP_LEFT_CORNER 0x2554
#define HORIZONTAL_WALL 0x2550
#define TOP_RIGHT_CORNER 0x2557
#define VERTICAL_WALL 0x2551
#define BOTTOM_LEFT_CORNER 0x255A
#define BOTTOM_RIGHT_CORNER 0x255D

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

char _getch(void)
{
	int ch;
	struct termios oldt, newt;

	// Get current terminal attributes
	tcgetattr(STDIN_FILENO, &oldt);

	// Make a copy of the attributes
	newt = oldt;

	// Modify the attributes to disable canonical mode and echo
	newt.c_lflag &= ~(ICANON | ECHO);

	// Apply the modified attributes
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	// Read a character
	ch = getchar();

	// Restore the original terminal attributes
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	return ch;
}

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
	wprintf(L"\033[%d;%dH", y, x);
}

void ClearScreen()
{
	wprintf(L"\e[1;1H\e[2J");
}

void DrawShips(Field drawnTable[SIZE][SIZE], DrawingTable table[])
{
	for (int i = 0; i < SIZE - 2; i++)
	{
		for (int j = 0; j < SIZE - 2; j++)
		{
			drawnTable[i + 1][j + 1].Status = EMPTY;
		}
	}

	short direction;
	short ifOk;
	int helpIterator;
	for (int i = MAX_SHIP_SIZE; i > MIN_SHIP_SIZE - 1; i--)
	{
		ClearTable(table);
		helpIterator = 0;
		direction = 0;
		for (int x = 1; x < SIZE - 1; x++)
		{
			for (int y = 1; y < SIZE - 1 - i; y++)
			{
				ifOk = 1;
				for (int j = 0; j < i; j++)
				{
					if (drawnTable[x][y + j].Status != EMPTY)
					{
						ifOk = 0;
						break;
					}
				}
				if (ifOk)
				{
					table[helpIterator].X = x;
					table[helpIterator].Y = y;
					table[helpIterator].Direction = direction;
					table[helpIterator].Ok = 1;
					helpIterator++;
				}
			}
		}
		direction = 1;
		for (int y = 1; y < SIZE - 1; y++)
		{
			for (int x = 1; x < SIZE - i; x++)
			{
				ifOk = 1;
				for (int j = 0; j < i; j++)
				{
					if (drawnTable[x + j][y].Status != EMPTY)
					{
						ifOk = 0;
						break;
					}
				}
				if (ifOk)
				{
					table[helpIterator].X = x;
					table[helpIterator].Y = y;
					table[helpIterator].Direction = direction;
					table[helpIterator].Ok = 1;
					helpIterator++;
				}
			}
		}
		helpIterator = 0;
		while (table[helpIterator].Ok)
		{
			helpIterator++;
		}
		helpIterator = rand() % (helpIterator - 1);
		direction = table[helpIterator].Direction;
		if (!direction)
		{
			for (int j = 0; j < i; j++)
			{
				drawnTable[table[helpIterator].X][table[helpIterator].Y + j].Status = SHIP_INTACT;
				drawnTable[table[helpIterator].X - 1][table[helpIterator].Y + j].Status = NEARBY;
				drawnTable[table[helpIterator].X + 1][table[helpIterator].Y + j].Status = NEARBY;
				drawnTable[table[helpIterator].X][table[helpIterator].Y + j].ShipSize = i;
			}
			for (int j = 0; j < 3; j++)
			{
				drawnTable[table[helpIterator].X - 1 + j][table[helpIterator].Y - 1].Status = NEARBY;
				drawnTable[table[helpIterator].X - 1 + j][table[helpIterator].Y + i].Status = NEARBY;
			}
		}
		else
		{
			for (int j = 0; j < i; j++)
			{
				drawnTable[table[helpIterator].X + j][table[helpIterator].Y].Status = SHIP_INTACT;
				drawnTable[table[helpIterator].X + j][table[helpIterator].Y - 1].Status = NEARBY;
				drawnTable[table[helpIterator].X + j][table[helpIterator].Y + 1].Status = NEARBY;
				drawnTable[table[helpIterator].X + j][table[helpIterator].Y].ShipSize = i;
			}
			for (int j = 0; j < 3; j++)
			{
				drawnTable[table[helpIterator].X - 1][table[helpIterator].Y - 1 + j].Status = NEARBY;
				drawnTable[table[helpIterator].X + i][table[helpIterator].Y - 1 + j].Status = NEARBY;
			}
		}
	}
	for (int i = 0; i < SIZE; i++)
	{
		drawnTable[i][0].Status = FRAME;
		drawnTable[i][SIZE - 1].Status = FRAME;
	}
	for (int i = 0; i < SIZE; i++)
	{
		drawnTable[0][i].Status = FRAME;
		drawnTable[SIZE - 1][i].Status = FRAME;
	}
}

void PrepareBoard()
{
	ClearScreen();

	// RESULTS
	GoToXY(RESULTS_X, RESULTS_Y);
	wprintf(L"%lc", TOP_LEFT_CORNER);
	for (int i = 0; i < 5; i++)
		wprintf(L"%lc", HORIZONTAL_WALL);

	wprintf(L"%lc", TOP_RIGHT_CORNER);
	GoToXY(RESULTS_X, RESULTS_Y + 1);
	wprintf(L"%lc", VERTICAL_WALL);
	GoToXY(RESULTS_X + 3, RESULTS_Y + 1);
	wprintf(L":");
	GoToXY(RESULTS_X + 6, RESULTS_Y + 1);
	wprintf(L"%lc", VERTICAL_WALL);
	GoToXY(RESULTS_X, RESULTS_Y + 2);
	wprintf(L"%lc", VERTICAL_WALL);
	GoToXY(RESULTS_X + 3, RESULTS_Y + 2);
	wprintf(L":");
	GoToXY(RESULTS_X + 6, RESULTS_Y + 2);
	wprintf(L"%lc", VERTICAL_WALL);
	GoToXY(RESULTS_X, RESULTS_Y + 3);
	wprintf(L"%lc", BOTTOM_LEFT_CORNER);
	for (int i = 0; i < 5; i++)
		wprintf(L"%lc", HORIZONTAL_WALL);
	wprintf(L"%lc", BOTTOM_RIGHT_CORNER);

	// PLAYER TABLE
	GoToXY(PLAYER_X + 2, PLAYER_Y);
	wprintf(L"0123456789");
	GoToXY(PLAYER_X + 1, PLAYER_Y + 1);
	wprintf(L"%lc", TOP_LEFT_CORNER);
	for (int i = 0; i < SIZE - 2; i++)
		wprintf(L"%lc", HORIZONTAL_WALL);
	wprintf(L"%lc", TOP_RIGHT_CORNER);
	for (int i = 0; i < SIZE - 2; i++)
	{
		GoToXY(PLAYER_X + 1, PLAYER_Y + 2 + i);
		wprintf(L"%lc", VERTICAL_WALL);
		GoToXY(PLAYER_X + SIZE, PLAYER_Y + 2 + i);
		wprintf(L"%lc", VERTICAL_WALL);
	}
	GoToXY(PLAYER_X + 1, PLAYER_Y + SIZE);
	wprintf(L"%lc", BOTTOM_LEFT_CORNER);
	for (int i = 0; i < SIZE - 2; i++)
		wprintf(L"%lc", HORIZONTAL_WALL);
	wprintf(L"%lc", BOTTOM_RIGHT_CORNER);
	for (int i = 0; i < SIZE - 2; i++)
	{
		GoToXY(PLAYER_X, PLAYER_Y + 2 + i);
		wprintf(L"%lc", (wchar_t)(i + 'A'));
	}

	// BOT TABLE
	GoToXY(BOT_X + 2, BOT_Y);
	wprintf(L"0123456789");
	GoToXY(BOT_X + 1, BOT_Y + 1);
	wprintf(L"%lc", TOP_LEFT_CORNER);
	for (int i = 0; i < SIZE - 2; i++)
		wprintf(L"%lc", HORIZONTAL_WALL);
	wprintf(L"%lc", TOP_RIGHT_CORNER);
	for (int i = 0; i < SIZE - 2; i++)
	{
		GoToXY(BOT_X + 1, BOT_Y + 2 + i);
		wprintf(L"%lc", VERTICAL_WALL);
		GoToXY(BOT_X + SIZE, BOT_Y + 2 + i);
		wprintf(L"%lc", VERTICAL_WALL);
	}
	GoToXY(BOT_X + 1, BOT_Y + SIZE);
	wprintf(L"%lc", BOTTOM_LEFT_CORNER);
	for (int i = 0; i < SIZE - 2; i++)
		wprintf(L"%lc", HORIZONTAL_WALL);
	wprintf(L"%lc", BOTTOM_RIGHT_CORNER);
	for (int i = 0; i < SIZE - 2; i++)
	{
		GoToXY(BOT_X, BOT_Y + 2 + i);
		wprintf(L"%lc", (wchar_t)(i + 'A'));
	}

	// COMMENTS
	GoToXY(COMMENTS_X, COMMENTS_Y);
	wprintf(L"%lc", TOP_LEFT_CORNER);
	for (int i = 0; i < 11; i++)
		wprintf(L"%lc", HORIZONTAL_WALL);
	wprintf(L"%lc", TOP_RIGHT_CORNER);
	for (int i = 0; i < 3; i++)
	{
		GoToXY(COMMENTS_X, COMMENTS_Y + 1 + i);
		wprintf(L"%lc           %lc", VERTICAL_WALL, VERTICAL_WALL);
	}
	GoToXY(COMMENTS_X, COMMENTS_Y + 4);
	wprintf(L"%lc", BOTTOM_LEFT_CORNER);
	for (int i = 0; i < 11; i++)
		wprintf(L"%lc", HORIZONTAL_WALL);
	wprintf(L"%lc", BOTTOM_RIGHT_CORNER);

	// LEGEND
	GoToXY(LEGEND_X, LEGEND_Y);
	wprintf(GREEN L"H" RESET);
	wprintf(L" - SHIP");
	GoToXY(LEGEND_X, LEGEND_Y + 1);
	wprintf(L"O");
	wprintf(L" - EMPTY/UNDISCOVERED");
	GoToXY(LEGEND_X, LEGEND_Y + 2);
	wprintf(RED L"X" RESET);
	wprintf(L" - HIT");
	GoToXY(LEGEND_X, LEGEND_Y + 3);
	wprintf(BLUE L"Z" RESET);
	wprintf(L" - SUNK");
	GoToXY(LEGEND_X, LEGEND_Y + 4);
	wprintf(CYAN L"&" RESET);
	wprintf(L" - MISS");

	// DIALOG BOX
	GoToXY(DIALOG_X, DIALOG_Y);
	wprintf(L"%lc%lc%lc%lc", TOP_LEFT_CORNER, HORIZONTAL_WALL, HORIZONTAL_WALL, TOP_RIGHT_CORNER);
	GoToXY(DIALOG_X, DIALOG_Y + 1);
	wprintf(L"%lc  %lc", VERTICAL_WALL, VERTICAL_WALL);
	GoToXY(DIALOG_X, DIALOG_Y + 2);
	wprintf(L"%lc%lc%lc%lc", BOTTOM_LEFT_CORNER, HORIZONTAL_WALL, HORIZONTAL_WALL, BOTTOM_RIGHT_CORNER);

	fflush(stdout);
}

void DrawPlayer(Field playerTable[SIZE][SIZE], short ifPlayer, short visibilityTable[SIZE - 2][SIZE - 2])
{
	if (ifPlayer)
	{
		for (int x = 1; x < SIZE - 1; x++)
		{
			for (int y = 1; y < SIZE - 1; y++)
			{
				GoToXY(x + PLAYER_X + 1, y + PLAYER_Y + 1);
				switch (playerTable[x][y].Status)
				{
				case (EMPTY):
				{
					wprintf(L"O");
					break;
				}
				case (SHIP_INTACT):
				{
					wprintf(GREEN L"H" RESET);
					break;
				}
				case (SHIP_HIT):
				{
					wprintf(RED L"X" RESET);
					break;
				}
				case (SHIP_SUNK):
				{
					wprintf(BLUE L"Z" RESET);
					break;
				}
				case (NEARBY):
				{
					wprintf(L"O");
					break;
				}
				case (MISSED):
				{
					wprintf(CYAN L"&" RESET);
					break;
				}
				}
			}
		}
	}
	else
	{
		for (int x = 1; x < SIZE - 1; x++)
		{
			for (int y = 1; y < SIZE - 1; y++)
			{
				GoToXY(x + BOT_X + 1, y + BOT_Y + 1);
				if (visibilityTable[x - 1][y - 1])
				{
					switch (playerTable[x][y].Status)
					{
					case (FRAME):
						break;
					case (EMPTY):
					{
						wprintf(L"O");
						break;
					}
					case (SHIP_INTACT):
					{
						wprintf(GREEN L"H" RESET);
						break;
					}
					case (SHIP_HIT):
					{
						wprintf(RED L"X" RESET);
						break;
					}
					case (SHIP_SUNK):
					{
						wprintf(BLUE L"Z" RESET);
						break;
					}
					case (MISSED):
					{
						wprintf(CYAN L"&" RESET);
						break;
					}
					}
				}
				else
				{
					wprintf(L"O");
				}
			}
		}
	}
	fflush(stdout);
}

void ShowCursor(short show)
{
	if (show)
		wprintf(L"\e[?25h");
	else
		wprintf(L"\e[?25l");
}

void ClearDialog()
{
	GoToXY(DIALOG_X, DIALOG_Y);
	wprintf(L"%lc%lc%lc%lc", TOP_LEFT_CORNER, HORIZONTAL_WALL, HORIZONTAL_WALL, TOP_RIGHT_CORNER);
	GoToXY(DIALOG_X, DIALOG_Y + 1);
	wprintf(L"%lc  %lc", VERTICAL_WALL, VERTICAL_WALL);
	GoToXY(DIALOG_X, DIALOG_Y + 2);
	wprintf(L"%lc%lc%lc%lc", BOTTOM_LEFT_CORNER, HORIZONTAL_WALL, HORIZONTAL_WALL, BOTTOM_RIGHT_CORNER);
	fflush(stdout);
}

void ClearComments()
{
	GoToXY(COMMENTS_X + 1, COMMENTS_Y + 1);
	wprintf(L"           ");
	GoToXY(COMMENTS_X + 1, COMMENTS_Y + 2);
	wprintf(L"           ");
	GoToXY(COMMENTS_X + 1, COMMENTS_Y + 3);
	wprintf(L"           ");
	fflush(stdout);
}

int HitsSum(int table[])
{
	int sum = 0;
	for (int i = MIN_SHIP_SIZE - 2; i < MAX_SHIP_SIZE - 1; i++)
	{
		sum += table[i];
	}
	return sum;
}

void PrepareFields(Field fields[SIZE][SIZE])
{
	for (int y = 1; y < SIZE - 1; y++)
	{
		for (int x = 1; x < SIZE - 1; x++)
		{
			fields[x][y].Status = EMPTY;
		}
	}
	int var = rand() % 2;
	for (int y = 1; y < SIZE - 1; y++)
	{
		for (int x = 1; x < SIZE - 1; x += 2)
		{
			fields[x + (y + var) % 2][y].Status = TEMPORARILY_EXCLUDED;
		}
	}
	for (int i = 0; i < SIZE; i++)
	{
		fields[0][i].Status = FRAME;
		fields[SIZE - 1][i].Status = FRAME;
	}
	for (int i = 0; i < SIZE; i++)
	{
		fields[i][0].Status = FRAME;
		fields[i][SIZE - 1].Status = FRAME;
	}
}

int HandleMenu(int *currentState, Field playersTable[SIZE][SIZE], Field botsTable[SIZE][SIZE], Field helpTable[SIZE][SIZE], short visibilityTable[SIZE - 2][SIZE - 2])
{
	char character;
	ShowCursor(0);
	ClearScreen();

	GoToXY(MENU_X + 2, MENU_Y);
	wprintf(L"MENU");
	GoToXY(MENU_X, MENU_Y + 1);
	wprintf(L"1) Play");
	GoToXY(MENU_X, MENU_Y + 2);
	wprintf(L"2) Help");
	GoToXY(MENU_X, MENU_Y + 3);
	wprintf(L"3) Quit");

	character = _getch();
	if (character == '1')
	{
		DrawingTable drawingTable[(SIZE - 2) * (SIZE - 2 - MIN_SHIP_SIZE + 1) * 2];
		DrawShips(playersTable, drawingTable);
		DrawShips(botsTable, drawingTable);
		PrepareBoard();
		GoToXY(RESULTS_X + 1, RESULTS_Y + 1);
		wprintf(L"00:00");
		GoToXY(RESULTS_X + 1, RESULTS_Y + 2);
		wprintf(L"00:00");
		DrawPlayer(playersTable, 1, visibilityTable);
		DrawPlayer(botsTable, 0, visibilityTable);
		PrepareFields(playersTable);
		*currentState = PLAYER;
	}
	else if (character == '2')
	{
		*currentState = HELP;
		return 0;
	}
	else if (character == '3')
		return 1;

	return 0;
}

void HandlePlayersMove(int *currentState, Field table[SIZE][SIZE], short visibilityTable[SIZE - 2][SIZE - 2], int hits[5], int *sinks)
{
	char coordinates[20] = {0};

	ClearComments();
	GoToXY(COMMENTS_X + 3, COMMENTS_Y + 1);
	wprintf(L"PLEASE");
	GoToXY(COMMENTS_X + 3, COMMENTS_Y + 2);
	wprintf(L"ENTER");
	GoToXY(COMMENTS_X + 1, COMMENTS_Y + 3);
	wprintf(L"COORDS");

	ShowCursor(1);

	char x = -1;
	char y = -1;

	while ((y > SIZE - 2) || (y < 0) || (x > SIZE - 2) || (x < 0))
	{
		ClearDialog();
		GoToXY(DIALOG_X + 1, DIALOG_Y + 1);

		scanf("%s", &coordinates);

		x = (coordinates[1] - '0');
		y = toupper(coordinates[0]) - 'A';
	}

	ClearComments();
	ShowCursor(0);

	visibilityTable[x][y] = 1;
	if ((table[x + 1][y + 1].Status == EMPTY) || (table[x + 1][y + 1].Status == NEARBY))
	{
		table[x + 1][y + 1].Status = MISSED;
		GoToXY(COMMENTS_X + 4, COMMENTS_Y + 2);
		wprintf(L"MISS");
	}
	else
	{
		if (table[x + 1][y + 1].Status == SHIP_INTACT)
		{
			GoToXY(COMMENTS_X + 2, COMMENTS_Y + 2);
			wprintf(L"HIT");
			table[x + 1][y + 1].Status = SHIP_HIT;
			hits[table[x + 1][y + 1].ShipSize - 2]++;
			if (hits[table[x + 1][y + 1].ShipSize - 2] == table[x + 1][y + 1].ShipSize)
			{
				GoToXY(COMMENTS_X + 2, COMMENTS_Y + 3);
				wprintf(L"SUNK");
				(*sinks)++;
				for (int i = 1; i < SIZE - 1; i++)
				{
					for (int j = 1; j < SIZE - 1; j++)
					{
						if (table[i][j].ShipSize == table[x + 1][y + 1].ShipSize)
							table[i][j].Status = SHIP_SUNK;
					}
				}
			}
		}
	}

	GoToXY(RESULTS_X + 1, RESULTS_Y + 1);
	wprintf(L"%02d", *sinks);
	GoToXY(RESULTS_X + 1, RESULTS_Y + 2);
	wprintf(L"%02d", HitsSum(hits));
	DrawPlayer(table, 0, visibilityTable);
	sleep(1);
	ClearComments();
	ClearDialog();

	if ((*hits) == 5)
	{
		GoToXY(COMMENTS_X + 1, COMMENTS_Y + 2);
		wprintf(L"YOU WIN!");
		_getch();
		*currentState = MENU;
		return;
	}

	*currentState = BOT;
}

void HandleBotsMove(int *currentState, Field table[SIZE][SIZE], Field helpTable[SIZE][SIZE], int hits[], int *sinks, Hit *thereWasAHit, Hit *firstHit, short visibilityTable[SIZE - 2][SIZE - 2])
{
	if (thereWasAHit->HitSuccessful)
	{
		Hit pool[4] = {0};
		if (thereWasAHit->Y != 0)
		{
			pool[0].X = thereWasAHit->X;
			pool[0].Y = thereWasAHit->Y - 1;
			pool[0].HitSuccessful = 2;
		}
		if (thereWasAHit->X != SIZE - 2)
		{
			pool[1].X = thereWasAHit->X + 1;
			pool[1].Y = thereWasAHit->Y;
			pool[1].HitSuccessful = 2;
		}
		if (thereWasAHit->Y != SIZE - 2)
		{
			pool[2].X = thereWasAHit->X;
			pool[2].Y = thereWasAHit->Y + 1;
			pool[2].HitSuccessful = 2;
		}
		if (thereWasAHit->X != 0)
		{
			pool[3].X = thereWasAHit->X - 1;
			pool[3].Y = thereWasAHit->Y;
			pool[3].HitSuccessful = 2;
		}

		for (int i = 0; i < 4; i++)
		{
			if (pool[i].HitSuccessful == 2)
			{
				if ((helpTable[pool[i].X + 1][pool[i].Y + 1].Status == EMPTY) || (helpTable[pool[i].X + 1][pool[i].Y + 1].Status == TEMPORARILY_EXCLUDED))
					pool[i].HitSuccessful = 1;
			}
		}

		Hit randomPool[4];
		int howMuchHits = 0;
		for (int i = 0; i < 4; i++)
		{
			if (pool[i].HitSuccessful == 1) // TODO rethink equals 1, should it be just if HitSuccessful?
			{
				randomPool[howMuchHits] = pool[i];
				howMuchHits++;
			}
		}
		if (howMuchHits == 0)
		{
			*thereWasAHit = *firstHit;
			*currentState = BOT;
			return;
		}

		int x = 0, y = 0, choice;

		choice = rand() % howMuchHits;
		x = randomPool[choice].X;
		y = randomPool[choice].Y;

		ClearComments();

		GoToXY(COMMENTS_X + 1, COMMENTS_Y + 2);
		wprintf(L"Target:");
		GoToXY(COMMENTS_X + 5, COMMENTS_Y + 3);
		wprintf(L"%c%d", y + 'A', x);
		sleep(1);

		ClearComments();

		if ((table[x + 1][y + 1].Status == EMPTY) || (table[x + 1][y + 1].Status == NEARBY))
		{
			table[x + 1][y + 1].Status = MISSED;
			helpTable[x + 1][y + 1].Status = MISSED;
			GoToXY(COMMENTS_X + 4, COMMENTS_Y + 2);
			wprintf(L"MISS");
		}
		else
		{
			if (table[x + 1][y + 1].Status == 1)
			{
				GoToXY(COMMENTS_X + 2, COMMENTS_Y + 2);
				wprintf(L"HIT");
				table[x + 1][y + 1].Status = SHIP_HIT;
				helpTable[x + 1][y + 1].Status = SHIP_HIT;
				if ((x != 0) && (y != 0))
					helpTable[x][y].Status = NEARBY;
				if ((x != 0) && (y != SIZE - 2))
					helpTable[x][y + 2].Status = NEARBY;
				if ((x != SIZE - 2) && (y != 0))
					helpTable[x + 2][y].Status = NEARBY;
				if ((x != SIZE - 2) && (y != SIZE - 2))
					helpTable[x + 2][y + 2].Status = NEARBY;
				thereWasAHit->HitSuccessful = 1;
				thereWasAHit->X = x;
				thereWasAHit->Y = y;
				hits[table[x + 1][y + 1].ShipSize - 2]++;
				if (hits[table[x + 1][y + 1].ShipSize - 2] == table[x + 1][y + 1].ShipSize)
				{
					GoToXY(COMMENTS_X + 2, COMMENTS_Y + 3);
					wprintf(L"SUNK");
					(*sinks)++;
					thereWasAHit->HitSuccessful = 0;
					for (int i = 1; i < SIZE - 1; i++)
					{
						for (int j = 1; j < SIZE - 1; j++)
						{
							if (table[i][j].ShipSize == table[x + 1][y + 1].ShipSize)
								table[i][j].Status = SHIP_SUNK;
						}
					}
				}
			}
		}
		GoToXY(RESULTS_X + 4, RESULTS_Y + 1);
		wprintf(L"%02d", *sinks);
		GoToXY(RESULTS_X + 4, RESULTS_Y + 2);
		wprintf(L"%02d", HitsSum(hits));
		DrawPlayer(table, 1, visibilityTable);
		sleep(1);
		ClearComments();
		ClearDialog();

		if ((*sinks) == 5)
		{
			GoToXY(COMMENTS_X + 3, COMMENTS_Y + 2);
			wprintf(L"YOU LOSE");
			_getch();
			*currentState = MENU;
		}
	}

	else
	{
		Hit randomPool[(SIZE - 2) * (SIZE - 2) / 2];
		int i = 0;
		for (int x = 0; x < SIZE - 2; x++)
		{
			for (int y = 0; y < SIZE - 2; y++)
			{
				if (helpTable[x + 1][y + 1].Status == EMPTY)
				{
					randomPool[i].X = x;
					randomPool[i].Y = y;
					i++;
				}
			}
		}
		int x = 0, y = 0, choice;

		choice = rand() % i;
		x = randomPool[choice].X;
		y = randomPool[choice].Y;

		ClearComments();

		GoToXY(COMMENTS_X + 1, COMMENTS_Y + 2);
		wprintf(L"Target:");
		GoToXY(COMMENTS_X + 5, COMMENTS_Y + 3);
		wprintf(L"%c%d", y + 'A', x);
		sleep(1);

		ClearComments();

		if ((table[x + 1][y + 1].Status == EMPTY) || (table[x + 1][y + 1].Status == NEARBY))
		{
			table[x + 1][y + 1].Status = MISSED;
			helpTable[x + 1][y + 1].Status = MISSED;
			GoToXY(COMMENTS_X + 4, COMMENTS_Y + 2);
			wprintf(L"MISS");
			thereWasAHit->HitSuccessful = 0;
		}
		else
		{
			if (table[x + 1][y + 1].Status == SHIP_INTACT)
			{
				GoToXY(COMMENTS_X + 2, COMMENTS_Y + 2);
				wprintf(L"HIT");
				table[x + 1][y + 1].Status = SHIP_HIT;
				helpTable[x + 1][y + 1].Status = SHIP_HIT;
				if ((x != 0) && (y != 0))
					helpTable[x][y].Status = NEARBY;
				if ((x != 0) && (y != SIZE - 2))
					helpTable[x][y + 2].Status = NEARBY;
				if ((x != SIZE - 2) && (y != 0))
					helpTable[x + 2][y].Status = NEARBY;
				if ((x != SIZE - 2) && (y != SIZE - 2))
					helpTable[x + 2][y + 2].Status = NEARBY;
				thereWasAHit->HitSuccessful = 1;
				thereWasAHit->X = x;
				thereWasAHit->Y = y;
				*firstHit = *thereWasAHit;
				hits[table[x + 1][y + 1].ShipSize - 2]++;
				if (hits[table[x + 1][y + 1].ShipSize - 2] == table[x + 1][y + 1].ShipSize)
				{
					GoToXY(COMMENTS_X + 2, COMMENTS_Y + 3);
					wprintf(L"SUNK");
					(*sinks)++;
					for (int i = 1; i < SIZE - 1; i++)
					{
						for (int j = 1; j < SIZE - 1; j++)
						{
							if (table[i][j].ShipSize == table[x + 1][y + 1].ShipSize)
								table[i][j].Status = SHIP_SUNK;
						}
					}
				}
			}
		}
		GoToXY(RESULTS_X + 4, RESULTS_Y + 1);
		wprintf(L"%02d", *sinks);
		GoToXY(RESULTS_X + 4, RESULTS_Y + 2);
		wprintf(L"%02d", HitsSum(hits));
		DrawPlayer(table, 1, visibilityTable);
		sleep(1);
		ClearComments();
		ClearDialog();

		if ((*sinks) == 5)
		{
			GoToXY(COMMENTS_X + 3, COMMENTS_Y + 2);
			wprintf(L"YOU LOSE");
			_getch();
			*currentState = MENU;
			return;
		}
	}
	*currentState = PLAYER;
}

void HandleHelp(int *currentState)
{
	PrepareBoard();

	GoToXY(DIALOG_X - 2, DIALOG_Y - 3);
	wprintf(L"Dialog");
	GoToXY(DIALOG_X - 2, DIALOG_Y - 2);
	wprintf(L"window");

	GoToXY(PLAYER_X + 3, PLAYER_Y + 5);
	wprintf(L"Player");
	GoToXY(PLAYER_X + 4, PLAYER_Y + 6);
	wprintf(L"board");

	GoToXY(BOT_X + 3, BOT_Y + 5);
	wprintf(L"Bot");
	GoToXY(BOT_X + 4, BOT_Y + 6);
	wprintf(L"board");

	GoToXY(RESULTS_X - 8, RESULTS_Y);
	wprintf(L"Results");
	GoToXY(RESULTS_X - 8, RESULTS_Y + 1);
	wprintf(L"table");
	GoToXY(RESULTS_X + 7, RESULTS_Y + 1);
	wprintf(L"Sinks");
	GoToXY(RESULTS_X + 7, RESULTS_Y + 2);
	wprintf(L"Hits");
	GoToXY(RESULTS_X - 2, RESULTS_Y + 4);
	wprintf(L"Player:Bot");

	GoToXY(COMMENTS_X + 1, COMMENTS_Y + 1);
	wprintf(L"Comments");
	GoToXY(COMMENTS_X + 2, COMMENTS_Y + 2);
	wprintf(L"window");

	GoToXY(LEGEND_X, LEGEND_Y - 2);
	wprintf(L"Legend:");

	_getch();
	*currentState = MENU;
}

Field PlayersBoard[SIZE][SIZE], BotsBoard[SIZE][SIZE], HelpBoard[SIZE][SIZE];
short VisibilityTable[SIZE - 2][SIZE - 2] = {0};
int PlayersHits[MAX_SHIP_SIZE - MIN_SHIP_SIZE + 1] = {0}, BotsHits[MAX_SHIP_SIZE - MIN_SHIP_SIZE + 1] = {0};
int PlayersSinks = 0, BotsSinks = 0;
Hit ThereWasAHit = {0, 0, 0}, FirstHit = {0, 0, 0};

int main()
{
	setlocale(LC_CTYPE, "");
	ShowCursor(0);

	srand(time(NULL));
	int CurrentState = MENU; // TODO make it MENU
	int GameFinished = 0;	 // TODO make it 0
	PrepareBoard();

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

	ClearScreen();
	return 0;
}
