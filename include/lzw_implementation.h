#include "uploadData.h"

/* function provided by author*/
typedef struct {
    unsigned int size, index, wrapped;
    unsigned char *buffer;
} streamer;

int read_buff(void*);
void write_buff(int, void*);
int compress(struct uploadData*, unsigned char*, int, int);
int decompress(struct uploadData*, unsigned char*);
void uploadData_to_string(unsigned char*, struct uploadData*);
void string_to_uploadData(struct uploadData*, unsigned char*);
int decompress_data(unsigned char*, unsigned char*);
void print_string_as_bytearray(unsigned char*, int);
void print_uploadData_as_bytearray(struct uploadData*);
void bytearray_to_bin(unsigned char*, char*, int);