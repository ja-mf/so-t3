all: palGato m_palGato

palGato: palGato.c comun.o tablero.o
		gcc -o palGato palGato.c tablero.o comun.o

m_palGato: moderador.c tablero.o comun.o
		gcc -o m_palGato moderador.c tablero.o comun.o

tablero.o: tablero.c tablero.h
		gcc -c tablero.c

comun.o: comun.c comun.h
		gcc -c comun.c

clean:
	rm -f *.o
	rm -f palGato m_palGato
	rm -f /tmp/palGato.lock
	./remove_shm

