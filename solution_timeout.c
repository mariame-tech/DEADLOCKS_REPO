#include "headers.h"


typedef struct {
    int id;
    pthread_mutex_t* lock_a;
    const char* lock_a_name; // <-- NOM
    pthread_mutex_t* lock_b;
    const char* lock_b_name; // <-- NOM
} timeout_args;

void* timeout_worker(void* arg) {
    timeout_args* args = (timeout_args*)arg;
    struct timespec timeout;

    clock_gettime(CLOCK_REALTIME, &timeout); timeout.tv_sec += 2;
    printf("Thread %d (Timeout): Tente de prendre '%s'.\n", args->id, args->lock_a_name);
    if (pthread_mutex_timedlock(args->lock_a, &timeout) != 0) {
        printf("Thread %d (Timeout): Échec sur '%s'.\n", args->id, args->lock_a_name);
        free(args); return NULL;
    }

    clock_gettime(CLOCK_REALTIME, &timeout); timeout.tv_sec += 2;
    printf("Thread %d (Timeout): Tente de prendre '%s'.\n", args->id, args->lock_b_name);
    if (pthread_mutex_timedlock(args->lock_b, &timeout) != 0) {
        printf("Thread %d (Timeout): Échec sur '%s'. Libération de '%s'.\n", args->id, args->lock_b_name, args->lock_a_name);
        pthread_mutex_unlock(args->lock_a);
        free(args); return NULL;
    }

    printf(">> Thread %d (Timeout): a pris '%s' et '%s' et travaille.\n", args->id, args->lock_a_name, args->lock_b_name);
    sleep(1);
    pthread_mutex_unlock(args->lock_b); pthread_mutex_unlock(args->lock_a);
    free(args);
    return NULL;
}


void run_solution_timeout() {
    pthread_t threads[3];
    printf("--- Solution: Timeout (avec détails) ---\n");

    timeout_args* args;
    args = malloc(sizeof(timeout_args)); *args = (timeout_args){1, &lock1, "lock1", &lock2, "lock2"};
    pthread_create(&threads[0], NULL, timeout_worker, args);
    args = malloc(sizeof(timeout_args)); *args = (timeout_args){2, &lock2, "lock2", &lock3, "lock3"};
    pthread_create(&threads[1], NULL, timeout_worker, args);
    args = malloc(sizeof(timeout_args)); *args = (timeout_args){3, &lock3, "lock3", &lock1, "lock1"};
    pthread_create(&threads[2], NULL, timeout_worker, args);

    for (int i = 0; i < 3; i++) pthread_join(threads[i], NULL);
    printf("--- Fin de la solution par Timeout (succès) ---\n");
}
