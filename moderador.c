#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>

#include "tablero.h"
#include "comun.h"

// secuencia para el termino del moderador (juego)
void termino (int sig) {
	int i;

	// matando procesos
	for (i = 0; i < 5; i++)
		if (shm_addr->pid_jugadores[i] != 0)
			kill(shm_addr->pid_jugadores[i], SIGTERM);

	// removiendo memoria compartida
	if (shmctl(shm_id, IPC_RMID, 0) == -1) { 
		printf("error en remover memoria compartida %d\n", errno);
		exit(1);
	}
	
	// removiendo conjunto de semaforos
	if (semctl(s, 0, IPC_RMID, NULL)== -1) {
		printf("error en remover el conjunto de semaforos %d\n", errno);
		exit(1);
	}

	// eliminando el archivo lock
	remove(lock);

	exit(0);
}

int main(int argc, char ** argv) {
	int i, j;

	struct shmid_ds shminfo;
	struct sigaction term_action;
	
	// secuencia de salida
	term_action.sa_handler = termino;
	sigemptyset(&term_action.sa_mask);
	term_action.sa_flags = 0;

	sigaction (SIGINT, &term_action, NULL);
	sigaction (SIGHUP, &term_action, NULL);
	sigaction (SIGTERM, &term_action, NULL);

	// creacion del archivo lock
	FILE * flock = fopen(lock, "w+");
	fclose(flock);

	key = ftok(lock, id);

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
	shm_addr->jugadores	= 0;

	for (i = 0; i < 10; i++)
		for (j = 0; j < 10; j++)
			shm_addr->tablero[i*10+j] = -1;

	// inicializacion de semaforos
	// conjunto de 6 semaforos, uno por cada jugador + semaforo comun
	// el semaforo comun indica inicio/termino de una jugada
	// su valor sera modificado segun la cantidad de jugadores
	s = semget (ftok(lock, id), 6, 0666 | IPC_CREAT);
	inicializar(s);

	shmctl(shm_id, IPC_STAT, &shminfo);
	
	// turno del jugador 0
	shm_addr->turno = 0;

	while(1) {
		// inicio del turno
		unlock_s(s, shm_addr->turno);
		printf("desbloqueado semaforo %d\n", shm_addr->turno);

		// esperar a que la instancia bloquee el semaforo
		sleep(1);

		// esperar fin de la jugada
		lock_s(s, shm_addr->turno);
		printf("bloqueado semaforo %d\n", shm_addr->turno);

		if (comprobar(shm_addr->tablero)) {
			shm_addr->gano = 1;
			break;
		}
		
//		printf("turno+1: %d, jugadores: %d, proximo turno: %d\n",shm_addr->turno+1,shm_addr->jugadores, (shm_addr->turno+1) % (shm_addr->jugadores - 1));

		// calculo del proximo turno
		shm_addr->turno = (shm_addr->turno+1) % (shm_addr->jugadores);
		
		// proximo turno == 0 indica nueva ronda
		if (shm_addr->turno == 0 && shm_addr->jugadas != 0)
			shm_addr->ronda++; 
		
		// actualizar semaforo comun a la cantidad actual de jugadores
		njugadores(s, shm_addr->jugadores - 1); 
		sleep(1);
	}
	// esperar un segundo y matar proceso
	sleep(1);
	kill(getpid(), SIGTERM);
}
