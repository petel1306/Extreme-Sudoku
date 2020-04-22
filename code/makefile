CC = gcc
OBJS = backtracking.o board_handler.o file_handler.o game.o io_handler.o list_handler.o LP_solver.o main_aux.o main.o

EXEC = sudoku
COMP_FLAG = -g -ansi -Wall -Wextra -Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
main.o: main.c main_aux.h  io_handler.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
main_aux.o: main_aux.c main_aux.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
io_handler.o: io_handler.c io_handler.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
game.o: game.c game.h backtracking.h LP_solver.h list_handler.h file_handler.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
backtracking.o: backtracking.c backtracking.h board_handler.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
LP_solver.o: LP_solver.c LP_solver.h game.h board_handler.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
list_handler.o: list_handler.c list_handler.h board_handler.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
file_handler.o: file_handler.c file_handler.h board_handler.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
board_handler.o: board_handler.c board_handler.h main_aux.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
clean:
	rm -f *.o $(EXEC)

