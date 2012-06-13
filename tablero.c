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
	
	for (i=0;i<10;i=i++){			//de izquierda a derecha
		for(j=1;j<10;j++)
			if(tablero[i][j]==-1)
				break;
			else if( !(tablero[i][j] == tablero[i][j-1]) )
				break;
			else if(j==9){
				return 1;
			}	
	}
	
	for (j=0;j<10;j++){			//de arriba a abajo
		for(i=1;i<10;i=j++)
			if(tablero[i][j]==-1)
				break;
			else if( !(tablero[i][j] == tablero[i-1][j]) )
				break;
			else if(i==9){
				return 1;
			}
	}
	
	for (i=1;i<10;i++){			//diagonal 1
		if(tablero[i][i]==-1)
			break;
		else if( !(tablero[i][i] == tablero[i-1][i-1]) )
			break;
		else if(i==9){
			return 1;
		}	
	}

	for (i=1;i<10;i=i++){			//diagonal 2
		if(tablero[i][9-i]==-1)
			break;
		else if( !(tablero[i][9-i] == tablero[i-1][i+1]) )
			break;
		else if(i==9){
			return 1;
		}	
	}	


	return 0;
}


void mostrarTablero(int *tablero){
	int i;
	int j;
	for(i=0;i<10;i++){
		printf("----------------------------------------------------------\n");
		pritnf("|   ||    ||    ||    ||    ||   ||    ||    ||    ||    |\n");
		for(j=0;j<10;j++){
			switch(tablero[10*i + j]){
				case 0:	printf("|" ANSI_COLOR_RED     " 0 " ANSI_COLOR_RESET "|");
				case 1:	printf("|" ANSI_COLOR_GREEN   " 1 " ANSI_COLOR_RESET "|");
				case 2:	printf("|" ANSI_COLOR_YELLOW  " 2 " ANSI_COLOR_RESET "|");
				case 3:	printf("|" ANSI_COLOR_BLUE    " 3 " ANSI_COLOR_RESET "|");			
				case 4:	printf("|" ANSI_COLOR_CYAN    " 4 " ANSI_COLOR_RESET "|");
				default:	printf("|   |");			
			}			
		}
		pritnf("\n|   ||    ||    ||    ||    ||   ||    ||    ||    ||    ||\n");
		printf("----------------------------------------------------------\n");
	}
}




