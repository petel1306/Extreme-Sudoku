/** this module create and run the GUROBI model to solve the board using LP/ILP
 * it is also contains implementation of each function that use LP/ILP
 */

#include "board_handler.h"
#define NON_SOLUTION 1
#define OPT_STOP_EARLY 2
#define GUROBI_FAILURE -1


int **create2D(int dim, int defaultValue);

void destroy2D(int **mat, int dim);

/**
 * @returns 0 - if board is solvable; 1 - if it isn't;
 */
int validateILP(Board *board);

/**
 * @param threshold pass to the guess function
 * @post board is filled by guess logic
 */
int guessLP(Board *board, float threshold);

/**
 * @returns 0 - if the board has solved; 1 - if isn't
 * @post board is filled by valid values
 */
int generateILP(Board *board);

/**
 * @param x i index
 * @param y j index
 * @param val pointer to hint
 * @post if successed - val contains the hint
 * @returns 0 - if board is solvable; 1 - if it isn't;
 */
int hintILP(Board *board, int x, int y, int *val);

/**
 * @param x i index
 * @param y j index
 * @pre scores is of length N
 * @post if successed scores[k] = Xxy(k+1)
 * @returns 0 - if board is solvable; 1 - if it isn't;
 */
int guess_hintLP(Board *board, int x, int y, double *scores);