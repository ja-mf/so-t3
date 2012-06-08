all: palGato m_palGato

palGato: palGato.c
		gcc -o palGato palGato.c
m_palGato: moderador.c
		gcc -o m_palGato moderador.c

clean:
	rm *.o
	rm /var/palGato.lock

