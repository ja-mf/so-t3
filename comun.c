#include "comun.h"

	/*Inicializar Semaforo*/
static void inicializar(int inicializacion,int id_semaphore)
{
    union semaphore_union sem_union;

    sem_union.val = inicializacion;
    if (semctl(id_semaphore, 0, SETVAL, sem_union) == -1) 
    {
        printf("Error. Inicializacion fallida\n");
        exit(1);
    }
}

	/*Destruir Semaforo*/
static void destruir(int id_semaphore)
{
    union semaphore_union sem_union;

    if (semctl(id_semaphore, 0, IPC_RMID, sem_union) == -1)
        printf( "Error. Destruccion fallida\n");
else printf("Destruccion de semaphore exitosa\n");

}

	/*Wait*/
static void wait(int id_semaphore)
{
    struct sembuf semaphore;

    semaphore.sem_num = 0;
    semaphore.sem_op = -1;
    semaphore.sem_flg = SEM_UNDO;
    if (semop(id_semaphore, &semaphore, 1) == -1) {
        printf( "Error. Wait fallido\n");
        exit(2);
    } 
}

	/*Signal*/
static void signal(int id_semaphore)
{
    struct sembuf semaphore;

    semaphore.sem_num = 0;
    semaphore.sem_op = 1;
    semaphore.sem_flg = SEM_UNDO;
    if (semop(id_semaphore, &semaphore, 1) == -1) {
        printf("Error. Signal fallido\n");
        exit(3);
    }
}
