#include "lzw_implementation.h"
#include "uploadData.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    char* input = "073fb5524200ff00dee43286a679c77bf4eb0b00100018e0f76f5f463d857efffeed172a03410b8383cbd39f3f486888a8c8e80829496989a9c9e9092a4a6a8aaacaea0a2b4b0b26466686a6c6e606e73f000000000000000000000000000000004142434445464748494a4b4c4d4e4f505152535455565758595a303132333435363738";

    unsigned char* compressed = (unsigned char*)malloc(sizeof(struct uploadData));
    for(int i = 0; i < sizeof(struct uploadData); i++)
    {
        sscanf((const char*)input+(i*2), "%2hhx", &compressed[i]);
    }
}