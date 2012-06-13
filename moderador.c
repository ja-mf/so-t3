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
	int s;
	int i;
	short jugadores;
	
	struct shmid_ds shminfo;

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
	shm_addr->turno 	= 0;
	shm_addr->gano 		= 0;
	shm_addr->jugadas	= 0;

	for (i = 0; i < 100; i++)
		shm_addr->tablero[i] = -1;

	// inicializacion de semaforos
	// conjunto de 2 semaforos: 0 -> lectura, 1 -> escritura
	s = semget (ftok(semaforo, id), 6, 0666 | IPC_CREAT);
	inicializar(s);

	shmctl(shm_id, IPC_STAT, &shminfo);
	
	// turno del jugador 1
	shm_addr->turno = 0;
//	unlock_s(s, 0);
	while(1) {
		// inicio del turno
		unlock_s(s, shm_addr->turno);
		printf("desbloqueado semaforo %d\n", shm_addr->turno);

		// esperar a que la instancia bloquee el semaforo
		sleep(1);

		// esperar fin de la jugada
		lock_s(s, shm_addr->turno);
		printf("bloqueado semaforo %d\n", shm_addr->turno);

		// ver numero de jugadores
		shmctl(shm_id, IPC_STAT, &shminfo);
		printf("proximo turno: %d\n", (shm_addr->turno+1) % ((int) shminfo.shm_nattch - 2));
		shm_addr->turno = (shm_addr->turno+1) % ((int) shminfo.shm_nattch - 2);
		sleep(1);
	}

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
