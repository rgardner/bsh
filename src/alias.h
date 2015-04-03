#ifndef ALIAS_H
#define ALIAS_H

/* Must be called before any alias commands can be used. */
void aliases_init();

/* The alias program. Aliases a name with a value for use with shell expansion.
 *
 * Returns 0 on success; any positive non-zero value on failure.
 */
int alias(int argc, const char** argv);

/* Print helpful information about the alias command, then exit. */
void alias_help();

/* The unalias program. Unalias a name with its corresponding value. */
int unalias(int argc, const char** argv);

void unalias_help();

int alias_exp(const char* string, char** output);
#endif
