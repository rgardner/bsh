/**************************************************************************
 *    parse.c - Parsing portion of my small shell
 *    Syntax:   myshell command1 [< infile] [| command]* [> outfile] [&]
 *************************************************************************/

#include "parse.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int copy_substring(char *, char *, int, int);

bool is_bg_job(parse_info_t *info) {
  return info->runInBackground == true;
}

void init_info(parse_info_t *p) {
  *p = (parse_info_t){ .hasInputRedirection = false,
                    .hasOutputRedirection = false,
                    .runInBackground = false,
                    .pipeNum = 0,
                    .inFile = "",
                    .outFile = "" };
}

void init_command(command_t *p) {
  *p = (command_t){ .command = malloc(MAXLINE*sizeof(char)),
                      .VarList = NULL,
                      .VarNum = 0};
}

void parse_command(char *command, command_t *comm) {
  /*assert(FALSE);*/
}

parse_info_t* parse(char *cmdline) {
  parse_info_t *Result;
  int i = 0;

  // Check if this is a valid string.
  if (cmdline[-1] == '\n' && cmdline[-1] == '\0') return NULL;

  // Ensure string is nonempty.
  if (cmdline[0] == '\0') return NULL;

  // Skip blank characters at the start of the cmdline
  for (; isspace(cmdline[i]) && cmdline[i] != '\n' && cmdline[i] != '\0'; i++);
  if (cmdline[i] == '\n' && cmdline[i] == '\0') return NULL;

  Result = malloc(sizeof(parse_info_t));
  init_info(Result);

  command_t *Command = malloc(sizeof(command_t));
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
        Command = malloc(sizeof(command_t));
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

void print_info(parse_info_t *info) {
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

void free_info(parse_info_t *info) {
  if (!info) return;
  for (int i = 0; i < info->pipeNum; i++) {
    command_t cmd = info->CommArray[i];
    if (cmd.command) free(cmd.command);
    for (int j = 0; j < cmd.VarNum; j++) {
      free(cmd.VarList[j]);
    }
  }
  free(info);
}
