# Co-authored-by: ChatGPT (gpt-4-o) <chatgpt@openai.com>
CC = gcc
CFLAGS = -Wall -Wextra -O2 -fPIC
PREFIX ?= /usr/local

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
TARGET = liblists.a

HEADERS = lists.h list_safe.h

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
	install -m 644 $(HEADERS) $(PREFIX)/include/
	@echo "Library installed to $(PREFIX)"

uninstall:
	rm -f $(PREFIX)/lib/$(TARGET)
	cd $(PREFIX)/include && rm -f $(HEADERS)
	@echo "Library uninstalled from $(PREFIX)"

clean:
	rm -f $(OBJ) $(TARGET)
