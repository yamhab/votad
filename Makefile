SRC_DIR := src
OBJ_DIR := obj
BIN := votad

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CFLAGS += -O2 -Wall -Wextra -Wpedantic $(shell pkg-config --cflags sdl3)
LDFLAGS += $(shell pkg-config --libs-only-L --libs-only-other sdl3)
LDLIBS += $(shell pkg-config --libs-only-l sdl3) -lm

.PHONY: clean

$(BIN): $(OBJS)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	$(RM) -r $(OBJ_DIR) $(BIN)
