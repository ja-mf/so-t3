#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>

#include "tablero.h"
#include "comun.h"

int main(int argc, char ** argv) {
	int shm_id;
	partida * shm_addr;
	key_t key;
	int wrt, rd, s;
	int i;
	
	struct shmid_ds shm_info;

	// creacion del archivo lock
	FILE * flock = fopen(lock, "w+");
	fclose(flock);

	key = ftok(lock, id);
//	printf("key: %d\n", (int) key);

	// verificacion de existencia de memoria con el mismo key (debug)
	if (shm_id = shmget(key, SHM_SIZE, 0) > 0) {
		printf("error, exists in %d, deleting\n", shm_id); 
		if(shmctl(shm_id, IPC_RMID, 0) == -1) 
			printf("error deleting: %d\n", errno);
	}

	// crea el nuevo segmento de memoria basado en key
	shm_id = shmget(key, SHM_SIZE, IPC_CREAT | IPC_EXCL | 0666);
	if (shm_id <= 0)
		printf("shmget failed: %d\n", errno);
	
	// atachar la memoria compartida al espacio actual
	shm_addr = (partida *) shmat(shm_id, NULL, 0);

	printf("moderador, shm_id %d\n", shm_id);

	// inicializacion de variables en memoria compartida
	shm_addr->jugadores = 0;
	shm_addr->turno=0;
	shm_addr->gano=0;
	shm_addr->jugadas=0;
	for (i=0;i<100;i++)
		shm_addr->tablero[i] = 0;

	// inicializacion de semaforos
	// conjunto de 2 semaforos: 0 -> lectura, 1 -> escritura
	s = semget (ftok(semaforo, id), 2, 0666 | IPC_CREAT);
	inicializar(0, s);
/*
	wrt = semget ( ftok(semaforo, 'w'), 1 , 0666 | IPC_CREAT ); 
	inicializar(0,wrt);
	rd = semget ( ftok(semaforo, 'r'), 1 , 0666 | IPC_CREAT ); 
	inicializar(0,rd);
*/
	/*printf("esperando jugadores\n");
	while(shm_addr->jugadores == 0);
	printf("llego jugador\n");
	*/
	while(1);
}
