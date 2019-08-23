/*
 * Game.h
 *
 *  Created on: Aug 17, 2019
 *      Author: ILAI
 */

#ifndef GAME_H_
#define GAME_H_

enum mode {INIT, EDIT, SOLVE}Mode;

typedef struct cell {
	int x; /*column*/
	int y; /* row*/
	int value;
	int isFixed; /*1 =>fixed 0 =>not fixed*/
	int hasError; /* 0- false, 1-true */
}Cell;

typedef struct node Node;

typedef struct game {
	Node* head;
	Node* curr;
	int n;
	int m;
	int hints;
	Cell** board;
/*	Cell** solution; */
	int markErr; /* 0-false, 1-true */
	enum mode gameMode; /* 0-INIT, 1-EDIT, 2-SOLVE */

}Game;

void printBoard(Game* game);

int markErrors(Game* game, int x);


#endif /* GAME_H_ */
