#include "board_handler.h"
#include "main_aux.h"
#include <stdlib.h>
#include <stdio.h>


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
	res->nonEmptyAmount = 0;
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
	newBoard->nonEmptyAmount = board->nonEmptyAmount;
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

	if (board == NULL)
		return;

	for (i=0; i<(board->m * board->n); i++) {
		free(board->cells[i]);
	}
	free(board->cells);
	free(board);
}

/**
 * Returns the char value of a cell state
 */
char stateToChar(CellState state, int printErroneous) {
	switch (state) {
	case REG: return ' ';
	case FIXED: return '.';
	case ERRONEOUS: return (printErroneous ? '*' : ' ');
	}
	return '\0';
}


void printSeparatorRow(int len) {
	int i;
	for (i=0; i<len; ++i) {
		printf("-");
	}
	printf("\n");
}
/**
 * Prints the board as instructed in the guide
 */
void printBoard(Board *board, int printErroneous) {
	Cell cell;
	int m = board->m;
	int n = board->n;
	int N = m * n;
	int printWidth = 4*N+m+1;
	int i; int ii;
	int j; int jj;

	printSeparatorRow(printWidth);
	for (i=0; i<n; ++i) { /* i represets row block index */
		for (ii=0; ii<m; ++ii) { /* ii represets row index inside a block */
			for(j=0; j<m; ++j) { /* j represets column block index */
				printf("|");
				for (jj=0; jj<n; ++jj) { /* jj represets column index inside a block */
					cell = board->cells[i*m + ii][j*n + jj];
					printf(" %2d%c", cell.value, stateToChar(cell.state, printErroneous));
				}
			}
			printf("|\n");
		}
		printSeparatorRow(printWidth);
	}
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
	int blockOffsetR = blockIndR * board->m;
	int blockOffsetC = blockIndC * board->n;
	int ind = 0;
	int i;
	int j;
	for (i = 0 ; i < board->m; ++i) {
		for (j = 0; j < board->n; ++j) {
			cells[ind] = &board->cells[blockOffsetR + i][blockOffsetC + j];
			++ind;
		}
	}
}

int getBlockInd(Board *board, int i, int j) {
	int blockIndR = i / board->m; /* block horizontal coordinate */
	int blockIndC = j / board->n; /* block vertical coordinate */
	return blockIndR * board->m + blockIndC;
}

/**
 * Mark erroneous cells in a set of cells' pointers (the set may be a logic board row/column/block)
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
				cells[idx]->state = ERRONEOUS;
			}
			else {
				cells[idx] = REG;
			}
		}
	}
	free(firstAppear);
	free(isSecondAppear);;
}

/**
 * Check if set of cells' pointers are valid in terms of constitute a non-erroneous row/column/block
 */
int isValidValue(Cell **cells, int N, int val) {
	int counter = 0;
	int i;
	for (i=0; i<N; ++i) {
		if (cells[i]->value == val) {
			counter++;
		}
	}
	if (counter < 2) {
		return 1;
	}
	else {
		return 0;
	}
}


/**
 * Unmarks neighbors of a specific value who are not erroneouse anymore.
 * Mainly used in "set" command
 */
void unmarkValidNeighbors(Board *board, int rowInd, int colInd, int val) {
	int N = board->m * board->n;
	int blockInd = getBlockInd(board, rowInd, colInd);
	int blockIndR = blockInd / board->m;
	int blockIndC = blockInd % board->m;
	Cell **cells = (Cell**) malloc(N * sizeof(Cell*));
	int *boolRows = (int*) malloc(N * sizeof(int));
	int *boolCols = (int*) malloc(N * sizeof(int));
	int *boolBlocks = (int*) malloc(N * sizeof(int));
	int i;
	int j;
	int k;

	for (k=0; k<N; ++k){
		getRowCells(board, k, cells); boolRows[k] = isValidValue(cells, N, val);
		getColumnCells(board, k, cells); boolCols[k] = isValidValue(cells, N, val);
		getBlockCells(board, k, cells); boolBlocks[k] = isValidValue(cells, N, val);
	}

	if (boolRows[rowInd]) {
		for (j=0; j<N; ++j) {
			if ( board->cells[rowInd][j].state == ERRONEOUS && boolCols[j] && boolBlocks[getBlockInd(board, rowInd, j)] ) {
				board->cells[rowInd][j].state = REG;
			}
		}
	}

	if (boolCols[colInd]) {
		for (i=0; i<N; ++i) {
			if ( board->cells[i][colInd].state == ERRONEOUS && boolRows[i] && boolBlocks[getBlockInd(board, i, colInd)] ) {
				board->cells[i][colInd].state = REG;
			}
		}
	}


	if (boolBlocks[blockInd]) {
		for (i = blockIndR * board->m; i < ((blockIndR + 1) * board->m); ++i) {
			for (j = blockIndC * board->n; j < ((blockIndC + 1) * board->n); ++j) {
				if( board->cells[i][j].state == ERRONEOUS && boolRows[i] && boolCols[j]) {
					board->cells[i][j].state = REG;
				}
			}
		}
	}
	free(cells);
	free(boolRows);
	free(boolCols);
	free(boolBlocks);
}

/**
 * Marks all the erroneous cells who are neighbors of a given cell
 * Mainly used in "set" command
 */
void markErroneousNeighbors(Board *board, int rowInd, int colInd) {
	int N = board->m * board->n;
	Cell **cells = (Cell**) malloc(N * sizeof(Cell*));
	getRowCells(board, rowInd, cells); updateErroneous(cells, N);
	getColumnCells(board, colInd, cells); updateErroneous(cells, N);
	getBlockCells(board, getBlockInd(board, rowInd, colInd), cells); updateErroneous(cells, N);
	free(cells);
}


/**
 * Mark all the erroneous cells on the board.
 */
void markErroneousBoard(Board *board) {
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
 * @pre board cells' state are updated
 * @post returns whether the board is erroneous or not
 */
int isErrBoard(Board *board) {
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


void updateSeenValues(Cell **cells, int N, int* seenValues) {
	int i;
	for (i=0; i<N; i++) {
		seenValues[cells[i]->value] = 1;
	}
}


/**
 * If the board has a single option to fill the cell with a value, it returns the value. otherwise returns 0.
 */
int hasSingleOption(Board *board, int rowInd, int colInd) {
	int N = board->m * board->n;
	int *seenValues;
	int val;
	Cell **cells;
	int counter = 0;
	int suspectedValue;

	if (board->cells[rowInd][colInd].value != 0) { /* Checks if empty cells */
		return 0;
	}

	/* Allocates arrays */
	seenValues = (int*) malloc((N+1) * sizeof(int));
	for (val=0; val<=N; val++) {
			seenValues[val] = 0;
		}
	cells = (Cell**) malloc(N * sizeof(Cell*));

	/* Updating "seenValues" according to the cell's neighbors */
	getRowCells(board, rowInd, cells); updateSeenValues(cells, N, seenValues);
	getColumnCells(board, colInd, cells); updateSeenValues(cells, N, seenValues);
	getBlockCells(board, getBlockInd(board, rowInd, colInd), cells); updateSeenValues(cells, N, seenValues);


	/* Counting the non-filled values */
	for (val=1; val<=N; val++) {
		if (!seenValues[val]) {
			counter++;
			suspectedValue = val;
		}
	}

	/* Free resources */
	free(seenValues);
	free(cells);

	/* Returns value */
	if (counter == 1) {
		return suspectedValue;
	}
	return 0;
}
