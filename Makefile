CC ?= gcc
CFLAGS ?= -Wall -g

all: lzwtester stair_compression

lzwtester: lzwtester.c lzwlib.c
	$(CC) $(CFLAGS) -o lzwtester.out $?

stair_compression: lzwlib.c lzw_implementation.c stair_compression.c 
	$(CC) $(CFLAGS) -o stair_compression.out $?

clean:
	rm *.out