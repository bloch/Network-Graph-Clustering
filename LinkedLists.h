#ifndef PROJECT_LINKEDLISTS_H
#define PROJECT_LINKEDLISTS_H

#include "spmat.h"
#include "Graph.h"

/* linked_list struct is a regular linked list object.
 * graph_list struct is a graph linked list object. */

struct linked_list
{
    int col;
    double data;
    struct linked_list *next;
};

typedef struct linked_list node;
typedef node* list;

struct graph_list
{
    Graph* graph;
    struct graph_list *next;
};

typedef struct graph_list graph_node;
typedef graph_node* ListG;

/* A method that adds a new graph node to a list. It adds to the beginning of the list. */
void freeListG(ListG);

/* A method that frees deeply a list of graphs. */
ListG addGraphToList(ListG, Graph*);

/* A method that frees a list. */
void delete_list(list);

/* A method that frees a list implementation of spmat*/
void freelist(struct _spmat *);


#endif
