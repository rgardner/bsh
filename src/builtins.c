#include "builtins.h"

#include <err.h>
#include <errno.h>
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
static int cd(int, char**);
static int history_wrapper(int, char**);
static int history_stifle_wrapper(const char*);
static int kill_wrapper(int, char**);
static int dirs(int, char**);
static int popd(int, char**);
static int pushd(int, char**);
static int which(int, char**);
static bool which_is_there(const char*);
static int which_print_matches(char*, const char*);
static int cd_try_save_cwd(const char* const);

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
      "Display the list of currently remembered directories. Directories are "
      "added to the stack with the `pushd` command and removed from the stack "
      "using the `popd` command.\n");
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
    warnx("There are background processes.\n");
  } else if (command == HISTORY) {
    history_wrapper(cmd.argc, cmd.argv);
  } else if (command == JOBS) {
    print_running_jobs();
  } else if (command == KILL) {
    kill_wrapper(cmd.argc, cmd.argv);
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

static int
cd(const int argc, char** const argv)
{
  char* dir = NULL;
  if (argc == 1 || (strlen(argv[1]) == 1 && *argv[1] == '~')) {
    if (!(dir = getenv("HOME"))) {
      warnx("cd: HOME not set");
      return EXIT_FAILURE;
    }
  } else if (strlen(argv[1]) == 1 && *argv[1] == '-') {
    if (!(dir = getenv("OLDPWD"))) {
      warnx("cd: OLDPWD not set");
      return EXIT_FAILURE;
    }
  } else {
    dir = argv[1];
  }

  if (cd_try_save_cwd(dir) < 0) {
    warn("cd: %s", dir);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

static int
history_wrapper(const int argc, char** const argv)
{
  if (!history_enabled()) {
    if (argc == 3 && strcmp(argv[1], "-s") == 0) {
      return history_stifle_wrapper(argv[2]);
    }

    warnx("history: history is disabled. Enable by unstifling history");
    return EXIT_FAILURE;
  }

  if (argc == 1) {
    history_print_all();
  } else if (argc == 2) {
    errno = 0;
    const long n_last_entries = strtol(argv[1], NULL, 10 /*base*/);
    if (n_last_entries < 0 || (n_last_entries == 0 && errno == EINVAL)) {
      warnx("history: %s: invalid option", argv[1]);
      return EXIT_FAILURE;
    }

    history_print(n_last_entries);
  } else if (argc == 3 && strcmp(argv[1], "-s") == 0) {
    return history_stifle_wrapper(argv[2]);
  } else {
    help(HISTORY);
  }

  return EXIT_SUCCESS;
}

static int
history_stifle_wrapper(const char* const arg)
{
  errno = 0;
  const long hist_capacity = strtol(arg, NULL, 10 /*base*/);
  if (hist_capacity < 0 || (hist_capacity == 0 && errno == EINVAL)) {
    warnx("history: %ld: invalid option", hist_capacity);
    return EXIT_FAILURE;
  }

  history_stifle(hist_capacity);
  return EXIT_SUCCESS;
}

static int
kill_wrapper(const int argc, char** const argv)
{
  if (argc == 1) {
    warnx("usage: kill [pid]");
    return EXIT_FAILURE;
  }

  errno = 0;
  pid_t pid = strtol(argv[1], NULL, 10 /*base*/);
  if (pid < -1 || (pid == 0 && errno == EINVAL)) {
    warnx("kill: %s: invalid option", argv[1]);
    return EXIT_FAILURE;
  }

  if (kill(pid, SIGKILL) < 0) {
    warn("kill: (%d)", pid);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

static int
which(const int argc, char** argv)
{
  if (argc == 1) {
    warnx("usage: which program ...");
    return EXIT_FAILURE;
  }

  char* p = getenv("PATH");
  if (!p) {
    warnx("which: PATH not set");
    return EXIT_FAILURE;
  }

  for (int i = 1; i < argc; i++) {
    char* const path = strdup(p);
    if (strlen(argv[i]) >= FILE_MAX_SIZE) continue;
    which_print_matches(path, argv[i]);
  }

  return EXIT_SUCCESS;
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
dirs(const int argc, char** const argv)
{
  UNUSED(argc);
  UNUSED(argv);

  char* const cwd = getcwd(NULL, 0);
  if (!cwd) {
    warnx("dirs: could not obtain current working directory");
    return EXIT_FAILURE;
  }

  printf("%s", cwd);
  free(cwd);

  const int size = stack_size(directory_stack);
  for (int i = 0; i < size; i++) {
    const char* const dir = stack_get(directory_stack, i);
    printf(" %s", dir);
  }

  printf("\n");
  return EXIT_SUCCESS;
}

static int
popd(const int argc, char** const argv)
{
  UNUSED(argc);
  UNUSED(argv);

  char* const dir = stack_pop(directory_stack);
  if (!dir) {
    warnx("popd: directory stack empty");
    return EXIT_FAILURE;
  }

  const bool succeeded = cd_try_save_cwd(dir) >= 0;
  if (!succeeded) {
    warnx("popd:");
  }

  free(dir);
  return succeeded ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
pushd(const int argc, char** const argv)
{
  if (argc == 1) {
    warnx("pushd: no other directory");
    return EXIT_FAILURE;
  }

  char* const cwd = getcwd(NULL, 0);
  if (!cwd) {
    warnx("pushd: could not obtain current working directory");
    return EXIT_FAILURE;
  }

  stack_push(directory_stack, cwd);
  if (cd_try_save_cwd(argv[1]) < 0) {
    warnx("pushd: %s", argv[1]);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

static int
cd_try_save_cwd(const char* const dir)
{
  char* const cwd = getcwd(NULL, 0);
  if (chdir(dir) < 0) {
    if (cwd) free(cwd);
    return -1;
  }

  if (cwd) {
    UNUSED(setenv("OLDPWD", cwd, 1));
    free(cwd);
  }

  return 0;
}
