#include "sudoku.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

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