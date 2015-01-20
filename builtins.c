#include "builtins.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "background_jobs.h"
#include "history.h"
#include "parse.h"


/* Print helpful information. */
void help(int command) {
  if (command == BG) {
    bg_help();
  } else if (command == CD) {
    printf("usage: cd <directory>\n\n"
           "change the working directory to <directory>.\n"
           "<directory> can be an absolute or relative path.\n");
  } else if (command == EXIT) {
    printf("usage: exit\n\n"
           "terminate the shell process unless there are background processes.\n");
  } else if (command == FG) {
    fg_help();
  } else if (command == HELP) {
    printf("bsh: a simple alternative to every other shell.\n"
           "usage: bsh\n\n"
           "builtin commands: bg, cd, exit, fg, help, history, jobs, kill\n"
           "use `help <command>` to learn more about a specific command.\n");
  } else if (command == HISTORY) {
    history_help();
  } else if (command == JOBS) {
    jobs_help();
  } else if (command == KILL) {
    printf("usage: kill %%num\n\n"
           "terminate the process numbered `num` in the list of background "
           "processes return by `jobs` (by sending it SIGKILL).\n");
  }
}

int is_builtin_command(char * cmd) {
  if (strncmp(cmd, "bg", strlen("bg")) == 0) return BG;
  if (strncmp(cmd, "cd", strlen("cd")) == 0) return CD;
  if (strncmp(cmd, "exit", strlen("exit")) == 0) return EXIT;
  if (strncmp(cmd, "fg", strlen("fg")) == 0) return FG;
  if (strncmp(cmd, "help", strlen("help")) == 0) return HELP;
  if (strncmp(cmd, "history", strlen("history")) == 0) return HISTORY;
  if (strncmp(cmd, "jobs", strlen("jobs")) == 0) return JOBS;
  if (strncmp(cmd, "kill", strlen("kill")) == 0) return KILL;

  return NO_SUCH_BUILTIN;
}

void execute_builtin_command(int command, command_t cmd) {
  if (command == CD) {
    char *path = cmd.VarList[0];
    chdir(path);
  } else if (command == HELP) {
    int command = HELP;
    if (cmd.VarList[0]) command = is_builtin_command(cmd.VarList[0]);
    help(command);
  } else if (command == EXIT) {
    if (!has_bg_jobs()) {
      exit(EXIT_SUCCESS);
    }
    printf("There are background processes.\n");
  } else if (command == JOBS) {
    print_running_jobs();
  } else if (command == KILL) {
    pid_t pid = strtol(cmd.VarList[0], (char **)NULL, 10);
    kill(pid, SIGKILL);
  } else if (command == HISTORY) {
    if (cmd.VarList[0] && cmd.VarList[1]) {  // set the history_size
      if (strncmp(cmd.VarList[0], "-s", strlen("-s")) == 0) {
        int hist_size = strtol(cmd.VarList[1], (char **)NULL, 10);
        history_stifle(hist_size);
      } else {
        help(HISTORY);
      }
     } else if (cmd.VarList[0]) {  // return the last num commands
      int num = strtol(cmd.VarList[0], (char **)NULL, 10);
      history_print(num);
    } else {  // print the entire history
      history_print(history_length);
    }
  }
}
