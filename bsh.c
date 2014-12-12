#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

void print_arguments(int, char**);


int main(int argc, char **argv) {
  printf("Welcome to Bob shell.\n");
  print_arguments(argc, argv);

  pid_t pid;
  if ((pid = fork()) == -1) {
    // An error occurred...
    exit(EXIT_FAILURE);
  }

  else if (pid == 0) {
    // Child process
    printf("Child: Play time!\n");
  }

  else {
    // Parent process
    printf("Parent: Go to bed!\n");
    int status;
    waitpid(pid, &status, 0);
    printf("Child exited with status code: %d.\n", status);
  }
  exit(EXIT_SUCCESS);
}

/* print arguments. */
void print_arguments(int argc, char **argv) {
  while (argc--) {
    printf("%s\n", *argv++);
  }
}
