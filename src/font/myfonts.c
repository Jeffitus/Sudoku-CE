#include <stddef.h>
#include <fontlibc.h>

#include "myfonts.h"

static const uint8_t calvetica_data[] = {
    #include "calvetica.inc"
};

const fontlib_font_t *calvetica = (fontlib_font_t *)calvetica_data;