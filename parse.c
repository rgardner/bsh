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
  *p = (parseInfo){ .hasInputRedirection = FALSE,
                    .hasOutputRedirection = FALSE,
                    .runInBackground = FALSE,
                    .pipeNum = 0,
                    .inFile = "",
                    .outFile = "" };
}

void init_command(commandType *p) {
  *p = (commandType){ .command = malloc(MAXLINE*sizeof(char)),
                      .VarList = NULL,
                      .VarNum = 0};
}

void parse_command(char *command, commandType *comm) {
  /*assert(FALSE);*/
}

parseInfo* parse(char *cmdline) {
  parseInfo *Result;
  int i = 0;
  int com_pos = -1;
  int infile_pos = 0;
  int outfile_pos = 0;

  // Is cmdline empty?
  if (cmdline[-1] == '\n' && cmdline[-1] == '\0') return NULL;

  // Skip blank characters at the start of the cmdline
  for (; isspace(cmdline[i]) && cmdline[i] != '\n' && cmdline[i] != '\0'; i++);
  if (cmdline[i] == '\n' && cmdline[i] == '\0') return NULL;

  Result = malloc(sizeof(parseInfo));
  init_info(Result);
  com_pos = 0;

  BOOL finished_command = FALSE;
  BOOL finished_argument = FALSE;
  BOOL finished_infile = FALSE;
  BOOL finished_outfile = FALSE;

  commandType *Command = malloc(sizeof(commandType));
  init_command(Command);
  for (; cmdline[i] != '\n' && cmdline[i] != '\0'; i++) {
    if (!finished_command) {
      if (isspace(cmdline[i])) {
        finished_command = TRUE;
        Command->command[com_pos] = '\0';
        Result->CommArray[Result->pipeNum] = *Command;
      } else {
        Command->command[com_pos] = cmdline[i];
        com_pos++;
      }
    } else {
      // Skip blank spaces.
      if (isspace(cmdline[i]) && finished_argument) continue;
    }
    /*// command1 < infile | command > outfile &*/
    /*if (!finished_command) {*/
      /*if (isspace(cmdline[i])) {*/
        /*finished_command = TRUE;*/
      /*} else {*/
        /*command[com_pos] = cmdline[i];*/
        /*com_pos++;*/
      /*}*/
    /*} else if (!finished_infile && Result->hasInputRedirection) {*/
      /*if (isspace(cmdline[i])) {*/
        /*if (strcmp(Result->inFile, "") == 0) continue;*/
        /*finished_infile = TRUE;*/
      /*} else {*/
        /*Result->inFile[infile_pos] = cmdline[i];*/
        /*infile_pos++;*/
      /*}*/
    /*} else if (!finished_outfile && Result->hasOutputRedirection) {*/
      /*if (isspace(cmdline[i])) {*/
        /*if (strcmp(Result->outFile, "") == 0) continue;*/
        /*finished_outfile = TRUE;*/
      /*} else {*/
        /*Result->outFile[outfile_pos] = cmdline[i];*/
        /*outfile_pos++;*/
      /*}*/
    /*} else {*/
      /*if (cmdline[i] == '<') {*/
        /*Result->hasInputRedirection = TRUE;*/
      /*} else if (cmdline[i] == '>') {*/
        /*Result->hasOutputRedirection = TRUE;*/
      /*} else if (cmdline[i] == '&') {*/
        /*Result->runInBackground = TRUE;*/
      /*} else if (isspace(cmdline[i])) {*/
        /*// end argument*/
      /*} else {*/
        /*// argument*/
      /*}*/
    /*}*/
  }
  Command->command[com_pos] = '\0';
  Result->CommArray[Result->pipeNum] = *Command;
  Result->pipeNum++;

  if (Result->hasInputRedirection) Result->inFile[infile_pos] = '\0';
  if (Result->hasOutputRedirection) Result->outFile[outfile_pos] = '\0';

  Result->pipeNum = 1;
  parse_command(Result->CommArray[0].command, &Result->CommArray[0]);
  return Result;
}

void print_info(parseInfo *info) {
  for (int i = 0; i < info->pipeNum; i++) {
    printf("prog: %s\n", info->CommArray[i].command);
    for (int j = 0; j < info->CommArray[i].VarNum; j++) {
      printf("arg%d: %s\n", j, info->CommArray[i].VarList[j]);
    }
  }

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
