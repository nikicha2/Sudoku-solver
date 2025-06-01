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

void free_3d(int *** arr);

int *** initialize(int ** board);
int is_singleton(int* row);
bool is_empty(int* row);
int row_size(int* row);
void set_singleton(int* row, int digit);
void propagate(int *** possibilities);
bool eliminate_from_peers(int*** possibilities, int r, int c, int digit, bool* changed);
int *** make_copy(int *** possibilities);

int *** search(int *** possibilities);
int ** exctract_solution(int *** possibilities);
int ** solve(int ** board);

#endif