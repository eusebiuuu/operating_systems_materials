#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char **argv) {
    struct stat sb;
    char buffer[1000];
    if (stat(argv[1], &sb) < 0) {
        return errno;
    }

    int desc_read = open(argv[1], O_RDWR);
    int desc_write = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);

    int read_bytes = 0, written_bytes = 0, tot_size = sb.st_size;
    while (read_bytes < tot_size || written_bytes < tot_size) {
        int nread = read(desc_read, buffer + read_bytes, tot_size - read_bytes);
        if (nread < 0) {
            printf("%d", errno);
            return errno;
        }
        read_bytes += nread;
        
        int nwritten = write(desc_write, buffer + written_bytes, tot_size - written_bytes);
        if (nwritten < 0) {
            printf("%d", errno);
            return errno;
        }
        written_bytes += nwritten;
    }

    close(desc_read);
    close(desc_write);
    return 0;
}
