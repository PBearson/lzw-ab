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

    // Convert GPS
    char gps_string[28];
    sprintf(gps_string, "%f,%f", decompressed_struct->lat, decompressed_struct->lng);
    gps_string[19] = 0;

    // Convert timestamp
    char year_string[5] = "20";
    strncpy(year_string + 2, decompressed_struct->ts, 2);
    year_string[4] = 0;

    char month_string[3];
    strncpy(month_string, decompressed_struct->ts + 2, 2);
    month_string[2] = 0;

    char day_string[3];
    strncpy(day_string, decompressed_struct->ts + 4, 2);
    day_string[2] = 0;

    char hour_string[3];
    strncpy(hour_string, decompressed_struct->ts + 6, 2);
    hour_string[2] = 0;

    char minute_string[3];
    strncpy(minute_string, decompressed_struct->ts + 8, 2);
    minute_string[2] = 0;

    char second_string[3];
    strncpy(second_string, decompressed_struct->ts + 10, 2);
    second_string[2] = 0;

    char ts_string[21];
    sprintf(ts_string, "%s-%s-%sT%s:%s:%sZ", year_string, month_string, day_string, hour_string, minute_string, second_string);
    ts_string[20] = 0;

    // Convert tz
    char tz_string[4];
    sprintf(tz_string, "%d", decompressed_struct->tz);
    tz_string[3] = 0;

    // Convert UID
    char uid_string[33];
    int j = 0;
    for(int i = 0; i < 32; i+=2)
    {
        unsigned char tmp[2];
        memcpy(tmp, decompressed_struct->serialNum + j, 1);

        sprintf(uid_string+i, "%.2X", tmp[0]);
        j++;
    }
    uid_string[32] = 0;
    
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
    printf("L: %s\n", gps_string);
    printf("ts: %s\n", ts_string);
    printf("TZ: %s\n", tz_string);
    printf("uid: %s\n", uid_string);

    return 0;
}