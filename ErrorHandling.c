#include <stdlib.h>
#include <stdio.h>
#include "Graph.h"

/* A method that checks the validity of the file */
void checkFile(FILE* file) {
    if(file == NULL) {
        printf("ERROR! Failed to open file.\n");
        exit(EXIT_FAILURE);
    }
}

/* A method that checks the validity of the allocation */
void checkAllocation(void* pointer) {
    if(pointer == NULL) {
        printf("ERROR! Failed to allocate memory.\n");
        exit(EXIT_FAILURE);
    }
}

/* A method that checks the validity of the read elements */
void checkRead(int read, int expected) {
    if(expected != read) {
        printf("ERROR! Failed to read: expected %d but indeed read %d elements.\n", expected, read);
        exit(EXIT_FAILURE);
    }
}

/* A method that checks the validity of the writen elements */
void checkWrite(int read, int expected) {
    if(expected != read) {
        printf("ERROR! Failed to write: expected %d but indeed wrote %d elements.\n", expected, read);
        exit(EXIT_FAILURE);
    }
}

/* A method that checks that a division is well defined: Division by 0 not allowed */
void checkDivisionByZero(double x) {
    if(x == 0) {
        printf("ERROR! Division by zero.\n");
        exit(EXIT_FAILURE);
    }
}

/* A method that checks the validity of the expected matrix(and therefore the validity of B matrix). By def: E[i,j] = k[i]*k[j] / M.
 * -> If M = 0, then E(Expected Matrix) is undefined, and this might be happen in two cases:
 * Case 1: |V| = 0(no vertices) -> M = 0(no edges) -> E is undefined
 * Case 2: |V| > 0 and M = 0(no edges) -> E is yet undefined
 * */
void checkUndefinedGraph(Graph* graph) {
    if(graph->n == 0) {
        printf("ERROR! |V| = 0 -> M = 0 -> B MATRIX IS UNDEFINED. \n");
        exit(EXIT_FAILURE);
    }

    if(graph->original_M == 0) {
        printf("ERROR! M = 0 -> B MATRIX IS UNDEFINED. \n");
        exit(EXIT_FAILURE);
    }
}

/* This method checks the number of parameters given to the program. Expected number of parameters is 2(argc = 3).
 * The expected parameters in-order are: input-file, output-file */
void checkParameters(int x) {
    if(x != 3) {
        printf("ERROR! Incompatiable parameters. Cluster program expects two parameters: Input-File, Output-File. \n");
        exit(EXIT_FAILURE);
    }
}
