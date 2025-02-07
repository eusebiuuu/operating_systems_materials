#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>

void collatz(char *curr_num) {
    int num = atoi(curr_num);
    printf("%d: ", num);
    while (num > 1) {
        printf("%d ", num);
        if (num & 1) {
            num = 3 * num + 1;
        } else {
            num /= 2;
        }
    }
    printf("1\n");
}

int main(int argc, char *argv[]) {
    printf("Starting parent %d\n", getpid());
    for (int i = 1; i < argc; ++i) {
        pid_t curr_pid = fork();
        if (curr_pid == 0) {
            collatz(argv[i]);
            printf("Child %d of parent %d finished\n", getpid(), getppid());
            exit(0);
        } else if (curr_pid < 0) {
            perror("Didn't fork properly\n");
            return errno;
        }
    }
    for (int i = 1; i < argc; ++i) {
        printf("%d\n", getpid());
        wait(NULL);
    }
    printf("Parent %d finished\n", getpid());
    return 0;
}