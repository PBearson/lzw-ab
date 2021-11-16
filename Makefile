CC ?= gcc
CFLAGS ?= -Wall -g

default: main

main: lzwtester.c lzwlib.c
	$(CC) $(CFLAGS) -o lzw.out $?