#include "sector.h"
#include <stdlib.h>
#include <stdio.h>

#define CAPACIDAD 9
int sector_crear(sector_t* sector){
	celda_t** celdas = malloc(sizeof(celda_t*)*CAPACIDAD);
	if (sector == NULL){
		return 1;
	}
	sector->celdas = celdas;
	sector->cant_guardados = 0;
	return 0;
}

int sector_agregar_celda(sector_t* sector, celda_t* celda){
	if (sector->cant_guardados == CAPACIDAD){
		return 1;
	}
	sector->celdas[sector->cant_guardados] = celda;
	sector->cant_guardados++;
	return 0;
}

int sector_contar_apariciones(sector_t* sector, int numero){
	int contador = 0;
	for (int i = 0; i < sector->cant_guardados; i++){
		if (celda_obtener_numero(sector->celdas[i]) == numero){
			contador++;
		}
	}
	return contador;
}

void sector_destruir(sector_t* sector){
	free(sector->celdas);
}
