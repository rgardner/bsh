#include <assert.h>
#include <err.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef __linux__
#include <bsd/stdlib.h>  // reallocf
#include <bsd/string.h>
#else
#include <string.h>
#endif
#include <readline/readline.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>

#include "alias.h"
#include "bg_jobs.h"
#include "builtins.h"
#include "history.h"
#include "job.h"
#include "parse.h"
#include "util.h"

#define MAX_PROMPT_LENGTH 1024
#define COMMAND_NOT_FOUND_EXIT_CODE 127
static const size_t c_default_history_capacity = 10;

pid_t shell_pgid;
struct termios shell_tmodes;
int shell_terminal;
int shell_is_interactive;

/** Build command prompt
 *
 *  @param buf Copies the prompt into the memory referenced by buf.
 *  @param size The size, in bytes, of the array referenced by buf.
 */
void
build_prompt(char* buf, size_t size)
{
  if (!getcwd(buf, size)) {
    warn(NULL);
  }
  strlcat(buf, "$ ", size);
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
shell_init()
{
  // See if we are running interactively.
  shell_terminal = STDIN_FILENO;
  shell_is_interactive = isatty(shell_terminal);

  if (shell_is_interactive) {
    // Loop until we are in the foreground.
    while (tcgetpgrp(shell_terminal) != (shell_pgid = getpgrp())) {
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
    shell_pgid = getpid();
    if (setpgid(shell_pgid, shell_pgid) < 0) {
      perror("Couldn't put the shell in its own process group");
      exit(EXIT_FAILURE);
    }

    /* Grab control of the terminal.  */
    tcsetpgrp(shell_terminal, shell_pgid);

    /* Save default terminal attributes for shell.  */
    tcgetattr(shell_terminal, &shell_tmodes);
  }
}

void
launch_process(process* p,
               pid_t pgid,
               int infile,
               int outfile,
               int errfile,
               bool foreground)
{
  pid_t pid;
  if (shell_is_interactive) {
    /* Put the process into the process group and give the process group the
     * terminal, if appropriate. This has to be done both by the shell and in
     * the individual child processes beacuse of potential race conditions. */
    pid = getpid();
    if (pgid == 0) pgid = pid;
    setpgid(pid, pgid);
    if (foreground) tcsetpgrp(shell_terminal, pgid);

    /* Set the handling for job control signals back to the default. */
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    signal(SIGTSTP, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
    signal(SIGCHLD, SIG_DFL);
  }
  if (infile != STDIN_FILENO) {
    dup2(infile, STDIN_FILENO);
    close(infile);
  }
  if (outfile != STDOUT_FILENO) {
    dup2(outfile, STDOUT_FILENO);
    close(outfile);
  }
  if (errfile != STDERR_FILENO) {
    dup2(errfile, STDERR_FILENO);
    close(errfile);
  }
  if (execvp(p->argv[0], p->argv) < -1) {
    printf("-bsh: %s: command not found\n", p->argv[0]);
    exit(COMMAND_NOT_FOUND_EXIT_CODE);
  }
}

void
launch_job(job* j, const bool foreground)
{
  job_print(j);
  // Currently, bsh does not support pipes with builtin commands.
  enum BuiltinCommands command = is_builtin_command(j->first_process->argv[0]);
  if (command != NO_SUCH_BUILTIN) {
    execute_builtin_command(command, *j->first_process);
    return;
  }

  // set up piping
  pid_t pid;
  int infile = j->infile;
  int pipefd[2];
  for (process* p = j->first_process; p; p = p->next) {
    int outfile;
    if (p->next) {
      if (pipe(pipefd) < 0) {
        err(EXIT_FAILURE, NULL);
      }
      outfile = pipefd[1];
    } else {
      outfile = j->outfile;
    }

    // Fork the child process.
    pid = fork();
    if (pid == 0) {  // child process
      launch_process(p, j->pgid, infile, outfile, j->errfile, foreground);
    } else if (pid < 0) {  // fork failed
      err(EXIT_FAILURE, NULL);
    } else {  // parent process
      p->pid = pid;
      if (shell_is_interactive) {
        if (!j->pgid) {
          j->pgid = pid;
          setpgid(pid, j->pgid);
        }
      }
    }
    // clean up pipes
    if (infile != j->infile) {
      close(infile);
    }
    if (outfile != j->outfile) {
      close(outfile);
    }
    infile = pipefd[0];
  }

  if (shell_is_interactive) {
    int status;
    waitpid(pid, &status, 0);
  } else if (foreground) {
    // put job in foreground.
  } else {
    // FIXME
    /*put_job_in_background(info, pid);*/
  }
}

int
main(int argc, char** argv)
{
  // Ignore argc and argv until we add command line arguments.
  UNUSED(argc);
  UNUSED(argv);

  // Initialization
  shell_init();
  builtins_init(c_default_history_capacity);

  if (signal(SIGCHLD, handle_sigchld) == SIG_ERR) {
    err(EXIT_FAILURE, NULL);
  }

  print_login_message();

  while (true) {
    // Read input, parse history, parse alias, execute.

    char prompt[MAX_PROMPT_LENGTH];
    build_prompt(prompt, sizeof(prompt));

    char* cmd_line = NULL;
    if (!(cmd_line = readline(prompt))) {
      // Quit on EOF terminated empty line.
      printf("\n");
      break;
    }

    // check command line length
    if (strlen(cmd_line) > MAXLINE) {
      warnx("The command you entered is too long.\n");
      free(cmd_line);
      continue;
    }

    if (cmd_line[0] == '\0') {
      free(cmd_line);
      continue;
    }

    trim_right(cmd_line);

    char* expansion = NULL;
    const int his_res = history_exp(cmd_line, &expansion);
    if (his_res < 0) {
      free(cmd_line);
      continue;
    } else if (his_res == 1) {
      const size_t oldsz = (strlen(cmd_line) + 1) * sizeof(char);
      const size_t newsz = strlcpy(cmd_line, expansion, oldsz) + 1;
      if (newsz > oldsz) {
        if (!(cmd_line = reallocf(cmd_line, newsz))) {
          free(expansion);
          continue;
        }

        const size_t check __attribute__((unused)) = strlcpy(cmd_line, expansion, newsz) + 1;
        assert(check == newsz);
      }

      free(expansion);
    }

    history_add(cmd_line);

    struct ParseInfo* info = parse(cmd_line);
    free(cmd_line);
    if (!info) {
      continue;
    }

    // Expand aliases in info commands.
    for (int i = 0; i <= info->pipeNum; i++) {
      struct Command* const cmd = &info->CommArray[i];
      char* expansion;
      if (alias_exp(cmd->command, &expansion) <= 0) {
        continue;
      }

      // replace command with alias expansion
      free(cmd->command);
      cmd->command = expansion;
    }

#ifndef NDEBUG
    print_info(info);
#endif

    // com contains the info. of the command before the first "|"
    const struct Command* cmd = &info->CommArray[0];
    if (!cmd || !cmd->command) {
      free_info(info);
      continue;
    }

    // Convert ParseInfo to Job.
    job* j = malloc(sizeof(job));
    init_job(j, info, shell_pgid, shell_tmodes);
    bool foreground = !info->runInBackground;
    free_info(info);

    launch_job(j, foreground);
    if (foreground) {
      job_free(j);
    }
  }

  history_free();
  return EXIT_SUCCESS;
}
