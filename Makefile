SRC_DIR := src
BUILD_DIR := build

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.d)
BIN := votad

CFLAGS += -MD -Wall -Wextra -Wpedantic -O2 $(shell pkg-config --cflags sdl3)
LDFLAGS += $(shell pkg-config --libs-only-L --libs-only-other sdl3)
LDLIBS += $(shell pkg-config --libs-only-l sdl3) -lm

$(BUILD_DIR)/$(BIN): $(OBJS)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

-include $(DEPS)

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)
