#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#include"sudoku.h"
void printBoard(int ** board) {
    FOR_EACH_CELL {
        printf("%d %s", board[i][j], (j == 8 ? "\n" : ""));
    }
    puts("");
}
int main(int argc, char *argv[]) {
    printf("%d\n", argc);
    for(int i=0;i<argc;i++) {
        printf("%s\n", argv[i]);
    }
    FILE* f = fopen(argv[1], "r");
    if(!f) {
        printf("Error opening file");
        exit(1);
    }
    int ** board = read_sudoku(f);
    printBoard(board);
    if(brute_force(board)) {
        printBoard(board);
    }
    fclose(f);
    return 0;
}