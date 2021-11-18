CC ?= gcc
CFLAGS ?= -Wall -g
SRC ?= src
TESTS ?= tests
FUZZING ?= fuzzing
INCLUDE ?= include
BUILD_CMD ?= mkdir -p $(BIN_DIR); $(CC) $(CFLAGS) -I $(INCLUDE) -o $(BIN_DIR)/$@.out $?

BIN_DIR ?= bin

SRC_TARGETS ?= lzwtester stair_compression
TEST_TARGETS ?= test_decompression_raw_decoded test_decompression_raw_encoded test_decompression_json_encoded
FUZZING_TARGETS ?= fuzz_decompression_json_encoded

LZW_IMPL_DEPS ?= $(SRC)/lzwlib.c $(SRC)/lzw_implementation.c

CJSON_DEPS ?= /home/ubuntu/Documents/cJSON/cJSON.c

default: $(SRC_TARGETS)

tests: $(TEST_TARGETS)

fuzzing: $(FUZZING_TARGETS)

all: $(SRC_TARGETS) $(TEST_TARGETS) $(FUZZING_TARGETS)

lzwtester: $(SRC)/lzwtester.c $(SRC)/lzwlib.c
	$(BUILD_CMD)

stair_compression: $(LZW_IMPL_DEPS) $(SRC)/stair_compression.c 
	$(BUILD_CMD)

test_decompression_raw_decoded: $(LZW_IMPL_DEPS) $(TESTS)/test_decompression_raw_decoded.c
	$(BUILD_CMD)

test_decompression_raw_encoded: $(LZW_IMPL_DEPS) $(SRC)/base64.c $(TESTS)/test_decompression_raw_encoded.c 
	$(BUILD_CMD)

test_decompression_json_encoded: $(LZW_IMPL_DEPS) $(CJSON_DEPS) $(SRC)/base64.c $(TESTS)/test_decompression_json_encoded.c
	$(BUILD_CMD)

fuzz_decompression_json_encoded: $(LZW_IMPL_DEPS) $(CJSON_DEPS) $(SRC)/base64.c $(FUZZING)/fuzz_decompression_json_encoded.c
	$(BUILD_CMD)

clean:
	rm $(BIN_DIR)/*.out