#ifndef UTIL_H
#define UTIL_H

/* Remove trailing newline */
char* trim(char* s);

/* Call perror(s) and exit. */
void handle_error(const char* s);
#endif
