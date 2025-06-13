#include "headers.h"

typedef struct {
    int id;
    pthread_mutex_t* first_lock;
    const char* first_lock_name;
    pthread_mutex_t* second_lock;
    const char* second_lock_name;
} thread_args;

void* worker_verbose(void* arg) {
    thread_args* args = (thread_args*)arg;

    printf("Thread %d: tente de prendre le verrou '%s'.\n", args->id, args->first_lock_name);
    pthread_mutex_lock(args->first_lock);
    printf("Thread %d: a pris le verrou '%s'.\n", args->id, args->first_lock_name);

    sleep(1); // On attend un peu pour que le deadlock se produise à coup sûr

    printf("Thread %d: tente de prendre le verrou '%s'.\n", args->id, args->second_lock_name);
    pthread_mutex_lock(args->second_lock);
    printf("Thread %d: a pris les deux verrous '%s' et '%s'.\n", args->id, args->first_lock_name, args->second_lock_name);

    pthread_mutex_unlock(args->second_lock);
    pthread_mutex_unlock(args->first_lock);

    return NULL;
}

void run_deadlock() {
    pthread_t threads[3];
    // On crée un tableau pour contenir les "instructions" pour nos 3 threads
    thread_args args[3];

    printf("--- Scénario: Deadlock à 3 Threads (avec détails) ---\n");
    printf("Le programme va se bloquer. Utilisez Ctrl+C pour arrêter.\n");

    // Thread 1: Prend lock1, attend lock2
    args[0] = (thread_args){1, &lock1, "lock1", &lock2, "lock2"};
    pthread_create(&threads[0], NULL, worker_verbose, &args[0]); // On passe l'adresse du 1er élément

    // Thread 2: Prend lock2, attend lock3
    args[1] = (thread_args){2, &lock2, "lock2", &lock3, "lock3"};
    pthread_create(&threads[1], NULL, worker_verbose, &args[1]); // On passe l'adresse du 2ème élément

    // Thread 3: Prend lock3, attend lock1 (ferme le cycle !)
    args[2] = (thread_args){3, &lock3, "lock3", &lock1, "lock1"};
    pthread_create(&threads[2], NULL, worker_verbose, &args[2]); // On passe l'adresse du 3ème élément

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }
}
