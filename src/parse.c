/** Parser implementation.
 *
 *  @file
 *
 *  Syntax: myshell command1 [< infile] [| command]* [> outfile] [&]
 */

#include "parse.h"

#include <err.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

/* Function prototypes. */
size_t
copy_substring(char*, const char*, int, int);

void free_command(const struct Command* command) {
  if (!command) return;
  free(command->command);
  for (int i = 0; i < command->VarNum; i++) {
    free(command->VarList[i]);
  }
}

void
init_info(struct ParseInfo* p)
{
  *p = (struct ParseInfo){ .hasInputRedirection = false,
                           .hasOutputRedirection = false,
                           .runInBackground = false,
                           .pipeNum = 0,
                           .inFile = "",
                           .outFile = "" };
}

struct ParseInfo*
parse(const char* cmdline)
{
  const size_t cmdline_len = strlen(cmdline);

  // Ensure cmdline is nonempty
  if (cmdline_len == 0) return NULL;

  // Skip blank characters at the start of the cmdline
  size_t i = 0;
  for (; i < cmdline_len && isspace(cmdline[i]) && cmdline[i] != '\n'; i++)
    ;

  // After removing blanks, is the string now empty?
  if (i == cmdline_len || cmdline[i] == '\n') return NULL;

  struct ParseInfo* result = malloc(sizeof(struct ParseInfo));
  if (!result) goto error;
  init_info(result);

  struct Command cmd = { .command = NULL,
                         .VarList = { NULL },
                         .VarNum = 0 };

  for (; i < cmdline_len && cmdline[i] != '\n'; i++) {
    // command1 < infile | command > outfile &
    if (isspace(cmdline[i])) continue;

    if (!cmd.command) {
      cmd.command = malloc(MAXLINE * sizeof(char));
      if (!cmd.command) goto error;

      i = copy_substring(cmd.command, cmdline, i, MAXLINE);
      if (i == SIZE_MAX) {
        fprintf(stderr,
                "Error. The command exceeds the %d character limit.\n",
                MAXLINE);
        goto error;
      }
    } else if (result->hasInputRedirection && result->inFile[0] == '\0') {
      i = copy_substring(result->inFile, cmdline, i, FILE_MAX_SIZE);
      if (i == SIZE_MAX) {
        fprintf(stderr,
                "Error. The input redirection filename exceeds the "
                "%d character limit.\n",
                FILE_MAX_SIZE);
        goto error;
      }
    } else if (result->hasOutputRedirection && result->outFile[0] == '\0') {
      i = copy_substring(result->outFile, cmdline, i, FILE_MAX_SIZE);
      if (i == SIZE_MAX) {
        fprintf(stderr,
                "Error. The output redirection filename exceeds the "
                "%d character limit.\n",
                FILE_MAX_SIZE);
        goto error;
      }
    } else {
      if (cmdline[i] == '<') {
        result->hasInputRedirection = true;
      } else if (cmdline[i] == '>') {
        result->hasOutputRedirection = true;
      } else if (cmdline[i] == '&') {
        result->runInBackground = true;
        break;  // '&' should be the last character
      } else if (cmdline[i] == '|') {
        result->CommArray[result->pipeNum] = cmd;
        result->pipeNum++;

        // Reset cmd
        cmd.command = NULL;
        memset(cmd.VarList, 0, sizeof(cmd.VarList));
        cmd.VarNum = 0;
      } else if (!isspace(cmdline[i])) {
        char* arg = malloc(MAXLINE * sizeof(char));
        if (!arg) goto error;

        i = copy_substring(arg, cmdline, i, MAXLINE);
        if (i == SIZE_MAX) {
          fprintf(stderr,
                  "Error. The variable exceeds the %d character limit.\n",
                  MAXLINE);
          free(arg);
          goto error;
        }
        cmd.VarList[cmd.VarNum] = arg;
        cmd.VarNum++;
      }
    }
  }

  result->CommArray[result->pipeNum] = cmd;
  return result;

error:
  free_command(&cmd);
  free_info(result);
  return NULL;
}

/**
 * Copy characters from src to dest until space, newline, or null-byte
 * is encountered. Returns the index of the last valid character found. Returns
 * -1 if the length of the new string is greater than the supplied limit.
 */
size_t
copy_substring(char* dest, const char* src, const int begin, const int limit)
{
  int end = begin;
  for (; !isspace(src[end]) && src[end] != '\n' && src[end] != '\0'; end++)
    ;

  if (end - begin > limit)
    return SIZE_MAX;  // length of string to copy too large

  strncpy(dest, src + begin, end - begin);
  dest[end] = '\0';
  return end - 1;
}

void
print_info(const struct ParseInfo* info)
{
  if (!info) return;
  for (int i = 0; i <= info->pipeNum; i++) {
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

void
free_info(const struct ParseInfo* info)
{
  if (!info) return;

  for (int i = 0; i <= info->pipeNum; i++) {
    free_command(&(info->CommArray[i]));
  }

  free((void*)info);
}
