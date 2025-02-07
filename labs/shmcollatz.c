#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>

int const N = 100, NUM = 10;

void collatz(char *curr_num, char *address, int offset) {
    int num = atoi(curr_num), pos = offset;
    char buff[NUM];

    sprintf(buff, "%d", num);
    for (int i = 0; buff[i] != '\0'; ++i) {
        address[pos++] = buff[i];
    }
    address[pos++] = ':';
    address[pos++] = ' ';

    while (num > 1) {
        sprintf(buff, "%d", num);
        for (int i = 0; buff[i] != '\0'; ++i) {
            address[pos++] = buff[i];
        }
        address[pos++] = ',';
        address[pos++] = ' ';

        if (num & 1) {
            num = 3 * num + 1;
        } else {
            num >>= 1;
        }
    }

    address[pos++] = '1';
    address[pos++] = '.';
    address[pos++] = '\n';
    address[pos++] = '\0';
}


int main(int argc, char *argv[]) {
    int const SZ = getpagesize();
    char shm_name[] = "shm_collatz";
    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (shm_fd < 0) {
        perror(NULL);
        return errno;
    }
    size_t shm_size = argc * SZ;

    if (ftruncate(shm_fd, shm_size) == -1) {
        perror(NULL);
        shm_unlink(shm_name);
        return errno;
    }

    char *shm_ptr = mmap(0, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror(NULL);
        shm_unlink(shm_name);
        return errno;
    }

    printf("Starting parent %d\n", getpid());
    for (int i = 1; i < argc; ++i) {
        pid_t curr_pid = fork();
        if (curr_pid == 0) {
            collatz(argv[i], shm_ptr, SZ * (i - 1));
            printf("Child %d of parent %d finished\n", getpid(), getppid());
            exit(0);
        } else if (curr_pid < 0) {
            perror("Didn't fork properly\n");
            return errno;
        }
    }
    for (int i = 1; i < argc; ++i) {
        // printf("%d\n", getpid());
        wait(NULL);
    }
    for (int i = 1; i < argc; ++i) {
        printf("%s", shm_ptr + (i - 1) * SZ);
    }
    printf("Parent %d finished\n", getpid());
    munmap(shm_ptr, shm_size);
    shm_unlink(shm_name);
    return 0;
}