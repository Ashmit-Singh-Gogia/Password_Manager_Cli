CC     := clang
CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -Werror -g -Isrc -MMD -MP
TARGET := pm

SRC := $(shell find src -name '*.c')
OBJ := $(SRC:.c=.o)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: clean

-include $(OBJ:.o=.d)