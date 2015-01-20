#ifndef BUILTINS_H
#define BUILTINS_H
#include "parse.h"

enum BuiltinCommands { NO_SUCH_BUILTIN=0, EXIT, CD, KILL, HISTORY, HELP,
                       JOBS, BG, FG };

/* Print help about a specific builtin command. */
void help(int command);

/* Return the enum that corresponds to cmd. */
int is_builtin_command(char *cmd);

void execute_builtin_command(int command, command_t cmd);
#endif
