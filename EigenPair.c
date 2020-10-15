#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Graph.h"
#include "EigenPair.h"
#include "ErrorHandling.h"

/* Power Iteration Method - returns leading eigen vector of shifted B[g]_hat matrix */
double* PowerIteration(Graph* graph) {
    double *vector; int i; int bool=0; double *result;
    vector = graph->aux->vector;
    for(i = 0; i < graph->n; i++) {
        vector[i] = (double)rand();
    }
    result = graph->aux->result;
    while(bool == 0) {
        multShiftedBgHat(graph, vector, result);
        bool = normalizeAndCheckEpsilon(vector, result, graph->n);
        memcpy(vector,result, graph->n * sizeof(double));
    }
    return vector;
}

/* A multiplication method of the shifted B[g]_hat matrix with a vector --> to result (pre-allocated)  */
void multShiftedBgHat(Graph* graph, double* vector, double* result) {
    int i; double product = 0.0;
    checkUndefinedGraph(graph);
    graph->A->mult(graph->A, vector, result);   /* first stage of multiplication - of a sparse matrix */
    for(i = 0; i < graph->n; i++) {
        product += (double)graph->original_degrees[graph->indices[i]] * vector[i];
    }
    for(i = 0; i < graph->n; i++) {
        result[i] -= product * (double)graph->original_degrees[graph->indices[i]] / graph->original_M;
        result[i] += graph->norm * vector[i];
        result[i] -= graph->fVector[i] * vector[i];
    }
}

/* Method that returns the leading eigen-value of the B[g]_hat matrix  */
double EigenValue(Graph* graph, double* eigen_vector) {
    double norm; double nominator; double* result;
    checkUndefinedGraph(graph);
    norm = graph->norm;
    result = graph->aux->result;
    multShiftedBgHat(graph, eigen_vector, result);
    nominator = dot_product(eigen_vector, result, graph->n);
    return nominator - norm;
}

/* Returns the dot product of two vectors of size columns */
double dot_product(double *p, double *q,int columns) {
    double dot_prod = 0.0; int i;
    for(i = 0; i < columns; i++) {
        dot_prod += p[i] * q[i];
    }
    return dot_prod;
}

/* A method which normalized the next vector in the power iteration, and check if power iteration is done by accuracy of epsilon */
int normalizeAndCheckEpsilon(double* vector, double* next_vector, int rows) {
    int i; int bool = 1; double epsilon = 0.00001;
    double magnitude = sqrt(dot_product(next_vector, next_vector, rows));
    checkDivisionByZero(magnitude);
    for(i = 0; i < rows; i++) {
        next_vector[i] /=  magnitude;
        if(fabs(vector[i] - next_vector[i]) >= epsilon)
            bool = 0;
    }
    return bool;
}
