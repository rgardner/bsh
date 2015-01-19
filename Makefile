CC=gcc
CFLAGS=-DUNIX -lreadline -lcurses
#CFLAGS=-Wextra -Wall -Wunreachable-code -Wformat=2
DEBUG=-g

all: bsh

bsh: bsh.c background_jobs.c background_jobs.h builtins.c builtins.h history.c history.h parse.c parse.h
	$(CC) $(CFLAGS) $(DEBUG) bsh.c background_jobs.c builtins.c history.c parse.c -o bsh

clean:
	rm -r bsh *.o
