CC=gcc
CFLAGS=-Wextra -Wall -Wunreachable-code -Wformat=2 -g

all: bsh

bsh: bsh.c
	$(CC) $(CFLAGS) bsh.c -o bsh

clean:
	rm -r bsh *.o
