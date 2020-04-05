void draw_three_boxes(uint8_t pos);
void draw_grid(void);
bool draw_puzzle(void);
void draw_pencils(uint8_t row, uint8_t col);
void draw_timer(uint24_t timer_count);
/* Neccessary because the font is modified to be smaller, so any case like 'y' has to be handled specially :/ 
   basically just does fontlib_DrawString but changes cursor position for 'y', etc. Right now only a few characters are implemented. */
void draw_string(char string[]);