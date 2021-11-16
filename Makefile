CC ?= gcc
CFLAGS ?= -Wall -g

lzwtester: lzwtester.c lzwlib.c
	$(CC) $(CFLAGS) -o lzwtester.out $?