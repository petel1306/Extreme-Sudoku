#include "list_handler.h"
#include <stdlib.h>

#define STRING_BUFFER 128

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
	destroyBoard(turn->board);
	free(turn->change);
	free(turn);
}

/**
 * Returns the state before the first turn of the player (i.e. the head of the list)
 */
Turn* getHead(Turn *curr) {
	for ( ; curr->prev!=NULL; curr = curr->prev);
	return curr;
}

/**
 * Deletes and frees all turns ahead of a given turn (including the turn itself)
 */
void deleteTurnsBeyond(Turn* curr) {
	if (curr!=NULL) {
			deleteTurnsBeyond(curr->next);
			destroyTurn(curr);
		}
}

/**
 * Adds new turn as the next of the current turn (has the same board)
 */
void addTurn(Turn *curr) {
	Turn* new = createTurn();
	deleteTurnsBeyond(curr->next);
	new->prev = curr;
	curr->next = new;
	new->board = cloneBoard(curr->board);
}
