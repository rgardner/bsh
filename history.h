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
void history_print();
