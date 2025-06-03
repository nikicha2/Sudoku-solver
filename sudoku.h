#ifndef SUDOKU_H
#define SUDOKU_H

#include<stdbool.h>
#include<stdio.h>

typedef struct {
    int i;
    int j;
} pair_t;

enum file_format {
    UNSUPPORTED = 0,
    SS,
    SDX,
    SDM
};

#define FOR_EACH_CELL for(int i = 0; i < 9; i++) for(int j = 0; j < 9; j++)


int ** read_ss(const char* file_name);
int ** read_sdx(const char* file_name);
int count_lines(const char* file_name);
int *** read_sdm(const char* file_name, int lines);
int ** get_board(const char* file_name, enum file_format file_format);
enum file_format get_format(const char* file_name);
int ** read_sudoku(FILE* file, enum file_format file_format);

void write_ss(int ** board);
void write_sdx(int ** board);
void write_board(int ** board, enum file_format file_format);
void write_sdm(int ** boards, FILE* file);

pair_t find_empty(int ** board);
bool valid(int ** board, int num, pair_t pos);
bool brute_force(int ** board);

void free_2d(int ** arr);
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