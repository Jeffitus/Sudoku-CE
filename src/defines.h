#define PLAYING_GRID_SIZE 235

#define PUZZLE_X 2
#define PUZZLE_Y 2
#define CELL_SIZE (PLAYING_GRID_SIZE - 1) / 9 - 1

#define UNDEFINED (1 << 23)
#define PENCIL_MARKS (511 << 14)
#define PENCIL_MARK(n) (1 << (13 + n))
#define SOLUTION_DATA 240
#define SOLUTION(n) ((n & SOLUTION_DATA) >> 4)
/*For cases where want just the value or just the data*/
#define DATA (UNDEFINED & PENCIL_MARKS & SOLUTION_DATA)
#define VALUE 15

#define EASY 40
#define MEDIUM 30
#define HARD 25
#define VERY_HARD 20

#define BLUE 0
#define BLACK 1
#define WHITE 2

#define ONE_SECOND 32768

extern uint24_t puzzle[9][9];