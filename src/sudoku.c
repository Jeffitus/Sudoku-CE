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
extern uint8_t penciled[9][9];

uint8_t number_list[9] = {1,2,3,4,5,6,7,8,9};

void shuffle_array(uint8_t array[9]) {
    uint8_t i;
    uint8_t j;
    uint8_t temp;
    for (i = 8; i > 0; i--) {
        j = randInt(0, i);
        temp = array[i];
        array[i] = array[j];
        array[j] = array[temp];
    }
    
}

bool solve_sudoku(uint8_t grid[9][9]) {
    uint8_t row;
    uint8_t col;
    uint8_t i;
    uint8_t num;
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

bool find_unassigned_cell(uint8_t grid[9][9], uint8_t *p_row, uint8_t *p_col) {
    uint8_t row;
    uint8_t col;
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

bool used_in_row(uint8_t grid[9][9], uint8_t row, uint8_t num) {
    uint8_t col;
    for (col = 0; col < 9; col++) {
        if (grid[row][col] == num) {
            return true;
        }
    }
    return false;
}

bool used_in_col(uint8_t grid[9][9], uint8_t col, uint8_t num) {
    uint8_t row;
    for (row = 0; row < 9; row++) {
        if (grid[row][col] == num) {
            return true;
        }
    }
    return false;
}

bool used_in_box(uint8_t grid[9][9], uint8_t start_row, uint8_t start_col, uint8_t num) {
    uint8_t row;
    uint8_t col;
    for (row = 0; row < 3; row++) {
        for (col = 0; col < 3; col++) {
            if (grid[row + start_row][col + start_col] == num) {
                return true;
            }
        }
    }
    return false;
}

bool valid_value(uint8_t grid [9][9], uint8_t row, uint8_t col, uint8_t num) {
    return !used_in_row(grid, row, num) && !used_in_col(grid, col, num) && !used_in_box(grid, row - row%3, col - col%3, num) && grid[row][col] == 0;
}

void game_loop(void) {
    uint8_t i;
    uint8_t j;

    uint8_t cell_x_pos;
    uint8_t cell_y_pos;
    bool pencil;
    bool up, down, left, right;
    bool prevkey;
    kb_key_t arrows;
    kb_key_t numpad;
    uint8_t num;

    uint8_t counter;

    bool puzzle_filled;
    bool win;

    cell_x_pos = 0;
    cell_y_pos = 0;
    pencil = false;
    counter = 0;
    prevkey = false;

    num = 0;

    win = false;

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
                    num = 0;
                    break;
                case kb_1:
                    num = 1;
                    break;
                case kb_4:
                    num = 4;
                    break;
                case kb_7:
                    num = 7;
                    break;
                default:
                    break;
            }
            switch (kb_Data[4]) {
                case kb_2:
                    num = 2;
                    break;
                case kb_5:
                    num = 5;
                    break;
                case kb_8:
                    num = 8;
                    break;
                default:
                    break;
            }
            switch (kb_Data[5]) {
                case kb_3:
                    num = 3;
                case kb_6:
                    num = 6;
                    break;
                case kb_9:
                    num = 9;
                    break;
                default:
                    break;
            }
            if (pencil) {
                if (num != 0) {
                    penciled[cell_y_pos][cell_x_pos] |= 1 << (num - 1);
                }
            } else {
                puzzle[cell_y_pos][cell_x_pos] = num | FIXED_NUM;
            }
        }

        /*toggle pencil mode*/
        if (kb_Data[1] & kb_2nd) {
            pencil = !pencil;
        }

        gfx_SetColor(160);
        gfx_Rectangle_NoClip(cell_x_pos * (CELL_SIZE + 1) + cell_x_pos / 3 + 1 + PUZZLE_X, cell_y_pos * (CELL_SIZE + 1) + cell_y_pos / 3 + 1 + PUZZLE_Y, CELL_SIZE, CELL_SIZE);

        puzzle_filled = draw_puzzle(puzzle);
        if (puzzle_filled) {
            win = win_check();
        }

        gfx_BlitBuffer();

        counter++;
    } while (!(kb_Data[6] & kb_Clear) && !win);
}

bool win_check(void) {
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

    /*if using a randomly generated puzzle, it is possible that puzzle == solution - solution will be zeroed otherwise*/
    if (puzzle == solution) {
        return true;
    }
    
    result_x = 0;
    result_y = 0;
    result_x_temp = 0;
    result_y_temp = 0;
    result_boxes = 0;
    result_boxes_temp = 0;
    win = false;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            result_y_temp |= 1 << (puzzle[i][j] & ~FIXED_NUM) - 1;
            result_x_temp |= 1 << (puzzle[j][i] & ~FIXED_NUM) - 1;
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
                        result_boxes_temp |= 1 << (puzzle[3 * i + k][3 * j + l] & ~FIXED_NUM) - 1;
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

void generate_puzzle(uint8_t difficulty) {
    uint8_t i;
    uint8_t row;
    uint8_t col;
    solve_sudoku(solution);
    for (i = 0; i < difficulty; i++) {
        do {
            row = randInt(0,8);
            col = randInt(0,8);
        } while (puzzle[row][col] != 0);
        puzzle[row][col] = solution[row][col];
    }
}