#ifndef DEADLOCK_UTILS_H
#define DEADLOCK_UTILS_H

// --- Inclusions des bibliothèques communes ---
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>


extern pthread_mutex_t lock1, lock2, lock3;

// --- Déclaration de toutes nos fonctions ---
void run_deadlock();
void run_solution_ordering();
void run_solution_timeout();
void run_solution_preacquisition();

#endif // DEADLOCK_UTILS_H
