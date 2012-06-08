#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>

int main (int argc, char **argv) {
	char * lock = "/tmp/palGato.lock";
	int id = "A";

	// chequea la existencia de /tmp/palGato.lock
	// si no existe, invoca al moderador
	if (!fopen(lock, "r")) 
		if (fork() == 0) {
			execl("m_palGato", "m_palGato");

	shm_key = ftok(lock, id);
	
}
