CC      := clang
CFLAGS  := -std=c11 -Wall -Wextra -Wpedantic -Werror -g -Iinclude
TARGET  := pm
SRC     := $(shell find src -name '*.c')
OBJ     := $(SRC:.c=.o)
CFLAGS  := -std=c11 -Wall -Wextra -Wpedantic -Werror -g -Iinclude -MMD -MP


$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(OBJ:.o=.d) $(TARGET)

.PHONY: clean

-include $(OBJ:.o=.d)