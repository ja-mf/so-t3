all: palGato m_palGato

palGato: palGato.c
		gcc -o palGato palGato.c

m_palGato: moderador.c tablero.o
		gcc -o m_palGato moderador.c tablero.o

tablero.o: tablero.c tablero.h
		gcc -c tablero.c

clean:
	rm *.o
	rm /var/palGato.lock

