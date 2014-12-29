/**************************************************************************
 *    parse.c - Parsing portion of my small shell
 *    Syntax:   myshell command1 [< infile] [| command]* [> outfile] [&]
 *************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

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
  int inFile_pos = 0;
  int outFile_pos = 0;

  if (cmdline[-1] == '\n' && cmdline[-1] == '\0') {
    return NULL;
  }

  Result = malloc(sizeof(parseInfo));
  init_info(Result);
  com_pos = 0;

  // skip blank characters at the start of the cmdline
  for (; cmdline[i] == ' ' && cmdline[i] != '\n' && cmdline[i] != '\0'; i++);

  BOOL finished_command = FALSE;
  BOOL finished_infile = FALSE;
  BOOL finished_outfile = FALSE;
  for (int i = 0; cmdline[i] != '\n' && cmdline[i] != '\0'; i++) {
    // command1 < infile | command > outfile &
    if (!finished_command) {
      if (cmdline[i] == ' ') {
        finished_command = TRUE;
        continue;
      }
      command[com_pos] = cmdline[i];
      com_pos++;
    } else if (!finished_infile && Result->hasInputRedirection) {
      if (cmdline[i] == ' ') {
        finished_infile = TRUE;
        continue;
      }
      Result->inFile[inFile_pos] = cmdline[i];
      inFile_pos++;
    } else if (!finished_outfile && Result->hasOutputRedirection) {
      if (cmdline[i] == ' ') {
        finished_outfile = TRUE;
        continue;
      }
      Result->outFile[outFile_pos] = cmdline[i];
      outFile_pos++;
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
  if (Result->hasInputRedirection) Result->inFile[inFile_pos] = '\0';
  if (Result->hasOutputRedirection) Result->outFile[outFile_pos] = '\0';

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
  /*assert(FALSE);*/
}
