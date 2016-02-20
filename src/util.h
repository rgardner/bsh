#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>

/* Remove trailing whitespace */
size_t trim_right(char* s);

/* Call perror(s) and exit. */
void handle_error(const char* s);
#endif
