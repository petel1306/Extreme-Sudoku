#include <stdlib.h>
#include <stdio.h>
#include "LP_solver.h"
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

void calculateCellsOptions(Cell **cells, int *cellsOptions, int n)
{
	int i, v;
	for (i = 0; i < n; i++)
	{
		v = cells[i]->value;
		if (v > 0)
			cellsOptions[v - 1] = 0;
	}
}

int calculateOptions(Board *board, int ***options, int **numberOfOptions)
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
		getRowCells(board, i, cells);
		calculateCellsOptions(cells, rowsOptions[i], N);
	}

	/* calculate columns */
	columnsOptions = create2D(N, 1);
	for (i = 0; i < N; i++)
	{
		getColumnCells(board, i, cells);
		calculateCellsOptions(cells, columnsOptions[i], N);
	}

	/* calculate blocks */
	blocksOptions = create2D(N, 1);
	for (i = 0; i < N; i++)
	{
		getBlockCells(board, i, cells);
		calculateCellsOptions(cells, blocksOptions[i], N);
	}

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			/* only for blank cells */
			if(!board->cells[i][j].value){
				for (v = 0; v < N; v++)
				{
					blockInd = getBlockInd(board, i, j);
					if (rowsOptions[i][v] && columnsOptions[j][v] && blocksOptions[blockInd][v])
					{
						options[i][j][v] = ++counter;
						numberOfOptions[i][j]++;
					}
				}
			}
		}
	}

	free(cells);
	destroy2D(blocksOptions, N);
	destroy2D(columnsOptions, N);
	destroy2D(rowsOptions, N);
	return counter;
}

int setTargetFunction(GRBmodel *model, GRBenv *env, int integer, double *obj, char *vtype, char type, int numberOfVariables){
	int i, error;
	double *bounds = integer ? NULL : (double*) malloc(numberOfVariables * sizeof(double));
	for(i=0; i<numberOfVariables; i++){
		obj[i] = (rand() % (2 * N)) + 1;
		vtype[i] = type;
		if(integer){
			bounds[i] = 1.0;
		}
	}
	/* add variables to model */
	error = GRBaddvars(model, numberOfVariables, 0, NULL, NULL, NULL, obj, NULL, bounds, vtype, NULL);
	if(integer){
		free(bounds);
	}
	if (error)
	{
		printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
		return GUROBI_FAILURE;
	}
	return 0;
}

int setSingalValueConstraints(GRBmodel *model, GRBenv *env, int *ind, double *val, int **numberOfOptions, int ***indexMapping, int N){
	int i, j, k, v, n, error;
	for(i=0; i<N; i++){
		for(j=0; j<N; j++){
			n = numberOfOptions[i][j];
			k = 0;
			v = 0;
			while(v<n){
				if(indexMapping[i][j][k]){
					ind[v] = indexMapping[i][j][k] - 1;
					val[v] = 1;
					v++;
				}
				k++;
			}
			/* doesnt set constraints for non-blank cells */
			if(n > 0){
				error = GRBaddconstr(model, n, ind, val, GRB_EQUAL, 1.0, NULL);
				if (error)
				{
					printf("ERROR %d at GRBaddconstr() for cell (%d, %d): %s\n", error, i, j, GRBgeterrormsg(env));
					return GUROBI_FAILURE;
				}
			}
		}
	}
	return 0;
}

int limitValueInArea(GRBmodel *model, int i_s, int i_f, int j_s, int j_f, int *ind, double *val, int ***indexMapping, int N){
	int i, j, k, n, error;
	for(k=0; k<N; k++){
		n = 0;
		for(i=i_s; i<i_f; i++){
			for(j=j_s; j<j_f; j++){
				if(indexMapping[i][j][k]){
					ind[n] = indexMapping[i][j][k] - 1;
					val[n] = 1;
					n++;
				}	
			}
		}
		if(n > 0){
				error = GRBaddconstr(model, n, ind, val, GRB_EQUAL, 1.0, NULL);
				if (error){
					return error;
				}
			}
	}
	return 0;
}

int limitValueInRows(GRBmodel *model, GRBenv *env, int *ind, double *val, int ***indexMapping, int N){
	int i, error;
	for(i=0; i<N; i++){
		error = limitValueInArea(model, i, i+1, 0, N, ind, val, indexMapping, N);
		if (error){
			printf("ERROR %d at GRBaddconstr() for row %d: %s\n", error, i, GRBgeterrormsg(env));
			return GUROBI_FAILURE;
		}
	}
	return 0;
}

int limitValueInColumns(GRBmodel *model, GRBenv *env, int *ind, double *val, int ***indexMapping, int N){
	int j, error;
	for(j=0; j<N; j++){
		error = limitValueInArea(model, 0, N, j, j+1, ind, val, indexMapping, N);
		if (error){
			printf("ERROR %d at GRBaddconstr() for column %d: %s\n", error, j, GRBgeterrormsg(env));
			return GUROBI_FAILURE;
		}
	}
	return 0;
}

int limitValueInBlocks(GRBmodel *model, GRBenv *env, int *ind, double *val, int ***indexMapping, int m, int n){
	int i, j, error;
	int N = m * n;
	for(i=0; i<n; i++){
		for(j=0; j<m; j++){
			error = limitValueInArea(model, i*m, (i+1)*m, j*n, (j+1)*n, ind, val, indexMapping, N);
			if (error){
				printf("ERROR %d at GRBaddconstr() for block %d: %s\n", error, j+i*m, GRBgeterrormsg(env));
				return GUROBI_FAILURE;
			}
		}
	}
	return 0;
}

int setAreaColissionConstraints(GRBmodel *model, GRBenv *env, int *ind, double *val, int ***indexMapping, int m, int n){
	int N = n * m;
	return (
		limitValueInColumns(model, env, ind, val, indexMapping, N)
		|| limitValueInRows(model, env, ind, val, indexMapping, N)
		||limitValueInBlocks(model, env, ind, val, indexMapping, m, n)
	);
}

void guessImp(Board *board, int*** indexMapping, int** numberOfOptions, int* ind, int N, int threshold, double* sol){
	int i, j, k, n, s, t;
	for(i=0; i<N; i++){
		for(j=0; j<N; j++){
			if(!numberOfOptions[i][j]){
				continue;
			}
			n = 0;
			s = 0;
			for(k=0; k<N; k++){
				sol[indexMapping[i][j][k] - 1];
				if(indexMapping[i][j][k] && sol[indexMapping[i][j][k] - 1] >= threshold){
					ind[n++] = k;
					s += sol[indexMapping[i][j][k] - 1];
				}
			}
			if(n==1){
				setCell(board, i, j, ind[0]+1);
			}
			else if(n>1){
				s *= rand() / RAND_MAX;
				t = 0;
				for(k=0; k<n; t++){
					t += sol[indexMapping[i][j][ind[k]] - 1];
					if(t >= s){
						break;
					}
				} 
				setCell(board, i, j, ind[k]+1);
			}
		}		
	}
}

void generateImp(Board *board, int*** indexMapping, int** numberOfOptions, int N, double* sol){
	int i, j, k;
	for(i=0; i<N; i++){
		for(j=0; j<N; j++){
			if(!numberOfOptions[i][j]){
				continue;
			}
			for(k=0; k<N; k++){
				if(indexMapping[i][j][k] && sol[indexMapping[i][j][k] - 1]){
					setCell(board, i, j, k+1);
					break;
				}
			}
		}		
	}
}

/**
 * @param board the board to solve
 * @param mode determine what to do in case of solution.
 * 0 - validate
 * 1 - guess
 * 2 - generate
 * 3 - hint
 * 4 - guess_hint
 * @param x row index
 * @param y column index
 * @param v pointer to hint	
 * @param scores pinter to score for every value in cell(x,y)
*/
int solveILP(Board *board, int mode, int x, int, int y, int *v, double threshold, int *scores)
{
	int N = board->m * board->n;
	int integer = (mode==1 || mode==4) ? 0 : 1;
	int i, j, k, n, s, t;
	int **numberOfOptions, ***indexMapping;
	int numberOfVariables;
	GRBenv *env = NULL;
	GRBmodel *model = NULL;
	int error = 0;
	double *sol, *val, *obj;
	int *ind;
	char *vtype, type;
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
		return GUROBI_FAILURE;
	}

	error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
	if (error)
	{
		printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
		return GUROBI_FAILURE;
	}

	/* Create an empty model named "mip1" */
	error = GRBnewmodel(env, &model, "solve1", 0, NULL, NULL, NULL, NULL, NULL);
	if (error)
	{
		printf("ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(env));
		return GUROBI_FAILURE;
	}

	/* Add variables */
	type = integer ? GRB_BINARY : GRB_CONTINUOUS;
	if(setTargetFunction(model, env, obj, vtype, type, numberOfVariables)){
		return GUROBI_FAILURE;
	}
	
	/* Change objective sense to maximization */
	error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
	if (error)
	{
		printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
		return GUROBI_FAILURE;
	}

	/* update the model - to integrate new variables */

	error = GRBupdatemodel(model);
	if (error)
	{
		printf("ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
		return GUROBI_FAILURE;
	}

	/* cell constraint: any cell is allowed to have a single value 
	Xij1 + Xij2 + ... + XijN = 1 */

	if(setSingalValueConstraints(model, env, ind, val, numberOfOptions, indexMapping, N)){
		return GUROBI_FAILURE;
	}

	/* area constraint: for any value k<=N only one is allowed in:
	for every row i: Xi1k + Xi2k + ... + XiNk = 1 
	for every column j: X1jk + X2jk + ... + XNjk = 1
	for every block b: SUM{Xck for any c in b} = 1 */

	if(setAreaColissionConstraints(model, env, ind, val, indexMapping, board->m, board->n)){
		return GUROBI_FAILURE;
	}

	/* Optimize model - need to call this before calculation */
	error = GRBoptimize(model);
	if (error)
	{
		printf("ERROR %d GRBoptimize(): %s\n", error, GRBgeterrormsg(env));
		return GUROBI_FAILURE;
	}

	/* Get solution information */

	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	if (error)
	{
		printf("ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
		return GUROBI_FAILURE;
	}

	/* get the objective -- the optimal result of the function */
	error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
	if (error)
	{
		return NON_SOLUTION;
	}

	/* get the solution - the assignment to each variable */
	/* 3-- number of variables, the size of "sol" should match */
	error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, numberOfVariables, sol);
	if (error)
	{
		printf("ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
		return GUROBI_FAILURE;
	}

	/* print results */
	printf("\nOptimization complete\n");

	error = 0;

	/* solution found */
	if (optimstatus == GRB_OPTIMAL)
	{
		switch (mode)
		{
		case 1: /* guess */
			guessImp(board, indexMapping, numberOfOptions, ind, N, threshold, sol);
			break;
		
		case 2:
			generateImp(board, indexMapping, numberOfOptions, N, sol);
			break;

		default:
			break;
		}
		
	}
	/* no solution found */
	else if (optimstatus == GRB_INF_OR_UNBD)
	{
		error = NON_SOLUTION;
	}
	/* error or calculation stopped */
	else
	{
		error = OPT_STOP_EARLY;
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
		destroy2D(indexMapping[i], N);
	}
	free(indexMapping);
	free(numberOfOptions);

	return error;
}