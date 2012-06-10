all: palGato m_palGato

palGato: palGato.c comun.h
		gcc -o palGato palGato.c

m_palGato: moderador.c tablero.o comun.h
		gcc -o m_palGato moderador.c tablero.o

tablero.o: tablero.c tablero.h
		gcc -c tablero.c

clean:
	rm -f *.o
	rm -f palGato m_palGato
	rm -f /tmp/palGato.lock
	./remove_shm

