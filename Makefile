CC ?= gcc
CFLAGS ?= -Wall -g
SRC ?= src
TESTS ?= tests
INCLUDE ?= include
BUILD_CMD ?= mkdir -p $(BIN_DIR); $(CC) $(CFLAGS) -I $(INCLUDE) -o $(BIN_DIR)/$@.out $?

BIN_DIR ?= bin

TEST_TARGETS ?= test_stair_decompression

LZW_IMPL_DEPS ?= $(SRC)/lzwlib.c $(SRC)/lzw_implementation.c

CJSON_DEPS ?= /home/ubuntu/Documents/cJSON/cJSON.c

BASE64_DEPS ?= $(SRC)/base64.c

default: all

tests: $(TEST_TARGETS)

all: $(TEST_TARGETS)

test_stair_decompression: $(LZW_IMPL_DEPS) $(CJSON_DEPS) $(BASE64_DEPS) $(TESTS)/test_stair_decompression.c
	$(BUILD_CMD)

clean:
	rm $(BIN_DIR)/*.out