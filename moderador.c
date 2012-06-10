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

	// creacion del archivo lock
	FILE * flock = fopen(lock, "w+");
	fclose(flock);

	key = ftok(lock, id);
	printf("key: %d\n", (int) key);

	if (shm_id = shmget(key, SHM_SIZE, 0) > 0) {
		printf("error, exists in %d, deleting\n", shm_id); 
		if(shmctl(shm_id, IPC_RMID, 0) == -1) 
			printf("error deleting: %d\n", errno);
	}

	// crea el nuevo segmento de memoria basado en key
	shm_id = shmget(key, SHM_SIZE, IPC_CREAT | IPC_EXCL | 0666);
	if (shm_id <= 0)
		printf("shmget failed: %d\n", errno);
	
	shm_addr = (partida *) shmat(shm_id, NULL, 0);

	printf("moderador, shm_id %d\n", shm_id);
	shm_addr->jugadores = 0;

	printf("esperando jugadores\n");
	while(shm_addr->jugadores == 0);
	printf("llego jugador\n");

	while(1);
}
