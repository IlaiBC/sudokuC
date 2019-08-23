/*
 * MainAux.c
 *
 *  Created on: Jun 10, 2019
 *      Author: ILAI
 */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "game.h"
#include "MainAux.h"
#include "parser.h"

/* prints error of invalid command and the reason */
void printInvalidCommand(char* reason, Command* command){
	printf("Invalid command: %s\n", command->errorReason);
}

/* prints error of the function that failed */
void printStdError(char* funcName) {
	printf("Error: %s has failed\n", funcName);
}

/* Cleans the buffer between user's inputs */
void bufferCleaner(){
	char input;
	while(isspace((input = getchar())));
	ungetc(input, stdin);
}

/* gets the number of hints to filled in the game from the user */
int numOfHints(){
	int H;
	int size = (N*N);
	while(1){
		printf("Please enter the number of cells to fill [0-%d]:\n", size-1);
		if(scanf("%d", &H)!=1){
			/*printStdError("scanf");*/
			printf("Error: not a number\n");
			exitUnintializedGame();
		}
		if(feof(stdin)){
			/*printf("exit in numofhints\n");*/
			exitUnintializedGame();
		}
		if(H>=0 && H<size)
			return H;
		else
			printf("Error: invalid number of cells to fill (should be between 0 and %d)\n", size -1);
	}
	return 0;
}

/*
 * Gets the command from the user and uses the parser to parse it.
 */
int getUserCommand(Game* game, Command* command){
	char str[COMMAND_LENGTH];
	if(str == NULL){
		printStdError("malloc");
		exit(0);
	}
	bufferCleaner();
	if (feof(stdin)){
		/*free(str);*/
		exitGame(game);
	}
	if(fgets(str, COMMAND_LENGTH, stdin) == NULL){
		return 1;
	}
	parse(str, command);
	/*printf("getusercommand - command name: %s\n", command->name);*/
	return 0;
}
