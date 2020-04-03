/**
 * This module contains auxiliary factions may be used throughout the program,
 * and not fit any other module.
 */

#include "list_handler.h"


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
	Turn turn;
	GameMode mode;
	unsigned int markErrors : 1;
} Game;
