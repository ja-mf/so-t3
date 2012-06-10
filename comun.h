#ifndef COMUN_H
#define COMUN_H

#include <sys/types.h>
#include <unistd.h>

#define SHM_SIZE sizeof(partida) 

const char * lock = "/tmp/palGato.lock";
int id = 'F';

typedef struct partida {
	int turno;
	int jugadores;
	int tablero[100];
} partida;

#endif
