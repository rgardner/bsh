/**************************************************************************
 *    parse.c - Parsing portion of my small shell
 *    Syntax:   myshell command1 [< infile] [| command]* [> outfile] [&]
 *************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "parse.h"

void init_info(parseInfo *p) {
  printf("init_info: initializing parseInfo\n");
  *p = (parseInfo){ FALSE, FALSE, FALSE, NULL, "", "", "" };
}

void parse_command(char *command, commandType *comm) {
  printf("parse_command: parsing a single command\n");
  printf("command: '%s'\n", command);
  assert(FALSE);
}

parseInfo* parse(char *cmdline) {
  parseInfo *Result;
  char command[MAXLINE];
  int com_pos = -1;

  if (cmdline[-1] == '\n' && cmdline[-1] == '\0') {
    return NULL;
  }

  Result = malloc(sizeof(parseInfo));
  init_info(Result);
  com_pos = 0;

  // find command position.
  // refact find command
  // add infile
  // add outfile
  // add background
  // add pipe
  BOOL start_of_command_found = FALSE;
  for (int i = 0; cmdline[i] != '\n' && cmdline[i] != '\0'; i++) {
    // command1 < infile | command > outfile &
    if (!start_of_command_found && cmdline[i] == ' ') continue;
    start_of_command_found = TRUE;
    if (cmdline[i] == ' ') {
      break;
    }
    command[com_pos] = cmdline[i];
    com_pos++;
  }

  command[com_pos] = '\0';
  parse_command(command, 0);
  return Result;
}

void print_info(parseInfo *info) {
  printf("print_info: printing info about parseInfo struct\n");
  /*printf("prog: %s\n", info->);*/
  // iterate through program arguments.
  /*printf("arg%d: %s\n");*/
  if (info->hasInputRedirection) {
    printf("inpipe: %s\n", info->inFile);
  } else {
    printf("inpipe: no\n");
  }

  if (info->hasOutputRedirection) {
    printf("outpipe: %s\n", info->outFile);
  } else {
    printf("outpipe: no\n");
  }
  printf("background: %s\n", (info->runInBackground) ? "yes" : "no");
}

void free_info(parseInfo *info) {
  printf("free_info: freeing memory associated to parseInfo struct\n");
  assert(FALSE);
}
