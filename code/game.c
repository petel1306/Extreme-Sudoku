#include "game.h"
#include <stdlib.h>
#include <stdio.h>

Game* createGame() {
	Game* game = (Game*) malloc(sizeof(Game));
	game->turn = NULL;
	game->mode = INIT;
	game->markErrors = 1; /* Default value (as explained in the project document)*/
	return game;
}

void deleteGameTurns(Game *game) {
	if (game->turn!=NULL) {
		deleteTurnsAhead(getHead(game->turn));
	}
}

void destroyGame(Game *game) {
	deleteGameTurns(game);
	free(game);
}



MarkError mark_errors(Game *game, int bool) {
	if (game->mode != SOLVE) {
		return MARK_NOT_AVAILABLE;
	}
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


FileError solve(Game *game, char* path) {
	FileError error;
	Board* newdBoard = NULL;

	error = loadFileToBoard(path, newdBoard, 1);
	if (error!=FILE_NONE) {
		return error;
	}

	/* Need to handle erroneous board */

	deleteGameTurns(game);
	game->mode = SOLVE;
	game->turn = createTurn();
	game->turn->board = newdBoard;
	sprintf(game->turn->change, "solve %s", path);

	return FILE_NONE;
}

FileError edit(Game *game, char* path) {
	FileError error;
	Board* newBoard = NULL;

	if (path == NULL) {
		newBoard = createBoard(3, 3);
	}
	else {
		error = loadFileToBoard(path, newBoard, 0);
		if (error!=FILE_NONE) {
			return error;
		}
	}

	/* Need to mark erroneous cells on the board */

	deleteGameTurns(game);
	game->mode = EDIT;
	game->turn = createTurn();
	game->turn->board = newBoard;
	sprintf(game->turn->change, "edit %s", path);

	return FILE_NONE;
}


SaveError save(Game *game, char *path) {
	Board board = *(game->turn->board);
	unsigned int isEdit = (game->mode == EDIT);

	if (game->mode == INIT) {
		return SAVE_NOT_AVAILABLE;
	}
	/*
	if (isEdit) {
		<Validates the board>
	}
	*/
	saveBoardToFile(path, board, isEdit);
	return SAVE_NONE;
}
