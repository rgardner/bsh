#ifndef ENV_H
#define ENV_H
/* Initialize internal data structures. */
void
variables_init();

void
bsh_printenv_help();

void
bsh_setenv_help();

int
bsh_printenv(const char *name);

int
bsh_setenv(const char *name, const char *value, int overwrite);

int
bsh_unset(const char *name);

char *
bsh_getenv(const char *name);
#endif
