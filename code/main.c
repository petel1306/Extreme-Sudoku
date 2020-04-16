/**
 * The main file of the project, contains the program's flow.
 *
 *
 * Remarks to Yali: (important to read!!!)
 *
 * Note: "parser" module's name has been change to "IO_handler"
 *
 * 1. After successful commands (edit, solve, set, autofill, undo, redo, reset, generate, guess) the board should be printed last.
 * 2. After successful commands (redo, undo) the change outputed should be printed.
 * 3. Parser responsible to manage both input and output of the game.
 *    First it parses the command.
 *    If the command is writen properly he passes it to the compatible function in game.
 *    After passing to the compatible function. In case invalid arguments - the function returns an error.
 *    otherwise the function makes the proper changes and returns an output (if has any).
 *    Finally: if the command was executed succesfuly it prints the fits output and the board,
 *    Else: It prints the related error with an explenation how to correct it.
 * 4. In case of exiting (follows exit command, error, etc...) parser should notify main (by returning non-zero integer in "getCommand" function).
 * 	  and print fits exit message.
 *	5. In the end of each successful move (i.e. set, autofill, generate, guess), parser uses the function isCompleted
 *	   and notifys the user (print compatible message) if the puzzle was solved successfully (accorrding to the guide).
 *
 *	Section 2:
 *	All BackTracking_solver and LP-ILP_solver solver function work on valid, not erroneous "Board" struct
 *	Example ("ILP_solver" moudle):
 *	int isSolvable(Board *board) {
 *	<bla bla bla>
 *	}
 */

#include "io_handler.h"
#include "main_aux.h"


int main() {
	Game* game = createGame(); /* Creates game on init mode */
	int isTimeToExit;

	printTitle();
	while (1) {
		isTimeToExit = getCommand(game);
		if (isTimeToExit) {
			destroyGame(game);
			return 0;
		}
	}
}

