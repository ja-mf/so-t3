#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include "comun.h"
#include "tablero.h"

// secuencia de salida para el jugador
void termino (int signum) {
	if (juego->gano)
		printf("Gano el jugador %d\n", juego->turno);

	printf("terminando instancia, liberando recursos\n");

	if (shmdt(juego) == -1) 
		printf("fallo al detachar memoria %d\n", errno);

	exit(0);	
}

int main (int argc, char **argv) {

	// manejando termino del proceso
	struct sigaction term_action;
	term_action.sa_handler = termino;
	sigemptyset(&term_action.sa_mask);
	term_action.sa_flags = 0;

	sigaction (SIGINT, &term_action, NULL);
	sigaction (SIGHUP, &term_action, NULL);
	sigaction (SIGTERM, &term_action, NULL);

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
	
	// atachar la memoria al proceso actual
	juego = (partida *) shmat(shm_id, NULL, 0);

	// informacion de la memoria compartida
	shmctl(shm_id, IPC_STAT, &shminfo);	

	// en este momento, las variables de la memoria compartida
	// son ocupables

	//////////////////////////////////////////////////////////
	/* Proceso Jugador */
	/*
	idea:
	1.- saber que jugador es (verificar si no hay mas de 4 jugadores/instancias)
	2.- esperar a que se termine una jugada
	3.- verificar turno
	4.- jugar
	4.1 .- actualizar tablero
	5.- desbloquear mi semaforo
	*/
	///////////////////////////////////////////////////////////

	// 1.- verificacion de numero de jugadores/ronda
	printf("hay %d jugadores atachados a memoria\n", (int) shminfo.shm_nattch - 1);
	
	if (shminfo.shm_nattch == 7 || juego->ronda > 0) {
		printf("No es posible entrar\nYa hay 5 jugadores o se completo una ronda");
		kill(getpid(), SIGTERM);
	}

	// jugador entro de forma exitosa
	int id_jugador = shminfo.shm_nattch - 2;
	int jugada;
	int sems;

	juego->pid_jugadores[id_jugador] = getpid(); 
	juego->jugadores++;

	// obteniendo el conjunto de semaforos creado por moderador
	sems = semget (ftok(lock, id), 0 , 0666); 
	printf("id jugador = %d", id_jugador);
	printf("turno (instancia): %d\n", juego->turno);

	// mostrar el tablero si es que no es el primer jugador (primer turno)
	if (id_jugador != 0)
		mostrarTablero(juego->tablero);

	while(1) {
		// esperar el termino de una jugada (semaforo comun)
		lock_s(sems, 5);
		mostrarTablero(juego->tablero);

		if (juego->turno != id_jugador)
			continue;

		// esperar mi turno
		lock_s(sems, id_jugador);
		printf("bloqueado semaforo (instancia) %d\n", juego->turno);

		do {
			printf("palGato> ");
			scanf("%d", &jugada);

			// comprobar jugada
			if (jugada < 0 || jugada > 99 || juego->tablero[jugada] != -1) {
				printf("jugada invalida\n");
				jugada = -1;
			}
		} while (jugada == -1);
			
		// actualizar variables de la memoria compartida
		juego->tablero[jugada] = id_jugador;
		juego->jugadas++;
		
		mostrarTablero(juego->tablero);
		
		// desbloquear mi semaforo
		unlock_s(sems, id_jugador);
		printf("desbloqueado semaforo (instancia)%d\n", juego->turno);
	}
}
