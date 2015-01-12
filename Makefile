CC=gcc
CFLAGS=-DUNIX -lreadline -lcurses
#CFLAGS=-Wextra -Wall -Wunreachable-code -Wformat=2
DEBUG=-g

all: shell

bsh: bsh.c
	$(CC) $(CFLAGS) bsh.c -o bsh

shell: shell.c history.c history.h background_jobs.c background_jobs.h parse.c parse.h
	$(CC) $(CFLAGS) $(DEBUG) shell.c history.c background_jobs.c parse.c -o shell

clean:
	rm -r bsh *.o
