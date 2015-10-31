#include "test_util.h"

#include "stdlib.h"
#include "string.h"

char *new_str(char *static_str) {
  const size_t len = strlen(static_str);
  char *new_str = malloc(sizeof(char) * (len + 1));
  strncpy(new_str, static_str, len);
  new_str[len] = '\0';
  return new_str;
}
