#ifndef _SECTOR_H_
#define _SECTOR_H_
#include "celda.h"

#define CAPACIDAD 9

typedef struct{
	celda_t** celdas;
	int cant_guardados;
}sector_t;

//Crea un sector valido con la CAPACIDAD establecida. Devuelve 0 si se creo
// correctamente 1 en caso contrario
int sector_crear(sector_t* sector);

//Agrega una celda al sector recibido. NO verifica si la celda ya fue
//agregada.
//Devuelve distinto de 0 si hubo un error, por ejemplo si el sector esta lleno.
int sector_agregar_celda(sector_t* sector, celda_t* celda);

//Cuenta cuantas celdas tienen ese numero almacenado y lo devuelve
int sector_contar_apariciones(sector_t* sector, int numero);

//Destruye el sector, liberando sus recursos. No modifica ni elimina
//las celdas a las cuales tiene referencia.
void sector_destruir(sector_t* sector);


#endif
