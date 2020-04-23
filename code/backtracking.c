/*
 * This module contains the implementation of the exhaustive backtracking,
 * as well as a Call structure and suported methodes for the recursive calls. 
 */

#include "backtracking.h"
#include "board_handler.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * the function replaces the return statement in recursive implementation
 * @param call the current call
 * @return the call that invoked the current call
 */
Call* returnCall(Call* call){
    Call* previousCall;
    previousCall = call->prev;
    destroyBoard(call->board);
    free(call);
    return previousCall;
}


/**
 * @param board the board to calculate solutions to
 * @param filled that one never changed along the program and satisfies:
 * filled[i*N+j] iff board[i][j]!=1 (in the original board)
 * @returns number of solutions for board
 * @invariant board is always leagle and not solved
 */
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
        
        /* if cell is filled */
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
        /* cell is mutable */
        else{
            if(val == N){
                /* out of options - backtracking */
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