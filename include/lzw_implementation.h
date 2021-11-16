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