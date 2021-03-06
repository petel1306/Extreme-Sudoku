/*
 * This module contains the Board structure and all function related to it, and may be used throughout the game
 */

#ifndef BOARD_HANDLER_H_
#define BOARD_HANDLER_H_

/**
 * Represents the state of a cell. whether regular, fixed or erroneous
 */
typedef enum {REG, FIXED, ERRONEOUS} CellState;

/**
 * Holds the cell value and its state.
 */
typedef struct {
	int value;
	CellState state;
} Cell;

/**
 * Represent the Sudoku board
 * m - the width of block in the Sudoku board
 * n - the hight of block in the Sudoku board
 * **cells - the cells of the whole board.
 */
typedef struct {
	int m;
	int n;
	Cell **cells;
	int nonEmptyAmount; /* Number of non empty cells in the board */
} Board;

Board* createBoard(int m, int n);

Board* cloneBoard(Board *board);

void destroyBoard(Board *board);

void printBoard(Board *board, int printErroneous);

int setCell(Board *board, int rowInd, int ColInd, int val);

void markErroneousBoard(Board *board);

int isErrBoard(Board *board);

int hasSingleOption(Board *board, int rowInd, int colInd);

int getBlockInd(Board *board, int i, int j);

void getBlockCells(Board *board, int blockInd, Cell** cells);

void getColumnCells(Board *board, int colInd, Cell** cells);

void getRowCells(Board *board, int rowInd, Cell** cells);

#endif
