#include "list_handler.h"
#include "file_handler.h"

/**
 * Represents the mode of the game. whether init, edit or solve.
 */
typedef enum {INIT_MODE, EDIT_MODE, SOLVE_MODE} GameMode;


/**
 * This struct represents the current game status and properties.
 * mode : init, edit, solve.
 * markErros: 0 - false, 1 - true.
 */
typedef struct {
	Turn* turn;
	GameMode mode;
	unsigned int markErrors : 1;
} Game;

Game* createGame();

void destroyGame(Game *game);

void deleteGameTurns(Game *game);


typedef enum {NOT_COMPLETED, ERRONEOUS_COMPLETED, SUCCESSFUL_COMPLETED, COMPLETED_UNAVAILABLE} BoardState;
/**
 * Notifies of the board's state. If successful completed, goes back to init mode.
 * The command available only in solve mode.
 */
BoardState isCompleted(Game *game);


/*
 * Note: enum of solve, edit errors appears in "file_handler.h"
 */


/**
 *Note: solve gets unuseful former game as input and returns an new GAME object as output (if no errors).
 */
FileError solve(Game *game, char* path);


/**
 * Note: edit gets unuseful former game as input and returns an new GAME object as output (if no errors).
 * Note: path is originally optimal.
 * @pre  file not supplied --> path = NULL.
 */
FileError edit(Game *game, char* path);


typedef enum {MARK_NONE, MARK_NOT_AVAILABLE, MARK_INCORRECT_VALUE} MarkError;

MarkError mark_errors(Game *game, int bool);


typedef enum {PRINT_NONE, PRINT_NOT_AVAILABLE} PrintError;

PrintError print_board(Game *game);


typedef enum {SET_NONE, SET_NOT_AVAILABLE, SET_INVALID_X, SET_INVALID_Y, SET_INVALID_VALUE, SET_FIXED_CELL} SetError;

SetError set(Game *game, int x, int y, int val);


typedef enum {VALID_NONE, VALID_NOT_AVAILABLE, VALID_ERRONEOUS, VALID_GUROBI_ERR} ValidateError;
/**
 * @post VALIDATE_NONE --> (*solvable == 0) || (*solvable == 1). (1 if board is solvable and 0 if not)
 */
ValidateError validate(Game *game, int *solvable);


typedef enum {GUESS_NONE, GUESS_NOT_AVAILABLE, GUESS_ERRONEOUS, GUESS_GUROBI_ERR} GuessError;

GuessError guess(Game *game, float x);


typedef enum {GEN_NONE, GEN_NOT_AVAILABLE,  GEN_INVALID_X, GEN_INVALID_Y, GEN_NOT_SUCCEEDED, GEN_GUROBI_ERR} GenerateError;

GenerateError generate(Game *game, int x, int y);


typedef enum {DO_NONE, DO_NOT_AVAILABLE, DO_NO_MOVES} UndoRedoError; /* Mutal errors enum for both undo & redo comands*/

/**
 * @post output = string of the change that "undo" command has made
 * Note: in case no error - parser should print the change and then board;
 */
UndoRedoError undo(Game *game, char *output);

/**
 * @post output = string of the change that "redo" command has made
 * Note: in case no error - parser should print the change and then board;
 */
UndoRedoError redo(Game *game, char *output);


/**
 * Note: enum of save errors appears in "file_handler.h"
 */
SaveError save(Game *game, char *path);


typedef enum {HINT_NONE, HINT_NOT_AVAILABLE, HINT_INVALID_X, HINT_INVALID_Y, HINT_ERRONEOUS, HINT_FIXED_CELL, HINT_NON_EMPTY, HINT_UNSOLVABLE, HINT_GUROBI_ERR} HintError;

/**
 * @post HINT_NONE --> *hintVal = the value of cell <X,Y> found by the *ILP* solution
 */
HintError hint(Game *game, int x, int y, int *hintVal);

/**
 * @post HINT_NONE --> *hintVal = array of all legal values of cell <X,Y> and their scores found by the *LP* solution
 */
HintError guess_hint(Game *game, int x, int y, int *n, int *values, double *scores);


typedef enum {NUM_NONE, NUM_NOT_AVAILABLE, NUM_ERRONEOUS} NumSolutionsError;

NumSolutionsError num_solutions(Game *game, int *sol_amount);


typedef enum {AUTO_NONE, AUTO_NOT_AVAILABLE, AUTO_ERRONEOUS} AutofillError;

AutofillError autofill(Game *game);


typedef enum {RESET_NONE, RESET_NOT_AVAILABLE} ResetError;

ResetError reset(Game *game);
