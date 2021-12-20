#ifndef _TABLERO_H_
#define _TABLERO_H_
#include "celda.h"
#include "sector.h"


#define NRO_FILAS 9 
#define NRO_COLUMNAS 9
typedef struct {
	celda_t** celdas;
	sector_t** sectores;
}tablero_t;

//Crea un tablero de 9 x 9 casilleros a partir de un archivo de texto
//que contiene los valores iniciales de las celdas. Estas celdas se
//identificaran como prefijadas y por lo tanto inmodificables.
//PRE: el arhivo debe tener el formato de una fila por linea, con 
//los numeros para cada celda separados con espacios y se entiende
//como una celda con 0 como una celda vacia.
//POST: crea un tablero con los valores del archivo. Devuelve distinto
//de 0 si ocurre algun error
int tablero_crear(tablero_t* tablero, const char* ruta_archivo);

//Verifica que todas las celdas del tablero
//tengan valores que cumplan con las reglas:
// *En cada fila, columna y sector hay solamente
//  numeros del 1 al 9 distintos o celdas vacias
//POST: Devuelve true si las reglas se cumplen para
//todas las celdas falso en caso contrario.
bool tablero_verificar(tablero_t* tablero);

//Devuelve una representacion del tablero como cadena
char* tablero_obtener(tablero_t* tablero);

//Modifica la celda en la posicion (fila, columna) del tablero poniendole
//el valor recibido.
//PRE: el tablero fue creado. Fila y columna deben ser dos valores validos
//POST: devuelve 0 si la celda se modifico correctamente, en caso de error:
// * 1 si (fila, columna) es una posicion invalida
// * 2 si la celda no es modificable
int tablero_modificar_celda(tablero_t* tablero, int fila, int col, int valor);

//Reinicia el tablero recibido a los valores prefijados 
//en la creacion.
//POST: Reinicia el tablero eliminando los valores de las
//celdas no fijadas en la creacion ¿y lo devuelve?
void tablero_reiniciar(tablero_t* tablero);

//Destruye el tablero recibido por parametro 
//liberando todos los recursos utilizados
void tablero_destruir(tablero_t* tablero);

#endif
