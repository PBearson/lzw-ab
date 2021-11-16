#include "lzw_implementation.h"
#include "uploadData.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int main()
{
    int len = sizeof(struct uploadData);

    char* input = "073fb5524200ff00dee43286a679c77bf4eb0b00100018e0f76f5f463d857efffeed172a03410b8383cbd39f3f486888a8c8e80829496989a9c9e9092a4a6a8aaacaea0a2b4b0b26466686a6c6e606e73f000000000000000000000000000000004142434445464748494a4b4c4d4e4f505152535455565758595a303132333435363738";

    char* check = "3b55242000000006f721943d3bce33d000000000000010002000300000000000000000000000000190000000a000000000000000000000000000000000000000000000000000000426520686170707900000000000000000000000000000000004142434445464748494a4b4c4d4e4f505152535455565758595a303132333435363738";

    unsigned char* compressed = (unsigned char*)malloc(len);
    bytearray_to_bin(compressed, input, len);

    unsigned char* decompressed = (unsigned char*)malloc(len);
    int ret = decompress_data(decompressed, compressed);
    if(ret != 0) return 0;

    unsigned char* decompressed_check = (unsigned char*)malloc(len);
    bytearray_to_bin(decompressed_check, check, len);

    assert(!strcmp((const char*)decompressed, (const char*)decompressed_check));
    

    

    return 0;
}