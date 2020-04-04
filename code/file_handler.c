#include "file_handler.h"
#include <stdio.h>

Board* solveBoardFile(char *path){
	fprintf(stdout, path);
	return createBoard(3,3);
}

Board* editBoardFile(char *path){
	fprintf(stdout, path);
	return createBoard(3,3);
}
