#include <stdlib.h>
#include "spmat.h"
#include "LinkedLists.h"
#include "ErrorHandling.h"

/* This method is used as spmat builder - linked list implementation */
void buildlist(struct _spmat *A, const double *row, int i) {
    int j; int firstnz = -1;
    list *pointer_to_private = (list*)A->private;
    list  head = NULL,tail = NULL;
    for (j= 0 ; j < A->n ; j++){
        if (row[j]!=0){
            firstnz = j;
            break;
        }
    }
    if (firstnz > -1){
        head = (node*) malloc(sizeof(node));
        checkAllocation(head);
        head->data = row[firstnz];
        head->col = firstnz;
        head->next = NULL;
        tail = head;
        for (j= firstnz+1 ; j < A->n ; j++){
            if (row[j] != 0){
                tail->next = (node*) malloc(sizeof(node));
                checkAllocation(tail->next);

                tail = tail->next;
                tail->data = row[j];
                tail->col = j;
                tail->next = NULL;
            }
        }
        tail->next = NULL;
    }
    pointer_to_private[i] = head;  /* it was A->private[i] = head; */
}

/* A method that multiplies a sparse matrix with a vector - linked list implementation */
void listmultvec(const struct _spmat *A, const double *v, double *result) {
    int i;
    list *pointer_to_private = (list*)A->private;
    for (i=0; i < A->n; i++){
        double sum = 0.0;
        if (pointer_to_private[i]!=NULL){
            node* node = pointer_to_private[i];
            sum += node->data * v[node->col];
            while (node->next!=NULL){
                node = node->next;
                sum += node->data * v[node->col];
            }
            result[i] = sum;
        }
        else{
            result[i] = sum; /* row is all zero */
        }
    }
}

/* Allocates a new linked-lists sparse matrix of size n */
spmat* spmat_allocate_list(int n) {
    list* a; spmat* res;
    res = (spmat*) malloc(sizeof(spmat));
    checkAllocation(res);
    a = (list*) malloc(n*sizeof(list));
    checkAllocation(a);
    res->n = n;
    res->private = a;
    res->add_row = buildlist;
    res->mult = listmultvec;
    res->free = freelist;
    return res;
}
