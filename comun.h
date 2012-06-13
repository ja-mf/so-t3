#ifndef COMUN_H
#define COMUN_H

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define SHM_SIZE sizeof(partida) 
#define lock "/tmp/palGato.lock"
#define id 'F'

//const char * lock = "/tmp/palGato.lock";
//const char * semaforo = "/tmp/semaforo.lock";
//int id = 'F';

typedef struct partida {
	int turno;
	int tablero[100];
	int gano;
	int jugadas;
	int jugadores;
	int ronda;
	char msg[30];
	pid_t pid_jugadores[5];
} partida;

union semaphore_union {
        int val;                    
        struct semid_ds *buf;      
        unsigned short int *array;
        struct seminfo *__buf; 
};

//semaforos
void inicializar(int);
void destruir(int);
void lock_s(int, int);
void unlock_s(int, int);

int shm_id, s;
partida * juego;
partida * shm_addr;
key_t key;

#endif
