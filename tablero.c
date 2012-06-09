#include "tablero.h"

//** idea tablero **//
/*
 0  1  2  3  4  5  6  7  8  9
10 11 12 13 14 15 16 17 18 19
20 21 22 23 24 25 26 27 28 29
30 31 32 33 34 35 36 37 38 39
40 41 42 43 44 45 46 47 48 49
50 51 52 53 54 55 56 57 58 59
60 61 62 63 64 65 66 67 68 69
70 71 72 73 74 75 76 77 78 79
80 81 82 83 84 85 86 87 88 89
90 91 92 93 94 95 96 97 98 99
*/

//** idea llenado **//
/*
Numero del proceso en la posicion del arreglo
0 0 0 0 0 0 0 0 0 5
0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 2 0
0 0 0 0 0 0 1 0 0 0
0 0 0 0 0 0 0 0 0 0
0 0 0 0 4 0 0 0 0 0
0 0 0 0 0 0 3 0 0 0
0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0
*/


int comprobar(int *tablero){
	int i,j;
	int gano = 0;
	
	for (i=0;i<100;i=i+10){			//de izquierda a derecha
		for(j=1;j<10;j++)
			if(tablero[i+j]==0)
				break;
			else if( !(tablero[i+j] == tablero[i+j-1]) )
				break;
			else if(j==9){
				gano = 1;
				return gano;
			}	
	}
	
	for (i=0;i<10;i++){			//de arriba a abajo
		for(j=1;j<100;j=j+10)
			if(tablero[i+j]==0)
				break;
			else if( !(tablero[i+j] == tablero[i+j-10]) )
				break;
			else if(j==90){
				gano = 1;
				return gano;
			}
	}
	
	for (i=11;i<100;i=i+11){			//diagonal 1
		if(tablero[i]==0)
			break;
		else if( !(tablero[i] == tablero[i-11]) )
			break;
		else if(i==99){
			gano = 1;
			return gano;
		}	
	}

	for (i=18;i<100;i=i+9){			//diagonal 2
		if(tablero[i]==0)
			break;
		else if( !(tablero[i] == tablero[i-11]) )
			break;
		else if(i==90){
			gano = 1;
			return gano;
		}	
	}	


	return gano;
}

int guardar_tablero(int *tablero) {
	/*
	FILE *file;
	int i,j;	
	file=fopen("tablero.txt","w");
	for(i=0;i<10;i++){
		for(j=0;j<10;j++)
			fprintf(file,tablero[i*10+j]);
		fprintf(file,"/n");	
	}
	fclose(file);
	*/
	return 0;
		
}
