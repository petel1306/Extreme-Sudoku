/*
 * This module contains function load and save from/ to files
 */


#include "board_handler.h"

/**
 * Represent errors may be found in the file during solve, edit functions
 */
typedef enum {FILE_NONE, FILE_CANT_OPEN, FILE_INCORRECT_FORMAT, FILE_INCORRECT_RANGE, FILE_ERRONEOUS_FIXED_CELLS} FileError;

FileError loadFileToBoard(char *path, Board **boardPointer, int isSolve);

typedef enum {SAVE_NONE, SAVE_NOT_AVAILABLE, SAVE_ERRONEOUS, SAVE_WITHOUT_SOLUTION, SAVE_CANT_OPEN, SAVE_GUROBI_ERR} SaveError;

SaveError saveBoardToFile(char *path, Board *board, int isEdit);
