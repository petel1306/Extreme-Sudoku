#include "board_handler.h"

typedef struct stack_node{
    Board* board;
    int index;
    struct stack_node* prev;
} call;