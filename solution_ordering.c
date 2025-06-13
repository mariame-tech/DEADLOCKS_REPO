#include "headers.h"

// La structure reste la même
typedef struct {
    int id;
    pthread_mutex_t* first_lock;
    const char* first_lock_name; // <-- NOM
    pthread_mutex_t* second_lock;
    const char* second_lock_name; // <-- NOM
} ordering_args;

void* ordering_worker(void* arg) {
    ordering_args* args = (ordering_args*)arg;
    printf("Thread %d (Ordre): tente de prendre le verrou '%s'.\n", args->id, args->first_lock_name);
    pthread_mutex_lock(args->first_lock);
    printf("Thread %d (Ordre): a pris le verrou '%s'.\n", args->id, args->first_lock_name);

    // Une petite pause pour simuler du travail et rendre l'ordonnancement visible
    sleep(1);

    printf("Thread %d (Ordre): tente de prendre le verrou '%s'.\n", args->id, args->second_lock_name);
    pthread_mutex_lock(args->second_lock);
    printf("Thread %d (Ordre): a pris les deux verrous.\n", args->id);

    pthread_mutex_unlock(args->second_lock);
    pthread_mutex_unlock(args->first_lock);

    return NULL;
}


void run_solution_ordering() {
    pthread_t threads[3];
    // On crée un tableau de 3 cases pour les arguments de nos threads
    ordering_args args[3];

    printf("--- Solution: Ordre Fixe (avec détails) ---\n");

    // Configurer et lancer le Thread 1
    args[0] = (ordering_args){1, &lock1, "lock1", &lock2, "lock2"};
    pthread_create(&threads[0], NULL, ordering_worker, &args[0]);

    // Configurer et lancer le Thread 2
    args[1] = (ordering_args){2, &lock2, "lock2", &lock3, "lock3"};
    pthread_create(&threads[1], NULL, ordering_worker, &args[1]);

    // Configurer et lancer le Thread 3
    printf("Note: Thread 3 va demander 'lock1' avant 'lock3' pour briser le cycle.\n");
    args[2] = (ordering_args){3, &lock1, "lock1", &lock3, "lock3"};
    pthread_create(&threads[2], NULL, ordering_worker, &args[2]);

    // Attendre la fin de tous les threads
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("--- Fin de la solution par Ordre Fixe (succès) ---\n");
}
