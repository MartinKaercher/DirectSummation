CC = gcc # Compiler
CF = -O3 -pedantic -Wall -Werror# compile flags
CL = -lm 

all: main

main: main.c
	$(CC) $(CF) $(CL) -c main.c
	$(CC) $(CF) $(CL) main.o -o main_exec

clean:
	rm -f main.o
