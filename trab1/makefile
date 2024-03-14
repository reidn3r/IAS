# Makefile do IAS

all: summation fibonacci fatorial instructions

summation: ias.out
	gcc main.c utils.c utils.h -o ias.out
	./ias.out -p summation.ias -i 8

fibonacci: ias.out
	gcc main.c utils.c utils.h -o ias.out
	./ias.out -p fibonacci.ias -i 16

fatorial: ias.out
	gcc main.c utils.c utils.h -o ias.out
	./ias.out -p fatorial.ias -i 6

instructions: ias.out
	gcc main.c utils.c utils.h -o ias.out
	./ias.out -p instructions.ias -i 10

ias.out:
	gcc main.c utils.c utils.h -o ias.out
