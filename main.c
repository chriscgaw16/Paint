#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "commands.h"
#include "com_args.h"
#include "canvas.h"

// Purpose: To allow for easy drawing of a Canvas on the terminal

int main(int argc, char** argv){
    int row_count = 10;
    int col_count = 10;
    if(argc != 1 && argc != 3){
        // Check if right number of command line arguments are entered
        printf("Wrong number of command line arguments entered.\n");
        printf("Usage: ./paint.out [num_rows num_cols]\n");
        printf("Making default board of 10 X 10.\n");
    }else if(argc == 3){
        // Check if valid integer is read
        row_count = atoi(argv[1]);
        col_count = atoi(argv[2]);
        // Check if row is valid
        if(row_count < 1){
            printf("The number of rows is less than 1.\n");
            printf("Making default board of 10 X 10.\n");
            row_count = 10;
            col_count = 10;
        }
        // Check if column is valid
        if(col_count < 1){
            printf("The number of columns is less than 1.\n");
            printf("Making default board of 10 X 10.\n");
            col_count = 10;
            row_count = 10;
        }
    }
    // Make new canvas and list of commands
    Canvas curr = make_canvas(row_count, col_count);
    // Reads until stopper is hit represented by null character \0
    Command coms[] = {{'h', help}, {'l', load}, {'q', quit}, {'e', erase}, {'w', write}, 
    {'s', save}, {'a', add}, {'d', delete}, {'r', resize}, {'\0', NULL}};
    while(1){
        bool is_valid = false;
        do{
            print_canvas(curr);
            printf("Enter your command: ");
            is_valid = execute_valid_command(coms, &curr);
        }while(!is_valid);
    }
    return 0;
}