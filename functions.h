#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "types.h"
#include "consts.h"

void ClearTable(DrawingTable table[]);

void GoToXY(int x, int y);

void DrawShips(Field drawnTable[SIZE][SIZE], DrawingTable table[]);

void PrepareBoard();

void DrawPlayer(Field playerTable[SIZE][SIZE], short ifPlayer, short visibilityTable[SIZE - 2][SIZE - 2]);

void SetTextColor(int color);

void ShowCursor(int showFlag);

void ClearDialog();

void ClearComments();

int HitsSum(int table[]);

void PrepareFields(Coordinates fields[(SIZE - 2) * (SIZE - 2)]);

int HandleMenu(int *currentState, Field playersTable[SIZE][SIZE], Field botsTable[SIZE][SIZE], Field helpTable[SIZE][SIZE], short visibilityTable[SIZE - 2][SIZE - 2]);

void HandlePlayersMove(int *currentState, Field table[SIZE][SIZE], short visibilityTable[SIZE - 2][SIZE - 2], int hits[5], int *sinks);

void HandleBotsMove(int *currentState, Field table[SIZE][SIZE], Field helpTable[SIZE][SIZE], int hits[], int *sinks, Hit *thereWasAHit, Hit *firstHit, short visibilityTable[SIZE - 2][SIZE - 2]);

void HandleHelp(int *currentState);

#endif
