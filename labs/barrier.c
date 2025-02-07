#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <semaphore.h>
#include <stdbool.h>

int const RESOURCES = 5, THREADS = 10, SEMS = THREADS / RESOURCES + 1;

int barrier, sem_idx;
pthread_mutex_t mutex;
sem_t sem_arr[10];
bool enter = true;

void init(int n) {
    barrier = n;
}

int barrier_point() {
    while (!enter);
    pthread_mutex_lock(&mutex);
    barrier--;

    if (barrier > 0) {
        pthread_mutex_unlock(&mutex);
        // puts("Enter\n");
        if (sem_wait(&sem_arr[sem_idx])) {
            perror(NULL);
            return errno;
        }
        // puts("Exit\n");
    } else {
        enter = false;
        pthread_mutex_unlock(&mutex);
    }
    
    pthread_mutex_lock(&mutex);
    barrier++;

    if (barrier < RESOURCES) {
        if (sem_post(&sem_arr[sem_idx])) {
            perror(NULL);
            return errno;
        }
    } else {
        sem_idx++;
        enter = true;
    }
    pthread_mutex_unlock(&mutex);
    
    return 0;
}

void *routine(void *arg) {
    int *tid = (int *) arg;
    // while (!enter);
    printf("%d reached the barrier\n", *tid);
    barrier_point();
    printf("%d passed the barrier\n", *tid);
    free(arg);
    return NULL;
}

int main() {
    for (int i = 0; i < SEMS; ++i) {
        sem_init(&sem_arr[i], 0, 0);
    }
    pthread_mutex_init(&mutex, NULL);
    pthread_t threads[THREADS];

    init(RESOURCES);

    for (int i = 0; i < THREADS; ++i) {
        int *arg = malloc(sizeof(int));
        *arg = i;
        pthread_create(&threads[i], NULL, routine, arg);
    }

    for (int i = 0; i < THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    puts("All threads finished");
    return 0;
}