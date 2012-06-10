#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <unistd.h>
#include <errno.h>

#include "comun.h"

int main (int argc, char **argv) {
	int shm_id;
	void * shm_addr;
	partida * juego;
	key_t key;

	struct shmid_ds shminfo;

	// chequea la existencia de /tmp/palGato.lock
	// si no existe, invoca al moderador
	if (!fopen(lock, "r")) 
		if (fork() == 0) 
			execl("m_palGato", "m_palGato\0");

	// esperar que el moderador cree el archivo comun
	while(!fopen(lock, "r"));
	key = ftok(lock, id);

	// esperar que el moderador genere el segmento de memoria
	while((shm_id = shmget(key, SHM_SIZE, 0666)) < 0);
	printf("instancia, shm_id %d\n", shm_id);
	
	// esperar para al menos la inicializacion de variables
	// en moderador
	sleep(1);

	// atachar la memoria al proceso actual
	juego = (partida *) shmat(shm_id, NULL, 0);

/*	
	if(shm_addr == (partida *) - 1) 
		fprintf(stderr, "shmat faild %d\n", errno);

	printf("shmat en %X\n", shm_addr);

	juego = (partida *) shm_addr;
*/
//	shmctl(shm_id, IPC_STAT, &shminfo);	
//	printf("lpid: %d\n", shminfo.shm_perm.mode);

	// en este momento, las variables de la memoria compartida
	// son ocupables

	
	juego->jugadores++;
	printf("j: %d\n", juego->jugadores);


	while(1);
}
