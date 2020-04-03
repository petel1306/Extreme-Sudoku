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
