/*
 * This module contains function load and save from/ to files
 */

#include "file_handler.h"
#include <stdio.h>

#ifndef NULL
#define NULL   ((void *) 0)
#endif

/**
 * Scans file and transforms it into a Board.
 * @param solveMode = true --> function called from solveBoardFile;
 * @pre path is existing file
 * @pre board != NULL
 * @post board contains the scaned file (if no errors)
 */
FileError loadFileToBoard(char *path, Board **boardPointer, int isSolve) {
	FileError error;
	Board* board;
	int m;
	int n;
	int N;
	int i;
	int j;
	int val;
	int c;

	FILE *file = fopen(path, "r");
	if (file == NULL ){
		error = FILE_CANT_OPEN;
		goto exit;
	}

	if (fscanf(file, "%d", &m)!=1 || m <= 0 || fscanf(file, "%d", &n)!=1 || n<=0) { /* Scanning m, n */
		error = FILE_INCORRECT_FORMAT;
		goto exit;
	}

	board = createBoard(m, n);
	N = m*n;

	for (i=0; i<N; ++i) { /* Scanning board cells and filling their values to the board */
		for (j=0; j<N; ++j) {
			if (fscanf(file, "%d", &val) != 1) {
				error = FILE_INCORRECT_FORMAT;
				goto exit;
			}
			if (val < 0 || val > N) {
				error = FILE_INCORRECT_RANGE;
				goto exit;
			}
			board->cells[i][j].value = val;
			if (val != 0) {
				++board->nonEmptyAmount;
			}
			c = fgetc(file);
			if (isSolve && c == '.') {/* Checking the cell's state */
				board->cells[i][j].state = FIXED;
			}
		}
	}
	error = FILE_NONE;
	*boardPointer = board;

	exit: /* Exiting clean */
	if (error != FILE_CANT_OPEN) {
		fclose(file);
	}
	if (error != FILE_NONE) {
		destroyBoard(board);
	}
	return error;
}

/**
 * Scans file and transforms it into a Board.
 * @param solveMode = true --> function called from solveBoardFile;
 * @pre path is existing file
 * @post file in "path" contains the board
 * @returns 0 if succeeded, otherwise: 1
 */
SaveError saveBoardToFile(char *path, Board *board, int isEdit) {
	int N;
	int i;
	int j;
	int val;
	CellState state;

	FILE *file = fopen(path, "w");
	if (file == NULL ){
		return SAVE_CANT_OPEN;
	}

	fprintf(file , "%d %d\n", board->m, board->n); /* Writing the board's sizes */
	N = board->m * board->n;
	for (i=0; i<N; ++i) { /* Scanning board cells and writing their values to the file */
		for (j=0; j<N-1; ++j) {
			val = board->cells[i][j].value;
			state = board->cells[i][j].state;
			if (val!=0 && (isEdit || state==FIXED)) { /* In edit mode, mark the cell as FIXED. */
				fprintf(file, "%d. ", val);
			}
			else {
				fprintf(file, "%d ", val);
			}
		}
		val = board->cells[i][N-1].value;
		state = board->cells[i][N-1].state;
		if (val!=0 && (isEdit || state==FIXED)) { /* Writing the most right cell value in each line */
			fprintf(file, "%d.\n", val);
		}
		else {
			fprintf(file, "%d\n", val);
		}
	}

	fclose(file);
	return SAVE_NONE;
}
