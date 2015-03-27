#include "builtins.h"

#include "linked_list.h"
#include "stack.h"

#include <sys/stat.h>
#include <sys/param.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "alias.h"
#include "bg_jobs.h"
#include "history.h"
#include "job.h"
#include "parse.h"
#include "variables.h"

#define UNUSED(x) (void)(x)

/* Function prototypes. */
static char *cd(int, char **);
static void history_wrapper(int, char **);
static int dirs(int, char **);
static int popd(int, char **);
static int pushd(int, char **);
static void which(int, char **);
static bool which_is_there(const char *);
static int which_print_matches(char *, const char *);

/* Global variables */
struct Stack *directory_stack;

void
builtins_init()
{
  aliases_init();
  jobs_init();
  directory_stack = stack_init();
  variables_init();
  history_init();
}

/* Print helpful information. */
void
help(int command)
{
  if (command == ALIAS) {
    alias_help();
  } else if (command == BG) {
    bg_help();
  } else if (command == CD) {
    printf("usage: cd <directory>\n\n"
           "change the working directory to <directory>.\n"
           "<directory> can be an absolute or relative path.\n");
  } else if (command == DIRS) {
    printf("usage: dirs\n\n"
           "list directories on the directory stack.\n");
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
  } else if (command == POPD) {
  } else if (command == UNALIAS) {
    unalias_help();
  } else if (command == WHICH) {
   printf("usage: which program ...\n");
 }
}

/* Returns an enum of type BuiltinCommands. */
enum BuiltinCommands
is_builtin_command(const char * cmd) {
  if (strncmp(cmd, "alias", strlen("alias")) == 0) return ALIAS;
  if (strncmp(cmd, "bg", strlen("bg")) == 0) return BG;
  if (strncmp(cmd, "cd", strlen("cd")) == 0) return CD;
  if (strncmp(cmd, "dirs", strlen("dirs")) == 0) return DIRS;
  if (strncmp(cmd, "exit", strlen("exit")) == 0) return EXIT;
  if (strncmp(cmd, "fg", strlen("fg")) == 0) return FG;
  if (strncmp(cmd, "help", strlen("help")) == 0) return HELP;
  if (strncmp(cmd, "history", strlen("history")) == 0) return HISTORY;
  if (strncmp(cmd, "jobs", strlen("jobs")) == 0) return JOBS;
  if (strncmp(cmd, "kill", strlen("kill")) == 0) return KILL;
  if (strncmp(cmd, "popd", strlen("popd")) == 0) return POPD;
  if (strncmp(cmd, "pushd", strlen("pushd")) == 0) return PUSHD;
  if (strncmp(cmd, "unalias", strlen("unalias")) == 0) return UNALIAS;
  if (strncmp(cmd, "which", strlen("which")) == 0) return WHICH;

  return NO_SUCH_BUILTIN;
}

void
execute_builtin_command(const enum BuiltinCommands command, process cmd)
{
  if (command == ALIAS) {
    alias(cmd.argc, cmd.argv);
  } else if (command == CD) {
    cd(cmd.argc, cmd.argv);
  } else if (command == HELP) {
    int command = HELP;
    if (cmd.argv[1]) command = is_builtin_command(cmd.argv[1]);
    help(command);
  } else if (command == DIRS) {
    dirs(cmd.argc, cmd.argv);
  } else if (command == EXIT) {
    if (!has_bg_jobs()) {
      exit(EXIT_SUCCESS);
    }
    printf("There are background processes.\n");
  } else if (command == HISTORY) {
    history_wrapper(cmd.argc, cmd.argv);
  } else if (command == JOBS) {
    print_running_jobs();
  } else if (command == KILL) {
    pid_t pid = strtol(cmd.argv[1], (char **)NULL, 10);
    kill(pid, SIGKILL);
  } else if (command == POPD) {
    popd(cmd.argc, cmd.argv);
  } else if (command == PUSHD) {
    pushd(cmd.argc, cmd.argv);
  } else if (command == UNALIAS) {
    unalias(cmd.argc, cmd.argv);
  } else if (command == WHICH) {
    which(cmd.argc, cmd.argv);
  }
}


static char *
cd(const int argc, char **argv)
{
  size_t len = 1024;
  char *dir = malloc(len);

  if (argc == 0) {
    dir = getenv("HOME");
  } else if (strncmp(argv[0], "~", strlen("~")) == 0) {
    dir = getenv("HOME");
  } else if (strncmp(argv[0], "-", strlen("-")) == 0) {
    dir = bsh_getenv("OLDPWD");
  } else {
    strncpy(dir, argv[0], sizeof(strlen(argv[0])));
  }

  char *cwd = malloc(len);
  if (!getcwd(cwd, len)) {
    fprintf(stderr, "Error. Could not obtain current working directory.\n");
  }
  bsh_setenv("OLDPWD", cwd, 1);

  int ret = chdir(dir);
  if (ret != 0) {
    fprintf(stderr, "-bsh: cd: %s: No such file or directory\n", dir);
  }
  return dir;
}

static void
history_wrapper(const int argc, char **argv)
{
  if (argc == 2) {  // set the history size
    if (strncmp(argv[0], "-s", strlen("-s")) == 0) {
      int hist_size = strtol(argv[1], (char **)NULL, 10);
      history_stifle(hist_size);
    } else {
      help(HISTORY);
    }
   } else if (argc == 1) {  // return the last num commands
    int num = strtol(argv[0], (char **)NULL, history_length);
    history_print(num);
  } else {  // print the entire history
    history_print(history_length);
  }
}

static void
which(const int argc, char **argv)
{
  char *p = getenv("PATH");
  size_t pathlen = strlen(p) + 1;
  char *path = malloc(pathlen);
  for (int i = 0; i < argc; i++) {
    memcpy(path, p, pathlen);
    if (strlen(argv[i]) >= FILE_MAX_SIZE) continue;
    which_print_matches(path, argv[i]);
  }
}

static bool
which_is_there(const char *candidate)
{
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
which_print_matches(char *path, const char *filename)
{
  if (strchr(filename, '/')) {
    return which_is_there(filename) ? 0 : -1;
  }

  const char *d;
  char candidate[PATH_MAX];
  bool found = false;
  while ((d = strsep(&path, ":")) != NULL) {
    if (*d == '\0') {
      d = ".";
    }
    if (snprintf(candidate, sizeof(candidate), "%s/%s", d,
        filename) >= (int)sizeof(candidate)) {
          continue;
    }
    if (which_is_there(candidate)) {
      found = true;
      break;
    }
  }
  return (found ? 0 : -1);
}

static int
dirs(const int argc, char** argv)
{
  UNUSED(argc);
  UNUSED(argv);

  size_t len = 1024;
  char *cwd = malloc(len);
  if (!getcwd(cwd, len)) {
    fprintf(stderr, "Error. Could not obtain current working directory.\n");
    return -1;
  }
  printf("%s", cwd);
  int size = stack_size(directory_stack);
  for (int i = 0; i < size; i++) {
    char *dir = stack_get(directory_stack, i);
    printf(" %s", dir);
  }
  printf("\n");
  return 0;
}

static int
popd(const int argc, char** argv)
{
  UNUSED(argc);

  if (stack_empty(directory_stack)) {
    printf("-bsh: popd: directory stack empty\n");
    return -1;
  }

  char *dir = (char *)stack_pop(directory_stack);
  const int new_argc = 1;
  argv[0] = dir;
  cd(new_argc, argv);
  free(dir);
  return 0;
}

static int
pushd(const int argc, char** argv)
{
  if (argc < 1) {
    printf("-bsh: pushd: no other directory\n");
    return -1;
  }

  size_t len = 1024;
  char *cwd = malloc(len);
  if (!getcwd(cwd, len)) {
    fprintf(stderr, "Error. Could not obtain current working directory.\n");
  }
  stack_push(directory_stack, cwd);
  cd(argc, argv);
  return 0;
}
