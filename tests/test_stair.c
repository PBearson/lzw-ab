#include "lzw_implementation.h"
#include "uploadData.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <cjson/cJSON.h>
#include "base64.h"

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("Please provide the input file\n");
        exit(1);
    }
    int struct_len = sizeof(struct uploadData);

    FILE* fp = fopen(argv[1], "r");
    const char* input_json = (const char*)malloc(1024);
    fread((char*)input_json, 1024, 1, fp);
    fclose(fp);

    cJSON *json = cJSON_Parse(input_json);

    if(json == NULL || json->child == NULL)
    {
        printf("JSON parse failure\n");
        return -1;
    }

    char* input_key = json->child->string;
    char* input_encoded = json->child->valuestring;

    if(input_key == NULL || input_encoded == NULL)
    {
        printf("JSON input has the wrong format\n");
        return -1;
    }
    
    int decoded_len = b64_decoded_size(input_encoded);
    unsigned char* input_decoded = (unsigned char*)malloc(decoded_len);

    int ret = b64_decode(input_encoded, input_decoded, decoded_len);
    if(!ret)
    {
        printf("Decode failere\n");
        return -1;
    }
    printf("Decode succeeded\n");

    struct uploadData* decoded_structed = (struct uploadData*)malloc(struct_len);
    string_to_uploadData(decoded_structed, input_decoded);

    // Convert GPS
    char gps_string[28];
    snprintf(gps_string, 28, "%.6f,%.6f", decoded_structed->lat, decoded_structed->lng);
    gps_string[27] = 0;

    // Convert timestamp
    time_t ts = decoded_structed->ts;
    struct tm lt = *gmtime(&ts);
    char ts_string[21];
    strftime(ts_string, 21, "%Y-%m-%dT%H:%M:%SZ", &lt);
    ts_string[20] = 0;

    // Convert tz
    char tz_string[4];
    snprintf(tz_string, 4, "%d", decoded_structed->tz);
    tz_string[3] = 0;

    // Convert UID
    char uid_string[33];
    int j = 0;
    for(int i = 0; i < 32; i+=2)
    {
        unsigned char tmp[2];
        memcpy(tmp, decoded_structed->serialNum + j, 1);

        sprintf(uid_string+i, "%.2X", tmp[0]);
        j++;
    }
    uid_string[32] = 0;
    
    printf("t1: %f\n", decoded_structed->avg_temp);
    printf("h1: %f\n", decoded_structed->avg_hum);
    printf("P251: %d\n", decoded_structed->pm2_5[0]);
    printf("P252: %d\n", decoded_structed->pm2_5[1]);
    printf("P253: %d\n", decoded_structed->pm2_5[2]);
    printf("P101: %d\n", decoded_structed->pm10[0]);
    printf("P102: %d\n", decoded_structed->pm10[1]);
    printf("P103: %d\n", decoded_structed->pm10[2]);
    printf("L: %s\n", gps_string);
    printf("ts: %s\n", ts_string);
    printf("TZ: %s\n", tz_string);
    printf("uid: %s\n", uid_string);

    return 0;
}