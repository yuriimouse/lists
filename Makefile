# Makefile
# Co-authored-by: ChatGPT (gpt-4-o) <chatgpt@openai.com>
CC = gcc
CFLAGS = -Wall -Wextra -O2 -fPIC -Isources
PREFIX ?= /usr/local

SRC = $(wildcard sources/*.c)
OBJ = $(SRC:sources/%.c=%.o)
TARGET = liblists.a

HEADERS = sources/lists.h

TEST_SRC = $(wildcard tests/unit/*.c)
TEST_BIN = $(TEST_SRC:tests/unit/%.c=tests/unit/%)

.PHONY: all clean install uninstall test tests/unit

all: $(TARGET)

$(TARGET): $(OBJ)
	ar rcs $@ $^

%.o: sources/%.c
	$(CC) $(CFLAGS) -c $< -o $@

tests/unit/%: tests/unit/%.c $(TARGET)
	$(CC) $(CFLAGS) -o $@ $< -L. -llists -pthread

tests/unit: $(TEST_BIN)

test: tests/unit
	@for t in $(TEST_BIN); do \
		echo "Running $$t"; \
		./$$t || exit 1; \
	done
	@echo "All tests passed."

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
