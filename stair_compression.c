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

#include "lzwlib.h"

#include "uploadData.h"

/* function provided by author*/
typedef struct {
    unsigned int size, index, wrapped;
    unsigned char *buffer;
} streamer;

/* function provided by author*/
static int read_buff (void *ctx)
{
    streamer *stream = (streamer*)ctx;
    
    if (stream->index == stream->size){
        return EOF;
    }
    
    return stream->buffer [stream->index++];
}

/* function provided by author*/
static void write_buff (int value, void *ctx)
{
    streamer *stream = (streamer*)ctx;
    
    if (stream->index == stream->size) {
        stream->index = 0;
        stream->wrapped++;
    }
    
    stream->buffer [stream->index++] = value;
}

/* fuction customized according to lzw_compress in lzwlib*/
int compress(struct uploadData *data, unsigned char *buffer, int buffer_size, int maxbits)
{
    int errors = 0;
    streamer reader, writer;
    
    memset (&reader, 0, sizeof (reader));
    memset (&writer, 0, sizeof (writer));
    
    long long struct_size = sizeof(*data);
    
    if(struct_size > 1024LL * 1024LL * 1024LL){
        printf("\nstruct is too big!\n");
        errors++;
        return errors;
    }
    
    reader.buffer = (unsigned char*)malloc(struct_size);
    reader.size = (unsigned int)struct_size;
    writer.size = (unsigned int)struct_size;
    writer.buffer = (unsigned char*)malloc (writer.size);
    
    if (!writer.buffer || !reader.buffer) {
        printf ("\nstruct is too big!\n");
        if (writer.buffer) free (writer.buffer);
        if (reader.buffer) free (reader.buffer);
        errors++;
        return errors;
    }
    
    memcpy(reader.buffer, data, struct_size);
    reader.index = writer.index = writer.wrapped = 0;
    
    if (lzw_compress (write_buff, &writer, read_buff, &reader, maxbits)) {
        printf ("\nlzw_compress() returned an error, maxbits = %d\n", maxbits);
        free (writer.buffer);
        free (reader.buffer);
        errors++;
        return errors;
    }
    
    if (writer.wrapped) {
        printf ("\nover 100%% inflation on struct, maxbits = %d!\n", maxbits);
        free (writer.buffer);
        free (reader.buffer);
        errors++;
        return errors;
    }
    
    if(buffer_size < writer.index){
        printf("compress(): buffer is not large enough, buffer is %d bytes,compressed data is %u bytes\n", buffer_size, writer.index);
        free (writer.buffer);
        free (reader.buffer);
        errors++;
        return errors;
    }
    
    for(int i=0;i<writer.index;i++){
        buffer[i] = writer.buffer[i];
    }
    
    free(reader.buffer);
    free(writer.buffer);
    return errors;
}


/* fuction customized according to lzw_decompress in lzwlib*/
int decompress(struct uploadData *data, unsigned char *buffer)
{
    int errors = 0;
    streamer reader, writer;
    
    memset (&reader, 0, sizeof (reader));
    memset (&writer, 0, sizeof (writer));
    
    long long struct_size = sizeof(*data);
    
    reader.buffer = buffer;
    reader.size = (unsigned int)struct_size;
    writer.size = (unsigned int)(struct_size*2 + 10);
    writer.buffer = (unsigned char*)malloc (writer.size);
    
    if (!writer.buffer || !reader.buffer) {
        printf ("\nstruct is too big!\n");
        if (writer.buffer) free (writer.buffer);
        errors++;
        return errors;
    }
    
    reader.index = writer.index = writer.wrapped = 0;
    
    if(lzw_decompress(write_buff, &writer, read_buff, &reader)){
        printf ("lzw_decompress() returned an error\n");
        free (writer.buffer);
        errors++;
        return errors;
    }
    
    if(writer.wrapped){
        printf("decompression generated %u extra bytes.\n", writer.wrapped);
        free (writer.buffer);
        errors++;
        return errors;
    }
    
    if(writer.index != struct_size){
        printf("warning: deconpressed data is larger than expected.\n");
    }
    
    memcpy(data, writer.buffer, struct_size);
    free(writer.buffer);
    return errors;
}

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
void print_string_as_bytearray(char* data, int len)
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

    for(int i = 0; i < len; i++)
    {
        printf("%.2x", data[i]);
    }
    printf("\n");
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
    print_string_as_bytearray(buffer, sizeof(struct uploadData));
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
