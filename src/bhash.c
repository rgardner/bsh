#include "bhash.h"

#include <limits.h>
#include <stdlib.h>
#include <string.h>

/* Create a new hashtable. */
hashtable_t *ht_create(int size) {
  hashtable_t *ht = NULL;

  /* Allocate the table itself. */
  if ((ht = malloc(sizeof(hashtable_t))) == NULL) {
    return NULL;
  }

  /* Allocate pointers to the head nodes. */
  if ((ht = malloc(sizeof(entry_t *) * size)) == NULL) {
    return NULL;
  }

  for (int i = 0; i < size; i++) {
    ht->table[i] = NULL;
  }
  ht->size = size;
  return ht;
}

/* Hash a string for a particular hash table. */
int ht_hash(hashtable_t *hashtable, char *key) {
  unsigned long int hashval;
  int i = 0;

  /* Convert our string to an integer. */
  while (hashval < ULONG_MAX && i < strlen(key)) {
    hashval = hashval << 8;
    hashval += key[i];
    i++;
  }
  return hashval % hashtable->size;
}

/* Create a key-value pair. */
entry_t *ht_newpair(char *key, char *value) {
  entry_t *newpair = NULL;

  if ((newpair = malloc(sizeof(entry_t))) == NULL) {
    return NULL;
  }

  if ((newpair->key = strdup(key)) == NULL) {
    return NULL;
  }

  if ((newpair->value = strdup(value)) == NULL) {
    return NULL;
  }

  newpair->next = NULL;
  return newpair;
}

/* Insert a key-value pair into a hash table. */
void ht_set(hashtable_t *hashtable, char *key, char *value) {
  int bin = ht_hash(hashtable, key);
  entry_t *next = hashtable->table[bin];
  entry_t *last = NULL;

  while (next != NULL && next->key != NULL && strcmp(key, next->key) > 0) {
    last = next;
    next = next->next;
  }

  entry_t *newpair = NULL;

  /* There's already a pair. Let's replace the string. */
  if (next != NULL && next->key != NULL && strcmp(key, next->key) == 0) {
    free(next->value);
    next->value = strdup(value);
  }

  /* Hope couldn't find it. Time to grow up a pair. */
  else {
    newpair = ht_newpair(key, value);

    /* We're at the start of the linked list in this bin. */
    if (next == hashtable->table[bin]) {
      newpair->next = next;
      hashtable->table[bin] = newpair;
    }

    /* We're at the end of the linked list in this bin. */
    else if (next == NULL) {
      last->next = newpair;
    }

    /* We're in the middle of the list. */
    else {
      newpair->next = next;
      last->next = newpair;
    }
  }
}

/* Retrieve a key-value pair from a hash table. */
char *ht_get(hashtable_t *hashtable, char *key) {
  int bin = ht_hash(hashtable, key);

  /* Step through the bin, looking for our value. */
  entry_t *pair = hashtable->table[bin];
  while (pair != NULL && pair->key != NULL && strcmp(key, pair->key) > 0) {
    pair = pair->next;
  }

  /* Did we actually find anything? */
  if (pair == NULL || pair->key == NULL || strcmp(key, pair->key) != 0) {
    return NULL;
  } else {
    return pair->value;
  }
}
