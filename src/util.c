#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char*
trim(char* s)
{
  const int i = strlen(s) - 1;
  if ((i > 0) && (s[i] == '\n')) {
    s[i] = '\0';
  }
  return s;
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
