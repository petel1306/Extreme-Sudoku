#include "io_handler.h"
#include <stdio.h>


/**
 * The entry function of this module.
 * Composite of: 1. get input from the user 2. parse it 3. operates if valid 4. prints fits output
 * @return non-value integer if the program should exit (because of exit command, error, etc...). else: 0
 */
int getCommand(Game *game) {
	return game->mode == INIT; /* Temporary meaningless implementation. Wll be filled by Yali. */
}


/**
 * Prints the title of the game (our choice)
 */
void printTitle() {
	printf("Welcome to Exxxtreme Sudoku.\nHave fun!!! ");
}
