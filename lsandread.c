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

    int fd;
    double start, end, wall_time1, wall_time2;
    

    if (argc != 3) {
        panic("Usage: ./lsandread <filename> <readsize>");
    }

    char *filename = argv[1];
    unsigned long int size = strtoul(argv[2],0L, 10);

    char *newfilename = "lscompareread";
	unsigned int block_size = 1;
	unsigned int block_count = (unsigned int)size;

    FILE* datafd;
    if ((datafd = fopen(newfilename, "w+")) < 0) {
        panic("Can not open file!");
    }
    unsigned int bestblock = 0;
    float besttime = 1000000000.0;
    while (1) {
        if(block_count < 1) break;

// GET THE READ SPEED
        srandom(time(NULL));
        start = now();
        if ((fd = open(filename, O_RDONLY)) < 0) {
            panic("Can not open file!!!");
        }
        printf("File is opened correctly!\n");
        read_mode(fd, block_size, block_count);
        close(fd);
        end = now();
        wall_time1 = end - start;
        printf("Read: Here are the blocksize: %u blockcount: %u\n", block_size, block_count);
        printf("Read: Finished file operation in %f seconds\n", wall_time1);
        printf("Read speed: %.2f B/s\n", block_count/wall_time1);

// GET THE LSEEK SPEED
        srandom(time(NULL));
        start = now();
        if ((fd = open(filename, O_RDONLY)) < 0) {
            panic("Can not open file!!!");
        }
        printf("File is opened correctly!\n");
        lseek_mode(fd, block_size, block_count);
        close(fd);
        end = now();
        wall_time2 = end - start;
        printf("Lseek: Here are the blocksize: %u blockcount: %u\n", block_size, block_count);
        printf("Lseek: Finished file operation in %f seconds\n", wall_time2);
        printf("Read speed: %.2f B/s\n", block_count/wall_time2);

        fprintf(datafd, "%d, %02f, %02f\n", block_count, wall_time1, wall_time2);

        block_size *= 2;
        block_count /= 2;
    }
    fclose(datafd);
    printf("\nFinished comparison between lseek and read\n");
    return 0;
}
