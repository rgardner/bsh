#ifndef BUILTINS_H
#define BUILTINS_H
#include "job.h"
#include "linked_list.h"
#include "parse.h"
#include "stack.h"

enum BuiltinCommands
{
  NO_SUCH_BUILTIN = 0,
  ALIAS,
  BG,
  CD,
  DIRS,
  EXIT,
  FG,
  HELP,
  HISTORY,
  JOBS,
  KILL,
  POPD,
  PRINTENV,
  PUSHD,
  SETENV,
  UNALIAS,
  UNSET,
  WHICH
};

/* Initialize all data structures for builtin commands. */
void builtins_init();

/* Print help about a specific builtin command. */
void help(const int command);

/* Return the enum that corresponds to cmd. */
enum BuiltinCommands is_builtin_command(const char* cmd);

/* Execute the builtin command in the same process as the caller. */
void execute_builtin_command(const enum BuiltinCommands command, process cmd);
#endif
