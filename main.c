#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <readline/readline.h>
#include <unistd.h>

#include "builtins.h"
#include "background_jobs.h"
#include "history.h"

#define MAX_PROMPT_LENGTH 1024
#define UNUSED(x) (void)(x)

char *buildPrompt() {
  char *prompt = malloc(MAX_PROMPT_LENGTH*sizeof(char));
  char cwd[1024];
  if (!getcwd(cwd, sizeof(cwd))) {
    fprintf(stderr, "Error. Could not obtain current working directory.");
  }
  snprintf(prompt, MAX_PROMPT_LENGTH, "%s$ ", cwd);
  return prompt;
}

void print_login_message()
{
  printf("                __\n");
  printf("    ___        |  \"---.\n");
  printf("  .\"   \". -o)  |      |\n");
  printf("<|_______|/`   |__,---;\n");
  printf("  UU   UU ====|_______|\n");
  printf("Welcome to Bob shell.\n");
}

void execute_command(parse_info_t *info, command_t cmd) {
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
  char *args[cmd.VarNum+2];  // command, *args, NULL
  args[0] = cmd.command;
  for (int i = 0; i < cmd.VarNum; i++) {
    args[i + 1] = cmd.VarList[i];
  }
  args[cmd.VarNum+1] = NULL;
  execvp(args[0], args);
}

int main(int argc, char **argv) {
  // Ignore argc and argv until we add command line arguments.
  UNUSED(argc);
  UNUSED(argv);

  /* Initialization. */
  num_bg_jobs = 0;
  pid_t child_pid;
  char *cmdLine;
  parse_info_t *info;   // all the information returned by parser.
  command_t *cmd;  // command name and Arg list for one command.
  history_init();

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

#ifdef UNIX
    cmdLine = readline(buildPrompt());
    if (!cmdLine) {
      fprintf(stderr, "Unable to read command\n");
      continue;
    }
#endif

    // check command line length
    if (strlen(cmdLine) > MAXLINE) {
      fprintf(stderr, "The command you entered is too long.\n");
      free(cmdLine);
      continue;
    }

    // Look up in history.
    char *expansion;
    int result = history_exp(cmdLine, &expansion);
    if (result < 0 || result == 2) {  // error or should not execute.
      free(expansion);
      continue;
    }
    history_add(expansion);

    // Copy expansion into cmdLine.
    int length = strlen(expansion) + 1;
    cmdLine = realloc(cmdLine, sizeof(char) * length);
    strncpy(cmdLine, expansion, length);
    free(expansion);

    //calls the parser
    info = parse(cmdLine);
    if (!info) {
      free(cmdLine);
      continue;
    }
    //prints the info struct
#ifdef DEBUG
    print_info(info);
#endif

    //com contains the info. of the command before the first "|"
    cmd=&info->CommArray[0];
    if (!cmd  || !cmd->command) {
      free_info(info);
      free(cmdLine);
      continue;
    }

    //com->command tells the command name of com
    int command;
    if ((command = is_builtin_command(cmd->command)) != 0) {
      execute_builtin_command(command, *cmd);
    } else {
      if ((child_pid = fork()) == 0) {
        execute_command(info, *cmd);
      } else {
        if (is_bg_job(info)) {
          bg_job_t *job = malloc(sizeof(bg_job_t));
          job->pid = child_pid;
          job->info = info;
          job->cmd = cmd;
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
