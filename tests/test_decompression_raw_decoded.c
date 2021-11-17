#include "lzw_implementation.h"
#include "uploadData.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int main()
{
    int len = sizeof(struct uploadData);

    FILE* fp = fopen("inputs/test_raw_decoded_compressed", "r");
    unsigned char* input = (unsigned char*)malloc(len);
    fread(input, len, 1, fp);
    fclose(fp);

    unsigned char* decompressed = (unsigned char*)malloc(len);
    int ret = decompress_data(decompressed, input);
    if(ret != 0)
    {
        printf("Decompression failed\n");
        return -1;
    }
    printf("Decompression succeeded.\n");

    struct uploadData* decompressed_struct = (struct uploadData*)malloc(len);
    string_to_uploadData(decompressed_struct, decompressed);

    printf("Serial: %s\n", decompressed_struct->serialNum);

    return 0;
}