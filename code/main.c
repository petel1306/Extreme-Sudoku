/**
 * The main file of the project, contains the program's flow.
 */

#include <stdio.h>
#include "game.h"

int main() {
	/*
	 * Temporary implementation. "init" will be used later.
	 */
	Game* init = createGame();
	unsigned long offset = (uintptr_t) init->turn;
	printf("Hello World: %ld", (unsigned long) offset);
	return 0;
}
