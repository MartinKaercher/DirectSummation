CC = gcc # Compiler
CF = -lm -pedantic -Wall -Werror# compile flags

all: main

main: main.c
	$(CC) $(CF) -c main.c
	$(CC) $(CF) main.o -o main_exec

clean:
	rm -f main.o
