#include <stdlib.h>
#include <math.h>
#include "Graph.h"
#include "ErrorHandling.h"
#include "LinkedLists.h"
#include "spmat.h"

/* Build a Graph struct from an input file.  */
Graph* BuildGraphFromFile(char* filename) { 					/*receives argv[..] */
    int assertions; int num_of_vertex; int i; int deg; int j; double *row; FILE* file; int tmp;
    Graph* g = (Graph*) malloc(sizeof(Graph));
    checkAllocation(g);
    file = fopen(filename,"r");
    checkFile(file);
    assertions = fread(&num_of_vertex,sizeof(int),1,file);
    checkRead(assertions, 1);
    g->n = num_of_vertex;
    g->M = 0;
    g->A = spmat_allocate_list(num_of_vertex);
    g->original_degrees = (int*) malloc(g->n * sizeof(int));
    checkAllocation(g->original_degrees);
    g->indices = (int*) malloc(g->n * sizeof(int));
    checkAllocation(g->indices);
    row = (double*)malloc(g->n * sizeof(double));
    checkAllocation(row);
    for(i = 0; i < g->n; i++) {
        assertions = fread(&deg,sizeof(int),1,file);
        checkRead(assertions, 1);
        g->original_degrees[i] = deg;
        g->M += deg;
        g->indices[i] = i;							                    	/* INDEXES ARE 0,....,n-1 */
        for(j = 0; j < g->n; j++) {
            row[j] = 0;
        }
        for(j = 0; j < deg; j++) {
            assertions = fread(&tmp,sizeof(int),1,file);
            checkRead(assertions, 1);
            row[tmp] = 1;
        }
        g->A->add_row(g->A,row,i);
    }
    fclose(file);
    free(row);
    g->original_M = g->M;
    checkUndefinedGraph(g);
    InitAux(g);
    g->fVector = f_Vector(g);
    g->norm = norm1(g);
    return g;
}

/* Build a Graph struct from an existing graph and a vector s representing a sub-group of vertices.  */
Graph* BuildGraphFromGroup(Graph* graph, int* s) {
    int j; int ng = 0; int i; Graph* subGraph; int cnt = 0; list cur_Row; int deg; double* row; int* mipui; list* arr_rows;
    for(i = 0; i < graph->n; i++) {
        if(s[i] == 1)
            ng++;
    }
    arr_rows = graph->A->private;
    subGraph = (Graph*) malloc(sizeof(Graph));
    checkAllocation(subGraph); subGraph->n = ng;
    subGraph->original_degrees = graph->original_degrees;
    subGraph->original_M = graph->original_M;
    subGraph->A = spmat_allocate_list(ng);
    subGraph->indices = (int*) malloc(ng * sizeof(int));
    checkAllocation(subGraph->indices);
    row = (double*) malloc(ng * sizeof(double));
    checkAllocation(row);
    mipui = (int*) malloc(graph->n * sizeof(int));
    checkAllocation(mipui);
    for(i = 0; i < graph->n; i++) {
        if(s[i] == 1) {
            mipui[i] = cnt;
            cnt++;
        }
        else
            mipui[i] = -1;
    }
    cnt = 0;
    for(i  = 0; i < graph->n; i++) {
        if(s[i] == 1) {
            for(j = 0; j < ng; j++)
                row[j] = 0;
            subGraph->indices[cnt] = graph->indices[i];
            deg = 0; cur_Row = arr_rows[i];
            while(cur_Row != NULL) {
                int col = cur_Row->col;
                if(s[col] == 1) {
                    deg++;
                    row[mipui[col]] = 1;
                }
                cur_Row = cur_Row->next;
            }
            subGraph->M +=deg;
            subGraph->A->add_row(subGraph->A, row, cnt);
            cnt++;
        }
    }
    free(row); free(mipui);
    if(subGraph->n > 0) {
        subGraph->aux = graph->aux;
        subGraph->fVector = f_Vector(subGraph);
        subGraph->norm = norm1(subGraph);
    }
    return subGraph;
}


/* An auxiliary method that allocates all necessary data of the graph */
void InitAux(Graph* g) {
    g->aux = (Aux*) malloc(sizeof(Aux));
    checkAllocation(g->aux);

    g->aux->vector = (double*) malloc(g->n * sizeof(double));
    checkAllocation(g->aux->vector);

    g->aux->result = (double*) malloc(g->n * sizeof(double));
    checkAllocation(g->aux->result);

    g->aux->row = (double*) malloc(g->n * sizeof(double));
    checkAllocation(g->aux->row);

    g->aux->s = (int*) malloc(g->n * sizeof(int));
    checkAllocation(g->aux->s);

    g->aux->s_double = (double*) malloc(g->n * sizeof(double));
    checkAllocation(g->aux->s_double);

    g->aux->unmoved = (int*) malloc(g->n * sizeof(int));
    checkAllocation(g->aux->unmoved);

    g->aux->score = (double*) malloc(g->n * sizeof(double));
    checkAllocation(g->aux->score);

    g->aux->indices = (int*) malloc(g->n * sizeof(int));
    checkAllocation(g->aux->indices);

    g->aux->improve = (double*) malloc(g->n * sizeof(double));
    checkAllocation(g->aux->improve);
}

/* An auxiliary method that calculates the f vector as defined in the project */
double* f_Vector(Graph* graph) {
    int i; list cur_Row; list* arr_rows; int j; double* f;
    double exp; double exp_i; double exp_j;
    checkUndefinedGraph(graph);
    f = (double*) malloc(graph->n * sizeof(double));
    checkAllocation(f);
    for(i = 0; i < graph->n; i++) {
        f[i] = 0;
    }
    arr_rows = graph->A->private;
    for(i = 0; i < graph->n; i++) {
        cur_Row = arr_rows[i];
        exp_i = graph->original_degrees[graph->indices[i]];
        for(j = 0; j < graph->n; j++) {
            exp_j = graph->original_degrees[graph->indices[j]];
            exp = exp_i * exp_j;
            exp /= graph->original_M;
            if(cur_Row != NULL && j == cur_Row->col) {
                f[j] += 1 - exp;
                cur_Row = cur_Row->next;
            }
            else {
                f[j] += 0 - exp;
            }
        }
    }
    return f;
}

/* An auxiliary method that calculates the norm1 of B[g]_hat as defined in the project */
double norm1(Graph* graph) {
    int i; list cur_Row; list* arr_rows; double max; int j; double* f;
    double* abs; double exp; double exp_i; double exp_j;
    checkUndefinedGraph(graph);
    abs = graph->aux->row;
    for(i = 0; i < graph->n; i++) {
        abs[i] = 0;
    }
    f = graph->fVector;
    arr_rows = graph->A->private;
    for(i = 0; i < graph->n; i++) {
        cur_Row = arr_rows[i];
        for(j = 0; j < graph->n; j++) {
            exp_i = graph->original_degrees[graph->indices[i]];
            exp_j = graph->original_degrees[graph->indices[j]];
            exp = exp_i * exp_j;
            exp /= graph->original_M;
            if(cur_Row != NULL && j == cur_Row->col) {
                if(i == j)
                    abs[j] += fabs(1- exp - f[i]);
                else
                    abs[j] += fabs(1 - exp);
                cur_Row = cur_Row->next;
            }
            else {
                if(i == j)
                    abs[j] += fabs(0 - exp - f[i]);
                else
                    abs[j] += fabs(0 - exp);
            }
        }
    }
    max = abs[0];
    for(i = 1; i < graph->n; i++) {
        if(abs[i] > max)
            max = abs[i];
    }
    return max;
}

/*  A method that frees the Graph struct */
void freeGraph(Graph* graph) {
    free(graph->indices);
    graph->A->free(graph->A);
    if(graph->n > 0) {
        free(graph->fVector);
    }
    free(graph);
}

/*  A method that frees all auxiliary data of the program */
void freeAux(Aux* aux) {
    free(aux->vector);
    free(aux->result);
    free(aux->row);
    free(aux->s);
    free(aux->s_double);
    free(aux->unmoved);
    free(aux->score);
    free(aux->indices);
    free(aux->improve);
    free(aux);
}
