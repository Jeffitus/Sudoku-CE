#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <graphx.h>
#include <fontlibc.h>
#include <keypadc.h>
#include <debug.h>

#include "defines.h"
#include "drawing.h"
#include "menu.h"
#include "game.h"
#include "solve.h"
#include "gfx/gfx.h"

void swap(uint8_t *a, uint8_t *b) {
    uint8_t temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

void shuffle_array(uint8_t array[9]) {
    uint8_t i;
    uint8_t j;
    for (i = 8; i > 0; i--) {
        j = randInt(0, i);
        swap(&array[i], &array[j]);
    }
    
}

bool solve_sudoku(void) {
    uint8_t row;
    uint8_t col;
    uint8_t i;
    uint8_t num;
    uint8_t number_list[9] = {1,2,3,4,5,6,7,8,9};
    row = 0;
    col = 0;
    if (!find_unassigned_cell(&row, &col)) {
        return true;
    }

    shuffle_array(number_list);

    for (i = 0; i < 8; i++) {
        num = number_list[i];
        if (valid_value(row, col, num)) {
            puzzle[row][col] = num;

            if (solve_sudoku()) {
                return true;
            }

            puzzle [row][col] = 0;
        }
    }
    return false;
}

bool find_unassigned_cell(uint8_t *p_row, uint8_t *p_col) {
    uint8_t row;
    uint8_t col;
    for (row = 0; row < 9; row++) {
        for (col = 0; col < 9; col++) {
            if (puzzle[row][col] == 0) {
                *p_row = row;
                *p_col = col;
                return true;
            }
        }
    }
    return false;
}

bool used_in_row(uint8_t row, uint8_t num) {
    uint8_t col;
    for (col = 0; col < 9; col++) {
        if (puzzle[row][col] == num) {
            return true;
        }
    }
    return false;
}

bool used_in_col(uint8_t col, uint8_t num) {
    uint8_t row;
    for (row = 0; row < 9; row++) {
        if (puzzle[row][col] == num) {
            return true;
        }
    }
    return false;
}

bool used_in_box(uint8_t start_row, uint8_t start_col, uint8_t num) {
    uint8_t row;
    uint8_t col;
    for (row = 0; row < 3; row++) {
        for (col = 0; col < 3; col++) {
            if (puzzle[row + start_row][col + start_col] == num) {
                return true;
            }
        }
    }
    return false;
}

bool valid_value(uint8_t row, uint8_t col, uint8_t num) {
    return !used_in_row(row, num) && !used_in_col(col, num) && !used_in_box(row - row%3, col - col%3, num) && puzzle[row][col] == 0;
}