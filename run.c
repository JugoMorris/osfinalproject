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
    double raw_performance,file_size;
    

    if (argc != 5) {
        panic("Usage: ./run <filename> [-r|-w|-s] <block_size> <block_count>");
    }
    // get the filename, blocksize and blockcount from the argument
    char *filename = argv[1];
    unsigned long int size = strtoul(argv[3],0L, 10);
    unsigned long int count = strtoul(argv[4],0L, 10);
    
    /* If the result is 0, test for an error */
    if (size == 0 || count == 0)
    {
    	panic("Convert error! Please check block_size and block_count to avoid out of state");
    }
	unsigned int block_size = (unsigned int)size;
	unsigned int block_count = (unsigned int)count;

    srandom(time(NULL));
    // check the flag and run read, write or lseek mode
    start = now();
    if ((strcmp(argv[2], "-r") == 0)) {
        // open file failed, panic and return
        if ((fd = open(filename, O_RDONLY)) < 0) {
            panic("Can not open file!!!");
        }
        printf("File is opened correctly!\n");

        // run in read mode
        read_mode(fd, block_size, block_count);
        
    } else if ((strcmp(argv[2], "-w") == 0)) {
        // open file failed, panic and return
        if ((fd = open(filename, O_RDWR | O_APPEND | O_CREAT, S_IRWXU)) < 0) {
            panic("Can not open file!!!");
        }
        printf("File is opened correctly!\n");
        // run in write mode
        write_mode(fd, block_size, block_count);
    } else if ((strcmp(argv[2], "-s") == 0)) {
        // open file failed, panic and return
        if ((fd = open(filename, O_RDONLY)) < 0) {
            panic("Can not open file!!!");
        }
        printf("File is opened correctly!\n");
        // run in lseek mode
        lseek_mode(fd, block_size, block_count);
    }
    close(fd);
    end = now();
    wall_time = end - start;
    printf("Finished file operation in %f seconds\n", wall_time);
    file_size = (double) block_size * block_count;
    raw_performance = file_size/wall_time/1048576;
    
    printf("Read speed: %.2f MiB/s\n", raw_performance);
    printf("Number of system calls per second: %.2f B/s\n", block_count/wall_time);
    return 0;
}
