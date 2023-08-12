#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define UBST_DEV "/dev/unique_binary_search_tree"

int main()
{
    long long sz;

    char buf[1];
    char write_buf[] = "testing writing";
    int offset = 20;

    int fd = open(UBST_DEV, O_RDWR);
    if (fd < 0) {
        perror("Failed to open character device");
        exit(1);
    }

    for (int i = 0; i <= offset; i++) {
        sz = write(fd, write_buf, strlen(write_buf));
        printf("Writing to " UBST_DEV ", returned the sequence %lld\n", sz);
    }

    for (int i = 0; i <= offset; i++) {
    	//pointer change in lseek file -> f_pos = new_pos;
        lseek(fd, i, SEEK_SET);
        sz = read(fd, buf, 1);
        printf("Reading from " UBST_DEV
               " at offset %d, returned the sequence "
               "%lld.\n",
               i, sz);
    }

    for (int i = offset; i >= 0; i--) {
        lseek(fd, i, SEEK_SET);
        sz = read(fd, buf, 1);
        printf("Reading from " UBST_DEV
               " at offset %d, returned the sequence %lld.\n", i, sz);
    }

    close(fd);
    return 0;
}
