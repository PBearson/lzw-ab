/* function provided by author*/
#include "lzw_implementation.h"
#include "lzwlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read_buff (void *ctx)
{
    streamer *stream = (streamer*)ctx;
    
    if (stream->index == stream->size){
        return EOF;
    }
    
    return stream->buffer [stream->index++];
}

/* function provided by author*/
void write_buff (int value, void *ctx)
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