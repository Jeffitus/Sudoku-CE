#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <graphx.h>
#include <keypadc.h>
#include <debug.h>

#include "defines.h"
#include "sudoku.h"
#include "drawing.h"

extern uint8_t puzzle[9][9];
extern uint8_t solution[9][9];

int number_list[9] = {1,2,3,4,5,6,7,8,9};

void swap(int *a, int *b) {
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

void shuffle_array(int array[9]) {
    int i;
    int j;
    for (i = 8; i > 0; i--) {
        j = randInt(0, i);
        swap(&array[i], &array[j]);
    }
    
}

bool solve_sudoku(uint8_t grid[9][9]) {
    int row;
    int col;
    int i;
    int num;
    row = 0;
    col = 0;
    if (!find_unassigned_cell(grid, &row, &col)) {
        return true;
    }

    shuffle_array(number_list);

    for (i = 0; i < 8; i++) {
        num = number_list[i];
        if (valid_value(grid, row, col, num)) {
            grid[row][col] = num;

            if (solve_sudoku(grid)) {
                return true;
            }

            grid [row][col] = 0;
        }
    }
    return false;
}

bool find_unassigned_cell(uint8_t grid[9][9], int *p_row, int *p_col) {
    int row;
    int col;
    for (row = 0; row < 9; row++) {
        for (col = 0; col < 9; col++) {
            if (grid[row][col] == 0) {
                *p_row = row;
                *p_col = col;
                return true;
            }
        }
    }
    return false;
}

bool used_in_row(uint8_t grid[9][9], int row, int num) {
    int col;
    for (col = 0; col < 9; col++) {
        if (grid[row][col] == num) {
            return true;
        }
    }
    return false;
}

bool used_in_col(uint8_t grid[9][9], int col, int num) {
    int row;
    for (row = 0; row < 9; row++) {
        if (grid[row][col] == num) {
            return true;
        }
    }
    return false;
}

bool used_in_box(uint8_t grid[9][9], int start_row, int start_col, int num) {
    int row;
    int col;
    for (row = 0; row < 3; row++) {
        for (col = 0; col < 3; col++) {
            if (grid[row + start_row][col + start_col] == num) {
                return true;
            }
        }
    }
    return false;
}

bool valid_value(uint8_t grid [9][9], int row, int col, int num) {
    return !used_in_row(grid, row, num) && !used_in_col(grid, col, num) && !used_in_box(grid, row - row%3, col - col%3, num) && grid[row][col] == 0;
}

void game_loop(uint8_t grid[9][9]) {
    uint8_t i;
    uint8_t j;

    uint8_t cell_x_pos;
    uint8_t cell_y_pos;
    bool up, down, left, right;
    bool prevkey;
    kb_key_t arrows;
    kb_key_t numpad;

    int counter;

    bool puzzle_filled;
    bool win;

    cell_x_pos = 0;
    cell_y_pos = 0;
    counter = 0;
    prevkey = false;

    win = false;

    gfx_SetColor(160);
    gfx_Rectangle_NoClip(1, 1, 18, 18);

    do {
        gfx_FillScreen(255);

        kb_Scan();

        arrows = kb_Data[7];

        right = arrows & kb_Right;
        left = arrows & kb_Left;
        up = arrows & kb_Up;
        down = arrows & kb_Down;

        numpad = kb_Data[3] ^ kb_Data[4] ^ kb_Data[5];

        /* if new key or held long enough */
        if (arrows && !prevkey | counter > 4) {
            if (right && cell_x_pos < 8) {
                cell_x_pos++;
            }
            if (left && cell_x_pos > 0) {
                cell_x_pos--;
            }
            if (up && cell_y_pos > 0) {
                cell_y_pos--;
            }
            if (down && cell_y_pos < 8) {
                cell_y_pos++;
            }
        }

        if (!prevkey) {
            counter = 0;
        }
        prevkey = arrows;

        if (numpad && (puzzle[cell_y_pos][cell_x_pos] & FIXED_NUM)) {
            switch (kb_Data[3]) {
                case kb_0:
                    puzzle[cell_y_pos][cell_x_pos] = 0 | FIXED_NUM;
                    break;
                case kb_1:
                    puzzle[cell_y_pos][cell_x_pos] = 1 | FIXED_NUM;
                    break;
                case kb_4:
                    puzzle[cell_y_pos][cell_x_pos] = 4 | FIXED_NUM;
                    break;
                case kb_7:
                    puzzle[cell_y_pos][cell_x_pos] = 7 | FIXED_NUM;
                    break;
                default:
                    break;
            }
            switch (kb_Data[4]) {
                case kb_2:
                    puzzle[cell_y_pos][cell_x_pos] = 2 | FIXED_NUM;
                    break;
                case kb_5:
                    puzzle[cell_y_pos][cell_x_pos] = 5 | FIXED_NUM;
                    break;
                case kb_8:
                    puzzle[cell_y_pos][cell_x_pos] = 8 | FIXED_NUM;
                    break;
                default:
                    break;
            }
            switch (kb_Data[5]) {
                case kb_3:
                    puzzle[cell_y_pos][cell_x_pos] = 3 | FIXED_NUM;
                    break;
                case kb_6:
                    puzzle[cell_y_pos][cell_x_pos] = 6 | FIXED_NUM;
                    break;
                case kb_9:
                    puzzle[cell_y_pos][cell_x_pos] = 9 | FIXED_NUM;
                    break;
                default:
                    break;
            }
        }

        gfx_SetColor(160);
        gfx_Rectangle_NoClip(1 + cell_x_pos * 19 + cell_x_pos / 3, 1 + cell_y_pos * 19 + cell_y_pos / 3, 18, 18);

        puzzle_filled = draw_puzzle(puzzle);
        if (puzzle_filled) {
            win = win_check(puzzle);
        }

        gfx_BlitBuffer();

        counter++;
    } while (!(kb_Data[6] & kb_Clear) && !win);
}

bool win_check(uint8_t grid[9][9]) {
    uint24_t result_x;
    uint24_t result_y;
    uint24_t result_x_temp;
    uint24_t result_y_temp;
    uint24_t result_boxes;
    uint24_t result_boxes_temp;
    bool win;
    uint8_t i;
    uint8_t j;
    uint8_t k;
    uint8_t l;

    result_x = 0;
    result_y = 0;
    result_x_temp = 0;
    result_y_temp = 0;
    result_boxes = 0;
    result_boxes_temp = 0;
    win = false;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            result_y_temp |= 1 << (grid[i][j] & ~FIXED_NUM) - 1;
            result_x_temp |= 1 << (grid[j][i] & ~FIXED_NUM) - 1;
        }
        if (result_y_temp == 511) {
            result_y |= 1 << i;
        }
        if (result_x_temp == 511) {
            result_x |= 1 << i;
        }
        result_y_temp = 0;
        result_x_temp = 0;
    }
    if (result_x == 511 && result_y == 511) {
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                for (k = 0; k < 3; k++) {
                    for (l = 0; l < 3; l++) {
                        result_boxes_temp |= 1 << (grid[3 * i + k][3 * j + l] & ~FIXED_NUM) - 1;
                    }
                }
                if (result_boxes_temp == 511) {
                    result_boxes |= 1 << 3 * i + j;
                }
            }
        }
        if (result_boxes == 511) {
            win = true;
        }
    }
    return win;
}

void generate_puzzle(int difficulty) {
    int i;
    int row;
    int col;
    solve_sudoku(solution);
    for (i = 0; i < difficulty; i++) {
        do {
            row = randInt(0,8);
            col = randInt(0,8);
        } while (puzzle[row][col] != 0);
        puzzle[row][col] = solution[row][col];
    }
}