.SUFFIXES:
.SUFFIXES: .c .h .o

SHELL    := /bin/bash
CC       := gcc
CFLAGS   := -g -std=c99 -Wall -Werror -Wextra -Wpedantic
SRCDIR   := ./src
OBJDIR   := ./obj
BINDIR   := ./bin
SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TARGET   := doit

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) $(OBJDIR)/* $(TARGET)

.PHONY: install
install: $(TARGET)
	mkdir ./bin
	mv $(TARGET) ./bin

.PHONY: uninstall
uninstall:
	rm -rf $(BINDIR)
