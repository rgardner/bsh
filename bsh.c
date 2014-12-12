#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

void parse_command();
void print_arguments(int, char**);
void print_prompt();


int main(int argc, char **argv)
{
  printf("Welcome to Bob shell.\n");
  print_arguments(argc, argv);

  while (1) {
    /*pid_t pid;*/
    char cmdline[100];

    print_prompt();
    fgets(cmdline, sizeof(cmdline), stdin);  // read from stdin.
    printf("cmd: %s", cmdline);

    /*cmdline = read_command_line();*/
    /*cmd = parse_command(cmdline);*/

    // record in history

    // if builtin command(cmd), execute it.
    // else: spawn subprocess to deal with it.
/*    if ((pid = fork()) == -1) {*/
      /*// An error occurred...*/
      /*exit(EXIT_FAILURE);*/
    /*}*/

    /*else if (pid == 0) {*/
      /*// Child process*/
      /*printf("Child: Play time!\n");*/
    /*}*/

    /*else {*/
      /*// Parent process*/
      /*printf("Parent: Go to bed!\n");*/
      /*int status;*/
      /*waitpid(pid, &status, 0);*/
      /*printf("Child exited with status code: %d.\n", status);*/
    /*}*/
  }
  exit(EXIT_SUCCESS);
}

void parse_command()
{
}

/* print arguments. */
void print_arguments(int argc, char **argv)
{
  while (argc--) {
    printf("%s\n", *argv++);
  }
}


void print_prompt()
{
  printf("bsh: ");
}
