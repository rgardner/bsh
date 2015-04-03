#ifndef VARIABLES_H
#define VARIABLES_H
/* Initialize internal data structures. */
void variables_init();

void bsh_printenv_help();

void bsh_setenv_help();

int bsh_printenv(const char *name);

int bsh_setenv(char *name, char *value, int overwrite);

int bsh_unset(const char *name);

char *bsh_getenv(const char *name);
#endif
