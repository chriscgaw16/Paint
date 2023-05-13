#ifndef COMMANDS_H
#define COMMANDS_H
#include <stdbool.h>
#include "canvas.h"

typedef struct Command_Def{
    char name;
    bool (*function)(int, char**, Canvas*);
} Command;

bool execute_valid_command(Command* coms, Canvas* c);
void free_argv(int argc, char** argv);

bool quit(int argc, char** argv, Canvas *c);
bool help(int argc, char** argv, Canvas *c);
bool erase(int argc, char** argv, Canvas *c);
bool write(int argc, char** argv, Canvas *c);
bool save(int argc, char** argv, Canvas *c);
bool load(int argc, char** argv, Canvas *c);
bool add(int argc, char** argv, Canvas *c);
bool delete(int argc, char** argv, Canvas *c);
bool resize(int argc, char** argv, Canvas *c);

#endif