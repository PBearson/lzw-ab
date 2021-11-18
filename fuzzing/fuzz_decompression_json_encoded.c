#include "lzw_implementation.h"
#include "uploadData.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <cjson/cJSON.h>
#include "base64.h"

int main(int argc, char* argv[1])
{
    int struct_len = sizeof(struct uploadData);
    if(argc < 2) return -1;

    FILE* fp = fopen(argv[1], "r");
    const char* input_json = (const char*)malloc(1024);
    fread((char*)input_json, 1024, 1, fp);
    fclose(fp);

    cJSON *json = cJSON_Parse(input_json);

    char* input_encoded = json->child->valuestring;

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