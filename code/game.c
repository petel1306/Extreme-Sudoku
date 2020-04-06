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
	Board* newBoard = NULL;
	Board* testBoard;
	int N;
	int i;
	int j;

	error = loadFileToBoard(path, newBoard, 1);
	if (error!=FILE_NONE) {
		return error;
	}

	/* Validates that when ignoring non-fixed cells the board is not erroneous. */
	testBoard = cloneBoard(newBoard);
	N = testBoard->m * testBoard->n;
	for (i=0; i<N; ++i){
		for (j=0; j<N; ++j) {
			if (testBoard->cells[i][j].state != FIXED) {
				testBoard->cells[i][j].value = 0;
			}
			testBoard->cells[i][j].state = REG;
		}
	}
	updateBoardStates(testBoard);
	if (isErrBoard(testBoard)) {
		return FILE_ERRONEOUS_FIXED_CELLS;
	}
	destroyBoard(testBoard);

	/* At this point the board was successfuly loaded from the path. Now we save the new configutation. */
	deleteGameTurns(game);
	game->mode = SOLVE;
	game->turn = createTurn();
	updateBoardStates(newBoard);
	game->turn->board = newBoard;
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
	updateBoardStates(newBoard);

	deleteGameTurns(game);
	game->mode = EDIT;
	game->turn = createTurn();
	updateBoardStates(newBoard);
	game->turn->board = newBoard;
	sprintf(game->turn->change, "edit %s", path);

	return FILE_NONE;
}


SaveError save(Game *game, char *path) {
	Board *board = game->turn->board;
	unsigned int isEdit = (game->mode == EDIT);

	if (game->mode == INIT) {
		return SAVE_NOT_AVAILABLE;
	}
	if (isEdit) {
		if (isErrBoard(board)) {
			return SAVE_ERRONEOUS;
		}
		/*
		if ( <board is not solvable> ) {
			return SAVE_WITHOUT_SOLUTION;
		}
		*/
	}
	saveBoardToFile(path, board, isEdit);
	return SAVE_NONE;
}
