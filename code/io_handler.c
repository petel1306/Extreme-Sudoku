#define LEN_LIM 256
#include "io_handler.h"
#include <stdio.h>
#include <stdlib.h>
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
		printf("ERROR: cannont open file\n");
		return 1;

	case FILE_ERRONEOUS_FIXED_CELLS:
		printf("ERROR: some fixed cells are erroneous\n");
		return 1;

	case FILE_INCORRECT_FORMAT:
		printf("ERROR: incoreeect file format\n");
		return 1;

	case FILE_INCORRECT_RANGE:
		printf("ERROR: some cells' value are not in the range\n");
		return 1;
	
	case FILE_NONE:
		break;
	default:
		printf("ERROR: general file error\n");
		return 1;
	}
	return 0;
}

int handle_mark_err(MarkError error){
	switch (error)
	{
	case MARK_INCORRECT_VALUE:
		printf("ERROR: value should be 0 or 1\n");
		return 1;

	case MARK_NOT_AVAILABLE:
		printf("ERROR: command available only in Solve mode\n");
		return 1;

	case MARK_NONE:
		break;
	default:
		printf("ERROR: general error\n");
		return 1;
	}
	return 0;
}


int handle_print_err(PrintError error){
	switch (error)
	{
	case PRINT_NOT_AVAILABLE:
		printf("ERROR: the command is only available in Edit and Solve mode\n");
		return 1;
	
	case PRINT_NONE:
		break;
	default:
		printf("ERROR: general print error\n");
		return 1;
	}
	return 0;
}

int handle_set_err(SetError error){
	switch (error)
	{
	case SET_FIXED_CELL:
		printf("ERROR: cannot set a fixed cell\n");
		return 1;

	case SET_INVALID_VALUE:
		printf("ERROR: value is invalid\n");
		return 1;

	case SET_INVALID_X:
		printf("ERROR: x is invalid\n");
		return 1;

	case SET_INVALID_Y:
		printf("ERROR: y is invalid\n");
		return 1;

	case SET_NOT_AVAILABLE:
		printf("ERROR: the command is only available in Edit and Solve mode\n");
		return 1;
	
	case SET_NONE:
		break;
	default:
		printf("ERROR: general set error\n");
		return 1;
	}
	return 0;
}

int handle_validate_err(ValidateError error){
	switch (error)
	{
	case VALID_ERRONEOUS:
		printf("ERROR: the board is erroneous\n");
		return 1;

	case VALID_GUROBI_ERR:
		printf("ERROR: gurobi error\n");
		return 1;

	case VALID_NOT_AVAILABLE:
		printf("ERROR: the command is only available in Edit and Solve mode\n");
		return 1;
	
	case VALID_NONE:
		break;
	default:
		printf("ERROR: general validate error\n");
		return 1;
	}
	return 0;
}

int handle_guess_err(GuessError error){
	switch (error)
	{
	case GUESS_ERRONEOUS:
		printf("ERROR: the board is erroneous\n");
		return 1;

	case GUESS_GUROBI_ERR:
		printf("ERROR: gurobi error\n");
		return 1;

	case GUESS_NOT_AVAILABLE:
		printf("ERROR: the command is only available in Solve mode\n");
		return 1;

	case GUESS_NONE:
		break;
	default:
		printf("ERROR: general guess error\n");
		return 1;
	}
	return 0;
}

int handle_generate_err(GenerateError error){
	switch (error)
	{
	case GEN_NOT_SUCCEEDED:
		printf("ERROR: generate command has failed\n");
		return 1;

	case GEN_GUROBI_ERR:
		printf("ERROR: gurobi error\n");
		return 1;

	case GEN_NOT_AVAILABLE:
		printf("ERROR: the command is only available in Edit mode\n");
		return 1;
	
	case GEN_INVALID_X:
		printf("ERROR: x is invalid\n");
		return 1;

	case GEN_INVALID_Y:
		printf("ERROR: y is invalid\n");
		return 1;

	case GEN_NONE:
		break;
	default:
		printf("ERROR: general generate error\n");
		return 1;
	}
	return 0;
}

int handle_undo_redo_err(UndoRedoError error){
	switch (error)
	{
	case PRINT_NOT_AVAILABLE:
		printf("ERROR: the command is only available in Edit and Solve mode\n");
		return 1;
	
	case DO_NO_MOVES:
		printf("ERROR: no moves\n");
		return 1;
	
	case DO_NONE:
		break;
	default:
		printf("ERROR: general undo/redo error\n");
		return 1;
	}
	return 0;
}

int handle_save_err(SaveError error){
	switch (error)
	{
	case SAVE_ERRONEOUS:
		printf("ERROR: the board is erroneous\n");
		return 1;

	case SAVE_CANT_OPEN:
		printf("ERROR: cannot open file\n");
		return 1;

	case SAVE_WITHOUT_SOLUTION:
		printf("ERROR: board doesnt have a solution\n");
		return 1;

	case SAVE_GUROBI_ERR:
		printf("ERROR: gurobi error\n");
		return 1;

	case SAVE_NOT_AVAILABLE:
		printf("ERROR: the command is only available in Edit and Solve mode\n");
		return 1;
	
	case SAVE_NONE:
		break;
	default:
		printf("ERROR: general save error\n");
		return 1;
	}
	return 0;
}

int handle_hint_err(HintError error){
	switch (error)
	{
	case HINT_ERRONEOUS:
		printf("ERROR: the board is erroneous\n");
		return 1;

	case HINT_GUROBI_ERR:
		printf("ERROR: gurobi error\n");
		return 1;

	case HINT_FIXED_CELL:
		printf("ERROR: the cell is fixed\n");
		return 1;

	case HINT_NON_EMPTY:
		printf("ERROR: the cell already has value\n");
		return 1;

	case HINT_NOT_AVAILABLE:
		printf("ERROR: the command is only available in Solve mode\n");
		return 1;
	
	case HINT_INVALID_X:
		printf("ERROR: x is invalid\n");
		return 1;

	case HINT_INVALID_Y:
		printf("ERROR: y is invalid\n");
		return 1;

	case HINT_NONE:
		break;
	default:
		printf("ERROR: general hint error\n");
		return 1;
	}
	return 0;
}

int handle_num_solutions_err(NumSolutionsError error){
	switch (error)
	{
	case NUM_ERRONEOUS:
		printf("ERROR: the board is erroneous\n");
		return 1;

	case NUM_NOT_AVAILABLE:
		printf("ERROR: the command is only available in Edit and Solve mode\n");
		return 1;
	
	case NUM_NONE:
		break;
	default:
		printf("ERROR: general num_solution error\n");
		return 1;
	}
	return 0;
}

int handle_autofill_err(AutofillError error){
	switch (error)
	{
	case AUTO_ERRONEOUS:
		printf("ERROR: the board is erroneous\n");
		return 1;

	case AUTO_NOT_AVAILABLE:
		printf("ERROR: the command is only available in Solve mode\n");
		return 1;

	case AUTO_NONE:
		break;
	default:
		printf("ERROR: general autofill error\n");
		return 1;
	}
	return 0;
}

int handle_reset_err(ResetError error){
	switch (error)
	{
	case RESET_NOT_AVAILABLE:
		printf("ERROR: the command is only available in Edit and Solve mode\n");
		return 1;

	case RESET_NONE:
		break;
	default:
		printf("ERROR: general reset error\n");
		return 1;
	}
	return 0;
}

int* alloc_values(Game *game){
	int N = game->turn->board->m * game->turn->board->n;
	return (int*) malloc(sizeof(int) * N);
}

double* alloc_scores(Game *game){
	int N = game->turn->board->m * game->turn->board->n;
	return (double*) malloc(sizeof(double) * N);
}

char* alloc_change(){
	return (char*) malloc(sizeof(char)*4096);
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
	float f;
	int x, y, z;
	size_t len;
	int EOLfound = 0;
    int parameters_amount = 0;
	char *change;
	int *values;
	double *scores;
	int needToPrint = 0;

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
		printf("board has successfully loaded\n");
		needToPrint = 1;
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
		printf("board has successfully loaded\n");
		needToPrint = 1;
		break;
	
	case MARK_ERRORS:
		if(missingParameters(parameters_amount, 1, 0, MARK_ERRORS)){
			return 0;
		}
		if(sscanf(parameters[0], "%d", &x) == EOF){
			printf("ERROR: x should be an integer\n");
			return 0;
		}
		error = mark_errors(game, x);
		if(handle_mark_err(error)){
			return 0;
		}
		printf("mark_errors has changed to %d\n", x);
		break;
	
	case PRINT_BOARD:
		if(missingParameters(parameters_amount, 0, 0, PRINT_BOARD)){
			return 0;
		}
		error = print_board(game);
		if(handle_print_err(error)){
			return 0;
		}
		break;
	
	case SET:
		if(missingParameters(parameters_amount, 3, 0, SET)){
			return 0;
		}
		if(sscanf(parameters[0], "%d", &x) == EOF){
			printf("ERROR: x should be an integer\n");
			return 0;
		}
		if(sscanf(parameters[1], "%d", &y) == EOF){
			printf("ERROR: y should be an integer\n");
			return 0;
		}
		if(sscanf(parameters[2], "%d", &z) == EOF){
			printf("ERROR: value should be an integer\n");
			return 0;
		}
		error = set(game, x, y, z);
		if(handle_set_err(error)){
			return 0;
		}
		printf("successfully set cell (%d, %d) to %d\n", x, y, z);
		needToPrint = 1;
		break;
	
	case VALIDATE:
		if(missingParameters(parameters_amount, 0, 0, VALIDATE)){
			return 0;
		}
		error = validate(game, &i);
		if(handle_print_err(error)){
			return 0;
		}
		if(i){
			printf("board is solvable, go for it\n");
		}
		else{
			printf("board is unsolvable\n");
		}
		break;
	
	case GUESS:
		if(missingParameters(parameters_amount, 1, 0, GUESS)){
			return 0;
		}
		if(sscanf(parameters[0], "%f", &f) == EOF){
			printf("ERROR: x should be float\n");
			return 0;
		}
		error = guess(game, f);
		if(handle_guess_err(error)){
			return 0;
		}
		needToPrint = 1;
		break;
	
	case GENERATE:
		if(missingParameters(parameters_amount, 2, 0, GENERATE)){
			return 0;
		}
		if(sscanf(parameters[0], "%d", &x) == EOF){
			printf("ERROR: x should be an integer\n");
			return 0;
		}
		if(sscanf(parameters[1], "%d", &y) == EOF){
			printf("ERROR: y should be an integer\n");
			return 0;
		}
		error = generate(game, x, y);
		if(handle_generate_err(error)){
			return 0;
		}
		needToPrint = 1;
		break;
	
	case UNDO:
		if(missingParameters(parameters_amount, 0, 0, UNDO)){
			return 0;
		}
		change = alloc_change();
		error = undo(game, change);
		if(handle_undo_redo_err(error)){
			free(change);
			return 0;
		}
		printf("%s\n", change);
		free(change);
		needToPrint = 1;
		break;
	
	case REDO:
		if(missingParameters(parameters_amount, 0, 0, REDO)){
			return 0;
		}
		change = alloc_change();
		error = redo(game, change);
		if(handle_undo_redo_err(error)){
			free(change);
			return 0;
		}
		printf("%s\n", change);
		free(change);
		needToPrint = 1;
		break;
	
	case SAVE:
		if(missingParameters(parameters_amount, 1, 0, SAVE)){
			return 0;
		}
		error = save(game, parameters[0]);
		if(handle_save_err(error)){
			return 0;
		}
		printf("board has saved to: %s\n", parameters[0]);
		needToPrint = 1;
		break;
	
	case HINT:
		if(missingParameters(parameters_amount, 2, 0, HINT)){
			return 0;
		}
		if(sscanf(parameters[0], "%d", &x) == EOF){
			printf("ERROR: x should be an integer\n");
			return 0;
		}
		if(sscanf(parameters[1], "%d", &y) == EOF){
			printf("ERROR: y should be an integer\n");
			return 0;
		}
		error = hint(game, x, y, &i);
		if(handle_hint_err(error)){
			return 0;
		}
		printf("--TOP SECRET-- your hint for cell (%d, %d) is %d. DESTROY AFTER READING\n", x, y, i);
		break;
	
	case GUESS_HINT:
		if(missingParameters(parameters_amount, 2, 0, GUESS_HINT)){
			return 0;
		}
		if(sscanf(parameters[0], "%d", &x) == EOF){
			printf("ERROR: x should be an integer\n");
			return 0;
		}
		if(sscanf(parameters[1], "%d", &y) == EOF){
			printf("ERROR: y should be an integer\n");
			return 0;
		}
		values = alloc_values(game);
		scores = alloc_scores(game);
		error = guess_hint(game, x, y, &z, values, scores);
		if(handle_hint_err(error)){
			free(values);
			free(scores);
			return 0;
		}
		if(z){
			printf("list of possible hints:\n");
			for(i=0; i<z; i++){
				printf("%d: %.2f\n", values[i], scores[i]);
			}
		}
		else{
			printf("no hints");
		}
		free(values);
		free(scores);
		break;
	
	case NUM_SOLUTIONS:
		if(missingParameters(parameters_amount, 0, 0, NUM_SOLUTIONS)){
			return 0;
		}
		error = num_solutions(game, &i);
		if(handle_num_solutions_err(error)){
			return 0;
		}
		printf("the board has %d solutions\n", i);
		break;
	
	case AUTOFILL:
		if(missingParameters(parameters_amount, 0, 0, AUTOFILL)){
			return 0;
		}
		error = autofill(game);
		if(handle_autofill_err(error)){
			return 0;
		}
		printf("board has autofilled");
		needToPrint = 1;
		break;
	
	case RESET:
		if(missingParameters(parameters_amount, 0, 0, RESET)){
			return 0;
		}
		error = reset(game);
		if(handle_reset_err(error)){
			return 0;
		}
		printf("Reset...");
		needToPrint = 1;
		break;
	
	case EXIT:
		return 1;

	case BADKEY:
		printf("invalid command\n");
		break;
	}
	
	if (needToPrint)
	{
		print_board(game);
	}
	return 0;
}


/**
 * Prints the title of the game (our choice)
 */
void printTitle() {
	printf("Welcome to Exxxtreme Sudoku.\nHave fun!!! \n");
}
