#include "sudoku.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

int ** read_ss(const char* file_name) {
    FILE* file = fopen(file_name, "r");
    if(!file) {
        printf("Error opening file\n");
        exit(1);
    }
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
    fclose(file);
    return board;
}

int ** read_sdx(const char* file_name) {
    FILE* file = fopen(file_name, "r");
    if(!file) {
        printf("Error opening file\n");
        exit(1);
    }
    int ** board = (int**)calloc(9, sizeof(int*));
    for (int i = 0; i < 9; i++) {
        board[i] = (int*)calloc(9, sizeof(int));
    }

    char line[100];
    int row = 0, col = 0;
    while (fgets(line, sizeof(line), file) && row < 9) {
        int col = 0;
        char* tok = strtok(line, " \n");

        while (tok && col < 9) {
            int val = 0;
            if (strlen(tok) == 1 && isdigit(tok[0])) {
                val = tok[0] - '0';
            } else {
                val = 0;
            }
            board[row][col] = val;
            tok = strtok(NULL, " \n");
            col++;
        }

        row++;
    }
    fclose(file);
    return board;
}

int count_lines(const char* file_name) {
    FILE* file = fopen(file_name, "r");
    if (!file) {
        printf("Error opening file\n");
        return -1;
    }

    int count = 0;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), file)) {
        count++;
    }

    fclose(file);
    return count;
}

int *** read_sdm(const char* file_name, int lines) {
    FILE* file = fopen(file_name, "r");
    if(!file) {
        printf("Error opening file\n");
        exit(1);
    }
    int *** boards = (int***)calloc(lines, sizeof(int**));
    int boardi = 0;
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        boards[boardi] = (int**)calloc(9, sizeof(int*));
        for(int i = 0; i < 81; i++) {
            if(i%9 == 0) {
                boards[boardi][i/9] = (int*)calloc(9,sizeof(int));
            }
            boards[boardi][i/9][i%9] = line[i] - '0';
        }
        boardi++;
    }
    fclose(file);
    return boards;
}

int ** get_board(const char* file_name, enum file_format file_format) {
    switch(file_format) {
        case SS:
            return read_ss(file_name);
        case SDX:
            return read_sdx(file_name);
    }
    return NULL;
}

enum file_format get_format(const char* file_name) {
    const char* dot = strrchr(file_name, '.');
    if (!dot || dot == file_name) return UNSUPPORTED;

    if (strcmp(dot, ".ss") == 0) return SS;
    if (strcmp(dot, ".sdx") == 0) return SDX;
    if (strcmp(dot, ".sdm") == 0) return SDM;

    return UNSUPPORTED;
}

void write_ss(int **board) {
    FILE *file = fopen("solution.ss", "w");
    if (!file) {
        printf("Error opening file\n");
        exit(1);
    }

    fprintf(file, " *-----------*\n");
    for (int i = 0; i < 9; i++) {
        fprintf(file, " |");
        for (int j = 0; j < 9; j++) {
            int val = board[i][j];
            fprintf(file, "%c", ('0' + val));

            if (j % 3 == 2)
                fprintf(file, "|");
        }
        fprintf(file, "\n");
        if (i % 3 == 2 && i != 8)
            fprintf(file, " |---+---+---|\n");
    }
    fprintf(file, " *-----------*\n");

    FOR_EACH_CELL {
        int val = board[i][j];
        if (val >= 1 && val <= 9) {
            fprintf(file, "I%d%d%d\n", i, j, val);
        }
    }

    fclose(file);
}

void write_sdx(int ** board) {
    FILE *file = fopen("solution.sdx", "w");
    if (!file) {
        printf("Error opening file\n");
        exit(1);
    }

    FOR_EACH_CELL {
        fprintf(file, "%d%c", board[i][j], j == 8 ? '\n' : ' ');
    }

    fclose(file);
}
void write_board(int ** board, enum file_format file_format) {
    switch(file_format) {
        case SS:
            write_ss(board);
            break;
        case SDX:
            write_sdx(board);
            break;
    }
}
void write_sdm(int ** board, FILE* file) {
    FOR_EACH_CELL {
        fputc((board[i][j] + '0'), file);
    }
    fputc('\n', file);
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

void free_2d(int ** arr) {
    for(int i = 0; i < 9; i++) {
        free(arr[i]);
    }
    free(arr);
}

void free_3d(int *** arr) {
    FOR_EACH_CELL {
        free(arr[i][j]);
    }
    for(int i = 0; i < 9; i++) {
        free(arr[i]);
    }
    free(arr);
}

int *** initialize(int ** board) {
    int *** possibilities = (int***)calloc(9, sizeof(int**));
    for(int i=0;i<9;i++) {
        possibilities[i] = (int**)calloc(9, sizeof(int*));
        for(int j=0;j<9;j++) {
            possibilities[i][j] = (int*)calloc(9, sizeof(int));
            if(board[i][j] == 0) {
                memcpy(possibilities[i][j], (int[]){1,2,3,4,5,6,7,8,9}, 9 * sizeof(int));
            }
            else {
                set_singleton(possibilities[i][j],board[i][j]);
            }
        }
    }
    return possibilities;
}

int is_singleton(int* row) {
    int count = 0,ind;
    for(int i = 0; i < 9; i++) {
        if(row[i]) {
            ind = i;
            count++;
        }
    }
    if(count == 1) return ind;
    return -1;
}

bool is_empty(int* row) {
    for(int i = 0; i < 9; i++) {
        if(row[i]) return false;
    }
    return true;
}

int row_size(int* row) {
    int count = 0;
    for(int i = 0; i < 9; i++)
        if(row[i]) count++;
    return count;
}

void set_singleton(int* row, int digit) {
    memset(row, 0, 9 * sizeof(int));
    row[digit - 1] = digit;
}

void propagate(int *** possibilities) {
    bool progress = true;
    while(progress) {
        progress = false;
        FOR_EACH_CELL {
            int single_ind = is_singleton(possibilities[i][j]);
            if(single_ind>=0) {
                int digit = possibilities[i][j][single_ind];
                bool changed = false;
                if(!eliminate_from_peers(possibilities, i, j, digit, &changed)) {
                    return;
                }
                progress = changed;
            }
        }

        //check for rows
        for(int row = 0; row < 9; row++) {
            for(int d = 1; d <= 9; d++) {
                int count = 0;
                int target_col = -1;

                for(int col = 0; col < 9; col++) {
                    if(possibilities[row][col][d - 1]) {
                        count++;
                        target_col = col;
                    }
                }

                if(count == 1 && row_size(possibilities[row][target_col]) > 1) {
                    set_singleton(possibilities[row][target_col], d);
                    progress = true;
                }
            }
        }

        //check for cols
        for(int col = 0; col < 9; col++) {
            for(int d = 1; d <= 9; d++) {
                int count = 0;
                int target_row = -1;

                for(int row = 0; row < 9; row++) {
                    if (possibilities[row][col][d - 1]) {
                        count++;
                        target_row = row;
                    }
                }

                if (count == 1 && row_size(possibilities[target_row][col]) > 1) {
                    set_singleton(possibilities[target_row][col], d);
                    progress = true;
                }
            }
        }
        //check for boxes
        for(int box_row = 0; box_row < 3; box_row++) {
            for(int box_col = 0; box_col < 3; box_col++) {
                for(int d = 1; d <= 9; d++) {
                    int count = 0;
                    int target_r = -1, target_c = -1;

                    for(int i = 0; i < 3; i++) {
                        for(int j = 0; j < 3; j++) {
                            int r = box_row * 3 + i;
                            int c = box_col * 3 + j;

                            if(possibilities[r][c][d - 1]) {
                                count++;
                                target_r = r;
                                target_c = c;
                            }
                        }
                    }

                    if(count == 1 && row_size(possibilities[target_r][target_c]) > 1) {
                        set_singleton(possibilities[target_r][target_c], d);
                        progress = true;
                    }
                }
            }
        }
    }
}

bool eliminate_from_peers(int*** possibilities, int r, int c, int digit, bool* changed) {
    FOR_EACH_CELL {
        if(i == r && j == c) continue;

        if(i == r || j == c || (i / 3 == r / 3 && j / 3 == c / 3)) {
            if(possibilities[i][j][digit-1]) {
                possibilities[i][j][digit-1] = 0;
                *changed = true;
                if(is_empty(possibilities[i][j])) {
                    return false;
                }
            }
        }
    }

    return true;  
}

int *** make_copy(int *** possibilities) {
    int *** new_possibilities = (int***)calloc(9, sizeof(int**));
    for(int i=0;i<9;i++) {
        new_possibilities[i] = (int**)calloc(9, sizeof(int*));
        for(int j=0;j<9;j++) {
            new_possibilities[i][j] = (int*)calloc(9, sizeof(int));
            memcpy(new_possibilities[i][j], possibilities[i][j], 9 * sizeof(int));
        }
    }
    return new_possibilities;
}


int *** search(int *** possibilities) {
    propagate(possibilities);
    FOR_EACH_CELL {
        if(row_size(possibilities[i][j]) == 0) {
            return NULL;
        }
    }
    bool found = false;
    FOR_EACH_CELL {
        if(is_singleton(possibilities[i][j]) < 0) {
            found = true;
            break;
        }
    }
    if(!found) return possibilities;

    int min_i,min_j,min_possibilities=10;
    FOR_EACH_CELL {
        if(min_possibilities > row_size(possibilities[i][j]) && row_size(possibilities[i][j]) > 1) {
            min_possibilities = row_size(possibilities[i][j]);
            min_i = i;
            min_j = j;
        }
    }
    for(int d = 1; d <=9; d++) {
        if(possibilities[min_i][min_j][d-1]) {
            int *** new_possibilities = make_copy(possibilities);
            set_singleton(new_possibilities[min_i][min_j],d);
            int *** attempt = search(new_possibilities);
            if(attempt) {
                return attempt;
            }
            else {
                free_3d(new_possibilities);
            }
        }
    }
    return NULL;
}

int ** exctract_solution(int *** possibilities) {
    int ** board = (int**)calloc(9, sizeof(int*));
    for(int i = 0; i < 9; i++) {
        board[i] = (int*)calloc(9, sizeof(int));
        for(int j = 0; j < 9; j++) {
            board[i][j] = possibilities[i][j][is_singleton(possibilities[i][j])];
        }
    }
    free_3d(possibilities);
    return board;
}
int ** solve(int ** board) {
    int *** possibilities = initialize(board);
    int *** result = search(possibilities);
    
    if (result == NULL || result != possibilities) {
        free_3d(possibilities);
    }

    if (result) {
        return exctract_solution(result);
    }

    return NULL;
}