#include "alias.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linked_List.h"

/* Structs. */
struct Alias {
  const char *name;
  const char *value;
};

/* Function prototypes. */
static bool alias_add(const char *, const char *, bool);
static bool alias_remove(const char *);
static struct Alias* alias_search(const char *);
static void aliases_print();
static void alias_free(const struct Alias *);

/* Global variables. */
struct LinkedList *aliases;

void
aliases_init()
{
  aliases = ll_init();
}

static struct Alias *
alias_init(const char *name, const char *value)
{
  struct Alias al = { .name = name, .value = value };
  struct Alias *alias = malloc(sizeof(struct Alias));
  memcpy(alias, &al, sizeof(struct Alias));
  return alias;
}

int
alias(const int argc, char **argv)
{
  /* Print all aliases. */
  if (argc == 0) {
    aliases_print();
    return 0;
  }

  /* Create an alias between a name/value pair. */
  char *name;
  if ((name = strsep((char **)&argv[0], "="))) {
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
unalias(const int argc, char **argv)
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
alias_exp(const char *string, char **output)
{
  int length = strlen(string) + 1;
  *output = malloc(sizeof(char) * length);
  strncpy(*output, string, length);

  struct Alias *result = alias_search(string);
  if (!result) return -1;

  const char *value = result->value;
  length = strlen(value) + 1;
  *output = realloc(*output, sizeof(char) * length);
  strncpy(*output, value, length);
  return 1;
}

static bool
alias_add(const char *name, const char *value, const bool overwrite)
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
  struct Alias *new = alias_init(name, value);

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
alias_remove(const char *name)
{
  struct Node *curr = NULL;
  struct Node *prev = NULL;
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
    alias_free(alias);
    node_free(curr);
    return true;
  }
  return false;
}

static struct Alias *
alias_search(const char *name)
{
  struct Node *current = NULL;
  for (current = aliases->head; current; current = current->next) {
    struct Alias *al = current->data;
    if (!al) break;

    if (strncmp(al->name, name, strlen(al->name)) == 0) return al;
  }
  return NULL;
}

/* Print all aliases matching names. If names is NULL, print all aliases. */
static void
aliases_print()
{
  struct Node *current;
  for (current = aliases->head; current; current = current->next) {
    struct Alias *al = current->data;
    printf("alias %s = %s\n", al->name, al->value);
  }
}

static void
alias_free(const struct Alias *alias)
{
  if (alias->name) free((char *)alias->name);
  if (alias->value) free((char *)alias->value);
  free((struct Alias *)alias);
}
