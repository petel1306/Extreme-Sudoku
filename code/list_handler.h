#include "board_handler.h"

/**
 * Constitutes a node of the double-linked-list that represnts the turns of the user.
 * Each node represents a turn. you can go over the turns as you wish.
 * char * change - The change that has been made on the board following the command.
 */
struct list_node{
	Board board;
	char* change;
	struct list_node *next;
	struct list_node *prev;
};
typedef struct list_node Turn;

Turn* createTurn();

Turn* getHead(Turn* curr);

void deleteFurtherTurns(Turn* curr);

void deleteAllTurns(Turn* curr);
