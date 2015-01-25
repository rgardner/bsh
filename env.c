#include <stdio.h>


void bsh_printenv_help() {
  printf("usage: printenv [name]\n\n"
         "prints out the names and values of the variables in the "
         "environment, with one name/value pair per line. If name is "
         "specified, only its value is printed.\n");
}

void bsh_setenv_help() {
  printf("usage: setenv <name> <value>\n\n"
         "change or add an environment variable.\n");
}

void bsh_printenv(const char *name) {

}

int bsh_setenv(const char *name, const char *value, int overwrite) {
  return -1;
}

int bsh_unsetenv(const char *name) {
  return -1;
}

char *
bsh_getenv(const char *name) {
  return NULL;
}
