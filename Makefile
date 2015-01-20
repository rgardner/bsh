CC=gcc
CFLAGS=-g -Wextra -Wall -Wunreachable-code -Wformat=2 -DUNIX
FLAGS = -lreadline -lcurses

TARGET=bsh

OBJS = main.o            \
			 background_jobs.o \
			 builtins.o        \
			 env.o             \
			 history.o         \
			 parse.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(FLAGS) $(CFLAGS) $(OBJS) -o $@

.c.o:
	$(CC) -c $(CFLAGS) $<

clean:
	rm $(OBJS) $(TARGET)
