#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "Graph.h"
#include "Algorithms.h"
#include "ErrorHandling.h"

int main(int argc, char* argv[]) {
    clock_t start = clock(); clock_t end;
    Graph* graph;
    checkParameters(argc);
    srand(time(NULL));
    graph = BuildGraphFromFile(argv[1]);
    Algorithm3(graph,argv[2]);
    end = clock();
    printf("Our Program took: %f \n", ((double)(end-start) / CLOCKS_PER_SEC));
    return 0;
}
