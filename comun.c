#include "comun.h"

// Inicializar Semaforos en 0
void inicializar(int init, int sem_id)
{
    union semaphore_union sem_union;

    sem_union.val = init;
    if (semctl(sem_id, 0, SETVAL, sem_union) == -1) 
    {
        printf("Error. Inicializacion 0 fallida %d\n", errno);
        exit(1);
    }

	if (semctl(sem_id, 1, SETVAL, sem_union) == -1) {
		printf("Error. Inicializacion 1 fallida %d\n", errno);
		exit(1);
	}
}

// Destruir semaforos
void destruir(int sem_id)
{
    union semaphore_union sem_union;

    if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
        printf( "Error. Destruccion fallida\n");
	else 
		printf("Destruccion de semaphore exitosa\n");

    if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
        printf( "Error. Destruccion fallida\n");
else printf("Destruccion de semaphore exitosa\n");


}

// Wait para sem_num
void wait(int sem_id, int sem_num)
{
    struct sembuf semaphore;

    semaphore.sem_num = sem_num;
    semaphore.sem_op = -1;
    semaphore.sem_flg = SEM_UNDO;
    if (semop(sem_id, &semaphore, 1) == -1) {
        printf( "Error. Wait fallido\n");
        exit(2);
    } 
}

	/*Signal*/
void signal(int sem_id, int sem_num)
{
    struct sembuf semaphore;

    semaphore.sem_num = sem_num;
    semaphore.sem_op = 1;
    semaphore.sem_flg = SEM_UNDO;
    if (semop(sem_id, &semaphore, 1) == -1) {
        printf("Error. Signal fallido\n");
        exit(3);
    }
}
