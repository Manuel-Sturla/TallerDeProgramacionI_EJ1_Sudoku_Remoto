#include "celda.h"
#include <stdio.h>


bool es_numero_valido(int numero){
	return (numero >= 0) && (numero <= 9);
}

int celda_crear(celda_t* celda, int numero, bool es_modificable){
	if ( !es_numero_valido(numero) ){
		return 1;
	}
	celda->numero = numero;
	celda->modificable = es_modificable;
	return 0;
}


int celda_obtener_numero(const celda_t* celda){
	return celda->numero;
}

bool celda_esta_vacia(const celda_t* celda){
	return celda->numero == 0;
}

bool celda_es_modificable(celda_t* celda){
	return celda->modificable;
}

void celda_poner_numero(celda_t* celda, int numero){
	if ( !es_numero_valido(numero) ){
		return;
	} else if ( !celda->modificable ){
		return;
	}

	celda->numero = numero;
}
