#include <stdio.h>
#include <stdbool.h>
#include "com_args.h"

/*
    Purpose: Gets a valid int arg from a command-line argument
    Input: Pointer to integer to put valid inputted value and the command-line argument arg
    Output: Puts valid int into var and returns bool representing if a valid arg was read
*/
bool get_valid_int_arg(int* var, const char* arg){
    int new_int;
    char byte_after = ' ';
    int num_args_read = sscanf(arg, "%d%c", &new_int, &byte_after);
    if(num_args_read == 1 && byte_after == ' '){
        *var = new_int;
        return true;
    }else{
        return false;
    }
}

/*
    Purpose: Gets a valid char arg from a command-line argument
    Input: Pointer to char to put valid inputted value and the command-line argument arg
    Output: Puts valid char into var and returns bool representing if a valid arg was read
*/
bool get_valid_char_arg(char* var, const char* arg){
    char new_char;
    char byte_after = ' ';
    int num_args_read = sscanf(arg, "%c%c", &new_char, &byte_after);
    if(num_args_read == 1 && byte_after == ' '){
        *var = new_char;
        return true;
    }else{
        return false;
    }
}

