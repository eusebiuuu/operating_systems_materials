#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *routine(void *arg) {
    int arg_int = *((int*) arg);
    printf("Heeyy %d\n", arg_int);
    free(arg);
}

int main() {
    int const THREADS = 10;
    pthread_t threads[THREADS];

    for (int i = 0; i < THREADS; ++i) {
        int *arg = malloc(sizeof(int));
        *arg = i;
        pthread_create(&threads[i], NULL, routine, arg);
        pthread_join(threads[i], NULL);
    }

    // for (int i = THREADS - 1; i >= 0; --i) {
    //     pthread_join(threads[i], NULL);
    // }

    puts("All threads finished");
    return 0;
}