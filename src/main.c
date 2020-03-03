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

#include "defines.h"
#include "drawing.h"
#include "sudoku.h"

uint8_t puzzle[9][9] = {
    {0,2,7,1,5,4,3,9,6},
    {9,6,5,3,2,7,1,4,8},
    {3,4,1,6,8,9,7,5,2},
    {5,9,3,4,6,8,2,7,1},
    {4,7,2,5,1,3,6,8,9},
    {6,1,8,9,7,2,4,3,5},
    {7,8,6,2,3,5,9,1,4},
    {1,5,4,7,9,6,8,2,3},
    {2,3,9,8,4,1,5,6,7}
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

    generate_puzzle(difficulty);
    
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (puzzle[i][j] == 0) {
                puzzle[i][j] |= FIXED_NUM;
            }
        }
    }

    gfx_Begin();
    gfx_SetDrawBuffer();
    game_loop(puzzle);
    gfx_SetTextFGColor(0);
    gfx_PrintStringXY("You Win!", 1, 176);
    gfx_BlitBuffer();
    while (!os_GetCSC());
    gfx_End();
}

