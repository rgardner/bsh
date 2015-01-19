CC=gcc
CFLAGS=-DUNIX -lreadline -lcurses
#CFLAGS=-Wextra -Wall -Wunreachable-code -Wformat=2
DEBUG=-g

all: shell

bsh: bsh.c
	$(CC) $(CFLAGS) bsh.c -o bsh

shell: shell.c background_jobs.c background_jobs.h builtins.c builtins.h history.c history.h parse.c parse.h
	$(CC) $(CFLAGS) $(DEBUG) shell.c background_jobs.c builtins.c history.c parse.c -o shell

clean:
	rm -r bsh *.o
