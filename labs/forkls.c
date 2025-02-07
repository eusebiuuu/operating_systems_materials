#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t curr_pid = fork();
    if (curr_pid != 0) {
        printf("My PID = %d\tChild PID = %d\n", getpid(), curr_pid);
        char* const argv[] = {"ls", NULL};
        execve("/usr/bin/ls", argv, NULL);
        // wait(curr_pid);
    } else {
        printf("Child %d finished\n", getpid());
    }
    return 0;
}