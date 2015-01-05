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

int copy_substring(char *, char *, int);

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

  // Is cmdline empty?
  if (cmdline[-1] == '\n' && cmdline[-1] == '\0') return NULL;

  // Skip blank characters at the start of the cmdline
  for (; isspace(cmdline[i]) && cmdline[i] != '\n' && cmdline[i] != '\0'; i++);
  if (cmdline[i] == '\n' && cmdline[i] == '\0') return NULL;

  Result = malloc(sizeof(parseInfo));
  init_info(Result);

  commandType *Command = malloc(sizeof(commandType));
  init_command(Command);
  for (; cmdline[i] != '\n' && cmdline[i] != '\0'; i++) {
    // command1 < infile | command > outfile &
    if (strcmp(Command->command, "") == 0) {
      i = copy_substring(Command->command, cmdline, i);
    } else if (Result->hasInputRedirection && strcmp(Result->inFile, "") == 0) {
      i = copy_substring(Result->inFile, cmdline, i);
    } else if (Result->hasOutputRedirection && strcmp(Result->outFile, "") == 0) {
      i = copy_substring(Result->outFile, cmdline, i);
    } else {
      if (cmdline[i] == '<') {
        Result->hasInputRedirection = TRUE;
      } else if (cmdline[i] == '>') {
        Result->hasOutputRedirection = TRUE;
      } else if (cmdline[i] == '&') {
        Result->runInBackground = TRUE;
      } else if (cmdline[i] == '|') {
          Result->CommArray[Result->pipeNum] = *Command;
          Result->pipeNum++;
          Command = malloc(sizeof(commandType));
          init_command(Command);
      } else if (!isspace(cmdline[i])) {
        char *arg = malloc(MAXLINE * sizeof(char));
        i = copy_substring(arg, cmdline, i);
        Command->VarList[Command->VarNum] = arg;
        Command->VarNum++;
      }
    }
  }
  Result->CommArray[Result->pipeNum] = *Command;
  Result->pipeNum++;

  parse_command(Result->CommArray[0].command, &Result->CommArray[0]);
  return Result;
}

/**
 * Copy characters from src to dest until space, newline, or null-byte
 * is encountered. Returns the index of the last valid character found.
 */
int copy_substring(char *dest, char *src, int begin) {
  int end = begin;
  for (; !isspace(src[end]) && src[end] != '\n' && src[end] != '\0'; end++);
  strncpy(dest, src + begin, end - begin);
  dest[end] = '\0';
  return end;
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
