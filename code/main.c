/**
 * The main file of the project, contains the program's flow.
 */

#include "io_handler.h"
#include <stdio.h>


int main() {
	Game* game = createGame(); /* Creates game on init mode */
	int isTimeToExit;

	initStat(game);
	printTitle();
	while (1) {
		isTimeToExit = getCommand(game);
		if (isTimeToExit) {
			printf("Exiting...");
			destroyGame(game);
			return 0;
		}
	}
}

