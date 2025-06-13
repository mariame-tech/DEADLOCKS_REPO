#include "headers.h"


typedef struct {
    int id;
    pthread_mutex_t* first_lock;
    const char* first_lock_name; // <-- NOM
    pthread_mutex_t* second_lock;
    const char* second_lock_name; // <-- NOM
} ordering_args;

// --- Worker amélioré ---
void* ordering_worker(void* arg) {
    ordering_args* args = (ordering_args*)arg;
    printf("Thread %d (Ordre): tente de prendre le verrou '%s'.\n", args->id, args->first_lock_name);
    pthread_mutex_lock(args->first_lock);
    printf("Thread %d (Ordre): a pris le verrou '%s'.\n", args->id, args->first_lock_name);
    sleep(1);
    printf("Thread %d (Ordre): tente de prendre le verrou '%s'.\n", args->id, args->second_lock_name);
    pthread_mutex_lock(args->second_lock);
    printf("Thread %d (Ordre): a pris les deux verrous.\n", args->id);
    pthread_mutex_unlock(args->second_lock);
    pthread_mutex_unlock(args->first_lock);
    free(args);
    return NULL;
}

// --- Lanceur amélioré ---
void run_solution_ordering() {
    pthread_t threads[3];
    printf("--- Solution: Ordre Fixe (avec détails) ---\n");

    ordering_args* args;

    args = malloc(sizeof(ordering_args)); *args = (ordering_args){1, &lock1, "lock1", &lock2, "lock2"};
    pthread_create(&threads[0], NULL, ordering_worker, args);

    args = malloc(sizeof(ordering_args)); *args = (ordering_args){2, &lock2, "lock2", &lock3, "lock3"};
    pthread_create(&threads[1], NULL, ordering_worker, args);

    printf("Note: Thread 3 va demander 'lock1' avant 'lock3' pour briser le cycle.\n");
    args = malloc(sizeof(ordering_args)); *args = (ordering_args){3, &lock1, "lock1", &lock3, "lock3"};
    pthread_create(&threads[2], NULL, ordering_worker, args);

    for (int i = 0; i < 3; i++) pthread_join(threads[i], NULL);
    printf("--- Fin de la solution par Ordre Fixe (succès) ---\n");
}
