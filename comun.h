#ifndef COMUN_H
#define COMUN_H

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define SHM_SIZE sizeof(partida) 

const char * lock = "/tmp/palGato.lock";
const char * semaforo = "/tmp/semaforo.lock";
int id = 'F';

typedef struct partida {
	int turno;
	int jugadores;
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
static void inicializar(int,int);
static void destruir(int);
static void wait(int);
static void signal(int);
#endif
