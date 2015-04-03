#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <readline/readline.h>
#include <unistd.h>

#include "alias.h"
#include "bg_jobs.h"
#include "builtins.h"
#include "history.h"
#include "parse.h"

#define MAX_PROMPT_LENGTH 1024
#define COMMAND_NOT_FOUND_EXIT_CODE 127
#define UNUSED(x) (void)(x)

char *buildPrompt() {
  char *prompt = malloc(MAX_PROMPT_LENGTH * sizeof(char));
  char cwd[1024];
  if (!getcwd(cwd, sizeof(cwd))) {
    fprintf(stderr, "Error. Could not obtain current working directory.");
  }
  snprintf(prompt, MAX_PROMPT_LENGTH, "%s$ ", cwd);
  return prompt;
}

void print_login_message() {
  printf("                __\n");
  printf("    ___        |  \"---.\n");
  printf("  .\"   \". -o)  |      |\n");
  printf("<|_______|/`   |__,---;\n");
  printf("  UU   UU ====|_______|\n");
  printf("Welcome to Bob shell.\n");
}

void execute_command(const struct ParseInfo *info, const struct Command cmd) {
  // setup file input/output redirection.
  if (info->hasInputRedirection) {
    int fd = open(info->inFile, O_RDONLY);
    dup2(fd, fileno(stdin));
  }
  if (info->hasOutputRedirection) {
    FILE *f = fopen(info->outFile, "w");
    if (!f) {
      fprintf(stderr, "Error opening file!\n");
      exit(EXIT_FAILURE);
    }
    dup2(fileno(f), fileno(stdout));
  }
  // construct args
  char *args[cmd.VarNum + 2];  // command, *args, NULL
  args[0] = cmd.command;
  for (int i = 0; i < cmd.VarNum; i++) {
    args[i + 1] = cmd.VarList[i];
  }
  args[cmd.VarNum + 1] = NULL;
  if (execvp(args[0], args) == -1) {
    printf("-bsh: %s: command not found\n", args[0]);
    exit(COMMAND_NOT_FOUND_EXIT_CODE);
  }
}

int main(int argc, char **argv) {
  // Ignore argc and argv until we add command line arguments.
  UNUSED(argc);
  UNUSED(argv);

  /* Initialization. */
  num_bg_jobs = 0;
  builtins_init();

  if (signal(SIGCHLD, handle_sigchld) == SIG_ERR) {
    perror("An error occurred while setting the SIGCHLD signal handler.");
  }

#ifdef UNIX
  printf("This is the UNIX version.\n");
#elif WINDOWS
  printf("This is the WINDOWS version.\n");
#endif
  print_login_message();

  while (true) {
    char *cmdLine;
#ifdef UNIX
    cmdLine = readline(buildPrompt());
    if (!cmdLine) {
      printf("\n");
      exit(EXIT_SUCCESS);
    }
#elif WINDOWS
    fprintf(stderr, "ERROR: Windows is currently not supported.\n");
    exit(EXIT_FAILURE);
#endif

    // check command line length
    if (strlen(cmdLine) > MAXLINE) {
      fprintf(stderr, "The command you entered is too long.\n");
      free(cmdLine);
      continue;
    }

    // Look up in history.
    char *expansion;
    const int his_res = history_exp(cmdLine, &expansion);
    if (his_res < 0 || his_res == 2) {  // error or should not execute.
      free(expansion);
      continue;
    }
    history_add(expansion);

    // Copy expansion into cmdLine.
    const int length = strlen(expansion) + 1;
    cmdLine = realloc(cmdLine, sizeof(char) * length);
    strncpy(cmdLine, expansion, length);
    free(expansion);

    // calls the parser
    const struct ParseInfo *info = parse(cmdLine);
    if (!info) {
      free(cmdLine);
      continue;
    }

    // Expand aliases in info commands.
    for (int i = 0; i < info->pipeNum; i++) {
      const struct Command *cmd = &info->CommArray[i];
      char *expansion;
      const int alias_res = alias_exp(cmd->command, &expansion);
      if (alias_res < 0 || alias_res == 2) {
        free(expansion);
        continue;
      }

      // Copy expansion into command.
      const int length = strlen(expansion) + 1;
      char *command = realloc(cmd->command, sizeof(char) * length);
      strncpy(command, expansion, length);
      free(expansion);
    }

#ifdef DEBUG
    print_info(info);
#endif

    // com contains the info. of the command before the first "|"
    const struct Command *cmd = &info->CommArray[0];
    if (!cmd || !cmd->command) {
      free_info(info);
      free(cmdLine);
      continue;
    }

    // com->command tells the command name of com
    enum BuiltinCommands command;
    if ((command = is_builtin_command(cmd->command) != NO_SUCH_BUILTIN)) {
      execute_builtin_command(command, *cmd);
    } else {
      pid_t child_pid;
      if ((child_pid = fork()) == 0) {
        execute_command(info, *cmd);
      } else {
        if (is_bg_job(info)) {
          struct BGJob *job = job_init(child_pid, info, cmd);
          background_jobs[num_bg_jobs] = job;
          num_bg_jobs++;
          printf("[%d] %d\n", num_bg_jobs, child_pid);
        } else {
          int status;
          waitpid(child_pid, &status, 0);
        }
      }
    }

    if (!is_bg_job(info)) free_info(info);
  }
}
