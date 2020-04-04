/**
 * The main file of the project, contains the program's flow.
 */

#include <stdio.h>
#include "game.h"
#include "main_aux.h"

/* Temporary implementation. "init" will be used later. */
int main() {
	Game* init = createGame();
	unsigned long offset = (uintptr_t) init->turn;
	printf("Hello World: %ld", (unsigned long) offset);
	return 0;
}
