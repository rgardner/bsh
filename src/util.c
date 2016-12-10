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

size_t
min(size_t a, size_t b)
{
  return (a <= b) ? a : b;
}

size_t
safe_sub(size_t lhs, size_t rhs, size_t default_value)
{
  return (lhs > rhs) ? lhs - rhs : default_value;
}
