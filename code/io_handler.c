#define LEN_LIM 256
#include "io_handler.h"
#include <stdio.h>
#include <string.h>

static t_command commands[] = {
	{"solve", SOLVE},
	{"edit", EDIT},
	{"mark_errors", MARK_ERRORS},
	{"print_board", PRINT_BOARD},
	{"set", SET},
	{"validate", VALIDATE},
	{"guess", GUESS},
	{"generate", GENERATE},
	{"undo", UNDO},
	{"redo", REDO},
	{"save", SAVE},
	{"hint", HINT},
	{"guess_hint", GUESS_HINT},
	{"num_solutions", NUM_SOLUTIONS},
	{"autofill", AUTOFILL},
	{"reset", RESET},
	{"exit", EXIT}
};

int keyFromString(char *key)
{
    int i;
    for (i=0; i < NKEYS; i++) {
        if (strcmp(commands[i].name, key) == 0)
            return commands[i].val;
    }
    return BADKEY;
}

/**
 * The entry function of this module.
 * Composite of: 1. get input from the user 2. parse it 3. operates if valid 4. prints fits output
 * @return non-value integer if the program should exit (because of exit command, error, etc...). else: 0
 */
int getCommand(Game *game) {
	char command_str[LEN_LIM+3];
    char* command;
    char* parameters[3];
    char* p;
    int i, ch;
	size_t len;
	int EOLfound = 0;
    int parameters_amount = 0;
    /*reads the input from the user */
    if(fgets(command_str, LEN_LIM + 3, stdin) == NULL){
        return 1;
    }

	len = strlen(command_str);
	if(len>0 && command_str[len-1] == '\n'){
		command_str[--len] = '\0';
		EOLfound = 1;
	}
	if(len>LEN_LIM){
		/* commant to long */
		printf("too many\n");
		while(!EOLfound){
			ch = fgetc(stdin);
			if(ch==EOF){
				return 1;
			}
			if(ch=='\n'){
				EOLfound = 1;
			}
		}
		return 0;
	}

    /*gets the command */
    command = strtok(command_str, " ");
    if(command==NULL){
		printf("invalid command\n");
        return 0;
    }
    /*gets the parameters, 3 at most */
    for(i=0; i<3; i++){
        p = strtok(NULL, " ");
        if(p==NULL){
            break;
        }
        parameters[i] = p;
        parameters_amount++;
    }
	printf("%u\n", game->markErrors);
	if(parameters_amount){
		printf("%s\n", parameters[0]);
	}
	switch (keyFromString(command))
	{
	case SOLVE:
		break;
	
	case EDIT:
		break;
	
	case MARK_ERRORS:
		break;
	
	case PRINT_BOARD:
		break;
	
	case SET:
		break;
	
	case VALIDATE:
		break;
	
	case GUESS:
		break;
	
	case GENERATE:
		break;
	
	case UNDO:
		break;
	
	case REDO:
		break;
	
	case SAVE:
		break;
	
	case HINT:
		break;
	
	case GUESS_HINT:
		break;
	
	case NUM_SOLUTIONS:
		break;
	
	case AUTOFILL:
		break;
	
	case RESET:
		break;
	
	case EXIT:
		return 1;

	case BADKEY:
		printf("invalid command\n");
		break;
	}
	return 0;
}


/**
 * Prints the title of the game (our choice)
 */
void printTitle() {
	printf("Welcome to Exxxtreme Sudoku.\nHave fun!!! \n");
}
