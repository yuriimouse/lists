# Makefile for lists library

CC = gcc
CFLAGS = -Wall -Wextra -O2 -fPIC
PREFIX ?= /usr/local

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
TARGET = liblists.a

.PHONY: all clean install uninstall

all: $(TARGET)

$(TARGET): $(OBJ)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

install: $(TARGET)
	install -d $(PREFIX)/lib
	install -d $(PREFIX)/include
	install -m 644 $(TARGET) $(PREFIX)/lib/
	install -m 644 lists.h $(PREFIX)/include/
	@echo "Library installed to $(PREFIX)"

uninstall:
	rm -f $(PREFIX)/lib/$(TARGET)
	rm -f $(PREFIX)/include/lists.h
	@echo "Library uninstalled from $(PREFIX)"

clean:
	rm -f $(OBJ) $(TARGET)
