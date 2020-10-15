#ifndef PROJECT_ALGORITHMS_H
#define PROJECT_ALGORITHMS_H

#include "LinkedLists.h"
#include "Graph.h"

/* Algorithm 2 of the project - dividing the graph into two groups as described in the algorithm. */
void Algorithm2(Graph*);

/* Algorithm 3 of the project - iteratively dividing the graphs into groups by Algorithm 2 including modularity maximization
 * Afterwards, outputs the groups to an output file. */
void Algorithm3(Graph*, char*);

/* Modularity maximization method - receives a division and returns an optimized division */
Division* ModularityMax(Graph*);

/* Algorithm 4 of the project - optimization of a division */
void Algorithm4(Graph*, int*);

/* An auxiliary method for computing score[k] for Algorithm 4
   This method was mathematically designed to replace lines 5-10 of the pseudo-code(matrix multiplications) -> complexity improvement */
double computeScoreK(Graph*,int*, int, double);

/*     A method that computes s*Bg_hat*s    */
double multBgHat(Graph*, double*);

/* An auxiliary method for computing score[k] */
double computeProduct(Graph* graph, int* s);

/* An auxiliary method for computing score[k] */
void computeAg(Graph*, int*);

/* An auxiliary method for computing score[k] */
void updateAg(Graph*, int*, int);

/* A method writing the groups in O to the output file given by filename */
void GroupsToOutputFile(char*, ListG);

#endif
