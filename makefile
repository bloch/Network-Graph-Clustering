FLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
LIBS = -lm

all: Algorithms.o EigenPair.o ErrorHandling.o Graph.o LinkedLists.o main.o spmat.o
	gcc Algorithms.o EigenPair.o ErrorHandling.o Graph.o LinkedLists.o main.o spmat.o -o cluster $(LIBS)

clean:
	rm -f *.o cluster

Algorithms.o: Algorithms.c Algorithms.h
	gcc $(FLAGS) -c Algorithms.c

EigenPair.o: EigenPair.c EigenPair.h
	gcc $(FLAGS) -c EigenPair.c

ErrorHandling.o: ErrorHandling.c ErrorHandling.h
	gcc $(FLAGS) -c ErrorHandling.c

Graph.o: Graph.c Graph.h
	gcc $(FLAGS) -c Graph.c

LinkedLists.o: LinkedLists.c LinkedLists.h
	gcc $(FLAGS) -c LinkedLists.c

main.o: main.c
	gcc $(FLAGS) -c main.c

spmat.o: spmat.c spmat.h
	gcc $(FLAGS) -c spmat.c