/*
	Credit: John (john@nachtimwald.com)
	https://nachtimwald.com/2017/11/18/base64-encode-and-decode-in-c/
*/

#pragma once

#include <stdlib.h>


extern int b64invs[];

extern const char b64chars[];

void b64_generate_decode_table();
size_t b64_decoded_size(const char *in);
int b64_isvalidchar(char c);
int b64_decode(const char *in, unsigned char *out, size_t outlen);