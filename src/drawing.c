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

void draw_three_boxes(uint8_t pos) {
    uint8_t i;
    uint8_t cell_size = (PLAYING_GRID_SIZE - 1) / 9;
    for (i = 0; i < 4; i++) {
        gfx_VertLine_NoClip((i * cell_size + (PLAYING_GRID_SIZE - 1) / 3 * pos + pos) + PUZZLE_X, PUZZLE_Y, PLAYING_GRID_SIZE + 2);
        gfx_HorizLine_NoClip(PUZZLE_X, (i * (PLAYING_GRID_SIZE - 1) / 9 + (PLAYING_GRID_SIZE - 1) / 3 * pos + pos) + PUZZLE_Y, PLAYING_GRID_SIZE + 2);
    }
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
            gfx_SetTextXY(j * PLAYING_GRID_SIZE / 9 + j / 3 + 6 + PUZZLE_X, i * PLAYING_GRID_SIZE / 9 + i / 3 + 6 + PUZZLE_Y);
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