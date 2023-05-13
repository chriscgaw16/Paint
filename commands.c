#include "commands.h"
#include "canvas.h"
#include "com_args.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#define GUARD 42791

/*
    Purpose: Get the input from the user and returns the command and arguments
    Input: Pointer to argc representing the number of arguments
    Output: Returns input broken up into an array of arguments
*/
char** get_argv(int* argc){
    char** argv = NULL;
    int curr_array_length = 0;
    int curr_string_length = 0;
    bool in_word = false;
    char curr = '\n';
    do{
        scanf("%c", &curr);
        if(!isspace(curr) && !in_word){
            // Changes in_word to true since we are inside a word
            in_word = true;
            // Makes space for new string
            argv = realloc(argv, (curr_array_length + 1) * sizeof(char*));
            // Appends current character to newly created string
            argv[curr_array_length] = calloc(1, sizeof(char));
            argv[curr_array_length][curr_string_length] = curr;
            curr_string_length++;
        }else if(!isspace(curr) && in_word){
            // Appends char to current string
            argv[curr_array_length] = realloc(argv[curr_array_length], (curr_string_length + 1) * sizeof(char));
            argv[curr_array_length][curr_string_length] = curr;
            curr_string_length++;
        }else if(isspace(curr) && in_word){
            // Changes in_word to false as we are no longer inside a word
            in_word = false;
            // Adds \0 character to end of current string and resets curr string length
            argv[curr_array_length] = realloc(argv[curr_array_length], (curr_string_length + 1) * sizeof(char));
            argv[curr_array_length][curr_string_length] = '\0';
            curr_string_length = 0;
            // Move to the next entry in the array
            curr_array_length++;
        }
    }while(curr != '\n');
    // Sets current argc to new length
    *argc = curr_array_length;
    return argv;
}

/*
    Purpose: Frees argv
    Input: Argc representing the number of arguments. Argv representing the array of arguments
    Output: None
*/
void free_argv(int argc, char** argv){
    for(int i = 0; i < argc; i++){
        free(argv[i]);
    }
    free(argv);
}

/*
    Purpose: Gets a valid command in the command list and execute the command
    Input: List of available commands and Pointer to a Canvas
    Output: Returns a boolean representing if a valid command was inputted
*/
bool execute_valid_command(Command* coms, Canvas *c){
    // Get argc and argv of command inputted
    int argc = 0;
    char** argv = get_argv(&argc);
    // Check if text is empty
    if(argc <= 0){
        free_argv(argc, argv);
        printf("Unrecognized command. Type h for help.\n");
        return false;
    }
    // Check if name matches any known commands in list
    for(int i = 0; coms[i].name != '\0'; i++){
        char com_name = argv[0][0];
        if(coms[i].name == com_name){
            bool is_valid = (coms[i]).function(argc, argv, c);
            free_argv(argc, argv);
            return is_valid;
        }
    }
    free_argv(argc, argv);
    printf("Unrecognized command. Type h for help.\n");
    return false;
}

/*
    Purpose: To end the program and free any malloced memory, checks for correct argument types
    Input: The argument count, List of arguments as string, and Pointer to a canvas
    Output: Returns a boolean representing if the command arguments were valid
*/
bool quit(int argc, char** argv, Canvas *c){
    if(argv[0][1] != '\0'){
        printf("Unrecognized command. Type h for help.\n");
        return false;
    }
    // Checks if correct amount of arguments
    if(argc > 1){
        printf("Too many arguments.\n");
        return false;
    }
    // Free's any malloced data
    free_canvas(*c);
    free_argv(argc, argv);
    // Ends program successfully (0)
    exit(0);
}

/*
    Purpose: Outputs list of available commands and how to use them, checks for correct argument types
    Input: The argument count, List of arguments as string, and Pointer to a canvas
    Output: Returns a boolean representing if the command arguments were valid
*/
bool help(int argc, char** argv, Canvas *c){
    if(argv[0][1] != '\0'){
        printf("Unrecognized command. Type h for help.\n");
        return false;
    }
    // Checks if correct amount of arguments
    if(argc > 1){
        printf("Too many arguments.\n");
        return false;
    }
    // Prints list of commands
    printf("Commands:\n");
    printf("Help: h\n");
    printf("Quit: q\n");
    printf("Draw line: w row_start col_start row_end col_end\n");
    printf("Resize: r num_rows num_cols\n");
    printf("Add row or column: a [r | c] pos\n");
    printf("Delete row or column: d [r | c] pos\n");
    printf("Erase: e row col\n");
    printf("Save: s file_name\n");
    printf("Load: l file_name\n");
    return true;
}

/*
    Purpose: Erases a tile on the canvas, checks for correct argument types
    Input: The argument count, List of arguments as string, and Pointer to a canvas
    Output: Returns a boolean representing if the command arguments were valid
*/
bool erase(int argc, char** argv, Canvas *c){
    if(argv[0][1] != '\0'){
        printf("Improper erase command.\n");
        return false;
    }
    // Check if correct amount of arguments
    if(argc != 3){
        printf("Improper erase command.\n");
        return false;
    }
    // Initialize row and column
    int row = 0;
    int col = 0;
    int args_read = 0;
    // Check if valid ints and the new point is in the board
    args_read += get_valid_int_arg(&row, argv[1]);
    args_read += get_valid_int_arg(&col, argv[2]);
    if(args_read != 2 || !point_in_board(*c, row, col)){
        printf("Improper erase command.\n");
        return false;
    }
    // Erase specific point with empty character *
    c->data[row][col] = '*';
    return true;
}

/*
    Purpose: Saves the current canvas in a file, checks for correct argument types
    Input: The argument count, List of arguments as string, and Pointer to a canvas
    Output: Returns a boolean representing if the command arguments were valid
*/
bool save(int argc, char** argv, Canvas *c){
    if(argv[0][1] != '\0'){
        printf("Improper save command or file could not be created.\n");
        return false;
    }
    // Check if correct amount of arguments
    if(argc != 2){
        printf("Improper save command or file could not be created.\n");
        return false;
    }
    // Open file
    FILE* fp = fopen(argv[1], "wb");
    // Put in file guard to check if file was made using save command
    int guard = GUARD;
    fwrite(&guard, sizeof(int), 1, fp);
    // Put in number of rows and columns
    fwrite(&(c->row_count), sizeof(int), 1, fp);
    fwrite(&(c->col_count), sizeof(int), 1, fp);
    // Write canvas data as chars
    for(int i = 0; i < c->row_count; i++){
        for(int j = 0; j <c->col_count; j++){
            fwrite(&(c->data[i][j]), sizeof(char), 1, fp);
        }
    }
    fclose(fp);
    return true;
}

/*
    Purpose: Loads a canvas from a file specified, checks for correct argument types
    Input: The argument count, List of arguments as string, and Pointer to a canvas
    Output: Returns a boolean representing if the command arguments were valid
*/
bool load(int argc, char** argv, Canvas *c){
    if(argv[0][1] != '\0'){
        printf("Improper load command or file could not be opened.\n");
        return false;
    }
    // Check if correct amount of arguments
    if(argc != 2){
        printf("Improper load command or file could not be opened.\n");
        return false;
    }
    // Check if file exists
    FILE* fp = fopen(argv[1], "rb");
    if(fp == NULL){
        printf("Improper load command or file could not be opened.\n");
        return false;
    }
    // Check if guard is present
    int guard = 0;
    fread(&guard, sizeof(int), 1, fp);
    if(guard != GUARD){
        printf("Improper load command or file could not be opened.\n");
        return false;
    }
    // Delete old canvas
    free_canvas(*c);
    // Read rows and columns into new_canvas
    int rows = 0;
    int cols = 0;
    fread(&rows, sizeof(int), 1, fp);
    fread(&cols, sizeof(int), 1, fp);
    Canvas new_canvas = make_canvas(rows, cols);
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            fread(&(new_canvas.data[i][j]), sizeof(char), 1, fp);
        }
    }
    // Update canvas to new canvas
    *c = new_canvas;
    fclose(fp);
    return true;
}

/*
    Purpose: Draws a line on the canvas, checks for correct argument types
    Input: The argument count, List of arguments as string, and Pointer to a canvas
    Output: Returns a boolean representing if the command arguments were valid
*/
bool write(int argc, char** argv, Canvas *c){
    if(argv[0][1] != '\0'){
        printf("Improper draw command.\n");
        return false;
    }
    // Check if correct amount of arguments
    if(argc != 5){
        printf("Improper draw command.\n");
        return false;
    }
    // Initialize arguments
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    int args_read = 0;
    // Check if valid integers and if line is valid
    args_read += get_valid_int_arg(&x1, argv[1]);
    args_read += get_valid_int_arg(&y1, argv[2]);
    args_read += get_valid_int_arg(&x2, argv[3]);
    args_read += get_valid_int_arg(&y2, argv[4]);
    Line new = make_line(x1, y1, x2, y2);
    if(new.dir == INVALID || !point_in_board(*c, new.x1, new.y1) || !point_in_board(*c, new.x2, new.y2) || (args_read != 4)){
        printf("Improper draw command.\n");
        return false;
    }
    // Draw valid line onto canvas
    draw_line(new, *c);
    return true;
}

/*
    Purpose: Adds a column or row on the canvas, checks for correct argument types
    Input: The argument count, List of arguments as string, and Pointer to a canvas
    Output: Returns a boolean representing if the command arguments were valid
*/
bool add(int argc, char** argv, Canvas *c){
    if(argv[0][1] != '\0'){
        printf("Improper add command.\n");
        return false;
    }
    if(argc != 3){
        printf("Improper add command.\n");
        return false;
    }
    char type = 0;
    int pos = 0;
    int args_read = 0;
    args_read += get_valid_char_arg(&type, argv[1]);
    args_read += get_valid_int_arg(&pos, argv[2]);
    if(!(type == 'r' || type == 'c') || args_read != 2 || pos < 0){
        printf("Improper add command.\n");
        return false;
    }
    if(type == 'r' && pos > c->row_count){
        printf("Improper add command.\n");
        return false;
    }
    if(type == 'c' && pos > c->col_count){
        printf("Improper add command.\n");
        return false;
    }
    if(type == 'r'){
        add_row(c, pos);
    }else{
        add_col(c, pos);
    }   
    return true;
}

/*
    Purpose: Deletes a row or column on the canvas, checks for correct argument types
    Input: The argument count, List of arguments as string, and Pointer to a canvas
    Output: Returns a boolean representing if the command arguments were valid
*/
bool delete(int argc, char** argv, Canvas *c){
    if(argv[0][1] != '\0'){
        printf("Improper delete command.\n");
        return false;
    }
    // Check if correct number of arguments
    if(argc != 3){
        printf("Improper delete command.\n");
        return false;
    }
    // Initialize type and position
    char type = 0;
    int pos = 0;
    // Check if arguments read are within column or row and that user selected column or row
    int args_read = 0;
    args_read += get_valid_char_arg(&type, argv[1]);
    args_read += get_valid_int_arg(&pos, argv[2]);
    if(!(type == 'r' || type == 'c') || args_read != 2 || pos < 0){
        printf("Improper delete command.\n");
        return false;
    }
    if(type == 'r' && pos >= c->row_count){
        printf("Improper delete command.\n");
        return false;
    }
    if(type == 'c' && pos >= c->col_count){
        printf("Improper delete command.\n");
        return false;
    }
    // Delete row or column in corresponding position
    if(type == 'r'){
        delete_row(c, pos);
    }else{
        delete_col(c, pos);
    }   
    return true;
}

/*
    Purpose: Resizes the canvas, checks for correct argument types
    Input: The argument count, List of arguments as string, and Pointer to a canvas
    Output: Returns a boolean representing if the command arguments were valid
*/
bool resize(int argc, char** argv, Canvas *c){
    if(argv[0][1] != '\0'){
        printf("Improper resize command.\n");
        return false;
    }
    // Check if correct number of arguments
    if(argc != 3){
        printf("Improper resize command.\n");
        return false;
    }
    // Initialize new row size and new column size
    int new_row_size = 1;
    int new_col_size = 1;
    int args_read = 0;
    // Check if valid integers within the row or column
    args_read += get_valid_int_arg(&new_row_size, argv[1]);
    args_read += get_valid_int_arg(&new_col_size, argv[2]);
    if(args_read != 2 || new_row_size < 1 || new_col_size < 1){
        printf("Improper resize command.\n");
        return false;
    }
    // Make a new canvas
    Canvas new_canvas = make_canvas(new_row_size, new_col_size);
    // Copy over data from old canvas to new canvas
    for(int i = 0; i < new_row_size; i++){
        for(int j = 0; j < new_col_size; j++){
            if(point_in_board(*c, i, j)){
                new_canvas.data[i][j] = (c->data)[i][j];
            }
        }
    }
    // Delete old canvas
    free_canvas(*c);
    // Make canvas the new canvas
    *c = new_canvas;
    return true;
}