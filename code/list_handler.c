#include "list_handler.h"
#include <stdlib.h>

#define STRING_BUFFER 256

/**
 * Creates an empty turn
 */
Turn* createTurn() {
	Turn *res = (Turn*) malloc(sizeof(Turn));
	res->board = NULL;
	res->change = (char*) malloc(STRING_BUFFER * sizeof(char));
	res->next = NULL;
	res->prev = NULL;
	return res;
}

void destroyTurn(Turn *turn) {
	free(turn->board);
	free(turn->change);
	free(turn);
}

/**
 * Returns the state before the first turn of the player (i.e. the head of the list)
 */
Turn* getHead(Turn* curr) {
	for ( ; curr->prev!=NULL; curr = curr->prev);
	return curr;
}

/**
 * Deletes and frees all turns ahead of a given turn (including the turn itself)
 */
void deleteTurnsAhead(Turn* curr) {
	if (curr!=NULL) {
			deleteTurnsAhead(curr->next);
			destroyTurn(curr);
		}
}
