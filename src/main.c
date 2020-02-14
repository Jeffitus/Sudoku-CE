/*
 *--------------------------------------
 * Program Name: Sudoku
 * Author: Jeffitus
 * License:
 * Description:
 *--------------------------------------
*/

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

#define PLAYING_GRID_SIZE 172

#define FIXED_NUM (1 << 7)

void draw_three_boxes(uint8_t pos);
bool draw_puzzle(uint8_t puzzle[9][9]);
void game_loop(uint8_t puzzle[9][9]);
bool win_check(uint8_t puzzle[9][9]);

void main(void) {

    uint8_t puzzle1[9][9] = {
        {0,1,7,8,4,6,9,3,2},
        {6,3,2,5,7,9,8,4,1},
        {8,4,9,1,3,2,6,7,5},
        {1,6,8,7,5,3,2,9,4},
        {4,7,5,2,9,8,1,6,3},
        {2,9,3,6,1,4,5,8,7},
        {7,8,1,4,6,5,3,2,9},
        {3,2,4,9,8,1,7,5,6},
        {9,5,6,3,2,7,4,1,8}
    };

    uint8_t i;
    uint8_t j;

    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (puzzle1[i][j] == 0) {
                puzzle1[i][j] |= FIXED_NUM;
            }
        }
    }

    gfx_Begin();
    gfx_SetDrawBuffer();
    game_loop(puzzle1);
    gfx_SetTextFGColor(0);
    gfx_PrintStringXY("You Win!", 1, 176);
    gfx_BlitBuffer();
    while (!os_GetCSC());
    gfx_End();
}

bool draw_puzzle(uint8_t puzzle[9][9]) {
    uint8_t i;
    uint8_t j;

    bool puzzle_filled;

    puzzle_filled = true;

    gfx_SetTextScale(2, 2);
    gfx_SetColor(0);
    
    draw_three_boxes(0);
    draw_three_boxes(1);
    draw_three_boxes(2);

    gfx_SetTextFGColor(74);

    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            gfx_SetTextXY(j * PLAYING_GRID_SIZE / 9 + 3 + j / 3, i * PLAYING_GRID_SIZE / 9 + 3 + i / 3);
            if (!(puzzle[i][j] & FIXED_NUM)) {
                gfx_SetTextFGColor(74);
                gfx_PrintInt(puzzle[i][j], 1);
            }
            if (puzzle[i][j] & FIXED_NUM && ((puzzle[i][j] & ~FIXED_NUM) != 0)) {
                gfx_SetTextFGColor(8);
                gfx_PrintInt(puzzle[i][j] & ~FIXED_NUM, 1);
            }
            if (puzzle[i][j] == 128) {
                puzzle_filled = false;
            }
        }
    }
    gfx_BlitBuffer();
    return puzzle_filled;
}

void draw_three_boxes(uint8_t pos) {
    uint8_t i;
    uint8_t cell_size = (PLAYING_GRID_SIZE - 1) / 9;
    for (i = 0; i < 4; i++) {
        gfx_VertLine_NoClip((i * cell_size + (PLAYING_GRID_SIZE - 1) / 3 * pos + pos), 0, PLAYING_GRID_SIZE + 2);
        gfx_HorizLine_NoClip(0, (i * (PLAYING_GRID_SIZE - 1) / 9 + (PLAYING_GRID_SIZE - 1) / 3 * pos + pos), PLAYING_GRID_SIZE + 2);
    }
}

void game_loop(uint8_t puzzle[9][9]) {
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

bool win_check(uint8_t puzzle[9][9]) {
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