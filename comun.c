#include "comun.h"

// Inicializar Semaforos en 0
void inicializar(int sem_id)
{
    union semaphore_union sem_union;

    sem_union.array = 0;
	semctl(sem_id, 0, SETALL, sem_union);

	sem_union.val = 1;
	semctl(sem_id, 5, SETVAL, sem_union);
	/*
    if (semctl(sem_id, 0, SETVAL, sem_union) == -1) 
    {
        printf("Error. Inicializacion 0 fallida %d\n", errno);
        exit(1);
    }

	if (semctl(sem_id, 1, SETVAL, sem_union) == -1) {
		printf("Error. Inicializacion 1 fallida %d\n", errno);
		exit(1);
	}
	*/
}
// numero de jugadores
void njugadores(int sem_id, int n)
{
	union semaphore_union sem_union;

	sem_union.val = n;
	semctl(sem_id, 5, SETVAL, sem_union);
}


// Destruir semaforos
void destruir (int sem_id) {
    union semaphore_union sem_union;
	
	if (semctl(sem_id, 0, IPC_RMID, NULL) == -1) {
		printf("error en la destruccion del semaforo! errno: %d", errno);
		exit(1);
	}
}

// bloqueo para sem_num
void lock_s(int sem_id, int sem_num)
{
    struct sembuf semaphore;

    semaphore.sem_num = sem_num;
    semaphore.sem_op = -1;
    semaphore.sem_flg = SEM_UNDO;
    if (semop(sem_id, &semaphore, 1) == -1) {
        printf( "fallo lock %d\n", errno);
        exit(2);
    } 
}

// unlock para sem_num
void unlock_s(int sem_id, int sem_num)
{
    struct sembuf semaphore;

    semaphore.sem_num = sem_num;
    semaphore.sem_op = 1;
    semaphore.sem_flg = SEM_UNDO;
    if (semop(sem_id, &semaphore, 1) == -1) {
        printf("fallo unlock %d\n", errno);
        exit(2);
    }
}
