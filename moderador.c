#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include "tablero.h"

int main(int argc, char ** argv) {
	// don memoria compartida
	char * lock = "/tmp/palGato.lock";
	int id = "A";
	int shm_id;
	key_t key;

	FILE * flock = fopen(lock, "w+");
	key = ftok(lock, id);

	// crea el nuevo segmento de memoria
	shm_id = shmget(key, 100, IPC_CREAT);
	shmat(shm_id, NULL, 0);

	int tablero[100];
	int jugadores, turno;
	int i;
//	pid_t jugadores[5];

	for (i=0;i<100;i++)
		tablero[i]=0;	

	while(1);
}
