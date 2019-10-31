/** Provides alias creation, expansion, and removal capabilities.
 *
 *  @file
 *
 *  Aliases map names to commands. Before aliases can be used, the alias data
 *  data structures must be initialized.
 */

#ifndef ALIAS_H
#define ALIAS_H

enum bsh_alias_error
{
  c_bsh_alias_error_success = 0,
  c_bsh_alias_error_general = 1,
  c_bsh_alias_error_usage = 2,
};

/** Initialize alias data structures.
 *
 *  @note Must be called before other alias functions can be used.
 */
void
aliases_init();

/** The alias builtin command that prints existing aliases and creates new ones.
 *
 *  @code{.sh}
 *  alias name=value
 *  alias name
 *  alias
 *  @endcode
 *  @return 0 on success; any positive value on failure.
 */
int
alias(int argc, char** argv);

/** Print alias usage and exit. */
void
alias_help();

/** Remove alias entry.
 *
 *  @return 0 if the alias was successfully removed; 1 if the alias does not
            exist.
 */
int
unalias(int argc, char** argv);

/** Print unalias usage and exit. */
void
unalias_help();

/** Expand alias value for string.
 *
 *  @param name The name of the alias.
 *  @param[out] output The memory area to copy the alias's value to. Remember to
 *                     free this.
 *  @return
 *  0 if no expansion occurred;
 *  1 if an expansion occurred;
 *  -1 if there was an error in the expansion.
 */
int
alias_exp(const char* name, char** output);

#endif
