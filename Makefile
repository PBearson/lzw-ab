CC ?= gcc
CFLAGS ?= -Wall -g
SRC ?= src
TESTS ?= tests
INCLUDE ?= include

SRC_TARGETS ?= lzwtester stair_compression
TEST_TARGETS ?= test_decompression_raw_decoded

default: $(SRC_TARGETS)

tests: $(TEST_TARGETS)

all: $(SRC_TARGETS) $(TEST_TARGETS)

lzwtester: $(SRC)/lzwtester.c $(SRC)/lzwlib.c
	$(CC) $(CFLAGS) -I $(INCLUDE) -o $@.out $?

stair_compression: $(SRC)/lzwlib.c $(SRC)/lzw_implementation.c $(SRC)/stair_compression.c 
	$(CC) $(CFLAGS) -I $(INCLUDE) -o $@.out $?

test_decompression_raw_decoded: $(SRC)/lzwlib.c $(SRC)/lzw_implementation.c $(TESTS)/test_decompression_raw_decoded.c
	$(CC) $(CFLAGS) -I $(INCLUDE) -o $@.out $?

clean:
	rm *.out