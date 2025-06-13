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

    // Libération des verrous
    pthread_mutex_unlock(args->lock_b);
    pthread_mutex_unlock(args->lock_a);

    return NULL;
}


void run_solution_preacquisition() {
    pthread_t threads[3];
    // On crée un tableau de 3 cases pour les arguments de nos threads
    preacq_args args[3];

    srand(time(NULL)); // Initialise le générateur de nombres aléatoires
    printf("--- Solution: Pré-acquisition (avec détails) ---\n");

    // Configurer et lancer le Thread 1 (lock1 -> lock2)
    args[0] = (preacq_args){1, &lock1, "lock1", &lock2, "lock2"};
    pthread_create(&threads[0], NULL, preacq_worker, &args[0]);

    // Configurer et lancer le Thread 2 (lock2 -> lock3)
    args[1] = (preacq_args){2, &lock2, "lock2", &lock3, "lock3"};
    pthread_create(&threads[1], NULL, preacq_worker, &args[1]);

    // Configurer et lancer le Thread 3 (lock3 -> lock1), créant le cycle potentiel
    args[2] = (preacq_args){3, &lock3, "lock3", &lock1, "lock1"};
    pthread_create(&threads[2], NULL, preacq_worker, &args[2]);

    // Attendre la fin de tous les threads
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("--- Fin de la solution par Pré-acquisition (succès) ---\n");
}
