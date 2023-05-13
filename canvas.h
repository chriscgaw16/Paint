#ifndef CANVAS_H
#define CANVAS_H
#include "stdbool.h"

typedef struct Canvas_Def{
    int row_count;
    int col_count;
    char** data;
} Canvas;

typedef enum Direction_Def{
    INVALID,
    COLUMN,
    ROW,
    RIGHT_DIAGONAL,
    LEFT_DIAGONAL
} Direction;

typedef struct Line_Def{
    int x1;
    int y1;
    int x2;
    int y2;
    int x_diff;
    int y_diff;
    Direction dir;
} Line;

Canvas make_canvas(int row_count, int col_count);
void free_canvas(Canvas c);
void print_canvas(Canvas c);
bool point_in_board(Canvas c, int row_attempt, int col_attempt);
Line make_line(int x1, int y1, int x2, int y2);
char get_dir_char(Direction d);
void draw_line(Line l, Canvas c);
void add_row(Canvas *c, int position);
void add_col(Canvas *c, int position);
void delete_row(Canvas *c, int position);
void delete_col(Canvas *c, int position);


#endif