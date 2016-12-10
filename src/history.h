/** Provides history capabilities to the shell.
 *
 *  @file
 *
 *  Strings are added to the history using history_add(). Those strings can be
 *  retrieved by passing a history command to history_exp(). A history command
 *  is a special shell syntax to retrieve commands from either the beginning or
 *  the end of the history. The number of entries stored in the history is
 *  fixed though it can be increased or decreased using history_stifle().
 */

#ifndef HISTORY_H
#define HISTORY_H

#include <stddef.h>  // size_t

/** Default maximum number of history entries. */
#define HISTSIZE 10

/** Initialize alias data structures.
 *
 *  @note Must be called before other history functions can be used.
 */
void history_init();

void history_free();

/** Expand string, placing the result into output, a pointer to a string.
 *
 *  @note Unlike GNU History, output only needs to be freed if expansion
 *        occurrs.
 *  @param string The history command.
 *  @param[out] output The command found in history if it exists.
 *  @return
 *    0: if no expansion took place;
 *    1: if expansions did take place;
 *   -1: if an error occurred.
 */
int history_exp(const char* string, char** output);

/** Append string to end of history list. */
void history_add(const char* string);

/** Stifle the history list, remembering only the last max entries. */
void history_stifle(int max);

/** Print all entries in history list. */
void history_print_all();

/** Print n_last_entries in history list. */
void history_print(size_t n_last_entries);

/** Print history usage string. */
void history_help();
#endif
