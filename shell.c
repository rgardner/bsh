#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <readline/readline.h>

#include "background_jobs.h"
#include "history.h"

#define MAX_PROMPT_LENGTH 1024

enum BuiltinCommands { NO_SUCH_BUILTIN=0, EXIT, JOBS, CD, KILL, HISTORY, HELP };

char *buildPrompt() {
  char *prompt = malloc(MAX_PROMPT_LENGTH*sizeof(char));
  char cwd[1024];
  if (!getcwd(cwd, sizeof(cwd))) {
    fprintf(stderr, "Error. Could not obtain current working directory.");
  }
  snprintf(prompt, MAX_PROMPT_LENGTH, "%s$ ", cwd);
  return prompt;
}

/* Print helpful information. */
void help(int command) {
  if (command == CD) {
    printf("usage: cd <directory>\n\n"
           "change the working directory to <directory>.\n"
           "<directory> can be an absolute or relative path.\n");
  } else if (command == HELP) {
      printf("shell: a simple alternative to every other shell.\n\n"
             "usage: shell\n"
             "builtin commands: cd, help, exit, jobs, kill, history\n"
             "use `help <command>` to learn more about a specific command.\n");
  } else if (command == EXIT) {
    printf("usage: exit\n\n"
           "terminate the shell process unless there are background processes.\n");
  } else if (command == JOBS) {
    printf("usage: jobs\n\n"
           "list the processes currently executing in the background.\n");
  } else if (command == KILL) {
    printf("usage: kill %%num\n\n"
           "terminate the process numbered `num` in the list of background "
           "processes return by `jobs` (by sending it SIGKILL).\n");
  } else if (command == HISTORY) {
    printf("usage: history [num] [-s num]\n\n"
           "history: print the list of previously executed commands.\n"
           "history num: return the last `num` commands.\n"
           "history -s num: set the size of the history buffer.\n"
           "!1 repeats the command numbered `1` in the list of commands "
           "return by history.\n"
           "!-1 repeats the last command.\n");
  }
}


int is_builtin_command(char * cmd) {
  if (strncmp(cmd, "cd", strlen("cd")) == 0) return CD;
  if (strncmp(cmd, "help", strlen("help")) == 0) return HELP;
  if (strncmp(cmd, "exit", strlen("exit")) == 0) return EXIT;
  if (strncmp(cmd, "jobs", strlen("jobs")) == 0) return JOBS;
  if (strncmp(cmd, "kill", strlen("kill")) == 0) return KILL;
  if (strncmp(cmd, "history", strlen("history")) == 0) return HISTORY;

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
    if (!has_bg_jobs) {
      exit(EXIT_SUCCESS);
    }
    printf("There are background processes.\n");
  } else if (command == JOBS) {
    print_running_jobs();
  } else if (command == KILL) {
    pid_t pid = strtol(cmd.VarList[0], (char **)NULL, 10);
    kill(pid, SIGKILL);
  } else if (command == HISTORY) {
    if (!cmd.VarList[0]) {
      // if no arguments are given, just print history and return.
      history_print();
      return;
    }

    if (strncmp(cmd.VarList[0], "-s", strlen("-s")) == 0) {
      int hist_size = strtol(cmd.VarList[1], (char **)NULL, 10);
      history_stifle(hist_size);
    }
  }
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
    if (!info){
      free(cmdLine);
      continue;
    }
    //prints the info struct
    print_info(info);

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
