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
bits 4-7: solution number (shown if the player gives up)
bits 14-22: contain pencil data
bit 23: set if undefined, cleared if immutable
*/
uint24_t puzzle[9][9] = {
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
/*uint24_t puzzle[9][9] = {0};*/

int main(void) {
    srandom(rtc_Time());
    gfx_Begin();
    gfx_SetPalette(sudoku_palette, sizeof_sudoku_palette, 0);
    gfx_SetTransparentColor(WHITE);
    fontlib_SetFont(calvetica, 0);
    fontlib_SetTransparency(true);
    gfx_SetDrawBuffer();
    main_menu();
    gfx_End();
}