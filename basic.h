// timer
double now(void);

// error message
void panic(char *msg);

// sample from professor that used to compute the xor checksum
unsigned int xorbuf(unsigned int *buffer, int size);

// read mode
void read_mode(int fd, unsigned int block_size, unsigned int block_count);
// write mode
void write_mode(int fd, unsigned int block_size, unsigned int block_count);

void lseek_mode(int fd, unsigned int block_size, unsigned int block_count);