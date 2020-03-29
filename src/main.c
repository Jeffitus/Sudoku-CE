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
#include <fontlibc.h>
#include <keypadc.h>
#include <debug.h>

#include "defines.h"
#include "drawing.h"
#include "menu.h"
#include "game.h"
#include "solve.h"
#include "gfx/gfx.h"
#include "font/myfonts.h"

/*
for each element of puzzle array:
bits 0-3: number
bits 14-22: contain pencil data
bit 23: set if undefined, cleared if immutable
*/
/*uint24_t puzzle[9][9] = {
    {0,2,7,0,5,4,0,9,6},
    {9,6,5,3,2,7,1,4,8},
    {3,4,1,6,8,9,7,5,2},
    {5,0,3,4,0,8,2,0,1},
    {4,7,2,5,1,3,6,8,9},
    {6,1,8,9,7,2,4,3,5},
    {7,8,0,2,3,0,9,1,0},
    {1,5,4,7,9,6,8,2,3},
    {2,3,9,8,4,1,5,6,7}
};*/

uint24_t puzzle[9][9] = {
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0}
};

uint8_t solution[9][9] = {
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0}
};

void main(void) {
    uint8_t i;
    uint8_t j;

    int difficulty;

    srandom(rtc_Time());

    difficulty = EASY;
    /*generate_puzzle(difficulty);
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (puzzle[i][j] == 0) {
                puzzle[i][j] |= UNDEFINED;
            }
        }
    }*/

    gfx_Begin();
    gfx_SetPalette(sudoku_palette, sizeof_sudoku_palette, 0);
    fontlib_SetFont(calvetica, 0);
    gfx_SetDrawBuffer();
    /*game_loop();*/
    menu_loop();
    gfx_End();
}

