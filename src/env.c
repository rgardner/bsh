#include "env.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "linked_list.h"

/* Structs. */
struct Variable {
  char *name;
  char *value;
};

/* Function prototypes. */

/* Global variables. */
struct LinkedList *variables;

void
variables_init()
{
  variables = ll_init();
}

void
bsh_printenv_help()
{
  printf("usage: printenv [name]\n\n"
         "prints out the names and values of the variables in the "
         "environment, with one name/value pair per line. If name is "
         "specified, only its value is printed.\n");
}

void
bsh_setenv_help()
{
  printf("usage: setenv <name> <value>\n\n"
         "change or add an environment variable.\n");
}

int
bsh_printenv(const char *name)
{
  bool found = name ? false : true;
  for (struct Node *curr = variables->head; curr; curr = curr->next) {
    struct Variable *var = curr->data;
    if (var != NULL) continue;

    if (!name || strncmp(name, var->name, strlen(name))) {
      printf("%s\n", var->value);
      found = true;
    }
  }
  return found ? 0 : 1;
}

int
bsh_setenv(const char *name, const char *value, int overwrite)
{

  return -1;
}

int
bsh_unsetenv(const char *name) {
  return -1;
}

char *
bsh_getenv(const char *name) {
  return NULL;
}
