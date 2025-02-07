#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int available_resources = 5;
pthread_mutex_t mutex;

int decrease_count(int count) {
    if (available_resources < count) {
        return -1;
    } else {
        pthread_mutex_lock(&mutex);
        available_resources -= count;
        printf("Released %d resources %d remaining\n", count, available_resources);
        pthread_mutex_unlock(&mutex);
    }
    return 0;
}

int increase_count(int count) {
    pthread_mutex_lock(&mutex);
    available_resources += count;
    printf("Got %d resources %d remaining\n", count, available_resources);
    pthread_mutex_unlock(&mutex);
    return 0;
}

void *routine(void *arg) {
    int *arg_int = (int *) arg;
    while (decrease_count(*arg_int) == -1);
    increase_count(*arg_int);
    free(arg_int);
    return NULL;
}

int main() {
    int const THREADS = 10;
    pthread_t threads[THREADS];
    int arr[THREADS] = {2, 2, 1, 3, 2, 5, 3, 1, 2, 1};

    pthread_mutex_init(&mutex, NULL);
    
    for (int i = 0; i < THREADS; ++i) {
        int *arg = (int *) malloc(sizeof(int));
        *arg = arr[i];
        pthread_create(&threads[i], NULL, routine, arg);
        // pthread_join(threads[i], NULL);
    }
    for (int i = 0; i < THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }
    
    pthread_mutex_destroy(&mutex);
    puts("All threads finished");
    return 0;
}