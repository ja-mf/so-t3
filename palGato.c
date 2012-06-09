#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>

#include "comun.h"

int main (int argc, char **argv) {
	int shm_id;
	key_t key;

	// chequea la existencia de /tmp/palGato.lock
	// si no existe, invoca al moderador
	if (!fopen(lock, "r")) 
		if (fork() == 0) 
			execl("m_palGato", "m_palGato\0");

	// esperar que el moderador cree el archivo comun
	while(!fopen(lock, "r"));
	key = ftok(lock, id);

	// esperar que el moderador genere el segmento de memoria
	while((shm_id = shmget(key, SHM_SIZE, 0)) < 0);
	shmat(shm_id, NULL, 0);

//	printf("instancia, shm_id %d\n", shm_id);

	while(1);
}
