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
void draw_puzzle(uint8_t puzzle[9][9]);
void game_loop(uint8_t puzzle[9][9]);


void main(void) {

    uint8_t puzzle1[9][9] = {
        {0,0,7,8,0,0,0,0,2},
        {0,0,0,0,0,0,8,4,0},
        {8,0,9,1,0,0,6,7,0},
        {0,6,0,7,0,0,0,0,4},
        {4,7,5,0,9,0,1,6,3},
        {2,0,0,0,0,4,0,8,0},
        {0,8,1,0,0,5,3,0,9},
        {0,2,4,0,0,0,0,0,0},
        {9,0,0,0,0,7,4,0,0}
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
    draw_puzzle(puzzle1);
    game_loop(puzzle1);
    gfx_End();
}

void draw_puzzle(uint8_t puzzle[9][9]) {
    uint8_t i;
    uint8_t j;

    gfx_SetTextScale(2, 2);
    gfx_SetColor(0);
    
    draw_three_boxes(0);
    draw_three_boxes(1);
    draw_three_boxes(2);

    gfx_SetTextFGColor(74);

    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (!(puzzle[i][j] & FIXED_NUM)) {
                gfx_SetTextXY(((((j * PLAYING_GRID_SIZE) / 9) + 3) + j / 3), ((((i * PLAYING_GRID_SIZE) / 9) + 3) + (i / 3)));
                gfx_PrintInt(puzzle[i][j], 1);
            }
        }
    }
    gfx_BlitBuffer();
}

void draw_three_boxes(uint8_t pos) {
    uint8_t i;
    uint8_t cell_size = (PLAYING_GRID_SIZE - 1) / 9;
    for (i = 0; i < 4; i++) {
        gfx_VertLine_NoClip((i * (cell_size) + (((PLAYING_GRID_SIZE - 1) / 3) * pos) + pos), 0, (PLAYING_GRID_SIZE + 2));
        gfx_HorizLine_NoClip(0, (i * (((PLAYING_GRID_SIZE - 1) / 9)) + (((PLAYING_GRID_SIZE - 1) / 3) * pos) + pos), (PLAYING_GRID_SIZE + 2));
    }
}

void game_loop(uint8_t puzzle[9][9]) {
    uint8_t i;
    uint8_t j;

    uint8_t cell_x_pos = 0;
    uint8_t cell_y_pos = 0;
    bool up, down, left, right;
    bool prevkey;
    kb_key_t arrows;
    kb_key_t numpad;

    int counter = 0;

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
        if (arrows && (!prevkey | (counter > 4))) {
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
        gfx_Rectangle_NoClip((1 + (cell_x_pos * 19) + (cell_x_pos / 3)), (1 + (cell_y_pos * 19) + (cell_y_pos / 3)), 18, 18);


        gfx_SetTextFGColor(8);
        for (i = 0; i < 9; i++) {
            for (j = 0; j < 9;  j++) {
                if ((puzzle[i][j] & FIXED_NUM) && ((puzzle[i][j] & ~FIXED_NUM) != 0)) {
                    gfx_SetTextXY(((((j * PLAYING_GRID_SIZE) / 9) + 3) + (j / 3)), ((((i * PLAYING_GRID_SIZE) / 9) + 3) + i / 3));
                    gfx_PrintInt(puzzle[i][j] & ~FIXED_NUM, 1);
                }
            }
        }

        draw_puzzle(puzzle);

        gfx_BlitBuffer();

        counter++;
    } while (!(kb_Data[6] & kb_Clear));
}