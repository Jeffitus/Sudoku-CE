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

uint8_t basic_menu(char title[], char *options[], uint8_t quit_option) {
    uint8_t i;
    uint8_t selected;
    uint24_t center;
    bool prevkey;

    selected = 0;
    prevkey = false;

    do {
        gfx_FillScreen(WHITE);
        gfx_SetColor(BLACK);

        set_color(BLACK);

        center = center_cursor_x(title);

        set_cursor(center, 4);
        draw_string(title);

        gfx_HorizLine_NoClip(center, 33, get_string_width(title));

        gfx_SetColor(BLUE);
        center = center_cursor_x(options[selected]);
        gfx_FillCircle_NoClip(center, selected * 30 + 50, 13);
        gfx_FillRectangle_NoClip(center, selected * 30 + 37, get_string_width(options[selected]), 27);
        gfx_FillCircle_NoClip(center + get_string_width(options[selected]), selected * 30 + 50, 13);

        for (i = 0; i < quit_option + 1; i++) {
            set_cursor(center_cursor_x(options[i]), i * 30 + 35);
            if (i == selected) {
                set_color(WHITE);
                draw_string(options[i]);
            } else {
                set_color(BLACK);
                draw_string(options[i]);
            }
        }

        kb_Scan();

        if (kb_Data[7] && !prevkey) {
            switch (kb_Data[7]) {
                case kb_Up:
                    selected--;
                    if (selected > quit_option + 1) {
                        selected = quit_option;
                    }
                    break;
                case kb_Down:
                    selected++;
                    if (selected == quit_option + 1) {
                        selected = 0;
                    }
                    break;
                default:
                    break;
            }
        }

        if (kb_Data[6] & kb_Clear) {
            selected = quit_option;
        }

        prevkey = kb_Data[7];

        gfx_BlitBuffer();        
    } while (!(kb_Data[6] & kb_Clear || kb_Data[1] & kb_2nd));
    while (kb_Data[6] & kb_Clear || kb_Data[1] & kb_2nd) {
        kb_Scan();
    }
    return selected;
}

uint24_t center_cursor_x(char string[]) {
    return 160 - get_string_width(string) / 2;
}

void main_menu(void) {
    uint8_t i;
    uint8_t j;
    char title[] = "Sudoku CE";
    char *options[] = {
        "Play",
        "About",
        "Quit"
    };
    char generate_message[] = "Generating Puzzle...";
    uint8_t difficulty;
    bool quit;
    quit = false;

    do {
        switch (basic_menu(title, options, 2)) {
            case 0:
                difficulty = difficulty_select();
                if (!difficulty) {
                    break;
                } else {
                    gfx_FillScreen(WHITE);
                    set_cursor(center_cursor_x(generate_message), 8);
                    draw_string(generate_message);
                    gfx_BlitBuffer();
                    /* Reset puzzle to zeroes */
                    for (i = 0; i < 9; i++) {
                        for (j = 0; j < 9; j++) {
                            puzzle[i][j] = 0;
                        }
                    }
                    generate_puzzle(difficulty);
                    game_loop();
                    break;
                }
                game_loop();
                break;
            case 1:
                about_screen();
                break;
            case 2:
                quit = true;
                break;
        }
    } while (!quit);
}

void about_screen(void) {
    gfx_FillScreen(WHITE);
    set_color(BLACK);
    set_cursor(10, 8);
    draw_string("Sudoku CE was made by");
    set_cursor(10, 31);
    draw_string("Jeffitus, with some help");
    set_cursor(10, 54);
    draw_string("from MateoConLechuga and");
    set_cursor(10, 77);
    draw_string("jacobly.");
    set_cursor(10, 100);
    draw_string("Use the arrow keys and");
    set_cursor(10, 123);
    draw_string("numpad to fill in the grid,");
    set_cursor(10, 146);
    draw_string("[graph] to toggle pencil");
    set_cursor(10, 169);
    draw_string("mode, and [clear] to quit.");
    set_color(BLUE);
    set_cursor(10, 211);
    draw_string("https://ceme.tech/t16261");
    gfx_BlitBuffer();
    wait_for_key_press();
    wait_for_key_release();
}

uint8_t difficulty_select(void) {
    char title[] = "Select a Difficulty";
    char *options[] = {
        "Easy",
        "Medium",
        "Hard",
        "Very Hard",
        "Back"
    };

    uint8_t difficulty;
    difficulty = 0;

    switch (basic_menu(title, options, 4)) {
        case 0:
            difficulty = EASY;
            break;
        case 1:
            difficulty = MEDIUM;
            break;
        case 2:
            difficulty = HARD;
            break;
        case 3:
            difficulty = VERY_HARD;
            break;
        case 4:
            difficulty = 0;
            break;
    }
    return difficulty;
}

void wait_for_key_press(void) {
    while (!kb_AnyKey());
}

void wait_for_key_release(void) {
    while (kb_AnyKey());
}