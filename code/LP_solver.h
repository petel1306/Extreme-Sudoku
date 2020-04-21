#include "board_handler.h"
#define NON_SOLUTION 1
#define OPT_STOP_EARLY 2
#define GUROBI_FAILURE -1


int **create2D(int dim, int defaultValue);

void destroy2D(int **mat, int dim);

int validateILP(Board *board);

int guessLP(Board *board, float threshold);

int generateILP(Board *board);

int hintILP(Board *board, int x, int y, int *val);

int guess_hintLP(Board *board, int x, int y, double *scores);