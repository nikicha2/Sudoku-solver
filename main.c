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
    enum file_format file_format = get_format(argv[1]);
    switch (file_format) {
        case SS: case SDX:
            int ** board = get_board(argv[1], file_format);
            int ** solved = solve(board);
            if(solved) {
                write_board(solved, file_format);
            }
            free_2d(board);
            free_2d(solved);
            break;
        case SDM:
            int lines = count_lines(argv[1]);
            int *** boards = read_sdm(argv[1], lines);
            FILE *file = fopen("solution.sdm", "w");
            if (!file) {
                printf("Error opening file\n");
                exit(1);
            }
            for(int i = 0; i < lines; i++) {
                int ** solved = solve(boards[i]);
                if(solved) {
                    write_sdm(solved, file);
                    free_2d(solved);
                }
            }
            for(int i = 0; i < lines; i++) free_2d(boards[i]);
            free(boards);
            fclose(file);
            break;

        default:
            printf("file format not supported");
            exit(1);
    }
    return 0;
}