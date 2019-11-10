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

void draw_puzzle(int grid_size, int puzzle[9][9]);

int puzzle1[9][9] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1,
};

int i;
int j;
int k;

void main(void) {
    gfx_Begin();
    draw_puzzle(235, puzzle1);
    while (!os_GetCSC());
    gfx_End();
}

void draw_puzzle(int grid_size, int puzzle[9][9]) {
	gfx_SetColor(0);
	for (i = 0; i < grid_size; i+=((grid_size-1)/9)) {
        gfx_VertLine(i, 0, grid_size);
        gfx_HorizLine(0, i, grid_size);
    }
    for (j=0; j<9; j++) {
        for (k=0; k<9; k++) {
        	gfx_SetTextXY(((j*grid_size)/9)+1, ((k*grid_size)/9)+1);
        	gfx_PrintInt(puzzle[j][k], 1);
        }
    }
}
