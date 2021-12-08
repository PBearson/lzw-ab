#include "lzw_implementation.h"
#include "uploadData.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "base64.h"

int main()
{
    int struct_len = sizeof(struct uploadData);

    FILE* fp = fopen("inputs/test_raw_encoded_compressed_revised", "r");
    const char* input_encoded = (const char*)malloc(1024);
    fread((char*)input_encoded, 1024, 1, fp);
    fclose(fp);

    int decoded_len = b64_decoded_size(input_encoded);
    unsigned char* input_decoded = (unsigned char*)malloc(decoded_len);

    int ret = b64_decode(input_encoded, input_decoded, decoded_len);
    if(!ret)
    {
        printf("Decode failere\n");
        return -1;
    }
    printf("Decode succeeded\n");

    unsigned char* decompressed = (unsigned char*)malloc(struct_len);
    ret = decompress_data(decompressed, input_decoded);
    if(ret != 0)
    {
        printf("Decompression failed\n");
        return -1;
    }
    printf("Decompression succeeded.\n");

    struct uploadData* decompressed_struct = (struct uploadData*)malloc(struct_len);
    string_to_uploadData(decompressed_struct, decompressed);

    printf("Serial: %s\n", decompressed_struct->serialNum);

    return 0;
}