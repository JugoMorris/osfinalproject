#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> // open
#include <sys/time.h>

#include "basic.h"


int main(int argc, char *argv[]) {

    int fd; // the file descriptor
    double start, end, wall_time;
    

    if (argc != 2) {
        panic("Usage: ./run <filename>");
    }
    // get the filename from argument
    char *filename = argv[1];

    srandom(time(NULL));
    start = now();

    // open file failed, panic and return
    if ((fd = open(filename, O_RDONLY)) < 0) {
        panic("Can not open file!!!");
    }
    printf("File is opened correctly!\n");

    // use the fastest block_size we found
    unsigned int block_size = 1228576;
    unsigned int bytes_read = 0;
    unsigned int file_size = 0;
    unsigned int checksum = 0;

    // init buffer
    unsigned int *buffer = (unsigned int*)malloc((block_size / 4) * sizeof(unsigned int));

    while ((bytes_read = read(fd, buffer, block_size)) > 0) {
        file_size += bytes_read;
        checksum = checksum ^ xorbuf(buffer, bytes_read / 4);
    }
    // close the file
    close(fd);
    end = now();
    wall_time = end - start;
    printf("XOR result: %08x\n", checksum);
    printf("Block size: %d\n", block_size);
    printf("Read speed: %.2f MiB/s\n", file_size/wall_time/1048576);
    printf("Finished file operation in %f seconds\n", wall_time);
    return 0;
}