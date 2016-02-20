#include "util.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** @brief Remove trailing whitespace
 *
 *  @return New length of s.
 */
size_t
trim_right(char* s)
{
  int i = strlen(s) - 1;
  while ((i >= 0 && isspace(s[i]))) {
    i--;
  }
  s[i + 1] = '\0';
  return i + 1;
}

/** @brief Print error message for failed system/library
 *         call and exit with failure.
 *  @param s Printed before error message. The name of the function that
 *         incurred the error.
 */
void
handle_error(const char* s)
{
  perror(s);
  exit(EXIT_FAILURE);
}
