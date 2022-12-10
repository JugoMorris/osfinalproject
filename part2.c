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
    double start, end, deal_time;
    

    if (argc != 3) {
        panic("Usage: ./part2 <filename> <block_size>");
    }
    // get the filename, blocksize and blockcount from the argument
    char *filename = argv[1];
    unsigned long int size = strtoul(argv[2],0L, 10);
    
    // get the block_size and init block_count to 1
	unsigned int block_size = (unsigned int)size;
	unsigned int block_count = 1;

    while (1) {
        srandom(time(NULL));
        // see if it is a read or write mode.
        start = now();
        // open file failed, panic and return
        if ((fd = open(filename, O_RDONLY)) < 0) {
            panic("Can not open file!!!");
        }
        printf("File is opened correctly!\n");

        // run in read mode and find the file size
        read_mode(fd, block_size, block_count);
        close(fd);
        end = now();
        deal_time = end - start;
        printf("Here are the block_count: %u\n", block_count);
        printf("Finished file operation in %f seconds\n", deal_time);
        if (deal_time > 1) {
            
            break;
        }
        block_count *= 2;
    }
    return 0;
}