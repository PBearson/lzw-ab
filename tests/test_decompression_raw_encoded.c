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

    FILE* fp = fopen("inputs/test_raw_encoded_compressed", "r");
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

    printf("t1: %f\n", decompressed_struct->avg_temp);
    printf("p1: %f\n", decompressed_struct->avg_press);
    printf("h1: %f\n", decompressed_struct->avg_hum);
    printf("c1: %f\n", decompressed_struct->scd30_ppm);
    printf("P251: %d\n", decompressed_struct->pm2_5[0]);
    printf("P252: %d\n", decompressed_struct->pm2_5[1]);
    printf("P253: %d\n", decompressed_struct->pm2_5[2]);
    printf("P101: %d\n", decompressed_struct->pm10[0]);
    printf("P102: %d\n", decompressed_struct->pm10[1]);
    printf("P103: %d\n", decompressed_struct->pm10[2]);
    printf("pv: %f\n", decompressed_struct->pv);
    printf("pc: %f\n", decompressed_struct->pc);
    printf("pp: %f\n", decompressed_struct->pp);
    printf("bv: %d\n", decompressed_struct->bv);
    printf("bc: %d\n", decompressed_struct->bv);
    printf("pt: %d\n", decompressed_struct->bt);
    printf("L: %s\n", decompressed_struct->gps);
    printf("ts: %s\n", decompressed_struct->ts);
    printf("TZ: %s\n", decompressed_struct->tz);
    printf("uid: %s\n", decompressed_struct->serialNum);

    return 0;
}