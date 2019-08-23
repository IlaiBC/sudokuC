/*
 * Game.c
 *
 *  Created on: Aug 17, 2019
 *      Author: ILAI
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "game.h"
#include "MainAux.h"

#define EMPTY 0
#define INVALID_CMD "Error: Invalid Command"
#define INVALID_MARK "Error: game mark error can only be set to 1 or 0"
#define FILE_ERR "Error: Unable to read file"
#define SCAN_ERR "Error: file scan failed"

/* The function checks if a certain cell is  row valid.
 *
 * It checks if the value already located in the cell's same row
 *
 * @param cell    - The cell to be checked.
 * 		  game    - The current game instance.
 *
 * @return 1 if is valid.
 *         0 otherwise.
 *
 */
int isRowSafe(Game* game, Cell* cell) {
	int j;
	int N = game->m * game->n;
	/*printf("isRowSafe()\n");*/
	for(j=0; j <N; j++) {
		if((game->board[cell->y][j]).value == cell->value) {
			/*printf("rowsafe returns 0\n");*/
			return 0;
		}
	}
	/*printf("rowsafe returns 1\n");*/
	return 1;
}

/* The function checks if a certain cell is  column valid.
 *
 * It checks if the value already located in the cell's same column
 *
 * @param cell    - The cell to be checked.
 * 		  game    - The current game instance.
 *
 * @return 1 if is valid.
 *         0 otherwise.
 *
 */
int isColSafe(Game* game, Cell* cell) {
	int i;
	int N = game->m * game->n;
	/*printf("isColSafe()\n");*/
	for(i=0; i<N; i++) {
		if((game->board[i][cell->x]).value == cell->value) {
			return 0;
		}
	}
	/*printf("colsafe 1\n");*/
	return 1;
}

/* The function checks if a certain cell is  block valid.
 *
 * It checks if the value already located in the cell's same block
 *
 * @param cell    - The cell to be checked.
 * 		  game    - The current game instance.
 *
 * @return 1 if is valid.
 *         0 otherwise.
 *
 */
int isBlockSafe(Game* game,int startY, int startX, Cell* cell) {
	int y;
	int x;
	/*printf("isBlockSafe()\n");*/
	/*printf("value to check if safe: %d\n", cell->value);*/
    for (y = 0; y < game->m; y++) {
        for (x = 0; x < game->n; x++) {
        	/*printf("BlockSafe checks cell [%d][%d] with value %d\n", y+startY,x+startX, board[y+startY][x+startX].value);*/
            if(game->board[y+startY][x+startX].value == cell->value) {
            	/*printf("block safe returns 0\n");*/
                return 0;
            }
        }
    }
    /*printf("block safe returns 1\n");*/
    return 1;
}
/* The function checks if a certain value is valid to specific cell.
 *
 * It checks if the value already located in the same row/column/block
 *
 * @param cell    - The cell to be checked.
 * 		  game    - The current game instance.
 *
 * @return 1 if is valid.
 *         0 otherwise.
 *
 */
int isSafe(Game* game,Cell* cell) {
	int startX = cell->x - cell->x%game->n;
	int startY = cell->y - cell->y%game->m;
	/*printf("isSafe()\n");*/
	if(isRowSafe(game, cell) && isColSafe(game, cell) && isBlockSafe(game, startY, startX, cell))
	{
		/*printf("issafe returns 1 !!!!\n");*/
		return 1;
	}
	/*printf("issafe returns 0 --> cell xy are: %d, %d\n", cell->x, cell->y);*/
	return 0;
}

/* The function creates a Cell instance:
 *
 *
 * @param x - The cell's column coordinate.
 * 		  y - The cell's row coordinate.
 * 		  value - The Cell's value.
 * 		  isFixed - whether the cell is fixed.
 * 		  hasError - whether the cell has error
 *
 * @return - The initialized Cell instance or null if memory allocation failed.
 *
 */
Cell* createCell(int x, int y, int value, int isFixed, int hasError) {
	Cell* res = (Cell*) malloc(sizeof(Cell));
	/*printf("createCell() with xy: %d, %d\n", x,y);*/
	if (!res) { /*allocation failed*/
		printStdError("malloc");
		free(res);
		exit(0);
	}
	res->x = x;
	res->y = y;
	res->value = value;
	res->isFixed = isFixed;
	res->hasError = hasError;
	/*printf("cell was created with xy: %d, %d\n", res->x, res->y);*/
	return res;
}

void printCell(Cell* cell, Game* game) {
	if (cell->value == EMPTY) {
		printf("    ");
	} else {
		printf(" %2d", cell->value);
		if (cell->isFixed && game->gameMode == 1) {
			printf(".");
		} else if ((cell->hasError)
				&& (game->gameMode == 1
						|| (game->gameMode == 2 && game->markErr == 1))) {
			printf("*");
		} else {
			printf(" ");
		}
	}
}

void printSepRow(Game* game) {
	int i;
	for (i = 0; i < (game->m * game->n * 4 + game->m + 1); i++) {
		printf("-");
	}
	printf("\n");
}

/* The function prints the game board:
 *
 * @param game - The current game instance.
 *
 */
void printBoard(Game* game) {
	int i, j;
	int N = game->m * game->n;
	if (game->gameMode == INIT) {
		printf(INVALID_CMD);
		return;
	}
	for (i = 0; i < N; i++) {
		if (i % game->m == 0) {
			printSepRow(game);
		}
		for (j = 0; j < N; j++) {
			if (j % game->n == 0) {
				printf("|");
			}
			printCell(&game->board[j][i], game);
		}
		printf("|\n");
	}
	printSepRow(game);
}

/* need to validate in parse that X is int else throw error
 * if func sucessful returns 1 else returns 0 */
int markErrors(Game* game, int x) {
	if (game->gameMode != 2) {
		printf(INVALID_CMD);
		return 0;
	}
	if (x != 1 && x != 0) {
		printf(INVALID_MARK);
	}
	game->markErr = x;
	return 1;
}

/* need to complete!!! */
void createGame(Game* game) {
	if (game->board) {
		freeGame(game); /* need to create free game */
	}
	if (game->gameMode != 0) {
		game->board = initializeEmptyBoard(Game * game);
	}
	game->curr = game->head;
	game->hints = 0;
}

int solve(Game* game, char* filePath) {
	int i, j, value, N;
	FILE* gameFile;
	char a;

	gameFile = fopen(filePath, "r");
	if (!gameFile) {
		printf(FILE_ERR);
		return 0;
	}
	if (fscanf(gameFile, "%d", &(game->m)) != 1) {
		printf(SCAN_ERR);
		return 0;
	}
	if (fscanf(gameFile, "%d", &(game->n)) != 1) {
		printf(SCAN_ERR);
		return 0;
	}
	game->gameMode = 2;
	createGame(game); /* fill rest of game parameters & init board */
	/*update game list here!!! */

	N = game->m * game->n;
	for (j = 0; j < N; j++) { 	/* retrive cells from file into game */
		for (i = 0; i < N; i++) {
			if (i != N - 1 || j != N - 1) { /* have not reached end of board */
				if (fscanf(gameFile, "%d%c", &value, &a) < 2) { /* read cell value and is fixed */
					printf(SCAN_ERR);
					return 0;
				}
			} else { /* is end of board */
				if (fscanf(gameFile, "%d%c", &value, &a) < 1) {
					printf(SCAN_ERR);
					return 0;
				}
			}
			game->board[i][j].value = value;
			game->board[i][j].isFixed = a == '.' ? 1 : 0;
			if (value != 0) {
				game->hints++;
			}
		}
	}
	/* updateAllErrors(game); need to addd!! */
	fclose(gameFile);
	printBoard(game);
	return 1;
}


