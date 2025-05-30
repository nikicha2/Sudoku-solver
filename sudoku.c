#include "sudoku.h"
#include<stdio.h>
#include<stdlib.h>


int ** read_sudoku(FILE* file) {
    int ** board = (int**)calloc(9, sizeof(int*));
    for (int i = 0; i < 9; i++) {
        board[i] = (int*)calloc(9, sizeof(int));
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'I') {
            // Parse line like "I426"
            int row = line[1] - '0';
            int col = line[2] - '0';
            int val = line[3] - '0';
            board[row][col] = val;
        }
    }

    return board;
}


pair_t find_empty(int ** board) {
    pair_t p = {-1,-1};
    FOR_EACH_CELL {
        if(board[i][j] == 0) {
            p.i = i;
            p.j = j;
        }
    }
    return p;
}

bool valid(int ** board, int num, pair_t pos) {
    for(int j=0;j<9;j++) {
        if(board[pos.i][j] == num && pos.j != j) {
            return false;
        }
    }
    for(int i=0;i<9;i++) {
        if(board[i][pos.j] == num && pos.i != i) {
            return false;
        }
    }

    int box_x = pos.j / 3;
    int box_y = pos.i / 3;
    for(int i = box_y * 3; i < box_y * 3 + 3; i++) {
        for(int j = box_x * 3; j < box_x * 3 + 3; j++) {
            if(board[i][j] == num && (i != pos.i && j != pos.j)) {
                return false;
            }
        }
    }
    return true;
}

bool brute_force(int ** board) {
    pair_t empty = find_empty(board);
    if(empty.i == -1) {
        return true;
    }
    int row,col;
    row = empty.i;
    col = empty.j;

    for(int i=1; i < 10; i++) {
        if(valid(board,i,(pair_t){row,col})) {
            board[row][col] = i;
            if(brute_force(board)) {
                return true;
            }
            board[row][col] = 0;
        }
    }
    return false;
}