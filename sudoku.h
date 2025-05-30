#ifndef SUDOKU_H
#define SUDOKU_H

#include<stdbool.h>
#include<stdio.h>

typedef struct {
    int i;
    int j;
} pair_t;

#define FOR_EACH_CELL for(int i = 0; i < 9; i++) for(int j = 0; j < 9; j++)

int ** read_sudoku(FILE* file);

pair_t find_empty(int ** board);
bool valid(int ** board, int num, pair_t pos);
bool brute_force(int ** board);

#endif