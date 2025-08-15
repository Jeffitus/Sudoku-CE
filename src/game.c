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

void game_loop(void) {

    uint8_t selected_row;
    uint8_t selected_col;
    uint8_t prev_row;
    uint8_t prev_col;
    bool pencil_mode;
    bool up, down, left, right;
    bool prevkey;
    kb_key_t arrows;
    kb_key_t numpad;
    uint8_t num;

    uint8_t counter;

    uint24_t timer_count;

    bool puzzle_filled;
    bool win;
    bool quit;

    char pause_menu_title[] = "Paused";
    char *pause_menu_options[] = {
        "Resume",
        "Give up",
        "Quit"
    };

    selected_col = 0;
    selected_row = 0;
    prev_col = 0;
    prev_row = 0;
    pencil_mode = false;
    counter = 0;
    timer_count = 0;
    prevkey = false;

    num = 0;

    win = false;
    quit = false;

    fontlib_SetForegroundColor(BLACK);

    draw_grid();
    setup_timer();

    do {

        if (pencil_mode) {
            gfx_SetColor(BLUE);
            gfx_HorizLine_NoClip(288, 204, 26);
            gfx_HorizLine_NoClip(288, 237, 26);
            gfx_VertLine_NoClip(284, 208, 26);
            gfx_VertLine_NoClip(317, 208, 26);
            gfx_FillRectangle_NoClip(285, 205, 32, 32);
        } else {
            gfx_SetColor(WHITE);
            gfx_FillRectangle_NoClip(283, 203, 36, 36);
        }
        gfx_TransparentSprite_NoClip(pencil, 283, 203);

        gfx_SetColor(WHITE);
        gfx_FillRectangle_NoClip(prev_col * (CELL_SIZE + 1) + prev_col / 3 + 1 + PUZZLE_X, prev_row * (CELL_SIZE + 1) + prev_row / 3 + 1 + PUZZLE_Y, CELL_SIZE, CELL_SIZE);
        gfx_SetColor(BLUE);
        gfx_Rectangle_NoClip(selected_col * (CELL_SIZE + 1) + selected_col / 3 + 1 + PUZZLE_X, selected_row * (CELL_SIZE + 1) + selected_row / 3 + 1 + PUZZLE_Y, CELL_SIZE, CELL_SIZE);
        if (!(puzzle[prev_row][prev_col] & VALUE)) {
            draw_pencils(prev_row, prev_col);
        }
        puzzle_filled = draw_puzzle();

        prev_row = selected_row;
        prev_col = selected_col;

        if (timer_IntStatus & TIMER1_RELOADED) {
            timer_count++;
            draw_timer(timer_count);
            timer_IntAcknowledge = TIMER1_RELOADED;
        }

        if (puzzle_filled) {
            win = win_check();
        }

        kb_Scan();

        arrows = kb_Data[7];

        right = arrows & kb_Right;
        left = arrows & kb_Left;
        up = arrows & kb_Up;
        down = arrows & kb_Down;

        numpad = kb_Data[3] ^ kb_Data[4] ^ kb_Data[5];

        /* if new key or held long enough */
        if (arrows && !prevkey | (counter > 4)) {
            if (right && selected_col < 8) {
                selected_col++;
            }
            if (left && selected_col > 0) {
                selected_col--;
            }
            if (up && selected_row > 0) {
                selected_row--;
            }
            if (down && selected_row < 8) {
                selected_row++;
            }
        }

        if (numpad && (puzzle[selected_row][selected_col] & UNDEFINED) && !prevkey) {
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
                    break;
                case kb_6:
                    num = 6;
                    break;
                case kb_9:
                    num = 9;
                    break;
                default:
                    break;
            }
            if (pencil_mode) {
                if (num != 0) {
                    puzzle[selected_row][selected_col] ^= PENCIL_MARK(num);
                }
            } else {
                puzzle[selected_row][selected_col] = (puzzle[selected_row][selected_col] & ~VALUE) | num;
                dbg_sprintf(dbgout, "%d\n", puzzle[selected_row][selected_col] & VALUE);
            }
        }

        /*toggle pencil mode*/
        if (kb_Data[1] & kb_Graph && !prevkey) {
            pencil_mode = !pencil_mode;
        }

        if (kb_Data[6] & kb_Clear && !prevkey) {
            /* To avoid exploiting the timer by holding down [clear] */
            gfx_FillScreen(WHITE);
            gfx_BlitBuffer();
            wait_for_key_release();
            switch (basic_menu(pause_menu_title, pause_menu_options, 2)) {
                case 0:
                    draw_grid();
                    break;
                case 1:
                    reveal_solution();
                    quit = true;
                    break;
                case 2:
                    quit = true;
                    break;
            }
        }

        if (!prevkey) {
            counter = 0;
        }
        prevkey = arrows | numpad | (kb_Data[1] & kb_Graph) | (kb_Data[6] & kb_Clear);

        counter++;

        gfx_BlitBuffer();
    } while (!quit && !win);
    if (win) {
        fontlib_SetForegroundColor(BLACK);
        fontlib_SetCursorPosition(255, 99);
        fontlib_DrawString("You");
        fontlib_SetCursorPosition(254, 117);
        fontlib_DrawString("Win!");
        gfx_BlitBuffer();
        wait_for_key_release();
        wait_for_key_press();
        wait_for_key_release();
    }
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

    result_x = 0;
    result_y = 0;
    result_x_temp = 0;
    result_y_temp = 0;
    result_boxes = 0;
    result_boxes_temp = 0;
    win = false;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            result_y_temp |= 1 << ((puzzle[i][j] & VALUE) - 1);
            result_x_temp |= 1 << ((puzzle[j][i] & VALUE) - 1);
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
                        result_boxes_temp |= 1 << ((puzzle[3 * i + k][3 * j + l] & VALUE) - 1);
                    }
                }
                if (result_boxes_temp == 511) {
                    result_boxes |= 1 << (3 * i + j);
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
    uint8_t j;
    uint8_t row;
    uint8_t col;
    solve_sudoku();

    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            puzzle[i][j] <<= 4;
        }
    }

    for (i = 0; i < difficulty; i++) {
        do {
            row = randInt(0,8);
            col = randInt(0,8);
        } while ((puzzle[row][col] & VALUE) != 0);
        puzzle[row][col] |= SOLUTION(puzzle[row][col]);
    }

    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if ((puzzle[i][j] & VALUE) == 0) {
                puzzle[i][j] |= UNDEFINED;
            }
        }
    }
}

void setup_timer(void) {
    timer_Control = TIMER1_DISABLE;

    timer_1_ReloadValue = timer_1_Counter = ONE_SECOND;

    timer_Control = TIMER1_ENABLE | TIMER1_32K | TIMER1_0INT | TIMER1_DOWN;
}

void reveal_solution(void) {
    uint8_t i;
    uint8_t j;

    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (puzzle[i][j] & UNDEFINED) {
                puzzle[i][j] = SOLUTION(puzzle[i][j] & SOLUTION_DATA) | UNDEFINED;
            }
        }
    }

    draw_grid();
    draw_puzzle();
    gfx_BlitBuffer();
    wait_for_key_press();
    wait_for_key_release();
}
