#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <readline/readline.h>
#include <termios.h>
#include <unistd.h>

#include "alias.h"
#include "bg_jobs.h"
#include "builtins.h"
#include "history.h"
#include "parse.h"

#define MAX_PROMPT_LENGTH 1024
#define COMMAND_NOT_FOUND_EXIT_CODE 127
#define UNUSED(x) (void)(x)

pid_t shell_pgid;
struct termios shell_tmodes;
int shell_terminal;
int shell_is_interactive;

char *
buildPrompt()
{
  char *prompt = malloc(MAX_PROMPT_LENGTH*sizeof(char));
  char cwd[1024];
  if (!getcwd(cwd, sizeof(cwd))) {
    fprintf(stderr, "Error. Could not obtain current working directory.");
  }
  snprintf(prompt, MAX_PROMPT_LENGTH, "%s$ ", cwd);
  return prompt;
}

void
print_login_message()
{
  printf("                __\n");
  printf("    ___        |  \"---.\n");
  printf("  .\"   \". -o)  |      |\n");
  printf("<|_______|/`   |__,---;\n");
  printf("  UU   UU ====|_______|\n");
  printf("Welcome to Bob shell.\n");
}

void
init_shell()
{
  /* See if we are running interactively.  */
  shell_terminal = STDIN_FILENO;
  shell_is_interactive = isatty (shell_terminal);

  if (shell_is_interactive) {
    /* Loop until we are in the foreground.  */
    while (tcgetpgrp (shell_terminal) != (shell_pgid = getpgrp ())) {
      kill(-shell_pgid, SIGTTIN);
    }

    /* Ignore interactive and job-control signals.  */
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);

    /* Put ourselves in our own process group.  */
    shell_pgid = getpid ();
    if (setpgid (shell_pgid, shell_pgid) < 0) {
      perror ("Couldn't put the shell in its own process group");
      exit(1);
    }

    /* Grab control of the terminal.  */
    tcsetpgrp(shell_terminal, shell_pgid);

    /* Save default terminal attributes for shell.  */
    tcgetattr(shell_terminal, &shell_tmodes);
  }
}

void
launch_process(char **argv, const int infile, const int outfile)
{
  if (infile != STDIN_FILENO) {
    dup2(infile, STDIN_FILENO);
    close(infile);
  }
  if (outfile != STDOUT_FILENO) {
    dup2(outfile, STDOUT_FILENO);
    close(outfile);
  }
  if (execvp(argv[0], argv) == -1) {
    printf("-bsh: %s: command not found\n", argv[0]);
    exit(COMMAND_NOT_FOUND_EXIT_CODE);
  }
}

void
launch_job(const struct ParseInfo *info)
{
  // Currently, bsh does not support pipes with builtin commands.
  const struct Command cmd = info->CommArray[0];
  enum BuiltinCommands command = is_builtin_command(cmd.command);
  if (command != NO_SUCH_BUILTIN) {
    execute_builtin_command(command, cmd);
    return;
  }

  // set up piping
  pid_t pid;
  int infile;
  if (info->hasInputRedirection) {
    infile = open(info->inFile, O_RDONLY);
  } else {
    infile = STDIN_FILENO;
  }
  int pipefd[2];
  for (int i = 0; i < info->pipeNum; i++) {
    int outfile;
    if (i < (info->pipeNum-1)) {
      if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
      }
      outfile = pipefd[1];
    } else {
      if (info->hasOutputRedirection) {
        outfile = open(info->outFile, O_WRONLY);
      } else {
        outfile = STDOUT_FILENO;
      }
    }

    // fork
    if ((pid = fork()) == -1) {
      perror("fork");
      exit(EXIT_FAILURE);
    } else if (pid == 0) {  /* Child process. */
      struct Command cmd = info->CommArray[i];
      char *argv[cmd.VarNum+2];  // command, *args, NULL
      argv[0] = cmd.command;
      for (int i = 0; i < cmd.VarNum; i++) {
        argv[i + 1] = cmd.VarList[i];
      }
      argv[cmd.VarNum+1] = NULL;
      launch_process(argv, infile, outfile);
    } else {                /* Parent process. */

    }
    // clean up pipes
    if (info->hasInputRedirection && (infile != open(info->inFile, O_RDONLY))) {
      close(infile);
    }
    if (info->hasOutputRedirection && (infile != open(info->outFile, O_RDONLY))) {
      close(outfile);
    }
    infile = pipefd[0];
  }
  if (is_bg_job(info)) {
    put_job_in_background(info, pid);
  } else {
    int status;
    waitpid(pid, &status, 0);
  }
}

int
main(int argc, char **argv)
{
  // Ignore argc and argv until we add command line arguments.
  UNUSED(argc);
  UNUSED(argv);

  /* Initialization. */
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

    //calls the parser
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

    //com contains the info. of the command before the first "|"
    const struct Command *cmd=&info->CommArray[0];
    if (!cmd  || !cmd->command) {
      free_info(info);
      free(cmdLine);
      continue;
    }

    launch_job(info);
    if (!is_bg_job(info)) free_info(info);
  }
}
