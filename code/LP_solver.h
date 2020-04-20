#include "board_handler.h"
#define NON_SOLUTION 1
#define OPT_STOP_EARLY 2
#define GUROBI_FAILURE -1

int solveILP(Board *board);

int **create2D(int dim, int defaultValue);

void destroy2D(int **mat, int dim);