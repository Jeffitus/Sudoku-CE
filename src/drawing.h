void draw_three_boxes(uint8_t pos);
void draw_grid(void);
bool draw_puzzle(void);
void draw_pencils(uint8_t row, uint8_t col);
void draw_timer(uint24_t timer_count);
/* neccessary because the font is modified to be smaller, so any case of 'y' have to be handled specially :/ 
   basically just does fontlib_DrawString but changes cursor position for 'y' <laziness>I will only add 'g' when I need to use it eventually</laziness> */
void draw_string_special(char string[]);