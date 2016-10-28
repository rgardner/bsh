#include "util.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
