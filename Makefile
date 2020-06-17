all: stresstest

stresstest: bintree.o stresstest.o
	gcc -o stresstest bintree.o stresstest.o

bintree.o: bintree.c bintree.h
	gcc -O -c bintree.c bintree.h

stresstest.o: stresstest.c
	gcc -O -c stresstest.c bintree.h

rndgen: rndgen.c
	gcc rndgen.c -o rndgen



