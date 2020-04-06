#include "board_handler.h"
#include <stdlib.h>

/**
 * Creates a blank board
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
	Board *newBoard = createBoard(board->m, board->n);
	int i;
	int j;
	int N = board->m * board->n;
	for (i=0; i<N; ++i) {
		for (j=0; j<N; ++j) {
			newBoard->cells[i][j] = board->cells[i][j]; /*Assure this assignment is legal*/
		}
	}
	return newBoard;
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

/**
 * @pre size(cells) > N
 * @post "cells" = <N pointers to the cells in row "rowInd" on "board">
 */
void getRowCells(Board *board, int rowInd, Cell** cells) {
	int N = board->m * board->n;
	int j;
	for (j=0; j<N; ++j) {
		cells[j] = &(board->cells[rowInd][j]);
	}
}

/**
 * @pre size(cells) > N
 * @post "cells" = <N pointers to the cells in column "colInd" on "board">
 */
void getColumnCells(Board *board, int colInd, Cell** cells) {
	int N = board->m * board->n;
	int i;
	for (i=0; i<N; ++i) {
		cells[i] = &board->cells[i][colInd];
	}
}

/**
 * @pre size(cells) > N
 * @post "cells" = <pointers to the cells in block "blockInd" on "board">
 */

void getBlockCells(Board *board, int blockInd, Cell** cells) {
	int blockIndR = blockInd / board->m; /* block horizontal coordinate */
	int blockIndC = blockInd % board->m; /* block vertical coordinate */
	int i;
	int j;
	for (i = blockIndR * board->m; i < board->m; ++i) {
		for (j = blockIndC * board->n; j < board->n; ++j) {
			cells[i * board->n + j] = &board->cells[i][j];
		}
	}
}

int getBlockInd(Board *board, int i, int j) {
	int blockIndR = i / board->m; /* block horizontal coordinate */
	int blockIndC = j / board->n; /* block vertical coordinate */
	return blockIndR * board->m + blockIndC;
}

/*
*
 * Checking if there are erroneous value in the board
 * @param checkAll!=0 --> check erroneous amoung all cells states.
 * @param checkAll==0 --> check erroneous amoung fixed cells


unsigned int isErroneousBoard(Board *board, int checkAll) {
	unsigned int errBool = 0;
	int N = board->m * board->n;
	unsigned int *isShown[3];
	Cell **cells[3];
	int val[3];
	int i;
	int j;
	int k;
	for (k=0; k<3; ++k) {
		isShown[k] = (unsigned int*) malloc(N * sizeof(unsigned int));
		cells[k] = (Cell**) malloc(N * sizeof(Cell*));
	}
	* Checks erroneous for all rows, columns and blocks
	for (i=0; i<N; ++i) {
		getRowCells(board, i, cells[0]);
		getColumnCells(board, i, cells[1]);
		getBlockCells(board, i, cells[2]);
		for (j=0; j<N; ++j) {  * Initialize isShown
			isShown[j] = 0;
		}
		for (j=0; j<N; ++j) {
			for (k=0; k<3; ++k) {
				* If encounter the same value again in a certein row/cell/root - jump to the end of the function and returns true
				if (checkAll || cells[k][j]->state == FIXED) {
					val[k] = cells[k][j]->value;
					if (isShown[k][val[k]]) {  There is same values in row/column/block
						errBool = 1;
						goto exit;
					}
					isShown[k][val[k]] = 1;
				}
			}
		}
	}
	exit:
	for (k=0; k<3; ++k) {
		free(isShown[k]);
		free(cells[k]);
	}
	return errBool;
}*/

/**
 * Mark erroneous cells of certain value in a set of cells' pointers (the set may be a logic board row/column/block)
 */
void updateErroneous(Cell **cells, int N) {
	int *firstAppear = (int*) malloc((N+1) * sizeof(int));
	int *isSecondAppear = (int*) malloc((N+1) * sizeof(int));
	int val;
	int i;
	int idx;

	for (val=1; val<=N; ++val) { /* Initializing */
		firstAppear[val] = -1;
		isSecondAppear[val] = 0;
	}

	for (i=0; i<N; ++i) {
		val = cells[i]->value;
		if (val)  { /* (val != 0 i.e. the cell has actual value) */
			if (firstAppear[val] != -1) {
				/* If so, we already encountered this value and therfore the cell is erroneous.
				 * Note we dont mark the "first appear" cells as erroneous now, and we will do it later.
				 */
				isSecondAppear[val] = 1;
				if (cells[i]->state != FIXED)
					cells[i]->state = ERRONEOUS;
			}
			else {
				firstAppear[val] = i;
			}
		}
	}

	/* Now we mark the "first appear" cells and update cells who aren't erroneous */
	for (val=1; val<=N; ++val) {
		idx = firstAppear[val];
		if (idx != -1 && cells[idx]->state != FIXED) {
			if (isSecondAppear[val]) {
				cells[i]->state = ERRONEOUS;
			}
			else {
				cells[idx] = REG;
			}
		}
	}
}


void updateNeighborsState(Board *board, int rowInd, int colInd) {
	int N = board->m * board->n;
	Cell **cells = (Cell**) malloc(N * sizeof(Cell*));
	getRowCells(board, rowInd, cells); updateErroneous(cells, N);
	getColumnCells(board, colInd, cells); updateErroneous(cells, N);
	getBlockCells(board, getBlockInd(board, rowInd, colInd), cells); updateErroneous(cells, N);
	free(cells);
}


void updateBoardStates(Board *board) {
	int N = board->m * board->n;
	Cell **cells = (Cell**) malloc(N * sizeof(Cell*));
	int i;
	for (i=0; i<N; i++) {
		getRowCells(board, i, cells); updateErroneous(cells, N);
		getColumnCells(board, i, cells); updateErroneous(cells, N);
		getBlockCells(board, i, cells); updateErroneous(cells, N);
	}
	free(cells);
}

/**
 * @pre board cells' state is updated
 * @post returns whether the board is erroneous or not
 */
unsigned int isErrBoard(Board *board) {
	int N = board->m * board->n;
	int i;
	int j;
	for (i=0; i<N; ++i) {
		for (j=0; j<N; ++j) {
			if (board->cells[i][j].state == ERRONEOUS) {
				return 1;
			}
		}
	}
	return 0;
}
