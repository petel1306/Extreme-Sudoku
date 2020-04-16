#include "backtracking.h"
#include "board_handler.h"
#include <stdlib.h>
#include <stdio.h>

int numberOfSolutions(Board* board){
    int N = board.n * board.m;
    int* x, y;
    int index, counter = 0; 
    Cell* cell;
    const int last = N * N - 1;
    
    /* first call */
    call * stack, next = NULL;
    stack = (call*) malloc(sizeof(call));
    stack->board = board;
    stack->index = 0;
    stack->prev = NULL;

    /*recursion*/
    while(stack != NULL){
        
        /*unpack variables*/
        index = stack->index;
        board = stack->board;
        
        indexToCords(index, N, x, y);
        cell = &(board->cells[x*][y*]);
        if(cell->state == FIXED){
            if(index == last){
                /* last cell is fixed - board is solved */
                counter++;
                stack = stack->prev;
            }
            else{
                /* skipes fixed cell */
                stack->index++;
            }
        }
        else{
            if(cell->value == N){
                /* backtracking */
                stack = stack->prev;
            }
            else{
                /* increament cell and check for errors */
                cell->value++;
                markErroneousBoard(board);
                if(!isErrBoard(board)){
                    if(index == last){
                        /* last cell is correct - got a solution */
                        counter++;
                        stack = stack->prev;
                    }
                    else{
                        /* cell is correct - recursive call to next cell */
                        next = (call*) malloc(sizeof(call));
                        next->board = cloneBoard(board);
                        next->index = index +1;
                        next->prev = stack;
                        stack = next;
                    }
                }
            }
        }
    }
}

void indexToCords(int index, int N, int* x, int* y){
    *x = index / N;
    *y = index - *x * N;
}