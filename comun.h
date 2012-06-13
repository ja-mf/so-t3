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

#define SHM_SIZE sizeof(partida) 
#define lock "/tmp/palGato.lock"
#define semaforo "/tmp/semaforo.lock"
#define id 'F'

//const char * lock = "/tmp/palGato.lock";
//const char * semaforo = "/tmp/semaforo.lock";
//int id = 'F';

typedef struct partida {
	int turno;
	int tablero[100];
	int gano;
	int jugadas;
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

// enumerando jugadores
enum { J1, J2, J3, J4, J5 };

#endif
