#include "headers.h"

// --- DÉFINITION DES MUTEX GLOBAUX ---
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock3 = PTHREAD_MUTEX_INITIALIZER;

void print_menu() {
    printf("\n--- SIMULATION DE DEADLOCK (3 PROCESSUS) ---\n");
    printf("1. Lancer le scénario de deadlock\n");
    printf("2. Lancer la solution par Ordre Fixe\n");
    printf("3. Lancer la solution par Timeout\n");
    printf("4. Lancer la solution par Pré-acquisition\n");
    printf("5. Quitter\n");
    printf("Votre choix : ");
}

int main() {
    int choice = 0;

    while (choice != 5) {
        print_menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1: run_deadlock(); break;
            case 2: run_solution_ordering(); break;
            case 3: run_solution_timeout(); break;
            case 4: run_solution_preacquisition(); break;
            case 5: printf("Au revoir !\n"); break;
            default: printf("Choix invalide.\n"); break;
        }
    }

    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);
    pthread_mutex_destroy(&lock3);

    return 0;
}
