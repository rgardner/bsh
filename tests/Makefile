CC=gcc
CFLAGS=
FLAGS=-lm
DEBUG=

TARGET=test

OBJS = test_stack.o \
			 ../stack.o      \


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(FLAGS) $(CFLAGS) $(DEBUG) $(OBJS) -o $@

.c.o:
	$(CC) -c $(CFLAGS) $(DEBUG) $<

clean:
	rm $(OBJS) $(TARGET)
