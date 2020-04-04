#include "list_handler.h"
#include "file_handler.h"

/**
 * Represents the mode of the game. whether init, edit or solve.
 */
typedef enum {INIT, EDIT, SOLVE} GameMode;


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


void print_board(Game *game);


typedef enum {SET_NONE} SetError; /* Will be filled later according to implementation needs*/

SetError set(Game *game, int x, int y, int val);


typedef enum {VALIDATE_NONE, VALIDATE_NOT_AVAILABLE, VALIDATE_ERRONEOUS} ValidateError;
/**
 * @post (solvable == 0) || (solvable == 1). (1 if board is solvable and 0 if not)
 */
ValidateError valdiate(Game *game, unsigned int *solvable);


typedef enum {GUESS_NONE} GuessError; /* Will be filled later according to implementation needs*/

GuessError guess(Game *game, float x);


typedef enum {GENERATE_NONE} GenerateError; /* Will be filled later according to implementation needs*/

GenerateError generate(Game *game, int x, int y);


typedef enum {DO_NONE, DO_NOT_AVAILABLE, DO_NO_MOVES} UndoRedoError; /* Mutal errors enum for both undo & redo comands*/

/**
 * @post change = string of the change that "undo" command has made
 */
UndoRedoError undo(Game *game, char *change);

/**
 * @post change = string of the change that "redo" command has made
 */
UndoRedoError redo(Game *game, char *change);


typedef enum {SAVE_NONE, SAVE_NOT_AVAILABLE, SAVE_ERRONEOUS, SAVE_WITHOUT_SOLUTION} SaveError;

SaveError save(Game *game, char *path);


typedef enum {HINT_NONE} HintError; /* Will be filled later according to implementation needs*/
/* Maybe have to add output print value as well*/

HintError hint(Game *game, int x, int y);


typedef enum {GUESSHINT_NONE} GuessHintError; /* Will be filled later according to implementation needs*/
/* Maybe have to add output print value as well*/

GuessHintError guess_hint(Game *game, int x, int y);


typedef enum {NUM_NONE, NUM_NOT_AVAILABLE, NUM_ERRONEOUS} NumSolutionsError;

NumSolutionsError num_solutions(Game *game, int *sol_amount);


typedef enum {AUTO_NONE, AUTO_NOT_AVAILABLE, AUTO_ERRONEOUS} AutofillError;

AutofillError autofill(Game *game);


typedef enum {RESET_NONE, RESET_NOT_AVAILABLE} ResetError;

ResetError reset(Game *game);

/**
 * note: an exit message should be printed
 */
void exitgame(Game *game);
