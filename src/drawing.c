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

void draw_three_boxes(uint8_t pos) {
    uint8_t i;
    uint8_t cell_size = (PLAYING_GRID_SIZE - 1) / 9;
    for (i = 0; i < 4; i++) {
        gfx_VertLine_NoClip((i * cell_size + (PLAYING_GRID_SIZE - 1) / 3 * pos + pos) + PUZZLE_X, PUZZLE_Y, PLAYING_GRID_SIZE + 2);
        gfx_HorizLine_NoClip(PUZZLE_X, (i * (PLAYING_GRID_SIZE - 1) / 9 + (PLAYING_GRID_SIZE - 1) / 3 * pos + pos) + PUZZLE_Y, PLAYING_GRID_SIZE + 2);
    }
}

void draw_grid(void) {
    gfx_FillScreen(WHITE);
    gfx_SetColor(BLACK);
    draw_three_boxes(0);
    draw_three_boxes(1);
    draw_three_boxes(2);
}

bool draw_puzzle(void) {
    uint8_t i;
    uint8_t j;

    bool puzzle_filled;

    puzzle_filled = true;

    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            set_cursor(j * PLAYING_GRID_SIZE / 9 + j / 3 + 6 + PUZZLE_X, i * PLAYING_GRID_SIZE / 9 + i / 3 + PUZZLE_Y - 1);
            if (!(puzzle[i][j] & UNDEFINED)) {
                set_color(BLACK);
                draw_uint(puzzle[i][j] & VALUE, 1);
            }
            if (puzzle[i][j] & UNDEFINED) {
                if ((puzzle[i][j] & VALUE) == 0) {
                    puzzle_filled = false;
                } else {
                    set_color(BLUE);
                    draw_uint(puzzle[i][j] & VALUE, 1);
                }
            }
        }
    }

    return puzzle_filled;
}

void draw_pencils(uint8_t row, uint8_t col) {
    uint8_t i;
    uint8_t j;
    gfx_SetTextFGColor(BLUE);
    gfx_SetTextScale(1, 1);
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (puzzle[row][col] & PENCIL_MARK(3 * i + j + 1)) {
                gfx_SetTextXY(col * PLAYING_GRID_SIZE / 9 + col / 3 + j * 8 + PUZZLE_X + 2, row * PLAYING_GRID_SIZE / 9 + row / 3 + i * 8 + PUZZLE_Y + 2);
                gfx_PrintUInt(3 * i + j + 1, 1);
            }
        }
    }
    gfx_SetTextScale(2, 2);
}

void draw_timer(uint24_t timer_count) {
    uint24_t hours;
    uint8_t minutes;
    uint8_t seconds;

    hours = floor(timer_count / 60 / 60);

    minutes = floor(timer_count / 60) - hours * 60;

    seconds = timer_count % 60;

    gfx_SetColor(WHITE);
    gfx_FillRectangle_NoClip(240, 5, 32, 54);

    set_cursor(240, 0);
    draw_uint(hours, 2);
    draw_string(":");
    set_cursor(240, 18);
    draw_uint(minutes, 2);
    draw_string(":");
    set_cursor(240, 36);
    draw_uint(seconds, 2);
}

/* a series of function pointers that will be set based on whether we have the right font */
void (*draw_string)(const char *);

void (*draw_uint)(unsigned int, uint8_t);

void (*set_cursor)(uint24_t, uint8_t);

void (*set_color)(uint8_t);

uint24_t (*get_string_width)(const char *);