CC = gcc
CFLAGS = -Wall -Wextra -g -I include
AR = ar
ARFLAGS = rcs
LIB_NAME = libwayc.a
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
SANDBOX_DIR = sandbox

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
SANDBOX_SRCS = $(wildcard $(SANDBOX_DIR)/*.c)
SANDBOX_BINS = $(SANDBOX_SRCS:$(SANDBOX_DIR)/%.c=$(BIN_DIR)/%)

all: dirs $(BUILD_DIR)/$(LIB_NAME) sandbox

sandbox: $(BUILD_DIR)/$(LIB_NAME) $(SANDBOX_BINS)

dirs:
	mkdir -p $(BUILD_DIR) $(BIN_DIR)

$(BUILD_DIR)/$(LIB_NAME): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/%: $(SANDBOX_DIR)/%.c $(BUILD_DIR)/$(LIB_NAME)
	$(CC) $(CFLAGS) $< -L$(BUILD_DIR) -lwayc -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

bear:
	bear -- make

.PHONY: all clean dirs sandbox