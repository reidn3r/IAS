# Makefile do IAS

all: summation fibonacci

summation: ias.out
	./ias.out -p summation.ias -i 8

fibonacci: ias.out
	./ias.out -p fibonacci.ias -i 16

fatorial: ias.out
	./ias.out -p fatorial.ias -i 6

ias.out:
	gcc main.c utils.c utils.h -o ias.out
