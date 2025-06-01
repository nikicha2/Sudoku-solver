#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"sudoku.h"
void printBoard(int ** board) {
    FOR_EACH_CELL {
        printf("%d %s", board[i][j], (j == 8 ? "\n" : ""));
    }
    puts("");
}
int main(int argc, char *argv[]) {
    FILE* f = fopen(argv[1], "r");
    if (!f) {
        printf("Error opening file\n");
        exit(1);
    }
    int ** board = read_sudoku(f);
    printBoard(board);
    int ** solved = solve(board);
    if(solved) {
        printBoard(solved);
    }
    fclose(f);
    return 0;
}