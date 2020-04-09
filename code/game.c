#include "game.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Game* createGame() {
	Game* game = (Game*) malloc(sizeof(Game));
	game->turn = NULL;
	game->mode = INIT;
	game->markErrors = 1; /* Default value (as instructed in the project document)*/
	return game;
}

void deleteGameTurns(Game *game) {
	if (game->turn!=NULL) {
		deleteTurnsBeyond(getHead(game->turn));
	}
}

void destroyGame(Game *game) {
	deleteGameTurns(game);
	free(game);
}


BoardState isCompleted(Game *game) {
	Board* board = game->turn->board;
	int N = board->m * board->n;
	if (game->mode != SOLVE) {
		return COMPLETED_UNAVAILABLE;
	}
	if (board->nonEmptyAmount < N * N) {
		return NOT_COMPLETED;
	}
	if (isErrBoard(board)) {
		return ERRONEOUS_COMPLETED;
	}
	deleteGameTurns(game);
	game->mode = INIT;
	return SUCCESSFUL_COMPLETED;
}


PrintError print_board(Game *game) {
	if (game->mode == INIT) {
		return PRINT_NOT_AVAILABLE;
	}
	printBoard(game->turn->board, game->mode == EDIT || game->markErrors == 1);;
	return PRINT_NONE;
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


UndoRedoError undo(Game *game, char *output) {
	if (game->mode == INIT) {
		return DO_NOT_AVAILABLE;
	}
	if (game->turn->prev == NULL) {
		return DO_NO_MOVES;
	}
	sprintf(output, "Undo - %s", game->turn->change);
	game->turn = game->turn->prev;
	return DO_NONE;
}


UndoRedoError redo(Game *game, char *output) {
	if (game->mode == INIT) {
		return DO_NOT_AVAILABLE;
	}
	if (game->turn->next == NULL) {
		return DO_NO_MOVES;
	}
	game->turn = game->turn->next;
	sprintf(output, "Redo - %s", game->turn->change);
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
	Board* newBoard;
	Board* testBoard;
	int N;
	int i;
	int j;

	error = loadFileToBoard(path, &newBoard, 1);
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
	markErroneousBoard(testBoard);
	if (isErrBoard(testBoard)) {
		destroyBoard(newBoard);
		return FILE_ERRONEOUS_FIXED_CELLS;
	}
	destroyBoard(testBoard);

	/* At this point the board was successfuly loaded from the path and valid. Now we save the new configutation. */
	deleteGameTurns(game);
	game->mode = SOLVE;
	game->turn = createTurn();
	markErroneousBoard(newBoard);
	game->turn->board = newBoard;
	sprintf(game->turn->change, "solve %s\n", path);

	return FILE_NONE;
}


FileError edit(Game *game, char* path) {
	FileError error;
	Board* newBoard;

	if (path == NULL) {
		newBoard = createBoard(3, 3);
	}
	else {
		error = loadFileToBoard(path, &newBoard, 0);
		if (error!=FILE_NONE) {
			return error;
		}

	}

	deleteGameTurns(game);
	game->mode = EDIT;
	game->turn = createTurn();
	markErroneousBoard(newBoard);
	game->turn->board = newBoard;
	sprintf(game->turn->change, "edit %s\n", path);

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

/**
 * Adds a new move and marks it as the current move.
 * Mainly used by {set, autofill, generate, guess}.
 */
void newMove(Game *game) {
	addTurn(game->turn);
	game->turn = game->turn->next;
}

/**
 * Adds a new move and marks it as the current move.
 * Mainly used by {set, autofill, generate, guess} to cancel newMove function because an error.
 */
void cancelMove(Game *game) {
	game->turn = game->turn->prev;
	destroyTurn(game->turn->next);
	game->turn->next = NULL;
}

SetError set(Game *game, int x, int y, int val) {
	int rowInd = y-1;
	int colInd = x-1;
	Board *preBoard = game->turn->board;
	Board *newBoard;
	int N = preBoard->m * preBoard->n;
	int preVal;

	/* Validates conditions */
	if (game->mode == INIT) {
		return SET_NOT_AVAILABLE;
	}
	if (x < 1 || x > N) {
		return SET_INVALID_X;
	}
	if (y < 1 || y > N) {
		return SET_INVALID_Y;
	}
	if (val < 0 || val > N) {
		return SET_INVALID_VALUE;
	}
	if (preBoard->cells[rowInd][colInd].state == FIXED) {
		return SET_FIXED_CELL;
	}

	preVal = preBoard->cells[rowInd][colInd].value;

	/* Updates the move */
	newMove(game);
	newBoard = game->turn->board;
	newBoard->cells[rowInd][colInd].value = val;
	if (preVal != 0) { newBoard->nonEmptyAmount--; }
	if (val != 0) { newBoard->nonEmptyAmount++; }
	unmarkValidNeighbors(newBoard, rowInd, colInd, preVal);
	markErroneousNeighbors(newBoard, rowInd, colInd);
	sprintf(game->turn->change, "set cell X=%d, Y=%d from %d to %d\n", x, y, preVal, val);
	return SET_NONE;
}


AutofillError autofill(Game *game) {
	Board *preBoard = game->turn->board;
	Board *newBoard;
	char* change;
	char addition[32];
	int N = preBoard->m * preBoard->n;
	int fillValue;
	int i;
	int j;

	/* Validates conditions */
	if (game->mode != SOLVE) {
		return AUTO_NOT_AVAILABLE;
	}
	if(isErrBoard(game->turn->board)) {
		return AUTO_ERRONEOUS;
	}

	/* Updates the move */
	newMove(game);
	newBoard = game->turn->board;
	change = game->turn->change;
	sprintf(change, "fill the following cells:\n");
	for(i=0; i<N; ++i) {
		for (j=0; j<N; ++j) {
			fillValue = hasSingleOption(preBoard, i, j);
			if(fillValue) { /* There is a single value */
				newBoard->cells[i][j].value = fillValue;
				newBoard->nonEmptyAmount++;
				sprintf(addition , "	<X=%d, Y=%d> : %d\n", j+1, i+1, fillValue);
				strcat(change, addition);
			}
		}
	}
	markErroneousBoard(newBoard);
	return AUTO_NONE;
}


ValidateError valdiate(Game *game, unsigned int *solvable) {
	/* Validates conditions */
	if (game->mode == INIT) {
		return VALID_NOT_AVAILABLE;
	}
	if (isErrBoard(game->turn->board)) {
		return VALID_ERRONEOUS;
	}

	/*
	 * <ILP solver functionality>
	 * input: game->turn->board
	 * *solvable = output
	 */

	/* Default implementation for the compiler */
	printf("%d\n", *solvable);

	return VALID_NONE;
}


GuessError guess(Game *game, float x) {
	/* Validates conditions */
	if (game->mode != SOLVE) {
		return GUESS_NOT_AVAILABLE;
	}
	if (isErrBoard(game->turn->board)) {
		return GUESS_ERRONEOUS;
	}

	/* Updates the move */
	newMove(game);
	sprintf(game->turn->change, "guess the following cells:\n");

	/*
	 * <LP solver functionality>
	 * input: game->turn->board, x
	 */

	/* Default implementation for the compiler */
	printf("%f\n", x);

	return GUESS_NONE;
}


GenerateError generate(Game *game, int x, int y) {
	int N = game->turn->board->m * game->turn->board->n;
	int N2 = N*N;

	/* Validates conditions */
	if (game->mode != EDIT) {
		return GEN_NOT_AVAILABLE;
	}
	if (x < 0 || x > N2 - game->turn->board->nonEmptyAmount) {
		return GEN_INVALID_X;
	}
	if (y <= 0 || y > N2) {
		return GEN_INVALID_Y;
	}

	/* Updates the move */
	newMove(game);
	sprintf(game->turn->change, "generate board with %d non-empty cells:\n", y);

	/*
	 * <ILP solver functionality>
	 * input: game->turn->board, x, y
	 *
	 * if (<not succeeded>) {
	 * 		cancelMove(game);
	 * 		return GEN_NOT_SUCCEEDED;
	 * }
	 */

	return GEN_NONE;
}


HintError hintValidate(Game *game, int x, int y) { /* Validates hint, guess_hint conditions */
	Board *board = game->turn->board;
	int rowInd = y-1;
	int colInd = x-1;
	int N = board->m * board->n;

	if (game->mode != SOLVE) {
		return HINT_NOT_AVAILABLE;
	}
	if (x < 1 || x > N) {
		return HINT_INVALID_X;
	}
	if (y < 1 || y > N) {
		return HINT_INVALID_Y;
	}
	if (isErrBoard(board)) {
		return HINT_ERRONEOUS;
	}
	if (board->cells[rowInd][colInd].state == FIXED) {
		return HINT_FIXED_CELL;
	}
	if (board->cells[rowInd][colInd].value == 0) {
		return HINT_NON_EMPTY;
	}
	return HINT_NONE;
}


HintError hint(Game *game, int x, int y, int *hintVal) {
	HintError error = hintValidate(game, x, y);
	if (error != HINT_NONE) {
		return error;
	}

	/*
	 * <ILP solver functionality>
	 * input: game->turn->board, x, y
	 * *hintVal = output
	 *
	 * if (<the board is unsolvabled>) {
	 * 		return HINT_UNSOLVABLE;
	 * }
	 */

	/* Default implementation for the compiler */
	printf("%d\n", *hintVal);

	return HINT_NONE;
}


HintError guess_hint(Game *game, int x, int y, int **hintValList) {
	HintError error = hintValidate(game, x, y);
	if (error != HINT_NONE) {
		return error;
	}

	/*
	 * <LP solver functionality>
	 * input: game->turn->board, x, y
	 * *hintValList = output (pointer to a list)
	 *
	 * if (<the board is unsolvabled>) {
	 * 		return HINT_UNSOLVABLE;
	 * }
	 */

	/* Default implementation for the compiler */
	printf("%d\n", **hintValList);

	return HINT_NONE;
}


NumSolutionsError num_solutions(Game *game, int *sol_amount) {
	/* Validates conditions */
	if (game->mode == INIT) {
		return NUM_NOT_AVAILABLE;
	}
	if (isErrBoard(game->turn->board)) {
		return NUM_ERRONEOUS;
	}

	/*
	 * <Exhaustive Backtracking functionality>
	 * input: game->turn->board
	 * *sol_amount = output
	 */

	/* Default implementation for the compiler */
	printf("%d\n", *sol_amount);

	return NUM_NONE;
}
