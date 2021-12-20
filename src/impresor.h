#ifndef _IMPRESOR_H_
#define _IMPRESOR_H_
#include "celda.h"

//Recibe una lista de listas de celdas, el numero de filas y
//de columnas que tiene el archivo y devuelve una representacion
//en forma de cadena del mismo
char* impresor_imprimir_celdas(const celda_t** celdas, \
	int nro_filas, int nro_col);

#endif
