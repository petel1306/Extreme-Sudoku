#include "backtracking.h"
#include "board_handler.h"
#include <stdlib.h>
#include <stdio.h>


Call* returnCall(Call* call){
    Call* previousCall;
    previousCall = call->prev;
    destroyBoard(call->board);
    free(call);
    return previousCall;
}

int numberOfSolutions(Board* board, int* filled){
    int N = board->n * board->m;
    int x, y, val;
    int index, counter = 0; 
    const int last = N * N - 1;
    
    /* first call */
    Call *stack, *next = NULL;
    stack = (Call*) malloc(sizeof(Call));
    stack->board = board;
    stack->index = 0;
    stack->prev = NULL;

    /*recursion*/
    while(stack != NULL){
        /*unpack variables*/
        index = stack->index;
        board = stack->board;
        
        /* get cords */
        x = index / N;
        y = index - x * N;
        val = board->cells[x][y].value;
        
        if(filled[x * N + y]){
            if(index == last){
                /* last cell is filled - board is solved */
                counter++;
                stack = returnCall(stack);
            }
            else{
                /* skipes filled cell */
                stack->index++;
            }
        }
        else{
            if(val == N){
                /* backtracking */
                stack = returnCall(stack);
            }
            else{
                /* increament cell and check for errors */
                setCell(board, x, y, val+1);
                if(!isErrBoard(board)){
                    if(index == last){
                        /* last cell is correct - got a solution */
                        counter++;
                        stack = returnCall(stack);
                    }
                    else{
                        /* cell is correct - recursive call to next cell */
                        next = (Call*) malloc(sizeof(Call));
                        next->board = cloneBoard(board);
                        next->index = index +1;
                        next->prev = stack;
                        stack = next;
                    }
                }
            }
        }
    }
    return counter;
}