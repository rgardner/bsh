#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "background_jobs.h"

#define MAX_PROMPT_LENGTH 1024

enum BuiltinCommands { NO_SUCH_BUILTIN=0, EXIT, JOBS, CD, KILL, HISTORY, HELP };

char *buildPrompt() {
  char *prompt = malloc(MAX_PROMPT_LENGTH*sizeof(char));
  char cwd[1024];
  if (getcwd(cwd, sizeof(cwd)) == NULL) {
    fprintf(stderr, "Error. Could not obtain current working directory.");
  }
  snprintf(prompt, MAX_PROMPT_LENGTH, "%s$ ", cwd);
  return prompt;
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

void execute_builtin_command(int command, commandType cmd) {
  if (command == EXIT) {
    // check to see if there are background jobs.
    exit(EXIT_SUCCESS);
  } else if (command == CD) {
    char *path = cmd.VarList[0];
    chdir(path);
  } else if (command == JOBS) {
    check_bg_jobs();
  } else if (command == KILL) {
    pid_t pid = strtol(cmd.VarList[0], (char **)NULL, 10);
    kill(pid, SIGKILL);
  }
}

void execute_command(parseInfo *info, commandType cmd) {
  // setup file input/output redirection.
  if (info->hasInputRedirection) {
    int fd = open(info->inFile, O_RDONLY);
    dup2(fd, fileno(stdin));
  }
  if (info->hasOutputRedirection) {
    FILE *f = fopen(info->outFile, "w");
    if (f == NULL) {
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
  /* Initialization. */
  num_bg_jobs = 0;
  pid_t child_pid;
  char *cmdLine;
  parseInfo *info;   // all the information returned by parser.
  commandType *cmd;  // command name and Arg list for one command.

/*  if (signal(SIGCHLD, handle_sigchld) == SIG_ERR) {*/
    /*perror("An error occurred while setting the SIGCHLD signal handler.");*/
  /*}*/

#ifdef UNIX
  printf("This is the UNIX version.\n");
#elif WINDOWS
  printf("This is the WINDOWS version.\n");
#endif

  while (true) {

#ifdef UNIX
    cmdLine = readline(buildPrompt());
    if (cmdLine == NULL) {
      fprintf(stderr, "Unable to read command\n");
      continue;
    }
#endif

    // check command line length
    if (strlen(cmdLine) > MAXLINE) {
      fprintf(stderr, "The command you entered is too long.");
      free(cmdLine);
      continue;
    }

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
    cmd=&info->CommArray[0];
    if ((cmd == NULL)  || (cmd->command == NULL)) {
      free_info(info);
      free(cmdLine);
      continue;
    }

    //com->command tells the command name of com
    int command;
    if ((command = isBuiltInCommand(cmd->command)) != 0) {
      execute_builtin_command(command, *cmd);
    } else {
      if ((child_pid = fork()) == 0) {
        execute_command(info, *cmd);
      } else {
        if (isBackgroundJob(info)) {
          BackgroundJob *job = malloc(sizeof(BackgroundJob));
          job->cmd = cmd;
          job->pid = child_pid;
          background_jobs[num_bg_jobs] = job;
          num_bg_jobs++;
          printf("[%d] %d\n", num_bg_jobs, child_pid);
        } else {
          int status;
          waitpid(child_pid, &status, 0);
        }
      }
    }
    check_bg_jobs();

    if (!isBackgroundJob(info)) free_info(info);
    free(cmdLine);
  }
}
