#include "impresor.h"
#include <string.h>
#include <stdlib.h>

#define HEADER "U===========U===========U===========U\n"
#define LONG_HEADER strlen(HEADER)+1
#define SEP_MEDIO "U---+---+---U---+---+---U---+---+---U\n"
#define LONG_SEP_MEDIO strlen(SEP_MEDIO)+1
#define SEPARADOR_SECTOR 'U'
#define SEPARADOR_CELDA '|'

char procesar_celda(const celda_t* celda){
	if (!celda_esta_vacia(celda)){
		return celda_obtener_numero(celda) + '0';
	}
	return ' ';
}

void procesar_fila(const celda_t** celdas, int fila,\
	int nro_col, char* repr_fila){
	int col;	
	for (col = 0; col <= nro_col*4; col++){
		if (col % 12 == 0){
			repr_fila[col] = SEPARADOR_SECTOR;
		}else if (col % 4 == 0){
			repr_fila[col] = SEPARADOR_CELDA;
		}else if (col % 2 == 0){
			repr_fila[col] = procesar_celda(\
				&celdas[(fila - 1) / 2 ][(col-2) /4 ]);
		}else{
			repr_fila[col] = ' ';
		}
	}
	repr_fila[col] = '\n';
}

char* impresor_imprimir_celdas(const celda_t** celdas, int nro_filas,\
	int nro_col){
	char* repr_tablero = malloc(sizeof(char)*(LONG_HEADER+1)*2*(nro_col+1)+1);
	strncpy(repr_tablero, HEADER, LONG_HEADER); 
	for (int fila = 1; fila < nro_filas*2; fila++){
		if (fila % 6 == 0){
			strncat(repr_tablero, HEADER, LONG_HEADER);
		}else if (fila % 2 == 0){
			strncat(repr_tablero, SEP_MEDIO, LONG_SEP_MEDIO);
		}else{
			char repr_fila[LONG_HEADER-1];
			procesar_fila(celdas, fila, nro_col, repr_fila);
			strncat(repr_tablero, repr_fila, LONG_HEADER-1);
		}
	}
	strncat(repr_tablero, HEADER, LONG_HEADER);	
	return repr_tablero;
}
