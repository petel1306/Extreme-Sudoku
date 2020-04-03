#include "game.h"

MarkError mark_errors(Game *game, int bool) {
	if ((bool>>1) != 0){
		return MARK_INCORRECT_VALUE;
	}
	game->markErrors = bool;
	return MARK_NONE;
}

