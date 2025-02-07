#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void *routine(void *arg) {
    char *str_to_rev = (char *) arg;
    int n = strlen(str_to_rev);
    char *rev_word = malloc((n + 1) * sizeof(char));
    for (int i = 0; i < n; ++i) {
        rev_word[n - i - 1] = str_to_rev[i];
    }
    rev_word[n] = '\0';
    return rev_word;
}

int main(int argc, char *argv[]) {
    pthread_t rev_thread;
    // printf("%s", argv[1]);
    pthread_create(&rev_thread, NULL, routine, argv[1]);
    char *result;
    pthread_join(rev_thread, (void**)&result);
    printf("%s\n", result);
    free(result);
    return 0;
}