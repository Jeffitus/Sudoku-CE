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

uint8_t menu_loop(void) {
    uint8_t i;
    uint8_t selected;
    char *options[] = {
        "Play",
        "About",
        "Quit"
    };

    selected = 0;
    do {
        gfx_FillScreen(WHITE);
        for (i = 0; i < 3; i++) {
            fontlib_SetCursorPosition(2, i * 20 + 2);
            draw_string_special(options[i]);
        }
        gfx_BlitBuffer();
    } while (!os_GetCSC());
    return 0;
}

