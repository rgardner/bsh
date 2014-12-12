CC=gcc
CFLAGS=-Wall

all: bsh

bsh: bsh.c
	$(CC) $(CFLAGS) bsh.c -o bsh

clean:
	rm -r bsh *.o
