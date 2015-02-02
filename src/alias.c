#include "alias.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linked_List.h"

/* Structs. */
struct Alias {
  char *name;
  char *value;
};

/* Function prototypes. */
static bool alias_add(char *, char *, bool);
static bool alias_remove(char *);
static struct Alias* alias_search(char *);
static void alias_print();

/* Global variables. */
struct LinkedList *aliases;

void
alias_init()
{
  aliases = ll_init();
}

int
alias(int argc, char **argv)
{
  /* Print all aliases. */
  if (argc == 0) {
    alias_print();
    return 0;
  }

  /* Create an alias between a name/value pair. */
  char *name;
  if ((name = strsep(&argv[0], "="))) {
    name = strdup(name);
    char *value = strdup(argv[0]);
    alias_add(name, value, 1);
    return 0;
  }

  /* Print all aliases matching name arguments. */
  bool all_found = true;
  for (int i = 0; i < argc; i++) {
    struct Alias *al = alias_search(argv[i]);
    if (al) {
      printf("alias %s = %s\n", al->name, al->value);
    } else {
      printf("bsh: alias: %s not found.\n", argv[i]);
      all_found = false;
    }
  }
  return all_found ? 0 : 1;
}

void
alias_help()
{
  printf("usage: alias [name[=value] ... ]\n\n"
         "`alias`: print all existing aliases.\n"
         "`alias name`: print the value associated with name.\n"
         "`alias name=value`: create / modify name to be an alias for value.\n");
}

int
unalias(int argc, char **argv)
{
  if (argc == 0) {
    unalias_help();
    return 2;
  }

  bool all_found = true;
  for (int i = 0; i < argc; i++) {
    all_found = alias_remove(argv[i]);
  }
  return all_found ? 0 : 1;
}

void
unalias_help()
{
  printf("usage: unalias [-a] name [name ...]\n");
}

int
alias_exp(char *string, char **output)
{
  int length = strlen(string) + 1;
  *output = malloc(sizeof(char) * length);
  strncpy(*output, string, length);

  struct Alias *result = alias_search(string);
  if (!result) return -1;

  char *value = result->value;
  length = strlen(value) + 1;
  *output = realloc(*output, sizeof(char) * length);
  strncpy(*output, value, length);
  return 1;
}

static bool
alias_add(char *name, char *value, bool overwrite)
{
  /* Search to see if alias already exists. */
  struct Node *current = NULL;
  for (current = aliases->head; current; current = current->next) {
    struct Alias *alias = current->data;
    if (strncmp(name, alias->name, strlen(name)) != 0) continue;
    if (!overwrite) return false;
    alias->value = value;
    return true;
  }

  /* Add a new alias. */
  struct Alias *new = malloc(sizeof(struct Alias));
  *new = (struct Alias ) { .name = name, .value = value };

  struct Node *previous = NULL;
  for (current = aliases->head; current; current = current->next) {
    struct Alias *alias = current->data;
    if (strncmp(name, alias->name, strlen(name)) < 0) break;
    previous = current;
  }
  ll_add_after(aliases, previous, new);
  return true;
}

/* Remove the alias corresponding to the name.
 *
 * Returns true if found; false otherwise. */
static bool
alias_remove(char *name)
{
  struct Node *curr;
  struct Node *prev;
  for (curr = aliases->head; curr; curr = curr->next) {
    struct Alias *alias = curr->data;
    int cmp = strncmp(name, alias->name, strlen(name));
    if (cmp > 0) break;
    if (cmp < 0) {
      prev = curr;
      continue;
    }

    /* Found the node. */
    if (prev) {
      prev->next = curr->next;
    } else {  // first element in the list
      aliases->head = curr->next;
    }
    return true;
  }
  return false;
}

static struct Alias *
alias_search(char *name)
{
  int len = ll_size(aliases);
  for (int i = 0; i < len; i++) {
    struct Alias *al = ll_get(aliases, i);
    if (!al) break;

    if (strncmp(al->name, name, strlen(al->name)) == 0) return al;
  }
  return NULL;
}

/* Print all aliases matching names. If names is NULL, print all aliases. */
static void
alias_print()
{
  struct Node *current;
  for (current = aliases->head; current; current = current->next) {
    struct Alias *al = current->data;
    printf("alias %s = %s\n", al->name, al->value);
  }
}
