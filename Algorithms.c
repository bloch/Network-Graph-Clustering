#include <stdlib.h>
#include <string.h>
#include "Graph.h"
#include "Algorithms.h"
#include "ErrorHandling.h"
#include "LinkedLists.h"
#include "EigenPair.h"

/* Algorithm 2 of the project - dividing the graph into two groups as described in the algorithm. */
void Algorithm2(Graph* graph) {
    int i; double result; int* s; double* s_double; double lambda;
    s = graph->aux->s; s_double = graph->aux->s_double;

    /* Stage 1 of Algorithm 2 - computing leading eigen pair of Bg_Hat */
    graph->aux->vector = PowerIteration(graph);
    lambda = EigenValue(graph, graph->aux->vector);

    if(IS_POSITIVE(lambda)) {
        for(i = 0; i < graph->n; i++) {
            if(IS_POSITIVE(graph->aux->vector[i])) {
                s[i] = 1;
                s_double[i] = 1;
            }
            else {
                s[i] = -1;
                s_double[i] = -1;
            }
        }
        result = multBgHat(graph, s_double);
        if(IS_POSITIVE(result)) {
            return;
        }
        else {
            /*	INDIVISIBLE 4.1  */
        }
    }
    else {
        /*	INDIVISIBLE 2.1  */
    }
    /* IF WE ARE HERE -> GRAPH IS INDIVISIBLE */
    memset(s, 1, sizeof(int) * graph->n);
}

/* Algorithm 3 of the project - iteratively dividing the graphs into groups by Algorithm 2 including modularity maximization
 * Afterwards, outputs the groups to an output file. */
void Algorithm3(Graph* graph, char* filename) {
    Graph* cur_graph; Division* division; ListG P; ListG O; ListG tmp;
    P = NULL; O = NULL;
    P = addGraphToList(P, graph);
    checkUndefinedGraph(graph);
    while(P != NULL) {
        /* Removing a graph from P and dividing the graph */
        cur_graph = P->graph;
        tmp = P->next; free(P);
        P=tmp;
        Algorithm2(cur_graph);
        division = ModularityMax(cur_graph);
        if(division->g1->n == 0 || division->g2->n == 0) { /* STAGE 3.3 OF ALGORITHM 3 */
            freeGraph(division->g1);
            freeGraph(division->g2);
            O = addGraphToList(O, cur_graph);
        }
        else { /* STAGE 3.4 OF ALGORITHM 3 */
            freeGraph(cur_graph);
            if(division->g1->n == 1) {
                /* HERE WE ADD g1 to O -> g1 of size 1 */
                O = addGraphToList(O, division->g1);
            }
            else {
                /* HERE WE ADD g1 to P */
                P = addGraphToList(P, division->g1);
            }

            if(division->g2->n == 1) {
                /* HERE WE ADD g2 to O -> g2 of size 1 */
                O = addGraphToList(O, division->g2);
            }
            else {
                /* HERE WE ADD g2 to P */
                P = addGraphToList(P, division->g2);
            }
        }
        free(division);
    }
    GroupsToOutputFile(filename, O);   /* Groups --> Output file */
    if(O != NULL && O->graph != NULL) {
        free(O->graph->original_degrees);
        freeAux(O->graph->aux);
    }
    freeListG(O);
}

/* Modularity maximization method - receives a division and returns an optimized division */
Division* ModularityMax(Graph* cur_graph) {
    int i; int*s; Division* opt;
    s = cur_graph->aux->s;
    Algorithm4(cur_graph, s);
    opt = (Division*) malloc(sizeof(Division));
    checkAllocation(opt);
    opt->g1 = BuildGraphFromGroup(cur_graph, s);
    for(i = 0; i < cur_graph->n; i++) {
        s[i] *= -1;
    }
    opt->g2 = BuildGraphFromGroup(cur_graph, s);
    return opt;
}

/* Algorithm 4 of the project - optimization of a division */
void Algorithm4(Graph* graph, int* s) {
    int* unmoved; int k; int i; int jtag; int itag; int flag; double product;
    int j; double deltaQ; double* score; int* indices; double* improve;
    unmoved = graph->aux->unmoved;
    score = graph->aux->score; indices = graph->aux->indices;
    improve = graph->aux->improve;
    product = computeProduct(graph, s);
    computeAg(graph, s);
    do {
        memset(unmoved, 0, graph->n * sizeof(int));
        for(i = 0; i < graph->n; i++) {
            flag = 0;
            for(k = 0; k < graph->n; k++) {
                if(unmoved[k] == 0) {
                    score[k] = computeScoreK(graph,s,k, product);
                    if(flag == 0) {
                        jtag = k;
                        flag = 1;
                    }
                    else {
                        if(score[k] > score[jtag])
                            jtag = k;
                    }
                }
            }
            s[jtag] *= -1;
            product += 2.0 * graph->original_degrees[graph->indices[jtag]] * s[jtag];
            updateAg(graph, s, jtag);
            indices[i] = jtag;
            if(i == 0) {
                improve[i] = score[jtag];
                itag = 0;
            }
            else {
                improve[i] = improve[i - 1] + score[jtag];
                if(improve[i] > improve[itag])
                    itag = i;
            }
            unmoved[jtag] = 1;
        }
        for(i = graph->n - 1; i >= itag + 1; i--) {
            j = indices[i];
            s[j] *= -1;
            product += 2.0 * graph->original_degrees[graph->indices[j]] * s[j];
            updateAg(graph, s, j);
        }
        if(itag == graph->n - 1)
            deltaQ = 0;
        else
            deltaQ = improve[itag];
    }while(IS_POSITIVE(deltaQ));
    /* RESULT OF OPTIMIZATION IS IN VECTOR S */
}

/* An auxiliary method of modularity maximization - scores array calculations */
double computeScoreK(Graph* graph, int* s, int k,double product) {
    double final; double a = graph->aux->s_double[k];
    int deg_k = graph->original_degrees[graph->indices[k]];
    checkUndefinedGraph(graph);
    product += -2.0 * s[k] * deg_k;
    product *= 1.0 * deg_k / (double)(graph->original_M);
    final = a - product;
    final *= -4.0 * s[k];
    final += 4.0 * deg_k * deg_k / (double)(graph->original_M);
    return final;
}

/* An auxiliary method for computing score[k] */
double computeProduct(Graph* graph, int* s) {
    int i; double prod = 0.0;
    for(i = 0; i < graph->n; i++) {
        prod += (double)graph->original_degrees[graph->indices[i]] * s[i];
    }
    return prod;
}

/* An auxiliary method for computing score[k] */
void computeAg(Graph* graph, int* s) {
    int i;int j;  list cur_Row; list* arr_rows;
    double* Ag = graph->aux->s_double;
    arr_rows = graph->A->private;
    for(i = 0; i < graph->n; i++) {
        Ag[i] = 0.0;
        cur_Row = arr_rows[i];
        for(j = 0; j < graph->n; j++) {
            if(cur_Row != NULL && j == cur_Row->col) {
                Ag[i] += (double)s[j];
                cur_Row = cur_Row->next;
            }
        }
    }
}

/* An auxiliary method for computing score[k] */
void updateAg(Graph* graph, int* s, int jtag) {
    list cur_Row; list* arr_rows; int i;
    double* Ag = graph->aux->s_double;
    arr_rows = graph->A->private;
    cur_Row = arr_rows[jtag];
    for(i = 0; i < graph->n; i++) {
        if(cur_Row != NULL && i == cur_Row->col) {
            Ag[i] += 2.0 * s[jtag];
            cur_Row = cur_Row->next;
        }
    }
}

/*     A method that computes s*Bg_hat*s    */
double multBgHat(Graph* graph, double* s) {
    double* result; int i; double product = 0.0; double final = 0.0;
    checkUndefinedGraph(graph);
    result = graph->aux->result;
    graph->A->mult(graph->A, s, result);   /* first stage of multiplication - of a sparse matrix */
    for(i = 0; i < graph->n; i++) {
        product += (double)graph->original_degrees[graph->indices[i]] * s[i];
    }
    for(i = 0; i < graph->n; i++) {
        result[i] -= product * (double)graph->original_degrees[graph->indices[i]] / graph->original_M;
        result[i] -= graph->fVector[i] * s[i];
        final += s[i] * result[i];
    }
    return final;
}

/* A method writing the groups in O to the output file given by filename */
void GroupsToOutputFile(char* filename, ListG O) {
    int j; ListG pointer; int assertions; int num_of_groups = 0;
    FILE* file = fopen(filename, "w");
    checkFile(file);
    pointer = O;
    while(pointer != NULL) {
        num_of_groups++;
        pointer = pointer->next;
    }
    assertions = fwrite(&num_of_groups, sizeof(int), 1, file);
    checkWrite(assertions, 1);
    while(O != NULL) {
        assertions = fwrite(&(O->graph->n), sizeof(int), 1, file);
        checkWrite(assertions, 1);
        for(j = 0; j < O->graph->n; j++) {
            assertions = fwrite(&(O->graph->indices[j]),sizeof(int), 1, file);
            checkWrite(assertions, 1);
        }
        O = O->next;
    }
    fclose(file);
}
