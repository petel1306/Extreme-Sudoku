#include "board_handler.h"

/**
 * Constitutes a node of the double-linked-list that represnts the turns of the user.
 * Each node represents a turn. you can go over the turns as you wish.
 * char * change - The change has been made on the board following the turn.
 */
struct list_node{
	Board* board;
	char* change;
	struct list_node *next;
	struct list_node *prev;
};
typedef struct list_node Turn;

Turn* createTurn();

void destroyTurn(Turn *turn);

Turn* getHead(Turn* curr);

void deleteTurnsBeyond(Turn* curr);

void addTurn(Turn* curr);
