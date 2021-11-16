////////////////////////////////////////////////////////////////////////////
//                            **** LZW-AB ****                            //
//               Adjusted Binary LZW Compressor/Decompressor              //
//                  Copyright (c) 2016-2020 David Bryant                  //
//                           All Rights Reserved                          //
//      Distributed under the BSD Software License (see license.txt)      //
////////////////////////////////////////////////////////////////////////////

#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "uploadData.h"
#include "lzw_implementation.h"

// Convert uploadData struct to string
// We assume buf is filled and data is allocated.
void uploadData_to_string(char* data, struct uploadData *buf)
{
    memcpy(data, buf, sizeof(struct uploadData));
}

// Convert string to uploadData struct
// We assume data is filled and buf is allocated
void string_to_uploadData(struct uploadData *buf, char* data)
{
    memcpy(buf, data, sizeof(struct uploadData));
}

// Wrapper function that can get the raw bytes of the decompressed buffer
int decompress_data(char* data, unsigned char* buffer)
{
    struct uploadData *data_struct = (struct uploadData*)malloc(sizeof(struct uploadData));
    int ret = decompress(data_struct, buffer);
    if(ret != 0) return ret;
    
    uploadData_to_string(data, data_struct);

    return 0;
}

// Print a (presumably binary) string as a byte array
void print_string_as_bytearray(unsigned char* data, int len)
{
    for(int i = 0; i < len; i++)
    {
        printf("%.2x", data[i]);
    }
    printf("\n");
}

// Print uploadData as a byte array
void print_uploadData_as_bytearray(struct uploadData* buf)
{
    int len = sizeof(struct uploadData);
    char* data = (char*)malloc(len);
    uploadData_to_string(data, buf);

    print_string_as_bytearray((unsigned char*)data, len);
}

int main (int argc, char **argv)
{
    struct uploadData* data = (struct uploadData*)malloc(sizeof(struct uploadData));
    data->avg_hum = 153.447;
    data->avg_temp = 52.677;
    data->bc = 10;
    data->bv = 25;
    data->pm10[0] = 1;
    data->pm10[1] = 2;
    data->pm10[2] = 3;
    data->scd30_ppm = 0.1112;
    data->ts[0] = 'B';
    data->ts[1] = 'e';
    data->ts[2] = ' ';
    data->ts[3] = 'h';
    data->ts[4] = 'a';
    data->ts[5] = 'p';
    data->ts[6] = 'p';
    data->ts[7] = 'y';
    strcpy(data->serialNum, "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

    printf("Raw:\n");
    print_uploadData_as_bytearray(data);
    printf("\n");

    FILE* fp = fopen("raw.txt", "wb");
    fwrite(data, sizeof(struct uploadData), 1, fp);
    fclose(fp);
    
    int maxbits = 16; // configure the maximum symbol size (9-16)
    char* buffer = (char*)malloc(sizeof(struct uploadData));
    if(compress(data, (unsigned char*)buffer, sizeof(struct uploadData), maxbits))
        return 1;
    printf("Data compression is finifshed.\n\n");

    printf("Compressed:\n");
    print_string_as_bytearray((unsigned char*)buffer, sizeof(struct uploadData));
    printf("\n");

    fp = fopen("compressed.txt", "wb");
    fwrite(buffer, sizeof(struct uploadData), 1, fp);
    fclose(fp);
    
    struct uploadData* de_data = (struct uploadData*)malloc(sizeof(struct uploadData));
    if(decompress(de_data, (unsigned char*)buffer))
        return 1;
    printf("Data decompression is finifshed.\n\n");

    printf("Decompressed:\n");
    print_uploadData_as_bytearray(de_data);
    printf("\n");

    fp = fopen("decompressed.txt", "wb");
    fwrite(de_data, sizeof(struct uploadData), 1, fp);
    fclose(fp);
    
    printf("avg_hum: %E\n",de_data->avg_hum);
    printf("avg_press: %E\n",de_data->avg_press);
    printf("avg_temp: %E\n",de_data->avg_temp);
    printf("scd30_ppm: %E\n",de_data->scd30_ppm);
    printf("pm2_5[3]: ");
    for(int i=0;i<3;i++){
        printf("%d",de_data->pm2_5[i]);
    }
    printf("\n");
    printf("pm10[3]: ");
    for(int i=0;i<3;i++){
        printf("%d",de_data->pm10[i]);
    }
    printf("\n");
    printf("pv: %E\n",de_data->pv);
    printf("pc: %E\n",de_data->pc);
    printf("pp: %E\n",de_data->pp);
    printf("bv: %d\n",de_data->bv);
    printf("bc: %d\n",de_data->bc);
    printf("bt: %d\n",de_data->bt);
    printf("gps[20]: %s\n",de_data->gps);
    printf("ts[21]: %s\n",de_data->ts);
    printf("tz[4]: %s\n",de_data->tz);
    printf("serialNum[33]: %c\n",de_data->serialNum[32]);//printf("serialNum[33]: %s\n",de_data->serialNum);
    return 0;
}
