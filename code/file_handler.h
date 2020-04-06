#include "board_handler.h"

/*
 * The module dont handle *yet* <stdio.h> functions' errors like: fopen, fclose, etc...
 */

/**
 * Represent errors may be found in the file during solve, edit functions
 */
typedef enum {FILE_NONE, FILE_NOT_EXIST, FILE_INCORRECT_FORMAT, FILE_INCORRECT_RANGE, FILE_ERRONEOUS_FIXED_CELLS} FileError;

FileError loadFileToBoard(char *path, Board *board, int solveMode);


void saveBoardToFile(char *path, Board *board, int isEdit);
