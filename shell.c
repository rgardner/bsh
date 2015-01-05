#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "parse.h"

void handle_builtin_command(int);

enum BuiltinCommands { NO_SUCH_BUILTIN=0, EXIT, JOBS, CD, KILL, HISTORY, HELP };

char *buildPrompt() {
  return  "%: ";
}

int isBuiltInCommand(char * cmd) {
  if (strncmp(cmd, "cd", strlen("cd")) == 0) return CD;
  if (strncmp(cmd, "help", strlen("help")) == 0) return HELP;
  if (strncmp(cmd, "exit", strlen("exit")) == 0) return EXIT;
  if (strncmp(cmd, "jobs", strlen("jobs")) == 0) return JOBS;
  if (strncmp(cmd, "kill", strlen("kill")) == 0) return KILL;
  if (strncmp(cmd, "history", strlen("history")) == 0) return HISTORY;

  return NO_SUCH_BUILTIN;
}


int main(int argc, char **argv) {
  char *cmdLine;
  parseInfo *info;   // all the information returned by parser.
  commandType *com;  // command name and Arg list for one command.

#ifdef UNIX
    fprintf(stdout, "This is the UNIX version\n");
#endif

#ifdef WINDOWS
    fprintf(stdout, "This is the WINDOWS version\n");
#endif

  while (TRUE) {
    //insert your code to print prompt here.

#ifdef UNIX
    cmdLine = readline(buildPrompt());
    if (cmdLine == NULL) {
      fprintf(stderr, "Unable to read command\n");
      continue;
    }

    // check command line length
    if (strlen(cmdLine) > MAXLINE) {
      fprintf(stderr, "The command you entered is too long.");
      free(cmdLine);
      continue;
    }
#endif

    //insert your code about history and !x !-x here

    //calls the parser
    info = parse(cmdLine);
    if (info == NULL){
      free(cmdLine);
      continue;
    }
    //prints the info struct
    print_info(info);

    //com contains the info. of the command before the first "|"
    com=&info->CommArray[0];
    if ((com == NULL)  || (com->command == NULL)) {
      free_info(info);
      free(cmdLine);
      continue;
    }

    //com->command tells the command name of com
    int command;
    if ((command = isBuiltInCommand(com->command)) != 0) {
      handle_builtin_command(command);
    }
    //insert your code here.

    free_info(info);
    free(cmdLine);
  }
}

void handle_builtin_command(int command) {
  if (command == EXIT) {
    exit(0);
  }
}
