#include <stdlib.h>
#include "LinkedLists.h"
#include "ErrorHandling.h"
#include "spmat.h"
#include "Graph.h"

/* A method that adds a new graph node to a list. It adds to the beginning of the list. */
ListG addGraphToList(ListG list, Graph* graph) {
    ListG node = (graph_node*) malloc(sizeof(graph_node));
    checkAllocation(node);
    node->graph = graph;
    node->next = list;
    return node;
}

/* A method that frees deeply a list of graphs. */
void freeListG(ListG lst) {
    if (lst != NULL){
        freeListG(lst->next);
        freeGraph(lst->graph);
        free(lst);
    }
}

/* A method that frees a list. */
void delete_list(list head) {
    if (head != NULL){
        delete_list(head->next);
        free(head);
    }
}

/* A method that frees a list implementation of spmat*/
void freelist(struct _spmat *A) {
    int i;
    list *pointer_to_private = (list*)A->private;
    for(i = 0; i < A-> n; i++) {
        delete_list(pointer_to_private[i]);
    }
    free(pointer_to_private);
    free(A);
}
