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

int copy_substring(char *, char *, int, int);

bool isBackgroundJob(parseInfo *info) {
  return info->runInBackground == true;
}

void init_info(parseInfo *p) {
  *p = (parseInfo){ .hasInputRedirection = false,
                    .hasOutputRedirection = false,
                    .runInBackground = false,
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
      i = copy_substring(Command->command, cmdline, i, MAXLINE);
      if (i == -1) {
        fprintf(stderr,
                "Error. The command exceeds the %d character limit.\n",
                MAXLINE);
        free_info(Result);
        return NULL;
      }
    } else if (Result->hasInputRedirection && strcmp(Result->inFile, "") == 0) {
      i = copy_substring(Result->inFile, cmdline, i, FILE_MAX_SIZE);
      if (i == -1) {
        fprintf(stderr, "Error. The input redirection filename exceeds the " \
                        "%d character limit.\n", FILE_MAX_SIZE);
        free_info(Result);
        return NULL;
      }
    } else if (Result->hasOutputRedirection && strcmp(Result->outFile, "") == 0) {
      i = copy_substring(Result->outFile, cmdline, i, FILE_MAX_SIZE);
      if (i == -1) {
        fprintf(stderr, "Error. The output redirection filename exceeds the " \
                        "%d character limit.\n", FILE_MAX_SIZE);
        free_info(Result);
        return NULL;
      }
    } else {
      if (cmdline[i] == '<') {
        Result->hasInputRedirection = true;
      } else if (cmdline[i] == '>') {
        Result->hasOutputRedirection = true;
      } else if (cmdline[i] == '&') {
        Result->runInBackground = true;
        break;  // '&' should be the last character
      } else if (cmdline[i] == '|') {
        Result->CommArray[Result->pipeNum] = *Command;
        Result->pipeNum++;
        Command = malloc(sizeof(commandType));
        init_command(Command);
      } else if (!isspace(cmdline[i])) {
        char *arg = malloc(MAXLINE * sizeof(char));
        i = copy_substring(arg, cmdline, i, MAXLINE);
        if (i == -1) {
          fprintf(stderr,
                  "Error. The variable exceeds the %d character limit.\n",
                  MAXLINE);
          free_info(Result);
          return NULL;
        }
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
 * is encountered. Returns the index of the last valid character found. Returns
 * -1 if the length of the new string is greater than the supplied limit.
 */
int copy_substring(char *dest, char *src, int begin, int limit) {
  int end = begin;
  for (; !isspace(src[end]) && src[end] != '\n' && src[end] != '\0'; end++);
  if (end - begin > limit) return -1;  // length of string to copy too large
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
  if (info == NULL) return;
  for (int i = 0; i < info->pipeNum; i++) {
    commandType Command = info->CommArray[i];
    if (Command.command != NULL) free(Command.command);
    for (int j = 0; j < Command.VarNum; j++) {
      free(Command.VarList[j]);
    }
  }
  free(info);
}
