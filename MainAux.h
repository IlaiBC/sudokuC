/*
 * MainAux.h
 *
 *  Created on: Jun 10, 2019
 *      Author: ILAI
 */

#ifndef MAINAUX_H_
#define MAINAUX_H_

#include "game.h"
#include "parser.h"

#define COMMAND_LENGTH 1024

#define NO_SUCH_COMMAND "No such command exists!"
#define COMMAND_UNAVAILABLE "Command is unavailable in the current mode!"
#define TOO_MANY_PARAMS "Too many parameters were entered!"
#define NOT_ENOUGH_PARAMS "Not enough parameters!"
#define INCORRECT_VALUE "parameter has an incorrect value!"

/**
 * MainAux Module:
 *
 * This module has functions to get input from the user and to print errors
 *
 * printStdError        - prints error of the function that failed.
 * bufferCleaner        - Cleans the buffer between user's inputs.
 * numOfHints           - gets the number of hints to filled in the game from the user.
 * getUserCommand       - Gets the command from the user and uses the parser to parse it.
 *
 */

int numOfHints();

void bufferCleaner();

void printStdError(char* funcName);

int getUserCommand(Game* game, Command* command);

#endif /* MAINAUX_H_ */
