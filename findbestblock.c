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
    

    if (argc != 3) {
        panic("Usage: ./findbestblock <filename> <readsize>");
    }
    // get the filename, blocksize and blockcount from the argument
    char *filename = argv[1];
    unsigned long int size = strtoul(argv[2],0L, 10);
    
// GENERAL TEST: blocksize range is 1 - 1073741824, step length: mutiply 2
    char *newfilename = "bstospeed1";
	unsigned int block_size = 1;
	unsigned int block_count = (unsigned int)size;
// PRECISE TEST: blocksize range is 1048576 - 4194304, per iteration: around 0.6 second, step length: 10000 => 1min
    // char *newfilename = "bstospeed2";
    // unsigned int block_size = 1048576;
	// unsigned int block_count = (unsigned int)size/block_size + 1;


    FILE* datafd;
    if ((datafd = fopen(newfilename, "w+")) < 0) {
        panic("Can not open file!!!");
    }
    unsigned int bestblock = 0;
    float besttime = 1000000000.0;
    while (1) {
        // if(block_count < 1) break;
        if(block_size > 4194304) break;
        srandom(time(NULL));
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
        wall_time = end - start;
        printf("Here are the blocksize: %u blockcount: %u\n", block_size, block_count);
        printf("Finished file operation in %f seconds\n", wall_time);
        float raw_performance = size/wall_time/1048576;
        printf("Read speed: %.2f MiB/s\n", raw_performance);
        if (wall_time < besttime) {
            besttime = wall_time;
            bestblock = block_size;
        }
        fprintf(datafd, "%d,%02f\n", block_size, wall_time);

//GENERAL TEST
        block_size *= 2;
        block_count /= 2;
//PRECISE TEST        
        // block_size += 10000;
        // block_count = size/block_size + 1;
    }
    fclose(datafd);
    printf("\nThe best block size is %u, used %f seconds\n", bestblock, besttime);
    return 0;
}
