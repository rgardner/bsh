TARGET=bsh

CC=gcc
CFLAGS=-Wall -Wextra -Werror -Wunreachable-code -Wformat=2 -DUNIX
FLAGS = -lreadline -lcurses
DEBUG =-g -DDEBUG

SRCDIR = src
OBJDIR = obj
BINDIR = bin

SRCS := $(wildcard $(SRCDIR)/*.c)
INCS := $(wildcard $(SRCDIR)/*.h)
OBJS := $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm = rm -f

$(BINDIR)/$(TARGET): $(OBJS)
	@$(CC) -o $@ $(FLAGS) $(OBJS)

$(OBJS): $(OBJDIR)/%.o: $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) $(DEBUG) -c $< -o $@

.PHONEY: clean
clean:
	@$(rm) $(OBJS)

.PHONEY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
