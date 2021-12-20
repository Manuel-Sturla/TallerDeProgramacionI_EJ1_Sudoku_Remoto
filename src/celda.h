#ifndef _CELDA_H_
#define _CELDA_H_

#include <stdbool.h>


typedef struct{
	int numero;
	bool modificable;
	bool validez;
}celda_t;

//Inicia una celda con el numero indicado por parametro.
//PRE: debe estar creada la celda_t. El numero debe ser del 0 al 9
//POST: inicializa la celda con el numero indicado y configurando
// si es modificable o no.
int celda_crear(celda_t* celda, int numero, bool es_modificable);

//Devuelve el numero contenido en la celda recibida.
int celda_obtener_numero(const celda_t* celda);

//Devuelve si la celda recibida es modificable
bool celda_es_modificable(celda_t* celda);

//Devuelve True si la celda esta vacia (tiene un 0), False si no lo esta.
bool celda_esta_vacia(const celda_t* celda);


//Intenta actualizar el numero de la celda recibida.
//PRE: la celda debe ser modificable y el numero debe ser del 0 al 9
//POST: actualiza el numero de la celda. Si este no es valido o la celda
//no es modificable imprime un mensaje por STDOUT y deja la celda intacta
void celda_poner_numero(celda_t* celda, int numero);

#endif
