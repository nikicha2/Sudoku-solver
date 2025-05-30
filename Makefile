all: main

main: main.c sudoku.o
	gcc main.c sudoku.o -o main

sudoku.o: sudoku.c sudoku.h
	gcc -c sudoku.c -o sudoku.o