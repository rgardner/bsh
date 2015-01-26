#ifndef HISTORY_H
#define HISTORY_H
#define HISTSIZE 10

/* Required before using history functions. */
void history_init();

/* Expand string, placing the result into output, a pointer to a string.
 *
 * Returns:
 *  0:  if no expansion took place
 *  1:  if expansions did take place
 *  -1: if there was an error in expansion
 */
int history_exp(char *string, char **output);

/* Place string at the end of the history list. */
void history_add(char *string);

/* Stifle the history list, remembering only the last max entries. */
void history_stifle(int max);

/* Print out history list. */
void history_print(int num);

void history_help();

/* History Variables. */
/* The number of entries currently stored in the history list. */
extern int history_length;

/* The maximum number of history entries. This must be changed using
 *   history_stifle() */
extern int history_max_entries;
#endif
