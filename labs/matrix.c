#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct thread_params {
    int *row, *col, dim;
};

void *routine(void *arg) {
    struct thread_params params = *((struct thread_params *) arg);
    int sum = 0;
    for (int i = 0; i < params.dim; ++i) {
        sum += params.row[i] * params.col[i];
    }
    // printf("%d\n", sum);
    int *p_sum = malloc(sizeof(int));
    *p_sum = sum;
    return p_sum;
}

int main() {
    int n, m, p;
    scanf("%d %d %d", &n, &m, &p);
    int const THREADS = n * p;
    pthread_t threads[THREADS];
    int mat1[n][m], mat2[p][m], res[n][p];
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            scanf("%d", &mat1[i][j]);
        }
    }
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < p; ++j) {
            scanf("%d", &mat2[j][i]);
        }
    }
    struct thread_params params[THREADS];
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < p; ++j) {
            params[i * p + j].row = mat1[i];
            params[i * p + j].col = mat2[j];
            params[i * p + j].dim = m;
            pthread_create(&threads[i * p + j], NULL, routine, &params[i * p + j]);
        }
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < p; ++j) {
            int *aux;
            pthread_join(threads[i * p + j], (void **) &aux);
            res[i][j] = *aux;
            printf("%d ", res[i][j]);
            free(aux);
        }
        puts("");
    }
    return 0;
}