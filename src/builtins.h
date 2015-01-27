#ifndef BUILTINS_H
#define BUILTINS_H
#include "linked_list.h"
#include "stack.h"
#include "parse.h"

enum BuiltinCommands { NO_SUCH_BUILTIN=0, ALIAS, BG, CD, DIRS, EXIT, FG, HELP,
                       HISTORY, JOBS, KILL, POPD, PRINTENV, PUSHD, SETENV,
                       WHICH };

/* Bsh aliases. */
extern struct LinkedList *aliases;

/* Directories saved for `dirs`, `pushd`, and `popd`. */
extern struct Stack *directory_stack;


/* Print help about a specific builtin command. */
void
help(int command);

/* Return the enum that corresponds to cmd. */
int
is_builtin_command(char *cmd);

void
execute_builtin_command(int command, struct Command cmd);
#endif
