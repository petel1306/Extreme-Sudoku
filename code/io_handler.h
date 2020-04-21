#include "game.h"
#define BADKEY -1
#define SOLVE 1
#define EDIT 2
#define MARK_ERRORS 3
#define PRINT_BOARD 4
#define SET 5
#define VALIDATE 6
#define GUESS 7
#define GENERATE 8
#define UNDO 9
#define REDO 10
#define SAVE 11
#define HINT 12
#define GUESS_HINT 13
#define NUM_SOLUTIONS 14
#define AUTOFILL 15
#define RESET 16
#define EXIT 17
#define NKEYS 17


typedef struct
{
    char* name;
    int  val;
} t_command;



int getCommand(Game *game);

void printTitle();
