#ifndef ALIAS_H
#define ALIAS_H

/* The alias program. Aliases a name with a value for use with shell expansion.
 *
 * Returns 0 on success; any positive non-zero value on failure.
 */
int
alias(int, char**);

/* Print helpful information about the alias command, then exit. */
void
alias_help();

/* The unalias program. Unalias a name with its corresponding value. */
int
unalias(int, char**);

void
unalias_help();
#endif
