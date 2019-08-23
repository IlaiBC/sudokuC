/*
 * parser.c
 *
 *  Created on: 1 αιεπ 2019
 *      Author: Tomer
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "MainAux.h"
#define DEL " \t\r\n"
//#define N 9

/* creates empty command instance */
Command* initEmptyCommand(){
	Command* command = (Command*) malloc(sizeof(Command));
	if(command == NULL){
		printStdError("malloc");
		exit(0);
	}
	command->name = "";
	command->x = "";
	command->y = "";
	command->z = "";
	command->errorReason = "";
	return command;
}

/* verifies no extra params were input by the user for a command (returns 0 for extra params, 1 otherwise).
 * updates the error reason of the command if extra params were input. */
int verifyNoExtraParams(Command* command){
	char* token;
	token = strtok(NULL, DEL);
	if(token != NULL){
		command->errorReason = TOO_MANY_PARAMS;
		return 0;
	}
	return 1;
}

/* verifies all values of the command are valid for commands:
 * mark_errors: X is 0 or 1
 * set: X,Y are between 1-N, Z is 0-N (0 means to empty a cell)
 * guess: X is a threshold, float between 0-1
 * generate: X,Y num of cells, between 0-N*N
 * hint: X,Y are between 1-N
 * guess_hint: X,Y are between 1-N */
void verifyCommandParamValues(Command* command){

	if(!strcmp(command->name, "mark_errors")){
		if(atoi(command->x) != 0 && atoi(command->x) != 1)
			sprintf(command->errorReason, "%s Param must be 0 or 1.", INCORRECT_VALUE);
		return;
	}
	if(!strcmp(command->name, "set") || !strcmp(command->name, "hint") || !strcmp(command->name, "guess_hint")){
		if(atoi(command->x) < 1 || atoi(command->x) > N){
			sprintf(command->errorReason, "%s First param must be between 1 to %d.", INCORRECT_VALUE, N);
			return;
		}
		if(atoi(command->y) < 1 || atoi(command->y) > N){
			sprintf(command->errorReason, "%s Second param must be between 1 to %d.", INCORRECT_VALUE, N);
			return;
		}
		if(!strcmp(command->name, "set"))
			if(atoi(command->z) < 0 || atoi(command->y) > N)
				sprintf(command->errorReason, "%s Third param must be between 1 to %d, or 0 to empty cell.", INCORRECT_VALUE, N);
		return;
	}
	if(!strcmp(command->name, "guess")){
		if(atof(command->x) < 0 || atof(command->x) > 1)
			sprintf(command->errorReason, "%s Param must be a float between 0 to 1.", INCORRECT_VALUE);
		return;
	}
	if(!strcmp(command->name, "generate")){
		if(atoi(command->x) < 1 || atoi(command->x) > N*N)
			sprintf(command->errorReason, "%s Param must be between 0 to %d.", INCORRECT_VALUE, N*N);
		return;
	}
}

/*
 * Parses the x/y/z params for 'set' & 'hint' commands.
 * If 'set' gets only 1 or 2 params (no z) or 0 params - invalid command.
 * If 'hint' gets only 1 param (no y) or 0 params - invalid command.
 * Valid values for x/y/z (if gets them) are assumed.
 * Ignores extra params.
 */
void parseFullCommand(Command* command, char* token){
	int i, n, value;
	if(!strcmp(token, "set"))
		n = 3;
	else if(!strcmp(token, "generate") || !strcmp(token, "hint") || !strcmp(token, "guess_hint"))
		n = 2;
	else // "solve", "edit", "mark_errors", "guess", "save" // "edit" ??????? !!!!!
		n = 1;
	command->name = token;
	for(i = 0; i < n; i++){
		token = strtok(NULL, DEL);
		if(token == NULL)
		{
			if(strcmp(command->name, "edit")){
				command->name = "invalid";
				command->errorReason = NOT_ENOUGH_PARAMS;
			}
			return;
		}
		if(!strcmp(command->x, "")){
			command->x = token;
			continue;
		}
		if(!strcmp(command->y, "")){
			command->y = token;
			continue;
		}
		command->z = token;
		if(!verifyNoExtraParams(command))
			return;
		verifyCommandParamValues(command); // CREATE !!!!
	}
}




/*
 * Verify and update the command's name.
 * "invalid" name for invalid command.
 * If it's 'set' or 'hint' --> call 'parseFullCommand' func for the other params.
 * Command must be initialized as {"", -1, -1, -1}
 */

/* !!!!!!
 * A command that is unavailable in the current mode (e.g., "print_board" is not available in Init mode) **
 * is considered an invalid command, and an error message should instruct the user that the command is
 * unavailable in the current mode, and which modes it is available in. This case is considered as part
 * of step 1 for the order of errors described above (i.e., we report it right after reading the command name,
 * regardless of the number and values of the rest of the command parameters).
 */
void parse(char* str, Command* command){
	char* token;
	token = strtok(str, DEL);
	if(!strcmp(token, "solve") || !strcmp(token, "edit") || !strcmp(token, "mark_errors") ||
		!strcmp(token, "set") || !strcmp(token, "generate") || !strcmp(token, "guess") ||
		!strcmp(token, "save") || !strcmp(token, "hint") || !strcmp(token, "guess_hint"))
	{
		verifyCommandAvailabilityInCurrMode(command); // !!!! CREATE **
		parseFullCommand(command, token);
		return;
	}
	if(!strcmp(token, "validate") || !strcmp(token, "reset") || !strcmp(token, "exit") ||
		!strcmp(token, "print_board") || !strcmp(token, "undo") || !strcmp(token, "redo") ||
		!strcmp(token, "num_solutions") || !strcmp(token, "autofill"))
	{
		command->name = token;
		verifyCommandAvailabilityInCurrMode(command); // !!!! CREATE **
		verifyNoExtraParams(command); // create !!!!!!!!
		return;
	}
	command->name = "invalid";	/* NULL or other */
	command->errorReason = NO_SUCH_COMMAND;
}
