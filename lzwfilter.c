////////////////////////////////////////////////////////////////////////////
//                            **** LZW-AB ****                            //
//               Adjusted Binary LZW Compressor/Decompressor              //
//                  Copyright (c) 2016-2020 David Bryant                  //
//                           All Rights Reserved                          //
//      Distributed under the BSD Software License (see license.txt)      //
////////////////////////////////////////////////////////////////////////////


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <fcntl.h>
#endif

#include "lzwlib.h"

/* This module provides a command-line filter for testing the lzw library.
 * It can also optionally calculate and display the compression ratio and
 * a simple checksum for informational purposes. Other command-line
 * arguments select decoding mode or the maximum symbol size (9 to 16 bits)
 * for encoding.
 */

static const char *usage =
" Usage:     lzwfilter [-options] [< infile] [> outfile]\n\n"
" Operation: compression is default, use -d to decompress\n\n"
" Options:  -d     = decompress\n"
"           -h     = display this \"help\" message\n"
"           -1     = maximum symbol size = 9 bits\n"
"           -2     = maximum symbol size = 10 bits\n"
"           -3     = maximum symbol size = 11 bits\n"
"           -4     = maximum symbol size = 12 bits\n"
"           -5     = maximum symbol size = 13 bits\n"
"           -6     = maximum symbol size = 14 bits\n"
"           -7     = maximum symbol size = 15 bits\n"
"           -8     = maximum symbol size = 16 bits (default)\n"
"           -v     = verbose (display ratio and checksum)\n\n"
" Web:       Visit www.github.com/dbry/lzw-ab for latest version and info\n\n";

typedef struct {
    unsigned char buffer [65536];
    int checksum, head, tail;
    size_t byte_count;
} streamer;

static int read_buff (void *ctx)
{
    streamer *stream = ctx;
    int value;

    if (stream->head == stream->tail)
        stream->tail = (stream->head = 0) + fread (stream->buffer, 1, sizeof (stream->buffer), stdin);

    if (stream->head < stream->tail) {
        value = stream->buffer [stream->head++];
        stream->checksum = stream->checksum * 3 + (unsigned char) value;
        stream->byte_count++;
    }
    else
        value = EOF;

    return value;
}

static void write_buff (int value, void *ctx)
{
    streamer *stream = ctx;

    if (value == EOF) {
        fwrite (stream->buffer, 1, stream->head, stdout);
        return;
    }

    stream->buffer [stream->head++] = value;

    if (stream->head == sizeof (stream->buffer)) {
        fwrite (stream->buffer, 1, stream->head, stdout);
        stream->head = 0;
    }

    stream->checksum = stream->checksum * 3 + (unsigned char) value;
    stream->byte_count++;
}

int main (int argc, char **argv)
{
    int decompress = 0, maxbits = 16, verbose = 0, error = 0;
    streamer reader, writer;

    memset (&reader, 0, sizeof (reader));
    memset (&writer, 0, sizeof (writer));
    reader.checksum = writer.checksum = -1;

    while (--argc) {
        if ((**++argv == '-') && (*argv)[1])
            while (*++*argv)
                switch (**argv) {
                    case '1':
                        maxbits = 9;
                        break;

                    case '2':
                        maxbits = 10;
                        break;

                    case '3':
                        maxbits = 11;
                        break;

                    case '4':
                        maxbits = 12;
                        break;

                    case '5':
                        maxbits = 13;
                        break;

                    case '6':
                        maxbits = 14;
                        break;

                    case '7':
                        maxbits = 15;
                        break;

                    case '8':
                        maxbits = 16;
                        break;

                    case 'D': case 'd':
                        decompress = 1;
                        break;

                    case 'H': case 'h':
                        fprintf (stderr, "%s", usage);
                        return 0;
                        break;

                    case 'V': case 'v':
                        verbose = 1;
                        break;

                    default:
                        fprintf (stderr, "illegal option: %c !\n", **argv);
                        error = 1;
                        break;
                }
        else {
           fprintf (stderr, "unknown argument: %s\n", *argv);
           error = 1;
        }
    }

    if (error) {
        fprintf (stderr, "%s", usage);
        return 0;
    }

#ifdef _WIN32
    setmode (fileno (stdin), O_BINARY);
    setmode (fileno (stdout), O_BINARY);
#endif

    if (decompress) {
        if (lzw_decompress (write_buff, &writer, read_buff, &reader)) {
            fprintf (stderr, "lzw_decompress() returned non-zero!\n");
            return 1;
        }

        write_buff (EOF, &writer);
            
        if (verbose && writer.byte_count)
            fprintf (stderr, "output checksum = %x, ratio = %.2f%%\n", writer.checksum, reader.byte_count * 100.0 / writer.byte_count);
    }
    else {
        if (lzw_compress (write_buff, &writer, read_buff, &reader, maxbits)) {
            fprintf (stderr, "lzw_compress() returned non-zero!\n");
            return 1;
        }

        write_buff (EOF, &writer);

        if (verbose && reader.byte_count)
            fprintf (stderr, "source checksum = %x, ratio = %.2f%%\n", reader.checksum, writer.byte_count * 100.0 / reader.byte_count);
    }

    return 0;
}
