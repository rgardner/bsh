#ifndef ENV_H
#define ENV_H
void bsh_printenv_help();
void bsh_setenv_help();

void bsh_printenv(const char *name);
int bsh_setenv(const char *name, const char *value, int overwrite);
int bsh_unsetenv(const char *name);
#endif
