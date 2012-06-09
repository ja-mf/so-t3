#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>

#include "tablero.h"
#include "comun.h"

int main(int argc, char ** argv) {
	int shm_id;
	key_t key;

	// creacion del archivo lock
	FILE * flock = fopen(lock, "w+");

	key = ftok(lock, id);

	// crea el nuevo segmento de memoria basado en key
	shm_id = shmget(key, SHM_SIZE, IPC_CREAT);
	shmat(shm_id, NULL, 0);

	int tablero[100];
	int jugadores, turno;
	int i;
//	pid_t jugadores[5];

	for (i=0;i<100;i++)
		tablero[i]=0;	
	
//	printf("moderador, shm_id %d\n", shm_id);

	while(1);
}
