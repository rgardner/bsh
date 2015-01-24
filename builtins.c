#include "builtins.h"

#include <sys/stat.h>
#include <sys/param.h>

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "background_jobs.h"
#include "env.h"
#include "history.h"
#include "parse.h"

/* Function prototypes. */
void history_wrapper();
void which(int, char **);
static int print_matches(char *, char *);

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
  } else if (command == PRINTENV) {
    bsh_printenv_help();
  } else if (command == SETENV) {
    bsh_setenv_help();
 } else if (command == WHICH) {
   printf("usage: which program ...\n");
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
  if (strncmp(cmd, "printenv", strlen("printenv")) == 0) return PRINTENV;
  if (strncmp(cmd, "setenv", strlen("setenv")) == 0) return SETENV;
  if (strncmp(cmd, "which", strlen("which")) == 0) return WHICH;

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
  } else if (command == HISTORY) {
    history_wrapper(cmd.VarList[0], cmd.VarList[1]);
  } else if (command == JOBS) {
    print_running_jobs();
  } else if (command == KILL) {
    pid_t pid = strtol(cmd.VarList[0], (char **)NULL, 10);
    kill(pid, SIGKILL);
  } else if (command == WHICH) {
    which(cmd.VarNum, cmd.VarList);
  }
}

void history_wrapper(char *arg1, char *arg2) {
  if (arg1 && arg2) {  // set the history_size
    if (strncmp(arg1, "-s", strlen("-s")) == 0) {
      int hist_size = strtol(arg2, (char **)NULL, 10);
      history_stifle(hist_size);
    } else {
      help(HISTORY);
    }
   } else if (arg1) {  // return the last num commands
    int num = strtol(arg1, (char **)NULL, history_length);
    history_print(num);
  } else {  // print the entire history
    history_print(history_length);
  }
}

void which(int argc, char **argv) {
  char *p, *path;
  ssize_t pathlen;

  p = getenv("PATH");
  pathlen = strlen(p) + 1;
  path = malloc(pathlen);
  while (argc > 0) {
    memcpy(path, p, pathlen);
    if (strlen(*argv) >= FILE_MAX_SIZE) continue;
    print_matches(path, *argv);
    argv++;
    argc--;
  }
}

static bool
is_there(char *candidate) {
  struct stat fin;

  /* XXX work around access(2) false positives for superuser */
  if (access(candidate, X_OK) == 0 &&
      stat(candidate, &fin) == 0 &&
      S_ISREG(fin.st_mode) &&
      (getuid() != 0 ||
      (fin.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) != 0)) {
        printf("%s\n", candidate);
        return true;
  }
  return false;
}

static int
print_matches(char *path, char *filename) {
  char candidate[PATH_MAX];
  const char *d;
  bool found;

  if (strchr(filename, '/')) {
    return is_there(filename) ? 0 : -1;
  }
  found = false;
  while ((d = strsep(&path, ":")) != NULL) {
    if (*d == '\0') {
      d = ".";
    }
    if (snprintf(candidate, sizeof(candidate), "%s/%s", d,
        filename) >= (int)sizeof(candidate)) {
          continue;
    }
    if (is_there(candidate)) {
      found = true;
      break;
    }
  }
  return (found ? 0 : -1);
}
