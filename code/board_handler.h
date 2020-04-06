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
} Board;

Board* createBoard(int m, int n);

Board* cloneBoard(Board *board);

void destroyBoard(Board *board);

void getRowCells(Board *board, int rowInd, Cell** cells);
void getColumnCells(Board *board, int colInd, Cell** cells);
void getBlockCells(Board *board, int blockInd, Cell** cells);

int getBlockInd(Board *board, int i, int j);

void updateErroneous(Cell **cells, int N);

void updateNeighborsState(Board *board, int rowInd, int colInd);

void updateBoardStates(Board *board);

unsigned int isErrBoard(Board *board);

#endif
