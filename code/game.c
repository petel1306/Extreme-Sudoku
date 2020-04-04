#include "game.h"
#include <stdlib.h>
#include <stdio.h>


Game* createGame() {
	Game* game = (Game*) malloc(sizeof(Game));
	game->turn = createTurn();
	game->mode = INIT;
	return game;
}

void destroyGame(Game *game) {
	deleteAllTurns(game->turn);
	free(game);
}

MarkError mark_errors(Game *game, int bool) {
	if ((bool>>1) != 0){
		return MARK_INCORRECT_VALUE;
	}
	game->markErrors = bool;
	return MARK_NONE;
}

UndoRedoError undo(Game *game, char *change) {
	if (game->mode == INIT) {
		return DO_NOT_AVAILABLE;
	}
	if (game->turn->prev == NULL) {
		return DO_NO_MOVES;
	}
	sprintf(change, "Undo: %s", game->turn->change);
	game->turn = game->turn->prev;
	return DO_NONE;
}


UndoRedoError redo(Game *game, char *change) {
	if (game->mode == INIT) {
		return DO_NOT_AVAILABLE;
	}
	if (game->turn->next == NULL) {
		return DO_NO_MOVES;
	}
	game->turn = game->turn->next;
	sprintf(change, "Redo: %s", game->turn->change);
	return DO_NONE;
}


ResetError reset(Game *game) {
	if (game->mode == INIT) {
			return DO_NOT_AVAILABLE;
		}
	game->turn = getHead(game->turn);
	return RESET_NONE;
}

SolveError solve(Game *game, char* path) {
	Game* newGame = createGame();
	newGame->mode = SOLVE;
	newGame->markErrors = 1;
	newGame->turn->board = solveBoardFile(path);
	sprintf(newGame->turn->change, "solve %s", path);


	destroyGame(game);
	game = newGame;
	return SOLVE_NONE;
}
