CC ?= gcc
CFLAGS ?= -Wall -g
SRC ?= src
TESTS ?= tests
INCLUDE ?= include
BUILD_CMD ?= $(CC) $(CFLAGS) -I $(INCLUDE) -o $@.out $?

SRC_TARGETS ?= lzwtester stair_compression
TEST_TARGETS ?= test_decompression_raw_decoded test_decompression_raw_encoded

LZW_IMPL_DEPS ?= $(SRC)/lzwlib.c $(SRC)/lzw_implementation.c

default: $(SRC_TARGETS)

tests: $(TEST_TARGETS)

all: $(SRC_TARGETS) $(TEST_TARGETS)

lzwtester: $(SRC)/lzwtester.c $(SRC)/lzwlib.c
	$(BUILD_CMD)

stair_compression: $(LZW_IMPL_DEPS) $(SRC)/stair_compression.c 
	$(BUILD_CMD)

test_decompression_raw_decoded: $(LZW_IMPL_DEPS) $(TESTS)/test_decompression_raw_decoded.c
	$(BUILD_CMD)

test_decompression_raw_encoded: $(LZW_IMPL_DEPS) $(SRC)/base64.c $(TESTS)/test_decompression_raw_encoded.c 
	$(BUILD_CMD)

clean:
	rm *.out