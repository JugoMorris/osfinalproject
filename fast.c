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
    double start, end, real_time;
    

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
    unsigned int block_size = 1048576;
    unsigned int readCount = 0;
    unsigned int checksum = 0;

    // init buffer
    unsigned int *buffer = (unsigned int*)malloc((block_size / 4) * sizeof(unsigned int));

    while ((readCount = read(fd, buffer, block_size)) > 0) {
        checksum = checksum ^ xorbuf(buffer, readCount / 4);
    }
    // close the file
    close(fd);
    end = now();
    real_time = end - start;
    printf("%08x\n", checksum);
    printf("Finished file operation in %f seconds\n", real_time);
    return 0;
}