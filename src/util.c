#include "util.h"

#include <string.h>

char *trim(char *s) {
  const int i = strlen(s) - 1;
  if ((i > 0) && (s[i] == '\n')) {
    s[i] = '\0';
  }
  return s;
}
