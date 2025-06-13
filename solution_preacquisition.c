#include "headers.h"

typedef struct {
    int id;
    pthread_mutex_t* lock_a;
    const char* lock_a_name; // <-- NOM
    pthread_mutex_t* lock_b;
    const char* lock_b_name; // <-- NOM
} preacq_args;

void* preacq_worker(void* arg) {
    preacq_args* args = (preacq_args*)arg;
    while (1) {
        if (pthread_mutex_trylock(args->lock_a) == 0) {
            // A réussi à prendre le premier
            if (pthread_mutex_trylock(args->lock_b) == 0) {
                // A réussi à prendre le second aussi, on sort de la boucle
                break;
            } else {
                // A échoué à prendre le second, on relâche le premier
                printf("Thread %d (Pré-acq): a pris '%s' mais pas '%s'. Relâche tout et réessaie.\n", args->id, args->lock_a_name, args->lock_b_name);
                pthread_mutex_unlock(args->lock_a);
            }
        }
        // Attente aléatoire pour éviter que tous les threads réessaient exactement en même temps
        usleep((rand() % 100) * 1000);
    }

    printf(">> Thread %d (Pré-acq): a enfin pris '%s' et '%s'. Travaille...\n", args->id, args->lock_a_name, args->lock_b_name);
    sleep(1);
    pthread_mutex_unlock(args->lock_b);
    pthread_mutex_unlock(args->lock_a);
    free(args);
    return NULL;
}

// --- Lanceur amélioré ---
void run_solution_preacquisition() {
    pthread_t threads[3];
    srand(time(NULL));
    printf("--- Solution: Pré-acquisition (avec détails) ---\n");

    preacq_args* args;
    args = malloc(sizeof(preacq_args)); *args = (preacq_args){1, &lock1, "lock1", &lock2, "lock2"};
    pthread_create(&threads[0], NULL, preacq_worker, args);
    args = malloc(sizeof(preacq_args)); *args = (preacq_args){2, &lock2, "lock2", &lock3, "lock3"};
    pthread_create(&threads[1], NULL, preacq_worker, args);
    args = malloc(sizeof(preacq_args)); *args = (preacq_args){3, &lock3, "lock3", &lock1, "lock1"};
    pthread_create(&threads[2], NULL, preacq_worker, args);

    for (int i = 0; i < 3; i++) pthread_join(threads[i], NULL);
    printf("--- Fin de la solution par Pré-acquisition (succès) ---\n");
}
