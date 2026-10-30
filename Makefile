CC     := clang
CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -Werror -g -Isrc -MMD -MP
TARGET := pm

SRC := $(shell find src -name '*.c')
OBJ := $(SRC:.c=.o)

PREFIX ?= /usr/local

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

install: $(TARGET)
	cp $(TARGET) $(PREFIX)/bin/pm

uninstall:
	rm -f $(PREFIX)/bin/pm

clean:
	rm -f $(OBJ) $(OBJ:.o=.d) $(TARGET)

.PHONY: clean install uninstall

-include $(OBJ:.o=.d)