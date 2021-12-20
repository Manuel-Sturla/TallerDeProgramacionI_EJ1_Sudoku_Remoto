#include "vector_dinamico.h"

void vector_dinamico_crear(vector_t* vector, int tamanio_inicial){
	void* vector_nuevo = malloc(sizeof(void*) * tamanio_inicial);
	if (vector_nuevo == NULL) return;
	vector->datos = vector_nuevo;
	vector->tamanio = tamanio;
	vector->cant_guardados = 0;
}

void vector_dinamico_