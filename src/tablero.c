#include "tablero.h"
#include "impresor.h"
#include <stdio.h>
#include <stdlib.h>

#define TAMANIO_BUFFER_LECTURA (2 * NRO_COLUMNAS)

int digito_a_int(const char caracter){
	return caracter - '0';
}

//Toma los caracteres en las posiciones pares, es decir, evita
//los separadores y convierte los que estes en estas posiciones
//a entero para crear una celda con ese valor. Si el valor es 0
//se entiende que la celda esta vacia. En caso contrario se entiende
//que es una de las celdas de pista y por lo tanto, no modificable.
celda_t* procesar_fila_leida(char* linea, size_t leidos){
	celda_t* fila = malloc(sizeof(celda_t) * NRO_COLUMNAS);
	if (fila == NULL){
		return NULL;
	}
	for (int i = 0; i < leidos; i++){
		if (i % 2 == 0){
			int valor = digito_a_int(linea[i]);
			if (valor == 0){
				celda_crear(&fila[i/2], valor, true);
			}else{
				celda_crear(&fila[i/2], valor, false);
			}
		}
	}
	return fila;
}

int leer_archivo(const char* ruta_archivo, celda_t** filas){
	FILE* archivo = fopen(ruta_archivo, "rt");
	if (archivo == NULL){
		fprintf(stderr,"No se pudo abrir el archivo\n");
		return 1;
	}
	int fila_actual = 0;
	size_t leidos;
	char buffer[TAMANIO_BUFFER_LECTURA];
	while ((leidos = fread(&buffer, 1, TAMANIO_BUFFER_LECTURA, archivo))) {
		if (leidos < TAMANIO_BUFFER_LECTURA-1) { //si la fila no esta completa
			fprintf(stderr,"El archivo de entrada tiene filas incompletas\n");
			fclose(archivo);
			return 2;
		}
		celda_t* fila = procesar_fila_leida(buffer, leidos);
		if (fila == NULL){
			fprintf(stderr,"No hay memoria suficiente\n");
			fclose(archivo);
			return 3;
		}
		filas[fila_actual] = fila;
		fila_actual++;
	}
	fclose(archivo);
	return 0;
}

int agregar_sectores(tablero_t* tablero){
	sector_t** sectores = malloc(sizeof(sector_t)* NRO_FILAS/3);
	if (sectores == NULL){
		return 1;
	}
	for (int i = 0; i < 3; i++){
		sector_t* fila_sector = malloc(sizeof(sector_t) * NRO_COLUMNAS/3);
		if (fila_sector == NULL){
			free(sectores);
			return 1;
		}
		sectores[i] = fila_sector;
		for (int j = 0; j < 3; j++){
			sector_t sector;
			if (sector_crear(&sector) != 0){
				return 1;
			}
			sectores[i][j] = sector;
		}
	}
	for (int fila = 0; fila < NRO_FILAS; fila ++){
		for (int col = 0; col < NRO_COLUMNAS; col ++){
			sector_agregar_celda(&sectores[fila/3][col/3], &tablero->celdas[fila][col]);
		}
	}
	tablero->sectores = sectores;
	return 0;
}

int tablero_crear(tablero_t* tablero, const char* ruta_archivo){
	celda_t** filas = malloc( sizeof(celda_t*) * NRO_FILAS);
	if ( !filas ){
		fprintf(stderr,"No hay memoria suficiente\n");
		return 3;
	}
	int salida;
	if ((salida = leer_archivo(ruta_archivo, filas)) == 1){
		fprintf(stderr,"Error al leer el archivo\n");
		free(filas);
		return salida;
	}
	tablero->celdas = filas;

	if (agregar_sectores(tablero) != 0){
		fprintf(stderr,"Error al crear los sectores\n");
		free(filas);
		return 4;
	}

	return 0;
}

void tablero_reiniciar(tablero_t* tablero){
	for (int i = 0; i < NRO_FILAS; i++){
		for (int j = 0; j < NRO_COLUMNAS; j++){
			if ( celda_es_modificable(&tablero->celdas[i][j]) ) {
				celda_poner_numero(&tablero->celdas[i][j], 0);
			}
		}
	}
}
//Cuenta las apariciones del numero recibido en la fila indicada
int contar_apariciones_columna(tablero_t* tablero, int numero, int columna){
	int apariciones = 0;
	for (int i = 0; i < NRO_FILAS; i++){
		if (celda_obtener_numero(&tablero->celdas[i][columna]) == numero){
			apariciones++;
		}
	}
	return apariciones;
}
//Cuenta las apariciones del numero recibido en la columna indicada
int contar_apariciones_fila(tablero_t* tablero, int numero, int fila){
	int apariciones = 0;
	for (int i = 0; i < NRO_COLUMNAS; i++){
		if (celda_obtener_numero(&tablero->celdas[fila][i]) == numero){
			apariciones++;
		}
	}
	return apariciones;
}


bool verificar_celda(tablero_t* tablero, int fila, int columna){
	int numero = celda_obtener_numero(&tablero->celdas[fila][columna]);
	if (numero == 0){ //esta vacia
		return true;
	}
	if(contar_apariciones_columna(tablero, numero, columna)>1){
		return false;
	}
	if(contar_apariciones_fila(tablero, numero, fila)>1){
		return false;
	}
	if(sector_contar_apariciones(&tablero->sectores[fila/3][columna/3], numero)>1){
		return false;	
	}

	return true;
}

bool tablero_verificar(tablero_t* tablero){
	for (int i = 0; i < NRO_FILAS; i++){
		for (int j = 0; j < NRO_COLUMNAS; j++){
			if (!verificar_celda(tablero, i,j)){
				return false;
			}
		}
	}
	return true;	
}


int tablero_modificar_celda(tablero_t* tablero, int fil, int col, int valor){
	if ((fil < 0) || (fil >= NRO_FILAS) || (col < 0) || (col >= NRO_COLUMNAS)){
		return 1;
	}
	celda_t* celda = &tablero->celdas[fil][col];

	if (!celda_es_modificable(celda)){
		return 2;
	}
	celda_poner_numero(celda, valor);
	return 0;
}

char* tablero_obtener(tablero_t* tablero){
	return impresor_imprimir_celdas((const celda_t **)tablero->celdas,\
	NRO_FILAS, NRO_COLUMNAS);
}

void tablero_destruir(tablero_t* tablero){
	for (int i = 0; i < NRO_FILAS/3; i++){
		for (int j = 0; j < NRO_COLUMNAS/3; j++){
			sector_destruir(&tablero->sectores[i][j]);
		}
		free(tablero->sectores[i]);
	}
	free(tablero->sectores);
	for (int i = 0; i < NRO_FILAS; i++){
		free(tablero->celdas[i]);	
	}
	free(tablero->celdas);
}
