CC=gcc
CFLAGS=-Wextra -Wall -Wunreachable-code -Wformat=2 -DUNIX
FLAGS = -lreadline -lcurses
DEBUG =-g -DDEBUG

TARGET=bsh

OBJS = main.o            \
			 background_jobs.o \
			 builtins.o        \
			 env.o             \
			 history.o         \
			 parse.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(FLAGS) $(CFLAGS) $(DEBUG) $(OBJS) -o $@

.c.o:
	$(CC) -c $(CFLAGS) $(DEBUG) $<

clean:
	rm $(OBJS) $(TARGET)
