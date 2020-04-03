#include "board_handler.h"
#include <stdlib.h>

/**
 * Creates an empty board
 */
Board* createBoard(int m, int n) {
	int i;
	int j;
	int N = m*n;
	Cell* row;
	Cell** mat;
	Board *res = (Board*) malloc(sizeof(Board));
	res->m = m;
	res->n = n;
	mat = (Cell**) malloc(N * sizeof(Cell*));
	for (i=0; i<N; ++i) {
		row = (Cell*) malloc (N * sizeof(Cell));
		for (j=0; j<N; ++j) {
			row[j].value = 0;
			row[j].state = REG;
		}
		mat[i] = row;
	}
	res->cells = mat;
	return res;
}

/**
 * Clones the board by-value.
 */
Board* cloneBoard(Board *board){
	Board *new_board = createBoard(board->m, board->n);
	int i;
	int j;
	int N = board->m * board->n;
	for (i=0; i<N; ++i) {
		for (j=0; j<N; ++j) {
			new_board->cells[i][j] = board->cells[i][j]; /*Assure this assignment is legal*/
		}
	}
	return new_board;
}

/**
 * Frees all memory resources associated with the board
 */
void destroyBoard(Board *board) {
	int i;
	int N = board->m * board->n;
	for (i=0; i<N; i++) {
		free(board->cells[i]);
	}
	free(board->cells);
	free(board);
}
