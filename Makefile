SRC_DIR := src
OBJ_DIR := obj
BIN := votad

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.d)

CFLAGS += -MD -O2 -Wall -Wextra -Wpedantic $(shell pkg-config --cflags sdl3)
LDFLAGS += $(shell pkg-config --libs-only-L --libs-only-other sdl3)
LDLIBS += $(shell pkg-config --libs-only-l sdl3) -lm

$(BIN): $(OBJS)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) $< -o $@

-include $(DEPS)

.PHONY: clean

clean:
	$(RM) -r $(OBJ_DIR) $(BIN)
