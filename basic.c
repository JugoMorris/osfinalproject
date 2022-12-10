#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> // open
#include <sys/time.h>
#include "basic.h"

// used to measure the time
double now() {
    struct timeval tv;
    gettimeofday(&tv, 0);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

// error exit and print the error message
void panic(char *msg) {
    printf("%s\n", msg);
    exit(1);
}

// sample code from professor that used to compute the xor checksum
unsigned int xorbuf(unsigned int *buffer, int size) {
    unsigned int result = 0;
    
    for (int i = 0; i < size; i++) {
        result ^= buffer[i];
    }
    return result;
}

void read_mode(int fd, unsigned int block_size, unsigned int block_count) {

    // malloc block_size / 4 memory since we are reading unsigend int
    unsigned int *buffer;
    if (block_size < 4) {
        // if the block size is not 4 bytes, we just allocate 1 unsigned int to it
        buffer = (unsigned int*)malloc(1 * sizeof(unsigned int));
    } else {
        // otherwise, since it is multiple of 4 bytes, we have blocksize / 4 ints
        buffer = (unsigned int*)malloc(block_size * sizeof(char));
    }

    unsigned int readCount;
    unsigned int counter = 0;
    unsigned int checksum = 0;

    // read from the file until we have nothing to read
    while ((readCount = read(fd, buffer, block_size)) > 0 && counter < block_count) {
        counter++;
        if (readCount >= 4) {
            // if we have read more than 4 bytes, normal xor
            checksum = checksum ^ xorbuf(buffer, readCount / 4);
        } else {
            // if we have less than 4 bytes read, just calculate once.
            checksum = checksum ^ xorbuf(buffer, 1);
        }
    }

    printf("%08x\n", checksum);
}

void write_mode(int fd, unsigned int block_size, unsigned int block_count) {
    // malloc block_size of buffer to write
    char *sentence = (char *)malloc(block_size * sizeof(char));
    for (int i = 0; i < block_size; i++) {
        sentence[i] = 'a';
    }
    // write block_count times
    for (int i = 0; i < block_count; i++) {
        write(fd, sentence, (size_t)block_size);
    }
}

void lseek_mode(int fd, unsigned int block_size, unsigned int block_count) {
    // malloc block_size of buffer to write
    for (int i = 0; i < block_count; i++) {
        lseek(fd, i*block_size, SEEK_CUR);
    }
}