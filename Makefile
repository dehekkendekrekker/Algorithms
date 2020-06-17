all: algotest

algotest: algo.o algotest.o
	gcc -o algotest algo.o algotest.o

algo.o: algo.c algo.h
	gcc -O -c algo.c algo.h

algotest.o: algotest.c
	gcc -O -c algotest.c algo.h


rndgen: rndgen.c
	gcc rndgen.c -o rndgen



