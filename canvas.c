#include "canvas.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/*
    Purpose: Makes an empty canvas
    Input: Number of rows and columns
    Output: Newly created empty canvas
*/
Canvas make_canvas(int row_count, int col_count){
    Canvas new_canvas = {row_count, col_count, NULL};
    char** new_data = calloc(row_count, sizeof(char*));
    for(int i = 0; i < row_count; i++){
        new_data[i] = calloc(col_count, sizeof(char*));
        for(int j = 0; j < col_count; j++){
            new_data[i][j] = '*';
        }
    }
    new_canvas.data = new_data;
    return new_canvas;
}

/*
    Purpose: Deletes / frees canvas data
    Input: The canvas
    Output: Nothing
*/
void free_canvas(Canvas c){
    for(int i = 0; i < c.row_count; i++){
        free(c.data[i]);
    }
    free(c.data);
}

/*
    Purpose: Prints canvas inputted
    Input: A canvas
    Output: Nothing, but prints canvas on standard output
*/
void print_canvas(Canvas c){
    for(int i = c.row_count - 1; i >= 0; i--){
        printf("%d ", i);
        for(int j = 0; j < c.col_count; j++){
            printf("%c ", c.data[i][j]);
        }
        printf("\n");
    }
    printf("  ");
    for(int j = 0; j < c.col_count; j++){
        printf("%d ", j);
    }
    printf("\n");
}

/*
    Purpose: Checks if a certain point is on a board in the canvas
    Input: A canvas, a row attempt, a column attempt
    Output: A boolean representing if the attempted access is in the board
*/
bool point_in_board(Canvas c, int row_attempt, int col_attempt){
    return row_attempt >= 0 && col_attempt >= 0 && row_attempt < c.row_count && col_attempt < c.col_count;
}

/*
    Purpose: Takes a number and normalizes it. 1 if poitive, -1 if negative and 0 if 0
    Input: A number
    Output: 1, -1, or 0
*/
int normalize(int num){
    if(num > 0){
        return 1;
    }else if(num < 0){
        return -1;
    }else{
        return 0;
    }
}

/*
    Purpose: Makes a new line
    Input: Coords (x1, y1) and Coords (x2, y2)
    Output: New line starting from coord 1 to coord 2
*/
Line make_line(int x1, int y1, int x2, int y2){
    Line new = {x1, y1, x2, y2, -normalize(x1 - x2), -normalize(y1 - y2), INVALID};
    if(new.x_diff == 0){
        new.dir = ROW;
    }else if(new.y_diff == 0){
        new.dir = COLUMN;
    }else if(abs(x1 - x2) == abs(y1 - y2)){
        if((x1 - x2) / (y1 - y2) > 0){
            new.dir = RIGHT_DIAGONAL;
        }else{
            new.dir = LEFT_DIAGONAL;
        }
    }
    return new;
}

/*
    Purpose: Takes in a direction and returns the corresponding char
    Input: A direction d
    Output: A char corresponding to the direction d
*/
char get_dir_char(Direction d){
    switch(d){
        case ROW:
            return '-';
        case COLUMN:
            return '|';
        case RIGHT_DIAGONAL:
            return '/';
        case LEFT_DIAGONAL:
            return '\\';
        default:
            printf("Invalid direction passed into get_dir_char");
            exit(0);
    }
}

/*
    Purpose: Draws a line l on canvas c
    Input: A canvas and a line
    Output: None
*/
void draw_line(Line l, Canvas c){
    char line_char = get_dir_char(l.dir);
    int x1 = l.x1;
    int y1 = l.y1;
    while(x1 != l.x2 || y1 != l.y2){
        if(c.data[x1][y1] != '*' && c.data[x1][y1] != line_char){
            c.data[x1][y1] = '+';
        }else{
            c.data[x1][y1] = line_char;
        }
        x1 += l.x_diff;
        y1 += l.y_diff;
    };
    if(c.data[x1][y1] != '*' && c.data[x1][y1] != line_char){
        c.data[x1][y1] = '+';
    }else{
        c.data[x1][y1] = line_char;
    }
}

/*
    Purpose: Adds a row at position to canvas c
    Input: A canvas and a position
    Output: None
*/
void add_row(Canvas *c, int position){
    (c->row_count)++;
    c->data = realloc(c->data, (c->row_count) * sizeof(char*));
    for(int i = (c->row_count - 2); i >= position; i--){
        (c->data)[i+1] = (c->data)[i];
    }
    (c->data)[position] = calloc(c->col_count, sizeof(char));
    for(int i = 0; i < c->col_count; i++){
        (c->data)[position][i] = '*';
    }
}

/*
    Purpose: Adds a column at position to canvas c
    Input: A canvas and a position
    Output: None
*/
void add_col(Canvas *c, int position){
    for(int i = 0; i < c->row_count; i++){
        (c->data)[i] = realloc((c->data)[i], (c->col_count + 1) * sizeof(char));
        for(int j = (c->col_count - 1); j >= position; j--){
            (c->data)[i][j+1] = (c->data)[i][j];
        }
        (c->data)[i][position] = '*';
    }
    (c->col_count)++;
}

/*
    Purpose: Deletes a row at position to canvas c
    Input: A canvas and a position
    Output: None
*/
void delete_row(Canvas *c, int position){
    free((c->data)[position]);
    for(int i = position; i < c->row_count - 1; i++){
        (c->data)[i] = (c->data)[i+1];
    }
    (c->row_count)--;
    c->data = realloc(c->data, (c->row_count) * sizeof(char*));
}

/*
    Purpose: Deletes a column at position to canvas c
    Input: A canvas and a position
    Output: None
*/
void delete_col(Canvas *c, int position){
    for(int i = 0; i < c->row_count; i++){
        for(int j = position; j < c->col_count - 1; j++){
            (c->data)[i][j] = (c->data)[i][j+1];
        }
        (c->data)[i] = realloc((c->data)[i], (c->col_count - 1) * sizeof(char));
    }
    (c->col_count)--;
}
