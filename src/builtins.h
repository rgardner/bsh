#ifndef BUILTINS_H
#define BUILTINS_H
#include "stack.h"
#include "parse.h"

enum BuiltinCommands { NO_SUCH_BUILTIN=0, BG, CD, DIRS, EXIT, FG, HELP, HISTORY,
                       JOBS, KILL, POPD, PRINTENV, PUSHD, SETENV, WHICH };

extern struct stack *directory_stack;

/* Print help about a specific builtin command. */
void help(int command);

/* Return the enum that corresponds to cmd. */
int is_builtin_command(char *cmd);

void execute_builtin_command(int command, command_t cmd);
#endif