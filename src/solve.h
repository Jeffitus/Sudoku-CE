void shuffle_array(uint8_t array[9]);
bool solve_sudoku(void);
bool find_unassigned_cell(uint8_t *p_row, uint8_t *p_col);
bool used_in_row(uint8_t row, uint8_t num);
bool used_in_col(uint8_t col, uint8_t num);
bool used_in_box(uint8_t start_row, uint8_t start_col, uint8_t num);
bool valid_value(uint8_t row, uint8_t col, uint8_t num);