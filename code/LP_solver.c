/* Copyright 2013, Gurobi Optimization, Inc. */

/* This example formulates and solves the following simple MIP model:

     maximize    x + 3 y + 2 z
     subject to  x + 2 y + 3 z <= 5
                 x +   y       >= 1
     x, y, z binary
*/

#include <stdlib.h>
#include <stdio.h>
#include "gurobi_c.h"
#include "board_handler.h"
#include "game.h"

/* creates 2D array ant initialize */
int **create2D(int dim, int defaultValue)
{
	int i, j;
	int **mat = (int **)malloc(sizeof(int *) * dim);
	for (i = 0; i < dim; i++)
	{
		mat[i] = (int *)malloc(sizeof(int) * dim);
		for (j = 0; j < dim; j++)
		{
			mat[i][j] = defaultValue;
		}
	}
	return mat;
}

void destroy2D(int **mat, int dim)
{
	int i;
	for (i = 0; i < dim; i++)
	{
		free(mat[i]);
	}
	free(mat);
}

void calculateCellsOptions(Board *board, Cell **cells, int *cellsOptions, int n)
{
	int i, v;
	for (i = 0; i < n, i++)
	{
		v = cells[i]->value;
		if (v > 0)
			cellsOptions[v - 1] = 0;
	}
}

void calculateOptions(Board *board, int ***options, int **numberOfOptions)
{
	int N = board->n * board->m;
	int **rowsOptions, **columnsOptions, **blocksOptions;
	int i, j, v, blockInd;
	int counter = 0;
	Cell **cells = (Cell **)malloc(N * sizeof(Cell **));

	/* calculate lines */
	rowsOptions = create2D(N, 1);
	for (i = 0; i < N; i++)
	{
		cells = getRowCells(board, i, cells);
		calculateCellsOptions(board, cells, rowsOptions[i], N);
	}

	/* calculate columns */
	columnsOptions = create2D(N, 1);
	for (i = 0; i < N; i++)
	{
		cells = getColumnCells(board, i, cells);
		calculateCellsOptions(board, cells, columnsOptions[i], N);
	}

	/* calculate blocks */
	blocksOptions = create2D(N, 1);
	for (i = 0; i < N; i++)
	{
		cells = getBlockCells(board, i, cells);
		calculateCellsOptions(board, cells, linesOptions[i], N);
	}

	for (i = 0; i < N; i++)
	{
		for (j = 0, j < N; j++)
		{
			for (v = 0, v < N; v++)
			{
				blockInd = getBlockInd(board, i, j);
				if (rowsOptions[i][v] && columnsOptions[j][v] && blocksOptions[blockInd][v])
				{
					options[i][j][v] = ++counter;
					numberOfOptions[i][j]++;
				}
				else
				{
					options[i][j][v] = 0;
				}
				
			}
		}
	}

	free(cells);
	destroyBoard(blocksOptions);
	destroyBoard(columnsOptions);
	destroyBoard(rowsOptions);
	return counter;
}

int solveLP(Board *board)
{
	int N = board->m * board->n;
	int i,j,k;
	int **numberOfOptions, ***indexMapping;
	int numberOfVariables;
	GRBenv *env = NULL;
	GRBmodel *model = NULL;
	int error = 0;
	double *sol, *val, *obj;
	int *ind;
	char *vtype;
	int optimstatus;
	double objval;

	numberOfOptions = create2D(N, 0);
	indexMapping = (int***) malloc(N * sizeof(int**));
	for(i=0; i<N; i++){
		indexMapping[i] = create2D(N, 0);
	};

	numberOfVariables = calculateOptions(board, indexMapping, numberOfOptions);
	
	sol = (double*) malloc(numberOfVariables * sizeof(double));
	obj = (double*) malloc(numberOfVariables * sizeof(double));
	val = (double*) malloc(N * sizeof(double));
	ind = (int*) malloc(N * sizeof(int));
	vtype = (char*) malloc(numberOfVariables * sizeof(char));

	/* Create environment - log file is mip1.log */
	error = GRBloadenv(&env, "solve.log");
	if (error)
	{
		printf("ERROR %d GRBloadenv(): %s\n", error, GRBgeterrormsg(env));
		return -1;
	}

	error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
	if (error)
	{
		printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
		return -1;
	}

	/* Create an empty model named "mip1" */
	error = GRBnewmodel(env, &model, "mip1", 0, NULL, NULL, NULL, NULL, NULL);
	if (error)
	{
		printf("ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(env));
		return -1;
	}

	/* Add variables */

	/* coefficients - for x,y,z (cells 0,1,2 in "obj") */
	obj[0] = 1;
	obj[1] = 3;
	obj[2] = 2;

	/* variable types - for x,y,z (cells 0,1,2 in "vtype") */
	/* other options: GRB_INTEGER, GRB_CONTINUOUS */
	vtype[0] = GRB_BINARY;
	vtype[1] = GRB_BINARY;
	vtype[2] = GRB_BINARY;

	/* add variables to model */
	error = GRBaddvars(model, 3, 0, NULL, NULL, NULL, obj, NULL, NULL, vtype, NULL);
	if (error)
	{
		printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
		return -1;
	}

	/* Change objective sense to maximization */
	error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
	if (error)
	{
		printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
		return -1;
	}

	/* update the model - to integrate new variables */

	error = GRBupdatemodel(model);
	if (error)
	{
		printf("ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
		return -1;
	}

	/* First constraint: x + 2 y + 3 z <= 5 */

	/* variables x,y,z (0,1,2) */
	ind[0] = 0;
	ind[1] = 1;
	ind[2] = 2;
	/* coefficients (according to variables in "ind") */
	val[0] = 1;
	val[1] = 2;
	val[2] = 3;

	/* add constraint to model - note size 3 + operator GRB_LESS_EQUAL */
	/* -- equation value (5.0) + unique constraint name */
	error = GRBaddconstr(model, 3, ind, val, GRB_LESS_EQUAL, 5, "c0");
	if (error)
	{
		printf("ERROR %d 1st GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
		return -1;
	}

	/* Second constraint: x + y >= 1 */
	ind[0] = 0;
	ind[1] = 1;
	val[0] = 1;
	val[1] = 1;

	/* add constraint to model - note size 2 + operator GRB_GREATER_EQUAL */
	/* -- equation value (1.0) + unique constraint name */
	error = GRBaddconstr(model, 2, ind, val, GRB_GREATER_EQUAL, 1.0, "c1");
	if (error)
	{
		printf("ERROR %d 2nd GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
		return -1;
	}

	/* Optimize model - need to call this before calculation */
	error = GRBoptimize(model);
	if (error)
	{
		printf("ERROR %d GRBoptimize(): %s\n", error, GRBgeterrormsg(env));
		return -1;
	}

	/* Write model to 'mip1.lp' - this is not necessary but very helpful */
	error = GRBwrite(model, "mip1.lp");
	if (error)
	{
		printf("ERROR %d GRBwrite(): %s\n", error, GRBgeterrormsg(env));
		return -1;
	}

	/* Get solution information */

	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	if (error)
	{
		printf("ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
		return -1;
	}

	/* get the objective -- the optimal result of the function */
	error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
	if (error)
	{
		printf("ERROR %d GRBgettdblattr(): %s\n", error, GRBgeterrormsg(env));
		return -1;
	}

	/* get the solution - the assignment to each variable */
	/* 3-- number of variables, the size of "sol" should match */
	error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, 3, sol);
	if (error)
	{
		printf("ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
		return -1;
	}

	/* print results */
	printf("\nOptimization complete\n");

	/* solution found */
	if (optimstatus == GRB_OPTIMAL)
	{
		printf("Optimal objective: %.4e\n", objval);
		printf("  x=%.2f, y=%.2f, z=%.2f\n", sol[0], sol[1], sol[2]);
	}
	/* no solution found */
	else if (optimstatus == GRB_INF_OR_UNBD)
	{
		printf("Model is infeasible or unbounded\n");
	}
	/* error or calculation stopped */
	else
	{
		printf("Optimization was stopped early\n");
	}

	/* IMPORTANT !!! - Free model and environment */
	GRBfreemodel(model);
	GRBfreeenv(env);

	free(vtype);
	free(ind);
	free(val);
	free(obj);
	free(sol);
	for(i=0; i<N; i++){
		destroy2D(indexMapping[i]);
	}
	free(indexMapping);
	free(numberOfOptions);

	return 0;
}