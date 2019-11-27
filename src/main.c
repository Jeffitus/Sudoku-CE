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

uint8_t i;
uint8_t j;
uint8_t k;
uint8_t l;

void draw_three_boxes(uint8_t pos);
void draw_puzzle(uint8_t puzzle[9][9]);
void game_loop(uint8_t puzzle[9][9]);

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

void main(void) {
    gfx_Begin();
    draw_puzzle(puzzle1);
    game_loop(puzzle1);
    gfx_End();
}

void draw_puzzle(uint8_t puzzle[9][9]) {

	gfx_SetDrawBuffer();

    gfx_SetTextScale(2, 2);
    gfx_SetColor(0);
    
    draw_three_boxes(0);
    draw_three_boxes(1);
    draw_three_boxes(2);

    gfx_SetTextFGColor(74);

    for (j=0; j<9; j++) {
        for (k=0; k<9; k++) {
        	if (puzzle[j][k] != 0) {
            	gfx_SetTextXY(((((j * PLAYING_GRID_SIZE) / 9) + 3) + (j / 3)), ((((k * PLAYING_GRID_SIZE) / 9) + 3) + k / 3));
            	gfx_PrintInt(puzzle[j][k], 1);
        	}	
        }
    }
    gfx_SwapDraw();
    gfx_BlitScreen();
}

void draw_three_boxes(uint8_t pos) {
    for (i = 0; i < 4; i++) {
        gfx_VertLine_NoClip((i * (((PLAYING_GRID_SIZE - 1) / 9)) + (((PLAYING_GRID_SIZE - 1) / 3) * pos) + pos), 0, (PLAYING_GRID_SIZE + 2));
        gfx_HorizLine_NoClip(0, (i * (((PLAYING_GRID_SIZE - 1) / 9)) + (((PLAYING_GRID_SIZE - 1) / 3) * pos) + pos), (PLAYING_GRID_SIZE + 2));
    }
}

void game_loop(uint8_t puzzle[9][9]) {

	uint8_t current_board[9][9];

	uint8_t cell_x_pos = 0;
	uint8_t cell_y_pos = 0;
	kb_key_t key;

	memcpy(current_board, puzzle, sizeof(current_board));

	gfx_SetDrawBuffer();

	gfx_SetColor(160);
	gfx_Rectangle_NoClip(1, 1, 18, 18);

    while (!(kb_Data[6] & kb_Clear)) {

		gfx_SetColor(255);
		gfx_Rectangle_NoClip((1 + (cell_x_pos * 19) + (cell_x_pos / 3)), (1 + (cell_y_pos * 19) + (cell_y_pos / 3)), 18, 18);

    	kb_Scan();

		switch (kb_Data[7]) {
			case kb_Down:
				if (cell_y_pos < 8){
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
			if (puzzle[cell_x_pos][cell_y_pos] == 0) {
				current_board[cell_x_pos][cell_y_pos] = 0;
				gfx_SetColor(255);
				gfx_FillRectangle_NoClip((2 + (cell_x_pos * 19) + (cell_x_pos / 3)), (2 + (cell_y_pos * 19) + (cell_y_pos / 3)), 16, 16);
			}
		}
		if (kb_IsDown(kb_Key1)) {
			if (puzzle[cell_x_pos][cell_y_pos] == 0) {
				current_board[cell_x_pos][cell_y_pos] = 1;
			}
		}
		if (kb_IsDown(kb_Key2)) {
			if (puzzle[cell_x_pos][cell_y_pos] == 0) {
				current_board[cell_x_pos][cell_y_pos] = 2;
			}
		}
		if (kb_IsDown(kb_Key3)) {
			if (puzzle[cell_x_pos][cell_y_pos] == 0) {
				current_board[cell_x_pos][cell_y_pos] = 3;
			}
		}
		if (kb_IsDown(kb_Key4)) {
			if (puzzle[cell_x_pos][cell_y_pos] == 0) {
				current_board[cell_x_pos][cell_y_pos] = 4;
			}
		}
		if (kb_IsDown(kb_Key5)) {
			if (puzzle[cell_x_pos][cell_y_pos] == 0) {
				current_board[cell_x_pos][cell_y_pos] = 5;
			}
		}
		if (kb_IsDown(kb_Key6)) {
			if (puzzle[cell_x_pos][cell_y_pos] == 0) {
				current_board[cell_x_pos][cell_y_pos] = 6;
			}
		}
		if (kb_IsDown(kb_Key7)) {
			if (puzzle[cell_x_pos][cell_y_pos] == 0) {
				current_board[cell_x_pos][cell_y_pos] = 7;
			}
		}
		if (kb_IsDown(kb_Key8)) {
			if (puzzle[cell_x_pos][cell_y_pos] == 0) {
				current_board[cell_x_pos][cell_y_pos] = 8;
			}
		}
		if (kb_IsDown(kb_Key9)) {
			if (puzzle[cell_x_pos][cell_y_pos] == 0) {
				current_board[cell_x_pos][cell_y_pos] = 9;
			}
		}


		gfx_SetColor(160);
		gfx_Rectangle_NoClip((1 + (cell_x_pos * 19) + (cell_x_pos / 3)), (1 + (cell_y_pos * 19) + (cell_y_pos / 3)), 18, 18);

		gfx_SetTextFGColor(8);
		for (j=0; j<9; j++) {
        	for (k=0; k<9; k++) {
        		if ((puzzle[j][k] == 0) && (current_board[j][k] != 0)) {
            		gfx_SetTextXY(((((j * PLAYING_GRID_SIZE) / 9) + 3) + (j / 3)), ((((k * PLAYING_GRID_SIZE) / 9) + 3) + k / 3));
            		gfx_PrintInt(current_board[j][k], 1);       			
        		}
        	}
    	}

		gfx_SwapDraw();
		gfx_Blit(gfx_screen);
	}
}