/**************************************************************************
 *    parse.c - Parsing portion of my small shell
 *    Syntax:   myshell command1 [< infile] [| command]* [> outfile] [&]
 *************************************************************************/

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"

void init_info(parseInfo *p) {
  *p = (parseInfo){ FALSE, FALSE, FALSE, NULL, "", "", "" };
}

void parse_command(char *command, commandType *comm) {
  /*assert(FALSE);*/
}

parseInfo* parse(char *cmdline) {
  parseInfo *Result;
  char command[MAXLINE];
  int i = 0;
  int com_pos = -1;
  int infile_pos = 0;
  int outfile_pos = 0;

  if (cmdline[-1] == '\n' && cmdline[-1] == '\0') {
    return NULL;
  }

  Result = malloc(sizeof(parseInfo));
  init_info(Result);
  com_pos = 0;

  // skip blank characters at the start of the cmdline
  for (; isspace(cmdline[i]) && cmdline[i] != '\n' && cmdline[i] != '\0'; i++);

  BOOL finished_command = FALSE;
  BOOL finished_infile = FALSE;
  BOOL finished_outfile = FALSE;
  for (int i = 0; cmdline[i] != '\n' && cmdline[i] != '\0'; i++) {
    // command1 < infile | command > outfile &
    if (!finished_command) {
      if (isspace(cmdline[i])) {
        finished_command = TRUE;
        continue;
      }
      command[com_pos] = cmdline[i];
      com_pos++;
    } else if (!finished_infile && Result->hasInputRedirection) {
      if (isspace(cmdline[i])) {
        if (strcmp(Result->inFile, "") == 0) continue;
        finished_infile = TRUE;
        continue;
      }
      Result->inFile[infile_pos] = cmdline[i];
      infile_pos++;
    } else if (!finished_outfile && Result->hasOutputRedirection) {
      if (isspace(cmdline[i])) {
        if (strcmp(Result->outFile, "") == 0) continue;
        finished_outfile = TRUE;
        continue;
      }
      Result->outFile[outfile_pos] = cmdline[i];
      outfile_pos++;
    } else {
      if (cmdline[i] == '<') {
        Result->hasInputRedirection = TRUE;
      } else if (cmdline[i] == '>') {
        Result->hasOutputRedirection = TRUE;
      } else if (cmdline[i] == '&') {
        Result->runInBackground = TRUE;
      }
    }
  }

  command[com_pos] = '\0';
  if (Result->hasInputRedirection) Result->inFile[infile_pos] = '\0';
  if (Result->hasOutputRedirection) Result->outFile[outfile_pos] = '\0';

  parse_command(command, 0);
  return Result;
}

void print_info(parseInfo *info) {
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
  free(info);
}
