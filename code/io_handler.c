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

int missingParameters(int givven, int requierd, int optional, int command){
	if(!optional){
		if(givven != requierd){
			printf("ERROR: command %s requiers %d parameters, %d has given\n", commands[command-1].name, requierd, givven);
			return 1;
		}
	}
	else{
		if(givven<requierd || givven>requierd+optional){
			printf("ERROR: command %s requiers %d parameters and %d optional, %d has given\n", commands[command-1].name, requierd, optional, givven);
			return 1;
		}
	}
	return 0;
}

int handle_file_err(FileError error){
	switch (error)
	{
	case FILE_CANT_OPEN:
		printf("ERROR: cannont open file");
		return 1;

	case FILE_ERRONEOUS_FIXED_CELLS:
		printf("ERROR: some fixed cells are erroneous");
		return 1;

	case FILE_INCORRECT_FORMAT:
		printf("ERROR: incoreeect file format");
		return 1;

	case FILE_INCORRECT_RANGE:
		printf("ERROR: some cells' value are not in the range");
		return 1;
	
	case FILE_NONE:
		break;
	default:
		printf("ERROR: general file error");
		return 1;
	}
	return 0;
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
    int i, ch, error;
	size_t len;
	int EOLfound = 0;
    int parameters_amount = 0;

	printf("please enter acommand\n");
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
	i = 0;
    p = strtok(NULL, " ");
    while(p != NULL){
		if(i<3){
        	parameters[i++] = p;
		}
        parameters_amount++;
        p = strtok(NULL, " ");
    }
	if(parameters_amount){
		p = parameters[0];
	}
	switch (keyFromString(command))
	{
	case SOLVE:
		if(missingParameters(parameters_amount, 1, 0, SOLVE)){
			return 0;
		}
		error = solve(game, parameters[0]);
		if(handle_file_err(error)){
			return 0;
		}
		break;
	
	case EDIT:
		if(missingParameters(parameters_amount, 0, 1, EDIT)){
			return 0;
		}
		p = parameters_amount ? parameters[0] : NULL;
		error = edit(game, p);
		if(handle_file_err(error)){
			return 0;
		}
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
		return 0;
	}
	print_board(game);
	return 0;
}


/**
 * Prints the title of the game (our choice)
 */
void printTitle() {
	printf("Welcome to Exxxtreme Sudoku.\nHave fun!!! \n");
}
