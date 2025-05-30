# Makefile
# Co-authored-by: ChatGPT (gpt-4-o) <chatgpt@openai.com>
CC = gcc
# CFLAGS = -Wall -Wextra -O2 -fPIC -Isources
CFLAGS = -Wall -Wextra -O0 -fPIC -g -Isources
PREFIX ?= /usr/local

SRC = $(wildcard sources/*.c)
OBJ = $(SRC:sources/%.c=%.o)
TARGET = liblists.a

HEADERS = sources/lists.h

.PHONY: all clean install uninstall test

all: $(TARGET)

$(TARGET): $(OBJ)
	ar rcs $@ $^

%.o: sources/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# tests/unit/%: tests/unit/%.c $(TARGET)
# 	$(CC) $(CFLAGS) -o $@ $< -L. -llists -lcunit -pthread

TEST_SRC = $(wildcard tests/unit/*.c)
TEST_OBJ = $(TEST_SRC:.c=.o)
TEST_BIN = tests/unit/test_runner

LDFLAGS += -L/lib/x86_64-linux-gnu

test: $(TEST_BIN)
	valgrind --leak-check=full --track-origins=yes $(TEST_BIN) 2>valgrind.out

$(TEST_BIN): $(TEST_OBJ) $(TARGET)
	$(CC) $(CFLAGS) -o $@ $^ -L. $(LDFLAGS) -lcunit -pthread -llists

install: $(TARGET)
	install -d $(PREFIX)/lib
	install -d $(PREFIX)/include
	install -m 644 $(TARGET) $(PREFIX)/lib/
	install -m 644 $(HEADERS) $(PREFIX)/include/
	@echo "Library installed to $(PREFIX)"

uninstall:
	rm -f $(PREFIX)/lib/$(TARGET)
	cd $(PREFIX)/include && rm -f $(notdir $(HEADERS))
	@echo "Library uninstalled from $(PREFIX)"

clean:
	rm -f $(OBJ) $(TARGET) $(TEST_BIN)
