#include "file_handler.h"
#include <stdio.h>

/**
 * Scans file and transforms it into a Board.
 * @param solveMode = true --> function called from solveBoardFile;
 * @pre path is existing file
 * @post board contains the scaned file (if no errors)
 */
FileError loadFileToBoard(char *path, Board *board, int isSolve) {
	int N;
	int i;
	int j;
	int val;
	int c;

	FILE *file = fopen(path, "r");
	if (file == NULL ){
		return FILE_NOT_EXIST;
	}

	if (fscanf(file, "%d", &board->m) != 1 || fscanf(file, "%d", &board->n) != 1) { /* Scanning m, n */
		return FILE_INCORRECT_FORMAT;
	}
	N = board->m * board->n;

	for (i=0; i<N; ++i) { /* Scanning board cells and filling their values to the board */
		for (j=0; j<N; ++j) {
			if (fscanf(file, "%d", &val) != 1) {
				return FILE_INCORRECT_FORMAT;
			}
			if (val < 0 || val > N) {
				return FILE_INCORRECT_RANGE;
			}
			board->cells[i][j].value = val;
			if (isSolve && (c = fgetc(file)) == '.') {/* Checking the cell's state */
				board->cells[i][j].state = FIXED;
			}
			else {
				board->cells[i][j].state = REG;
			}
		}
	}
	fclose(file);
	/* printf("Board: m=%d, n=%d", board->m, board->n) */;
	return FILE_NONE;
}

/**
 * Scans file and transforms it into a Board.
 * @param solveMode = true --> function called from solveBoardFile;
 * @pre path is existing file
 * @post file in "path" contains the board
 */
void saveBoardToFile(char *path, Board board, int isEdit) {
	int N;
	int i;
	int j;
	int val;
	CellState state;

	FILE *file = fopen(path, "w");
	fprintf(file , "%d %d\n", board.m, board.n); /* Writing the board's sizes */
	N = board.m * board.n;
	for (i=0; i<N; ++i) { /* Scanning board cells and writing their values to the file */
		for (j=0; j<N-1; ++j) {
			val = board.cells[i][j].value;
			state = board.cells[i][j].state;
			if (val!=0 && (isEdit || state==FIXED)) { /* Writing the inner cell values in each line */
				fprintf(file, "%d. ", val);
			}
			else {
				fprintf(file, "%d ", val);
			}
		}
		val = board.cells[i][N-1].value;
		state = board.cells[i][N-1].state;
		if (val!=0 && (isEdit || state==FIXED)) { /* Writing the most right cell value in each line */
			fprintf(file, "%d.\n", val);
		}
		else {
			fprintf(file, "%d\n", val);
		}
	}
	fclose(file);
}
