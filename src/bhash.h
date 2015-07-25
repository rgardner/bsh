/* bhash.h -- the data structures used in hashing in Bsh. */

/* This implementation uses buckets with linked lists.
 *
 * The API is based on "a quick hashtable implementation in c" by @tonious
 * https://gist.github.com/tonious/1377667
 *
 */

#ifndef BHASH_H
#define BHASH_H

typedef struct entry_s {
  char *key;
  char *value;
  struct entry_s *next;
} entry_t;

typedef struct hashtable_s {
  int size;
  struct entry_s **table;
} hashtable_t;

/* Create a new hashtable. */
hashtable_t *ht_create(int size);

/* Hash a string for a particular hash table. */
int ht_hash(hashtable_t *hashtable, char *key);

/* Create a key-value pair. */
entry_t *ht_newpair(char *key, char *value);

/* Insert a key-value pair into a hash table. */
void ht_set(hashtable_t *hashtable, char *key, char *value);

/* Retrieve a key-value pair from a hash table. */
char *ht_get(hashtable_t *hashtable, char *key);
#endif
