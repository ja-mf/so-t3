#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#define lado 10
#define max_players 5

int interfaz(int *tablero)
{

	mmask_t mascara;
	MEVENT raton;							//variables para el mouse
	int pulso;
	WINDOW *ventana[lado][lado];			//dimensiones de subventanas
	int matrix[lado][lado] = tablero;		//tablero debe ser matriz 2D
	int j,k,retorno,cont=0,turno=1,jugadores[max_players]={1,0,0,1,0};		//jugadores => array para saber que jugadores estan disponibles

	initscr();								//incializar biblio ncurses
	cbreak();								//tomar caracteres de entrada
	keypad(stdscr,TRUE);					//mapeo de teclado

	mvprintw(2,1,"*Ventana 1*\n\n");		//imprime en pantalla, (pos y,pos x,texo)

	box(stdscr,ACS_VLINE, ACS_HLINE);		//ventana con lineas horizontales y verticales
	refresh();
	
	for (j=0;j<lado;j++)
		for (k=0;k<lado;k++){
		//Crear ventanas
			if((ventana[k][j]=newwin(5,10,(j+1)*5,(k+1)*10))==NULL){ //(DimensionX, DimensionY, X,Y)
				perror("newwin");
				exit(-1);
			}
			else {
				keypad(ventana[k][j],TRUE);
				mvwprintw(ventana[k][j],2,1,"*VENT%d*\n\n",cont++);
				move(3,1);
				matrix[k][j]=0;
				box(ventana[k][j],ACS_VLINE,ACS_HLINE);
				wrefresh(ventana[k][j]);
			}
		}

	//box(stdscr,ACS_VLINE,ACS_HLINE);
	for (j=0;j<lado;j++)							//refrescar subventanas	
		for (k=0;k<lado;k++){
			box(ventana[k][j],ACS_VLINE,ACS_HLINE);
			refresh();
 			wrefresh(ventana[k][j]);
			}

 		start_color();								//color a subventanas, cuando se seleccione alguna
 		init_pair(1,COLOR_WHITE,COLOR_BLUE);
 		init_pair(2,COLOR_MAGENTA,COLOR_WHITE);
 		init_pair(3,COLOR_RED,COLOR_GREEN);
 		init_pair(4,COLOR_BLUE,COLOR_YELLOW);
 		init_pair(5,COLOR_GREEN,COLOR_MAGENTA);
			
 		mascara=mousemask(ALL_MOUSE_EVENTS,NULL);		//Registra click en mouse
 		while ((toupper(pulso=getch())) !='S'){
	 		if (pulso==KEY_MOUSE){
				getmouse(&raton);
				if(jugadores[turno-1]==FALSE)			//Salta posicion en caso que jugador sgte se haya retirado
					while(jugadores[turno-1]==FALSE){
						turno++;
						if (turno>max_players)
							turno=1;
					}	
				for (j=0;j<lado;j++)					//Se marca subventana que registro el click
					for (k=0;k<lado;k++) {
						if(wenclose(ventana[k][j],raton.y,raton.x) && matrix[k][j]==0){
			 				mvwprintw(ventana[k][j],3,1,"Acción");
							matrix[k][j]=turno;
			 				wbkgd(ventana[k][j],COLOR_PAIR(turno++));
							box(ventana[k][j],ACS_VLINE,ACS_HLINE);
							refresh();
							wrefresh(ventana[k][j]);
							retorno = j*lado+k;
						}
					}
 			
					//ventana principal
			 	if(wenclose(stdscr,raton.y,raton.x)){
					mvprintw(3,1,"Acción detectada\n");
				 	mvprintw(4,1,"en coordenadas (%d,%d)\n",raton.y,raton.x);
			 	}
			}

		 	//box(stdscr,ACS_VLINE,ACS_HLINE);
			for (j=0;j<lado;j++)					//refresca subventanas, ultimos cambios
				for (k=0;k<lado;k++) {		
		 			box(ventana[k][j],ACS_VLINE,ACS_HLINE);
		 			refresh();
		 			wrefresh(ventana[k][j]);
		 		}
			if (turno>max_players)				//resetea turnos
				turno=1;
		
		return retorno;
		
		} 

/*	FILE * tablero = fopen("chipamocli.txt","w");
	for (j=0;j<lado;j++){   
		for (k=0;k<lado;k++) {
			fprintf(tablero,"%d ",matrix[k][j]);
			//fprintf(tablero,"%d ",retorno);
		}
		fprintf(tablero,"\n");
	}

	fclose(tablero);		*/

	 //nocbreak();
	 endwin();					//salir de modo ncurses
	 exit(0);

} 
