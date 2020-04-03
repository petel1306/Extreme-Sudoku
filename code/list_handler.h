#include "board_handler.h"

/**
 * Constitutes a node of the double-linked-list that represnts the turns of the user.
 * Each node represents a turn. you can go over the turns as you wish.
 */
struct list_node{
	Board board;
	struct list_node *next;
	struct list_node *prev;
};
typedef struct list_node Turn;

Turn* createTurn();

Turn* getHead(Turn* curr);

void deleteFurtherTurns(Turn* curr);

void deleteAllTurns(Turn* curr);
