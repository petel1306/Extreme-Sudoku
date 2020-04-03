#include "list_handler.h"
#include <stdlib.h>

/**
 * Creates an empty turn
 */
Turn* createTurn() {
	Turn *res = (Turn*) malloc(sizeof(Turn));
	res->next = NULL;
	res->prev = NULL;
	return res;
}

/**
 * Returns the state before the first turn of the player (i.e. the head of the list)
 */
Turn* getHead(Turn* curr) {
	for ( ; curr->prev!=NULL; curr = curr->prev);
	return curr;
}

void deleteTurnsRec(Turn* curr) {
	if (curr!=NULL) {
			deleteTurnsRec(curr->next);
			free(curr);
		}
}

/**
 * Deletes and frees further turns of a given turn (excluding the turn itself)
 */
void deleteFurtherTurns(Turn* curr) {
	curr->next = NULL;
	deleteTurnsRec(curr->next);
}

/**
 * Deletes and frees all turns exist in the list
 */
void deleteAllTurns(Turn* curr) {
	deleteTurnsRec(getHead(curr));
}
