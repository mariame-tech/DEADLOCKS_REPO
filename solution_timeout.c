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

    // Tente de prendre le premier verrou avec un timeout de 2 secondes
    clock_gettime(CLOCK_REALTIME, &timeout);
    timeout.tv_sec += 2;
    printf("Thread %d (Timeout): Tente de prendre '%s'.\n", args->id, args->lock_a_name);
    if (pthread_mutex_timedlock(args->lock_a, &timeout) != 0) {
        printf("Thread %d (Timeout): Échec sur '%s' (timeout dépassé).\n", args->id, args->lock_a_name);
        // ON RETIRE free(args);
        return NULL;
    }

    // Tente de prendre le second verrou avec un timeout de 2 secondes
    clock_gettime(CLOCK_REALTIME, &timeout);
    timeout.tv_sec += 2;
    printf("Thread %d (Timeout): A pris '%s', tente de prendre '%s'.\n", args->id, args->lock_a_name, args->lock_b_name);
    if (pthread_mutex_timedlock(args->lock_b, &timeout) != 0) {
        printf("Thread %d (Timeout): Échec sur '%s'. Libération de '%s'.\n", args->id, args->lock_b_name, args->lock_a_name);
        pthread_mutex_unlock(args->lock_a);
        // ON RETIRE free(args);
        return NULL;
    }

    // A réussi à prendre les deux verrous
    printf(">> Thread %d (Timeout): a pris '%s' et '%s' et travaille.\n", args->id, args->lock_a_name, args->lock_b_name);
    sleep(1);

    // Libération des verrous
    pthread_mutex_unlock(args->lock_b);
    pthread_mutex_unlock(args->lock_a);

    return NULL;
}


void run_solution_timeout() {
    pthread_t threads[3];
    // On crée un tableau de 3 cases pour les arguments de nos threads
    timeout_args args[3];

    printf("--- Solution: Timeout (avec détails) ---\n");

    // Configurer et lancer le Thread 1
    args[0] = (timeout_args){1, &lock1, "lock1", &lock2, "lock2"};
    pthread_create(&threads[0], NULL, timeout_worker, &args[0]);

    // Configurer et lancer le Thread 2
    args[1] = (timeout_args){2, &lock2, "lock2", &lock3, "lock3"};
    pthread_create(&threads[1], NULL, timeout_worker, &args[1]);

    // Configurer et lancer le Thread 3
    args[2] = (timeout_args){3, &lock3, "lock3", &lock1, "lock1"};
    pthread_create(&threads[2], NULL, timeout_worker, &args[2]);

    // Attendre la fin de tous les threads
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("--- Fin de la solution par Timeout (succès) ---\n");
}
