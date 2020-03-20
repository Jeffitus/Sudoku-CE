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
#include "drawing.h"

extern uint24_t puzzle[9][9];
extern uint8_t solution[9][9];

void draw_three_boxes(uint8_t pos) {
    uint8_t i;
    uint8_t cell_size = (PLAYING_GRID_SIZE - 1) / 9;
    for (i = 0; i < 4; i++) {
        gfx_VertLine_NoClip((i * cell_size + (PLAYING_GRID_SIZE - 1) / 3 * pos + pos) + PUZZLE_X, PUZZLE_Y, PLAYING_GRID_SIZE + 2);
        gfx_HorizLine_NoClip(PUZZLE_X, (i * (PLAYING_GRID_SIZE - 1) / 9 + (PLAYING_GRID_SIZE - 1) / 3 * pos + pos) + PUZZLE_Y, PLAYING_GRID_SIZE + 2);
    }
}

void draw_grid(void) {
    gfx_FillScreen(255);
    gfx_SetColor(0);
    draw_three_boxes(0);
    draw_three_boxes(1);
    draw_three_boxes(2);
}

bool draw_puzzle(void) {
    uint8_t i;
    uint8_t j;

    bool puzzle_filled;

    puzzle_filled = true;

    gfx_SetTextScale(2, 2);
    gfx_SetTextFGColor(GRAY);

    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            gfx_SetTextScale(2, 2);
            gfx_SetTextXY(j * PLAYING_GRID_SIZE / 9 + j / 3 + 6 + PUZZLE_X, i * PLAYING_GRID_SIZE / 9 + i / 3 + 6 + PUZZLE_Y);
            if (!(puzzle[i][j] & UNDEFINED)) {
                gfx_SetTextFGColor(GRAY);
                gfx_PrintUInt(puzzle[i][j], 1);
            }
            if (puzzle[i][j] & UNDEFINED && ((puzzle[i][j] & VALUE) != 0)) {
                gfx_SetTextFGColor(BLUE);
                gfx_PrintUInt(puzzle[i][j] & VALUE, 1);
            }
            if (puzzle[i][j] == 128) {
                puzzle_filled = false;
            }
        }
    }
    gfx_BlitBuffer();
    return puzzle_filled;
}

void draw_pencils(uint8_t row, uint8_t col) {
    uint8_t i;
    uint8_t j;
    gfx_SetTextScale(1, 1);
    gfx_SetTextFGColor(BLUE);
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (puzzle[row][col] & PENCIL_MARK(3 * i + j + 1)) {
                gfx_SetTextXY(col * PLAYING_GRID_SIZE / 9 + col / 3 + j * 8 + PUZZLE_X + 2, row * PLAYING_GRID_SIZE / 9 + row / 3 + i * 8 + PUZZLE_Y + 2);
                gfx_PrintUInt(3 * i + j + 1, 1);
            }
        }
    }
}