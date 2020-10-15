#ifndef PROJECT_EIGENPAIR_H
#define PROJECT_EIGENPAIR_H

/* Power Iteration Method - returns leading eigen vector of shifted Bg_hat matrix */
double* PowerIteration(Graph*);

/* A multiplication method of the shifted B[g]_hat matrix with a vector --> to result (pre-allocated)  */
void multShiftedBgHat(Graph*, double*, double*);

/* Method that returns the leading eigen-value of the B[g]_hat matrix  */
double EigenValue(Graph*, double*);

/* Returns the dot product of two vectors */
double dot_product(double *, double *,int);

/* A method which normalized the next vector in the power iteration, and check if power iteration is done by accuracy of epsilon */
int normalizeAndCheckEpsilon(double* , double*, int);

#endif
