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
    kb_key_t key;

    gfx_SetColor(160);
    gfx_Rectangle_NoClip(1, 1, 18, 18);

    do {

        gfx_SetColor(255);
        gfx_Rectangle_NoClip((1 + (cell_x_pos * 19) + (cell_x_pos / 3)), (1 + (cell_y_pos * 19) + (cell_y_pos / 3)), 18, 18);

        kb_Scan();

        switch (kb_Data[7]) {
            case kb_Down:
                if (cell_y_pos < 8) {
                    cell_y_pos++;
                }
                delay(100);
                break;
            case kb_Left:
                if (cell_x_pos){
                    cell_x_pos--;
                }
                delay(100);
                break;
            case kb_Right:
                if (cell_x_pos < 8){
                    cell_x_pos++;
                }
                delay(100);
                break;
            case kb_Up:
                if (cell_y_pos){
                    cell_y_pos--;
                }
                delay(100);
                break;
            default:
                break;
        }

        if (kb_IsDown(kb_Key0)) {
            if (puzzle[cell_x_pos][cell_x_pos] & FIXED_NUM) {
                puzzle[cell_x_pos][cell_x_pos] = 0;
                
            }
        }
        if (kb_IsDown(kb_Key1)) {
            if (puzzle[cell_y_pos][cell_x_pos] & FIXED_NUM) {
                puzzle[cell_y_pos][cell_x_pos] = 1;
            }
        }
        if (kb_IsDown(kb_Key2)) {
            if (puzzle[cell_y_pos][cell_x_pos] & FIXED_NUM) {
                puzzle[cell_y_pos][cell_x_pos] = 2;
            }
        }
        if (kb_IsDown(kb_Key3)) {
            if (puzzle[cell_y_pos][cell_x_pos] & FIXED_NUM) {
                puzzle[cell_y_pos][cell_x_pos] = 3;
            }
        }
        if (kb_IsDown(kb_Key4)) {
            if (puzzle[cell_y_pos][cell_x_pos] & FIXED_NUM) {
                puzzle[cell_y_pos][cell_x_pos] = 4;
            }
        }
        if (kb_IsDown(kb_Key5)) {
            if (puzzle[cell_y_pos][cell_x_pos] & FIXED_NUM) {
                puzzle[cell_y_pos][cell_x_pos] = 5;
            }
        }
        if (kb_IsDown(kb_Key6)) {
            if (puzzle[cell_y_pos][cell_x_pos] & FIXED_NUM) {
                puzzle[cell_y_pos][cell_x_pos] = 6;
            }
        }
        if (kb_IsDown(kb_Key7)) {
            if (puzzle[cell_y_pos][cell_x_pos] & FIXED_NUM) {
                puzzle[cell_y_pos][cell_x_pos] = 7;
            }
        }
        if (kb_IsDown(kb_Key8)) {
            if (puzzle[cell_y_pos][cell_x_pos] & FIXED_NUM) {
                puzzle[cell_y_pos][cell_x_pos] = 8;
            }
        }
        if (kb_IsDown(kb_Key9)) {
            if (puzzle[cell_y_pos][cell_x_pos] & FIXED_NUM) {
                puzzle[cell_y_pos][cell_x_pos] = 9;
            }
        }


        gfx_SetColor(160);
        gfx_Rectangle_NoClip((1 + (cell_x_pos * 19) + (cell_x_pos / 3)), (1 + (cell_y_pos * 19) + (cell_y_pos / 3)), 18, 18);

        gfx_SetColor(255);
        gfx_FillRectangle_NoClip((2 + (cell_x_pos * 19) + (cell_x_pos / 3)), (2 + (cell_y_pos * 19) + (cell_y_pos / 3)), 16, 16);

        gfx_SetTextFGColor(8);
        for (i = 0; i < 9; i++) {
            for (j = 0; j < 9;  j++) {
                if ((puzzle[i][j] & FIXED_NUM) && ((puzzle[i][j] & ~FIXED_NUM) != 0)) {
                    gfx_SetTextXY(((((i * PLAYING_GRID_SIZE) / 9) + 3) + (i / 3)), ((((j * PLAYING_GRID_SIZE) / 9) + 3) + j / 3));
                    gfx_PrintInt(puzzle[i][j], 1);
                }
            }
        }

        draw_puzzle(puzzle);

        gfx_BlitBuffer();
    } while (!(kb_Data[6] & kb_Clear));
}