#ifndef HISTORY_H
#define HISTORY_H

/** Default maximum number of history entries. */
#define HISTSIZE 10

/* The number of entries currently stored in the history list. */
extern int history_length;

/** Maximum number of history entries.
 *
 * @see history_stifle
 */
extern int history_max_entries;

/** Initialize alias data structures.
 *
 * @note Must be called before other history functions can be used.
 */
void history_init();

/** Expand string, placing the result into output, a pointer to a string.
 *
 * @return
 *   0: if no expansion took place;
 *   1: if expansions did take place;
 *  -1: if there was an error in expansion.
 */
int history_exp(const char* string, char** output);

/** Append string to end of history list. */
void history_add(const char* string);

/** Stifle the history list, remembering only the last max entries. */
void history_stifle(int max);

/** Print history list. */
void history_print(int num);

/** Print history usage string. */
void history_help();
#endif
