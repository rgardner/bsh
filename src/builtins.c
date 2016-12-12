#include "builtins.h"

#include <err.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "alias.h"
#include "bg_jobs.h"
#include "history.h"
#include "job.h"
#include "linked_list.h"
#include "parse.h"
#include "stack.h"
#include "util.h"

// Function prototypes.
static void cd(int, char**);
static void history_wrapper(int, char**);
static void history_stifle_wrapper(const char*);
static int dirs(int, char**);
static int popd(int, char**);
static int pushd(int, char**);
static void which(int, char**);
static bool which_is_there(const char*);
static int which_print_matches(char*, const char*);

// Global variables
struct Stack* directory_stack;

void
builtins_init(const size_t history_capacity)
{
  aliases_init();
  jobs_init();
  directory_stack = stack_init();
  history_init(history_capacity);
}

/** Print helpful information. */
void
help(const enum BuiltinCommands cmd)
{
  if (cmd == ALIAS) {
    alias_help();
  } else if (cmd == BG) {
    bg_help();
  } else if (cmd == CD) {
    printf(
      "usage: cd [dir]\n\n"
      "Change the working directory to DIR. The default DIR is the value of "
      "the HOME shell variable.\n\n"
      "DIR can be an absolute or relative path.\n");
  } else if (cmd == DIRS) {
    printf(
      "usage: dirs\n\n"
      "list directories on the directory stack.\n");
  } else if (cmd == EXIT) {
    printf(
      "usage: exit\n\n"
      "terminate the shell process unless there are background processes.\n");
  } else if (cmd == FG) {
    fg_help();
  } else if (cmd == HELP) {
    printf(
      "bsh: a simple alternative to every other shell.\n"
      "usage: bsh\n\n"
      "builtin cmds: bg, cd, exit, fg, help, history, jobs, kill\n"
      "use `help <cmd>` to learn more about a specific cmd.\n");
  } else if (cmd == HISTORY) {
    history_help();
  } else if (cmd == JOBS) {
    jobs_help();
  } else if (cmd == KILL) {
    printf(
      "usage: kill %%num\n\n"
      "terminate the process numbered `num` in the list of background "
      "processes return by `jobs` (by sending it SIGKILL).\n");
  } else if (cmd == POPD) {
  } else if (cmd == UNALIAS) {
    unalias_help();
  } else if (cmd == WHICH) {
    printf("usage: which program ...\n");
  }
}

/* Returns an enum of type BuiltinCommands. */
enum BuiltinCommands
is_builtin_command(const char* cmd)
{
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
    pid_t pid = strtol(cmd.argv[1], (char**)NULL, 10);
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

static void
cd(const int argc, char** argv)
{
  char* dir = NULL;

  if (argc == 1 || (strlen(argv[1]) == 1 && *argv[1] == '~')) {
    if (!(dir = getenv("HOME"))) {
      warnx("cd: HOME not set");
      return;
    }
  } else if (strlen(argv[1]) == 1 && *argv[1] == '-') {
    if (!(dir = getenv("OLDPWD"))) {
      warnx("cd: OLDPWD not set");
      return;
    }
  } else {
    dir = argv[1];
  }

  // ignore failure, cwd is optional
  char* cwd = getcwd(NULL, 0);

  if (chdir(dir) < 0) {
    warn("cd: %s", dir);
    return;
  }

  if (cwd) {
    UNUSED(setenv("OLDPWD", cwd, 1));
    free(cwd);
  }
}

static void
history_wrapper(const int argc, char** argv)
{
  if (!history_enabled()) {
    if (argc == 3 && strncmp(argv[1], "-s", strlen("-s")) == 0) {
      history_stifle_wrapper(argv[2]);
      return;
    }

    fprintf(stderr, "-bsh: history is disabled. Enable by unstifling history.\n");
    return;
  }

  if (argc == 1) {
    history_print_all();
  } else if (argc == 2) {
    const long n_last_entries = strtol(argv[1], NULL, 10 /*base*/);
    if (n_last_entries < 0) {
      fprintf(stderr, "-bsh: history: %ld: invalid option\n", n_last_entries);
      return;
    }

    history_print(n_last_entries);
  } else if (argc == 3 && strncmp(argv[1], "-s", strlen("-s")) == 0) {
    history_stifle_wrapper(argv[2]);
  } else {
    help(HISTORY);
  }
}

static void
history_stifle_wrapper(const char* arg)
{
  const long hist_capacity = strtol(arg, NULL, 10 /*base*/);
  if (hist_capacity < 0) {
    fprintf(stderr, "-bsh: history: %ld: invalid option\n", hist_capacity);
    return;
  }

  history_stifle(hist_capacity);
}

static void
which(const int argc, char** argv)
{
  if (argc == 0) {
    printf("usage: which program ...\n");
    return;
  }
  char* p = getenv("PATH");
  if (!p) {
    fprintf(stderr, "-bsh: which: PATH not set\n");
    return;
  }
  size_t pathlen = strlen(p) + 1;
  char* path = malloc(pathlen);
  for (int i = 1; i < argc; i++) {
    memcpy(path, p, pathlen);
    if (strlen(argv[i]) >= FILE_MAX_SIZE) continue;
    which_print_matches(path, argv[i]);
  }
}

static bool
which_is_there(const char* candidate)
{
  struct stat fin;

  /* XXX work around access(2) false positives for superuser */
  if (access(candidate, X_OK) == 0 && stat(candidate, &fin) == 0 &&
      S_ISREG(fin.st_mode) &&
      (getuid() != 0 || (fin.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) != 0)) {
    printf("%s\n", candidate);
    return true;
  }
  return false;
}

static int
which_print_matches(char* path, const char* filename)
{
  if (strchr(filename, '/')) {
    return which_is_there(filename) ? 0 : -1;
  }

  const char* d;
  char candidate[PATH_MAX];
  bool found = false;
  while ((d = strsep(&path, ":")) != NULL) {
    if (*d == '\0') {
      d = ".";
    }
    if (snprintf(candidate, sizeof(candidate), "%s/%s", d, filename) >=
        (int)sizeof(candidate)) {
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
  char* cwd = malloc(len);
  if (!getcwd(cwd, len)) {
    fprintf(stderr, "Error. Could not obtain current working directory.\n");
    free(cwd);
    return -1;
  }
  printf("%s", cwd);
  int size = stack_size(directory_stack);
  for (int i = 0; i < size; i++) {
    char* dir = stack_get(directory_stack, i);
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

  char* dir = stack_pop(directory_stack);
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
  char* cwd = malloc(len);
  if (!getcwd(cwd, len)) {
    fprintf(stderr, "Error. Could not obtain current working directory.\n");
  } else {
    stack_push(directory_stack, cwd);
  }
  cd(argc, argv);
  free(cwd);
  return 0;
}
