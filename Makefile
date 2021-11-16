CC ?= gcc
CFLAGS ?= -Wall -g
SRC ?= src
INCLUDE ?= include

all: lzwtester stair_compression test_decompression

lzwtester: $(SRC)/lzwtester.c $(SRC)/lzwlib.c
	$(CC) $(CFLAGS) -I $(INCLUDE) -o lzwtester.out $?

stair_compression: $(SRC)/lzwlib.c $(SRC)/lzw_implementation.c $(SRC)/stair_compression.c 
	$(CC) $(CFLAGS) -I $(INCLUDE) -o stair_compression.out $?

test_decompression: $(SRC)/lzwlib.c $(SRC)/lzw_implementation.c $(SRC)/test_decompression.c
	$(CC) $(CFLAGS) -I $(INCLUDE) -o test_decompression.out $?

clean:
	rm *.out