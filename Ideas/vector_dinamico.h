#ifndef _VECTOR_DINAMICO_H_
#define _VECTOR_DINAMICO_H_

typedef struct vector{
	void* datos;
	int tamanio;
	int cantidad_guardados;
}vector_t;


//Crea un vector dinamico con el tamanio indicado. En caso de error no hace nada.
void vector_dinamico_crear(vector_t* vector, int tamanio_inicial);

//Agrega al vector dinamico un elemento 
void vector_dinamico_agregar(vector_t* vector, void* valor);

void vector_dinamico_destruir(vector_t* vector);


#endif