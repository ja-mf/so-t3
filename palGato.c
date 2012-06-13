#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <unistd.h>
#include <errno.h>

#include "comun.h"
#include "tablero.h"

void mostrarTablero(int tablero[][]) {
	int i, j;
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) 
			printf("%d ", tablero[i][j]);
		printf("\n");
	}
	printf("\n");
}

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
//	sleep(1);

	// atachar la memoria al proceso actual
	juego = (partida *) shmat(shm_id, NULL, 0);

	// informacion de la memoria compartida
	shmctl(shm_id, IPC_STAT, &shminfo);	

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

	//////////////////////////////////////////////////////////
	/* Proceso Jugador */
	/*
	idea:
	1.- saber que jugador es (verificar si no hay mas de 4 jugadores/instancias)
	2.- inicializar semaforos
	3.- intentar jugar
	3.1 .- actualizar tablero
	4.- pasar el turno
	*/
	///////////////////////////////////////////////////////////

	/*	
	juego->jugadores++;
	printf("j: %d\n", juego->jugadores);
	*/
	
	// 1.- verificacion de numero de jugadores/ronda
	printf("hay %d jugadores atachados a memoria\n", (int) shminfo.shm_nattch - 1);
	
	if (shminfo.shm_nattch == 7 || juego->ronda > 0) {
		printf("No es posible entrar\nYa hay 5 jugadores o se completo una ronda");
		exit(0);
	}
	
//	juego->msg = "Entro jugador"

	juego->jugadores++;

	int id_jugador = shminfo.shm_nattch - 2;
	int jugada[2];
	int sems;
	
	// obteniendo el conjunto de semaforos creado por moderador
	sems = semget (ftok(lock, id), 0 , 0666); 
	printf("id jugador = %d", id_jugador);
	printf("turno (instancia): %d\n", juego->turno);

	mostrarTablero(juego->tablero);

	while(1) {
		lock_s(sems, 5);

		// se termino una jugada, mostrar tablero y mensaje comun
		mostrarTablero(juego->tablero);
		printf("%s\n", juego->msg);

		if (juego->turno != id_jugador)
			continue;

		lock_s(sems, id_jugador);
		printf("bloqueado semaforo (instancia) %d\n", juego->turno);

		do {
			printf("palGato> ");
			scanf("%d %d", &jugada[0], &jugada[1]);

			// comprobar jugada
			if (jugada < 0 || jugada > 99 || juego->tablero[jugada[0]][jugada[1]] != -1) {
				printf("jugada invalida\n");
				jugada[0] = -1;
			}
		} while (jugada[0] == -1);
			
		juego->tablero[jugada[0]][jugada[1]] = id_jugador;
		juego->jugadas++;

		unlock_s(sems, id_jugador);
		printf("desbloqueado semaforo (instancia)%d\n", juego->turno);
	}
}
