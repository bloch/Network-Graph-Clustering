#ifndef PROJECT_ERRORHANDLING_H
#define PROJECT_ERRORHANDLING_H

#include <stdio.h>
#include "Graph.h"

/* According to the project's instructions, we define epsilon = 0.00001 as the accuracy.
 * If x > epsilon, we say it's positive. Othersise, non-positive. */
#define IS_POSITIVE(X) ((X) > 0.00001)

/* A method that checks the validity of the file */
void checkFile(FILE*);

/* A method that checks the validity of the allocation */
void checkAllocation(void*);

/* A method that checks the validity of the read elements */
void checkRead(int, int);

/* A method that checks the validity of the writen elements */
void checkWrite(int, int);

/* A method that checks that a division is well defined: Division by 0 not allowed */
void checkDivisionByZero(double);

/* A method that checks the validity of the expected matrix(and therefore the validity of B matrix). By def: E[i,j] = k[i]*k[j] / M.
 * -> If M = 0, then E(Expected Matrix) is undefined, and this might be happen in two cases:
 * Case 1: |V| = 0(no vertices) -> M = 0(no edges) -> E is undefined
 * Case 2: |V| > 0 and M = 0(no edges) -> E is yet undefined
 * */
void checkUndefinedGraph(Graph*);

/* This method checks the number of parameters given to the program. Expected number of parameters is 2(argc = 3).
 * The expected parameters in-order are: input-file, output-file */
void checkParameters(int);

#endif
