all: stresstest

stresstest: algo.o stresstest.o
	gcc -o stresstest algo.o stresstest.o

algo.o: algo.c algo.h
	gcc -O -c algo.c algo.h

stresstest.o: stresstest.c
	gcc -O -c stresstest.c algo.h


rndgen: rndgen.c
	gcc rndgen.c -o rndgen



