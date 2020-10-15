#ifndef PROJECT_GRAPH_H
#define PROJECT_GRAPH_H

#include "spmat.h"

/* Main Structs of the Program -
 * The Graph struct is the main struct used in the program.
 * It keeps data such as |V|, M, vertices, adj matrix (A), and etc.
 * The Division struct is used as a tuple after dividing a graph in to two sub-graphs according to Algorithm 2.
 * The Aux struct is used to hold auxiliary data for use of the graph.
 */


typedef struct {
    double* vector;
    double* result;
    double* row;
    int* s;
    double* s_double;
    int* unmoved;
    double* score;
    int* indices;
    double* improve;
}Aux;

typedef struct {
    int n;
    int M;
    int* indices;
    spmat* A;
    int* original_degrees;
    int original_M;
    double norm;
    double* fVector;
    Aux* aux;
}Graph;

typedef struct {
    Graph* g1;
    Graph* g2;
}Division;


/* Build a Graph struct from an input file.  */
Graph* BuildGraphFromFile(char*);

/* Build a Graph struct from an exisiting graph and a vector s representing a sub-group of vertices.  */
Graph* BuildGraphFromGroup(Graph*, int*);

/* An auxiliary method that allocates all necessary data of the graph */
void InitAux(Graph*);

/* An auxiliary method that calculates the f vector as defined in the project */
double* f_Vector(Graph*);

/* An auxiliary method that calculates the norm1 of B[g]_hat as defined in the project */
double norm1(Graph*);

/*  A method that frees the Graph struct */
void freeGraph(Graph*);

/*  A method that frees all auxiliary data of the program */
void freeAux(Aux*);

#endif
