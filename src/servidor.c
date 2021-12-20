#include "servidor.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define LONG_MSJ_LONGITUD 4
#define MSJ_OK "OK\n"
#define LONG_MSJ_OK strlen(MSJ_OK)
#define MSJ_ERROR "ERROR\n"
#define LONG_MSJ_ERROR strlen(MSJ_ERROR)
#define MSJ_CELDA_NO_MODIFICABLE "La celda indicada no es modificable\n"
#define LONG_MSJ_CELDA_NO_MOD strlen(MSJ_CELDA_NO_MODIFICABLE)

int servidor_crear(servidor_t* servidor, const char* ruta_archivo){
	tablero_t tablero;
	if (tablero_crear(&tablero, ruta_archivo) != 0){
		fprintf(stderr, "Error al crear tablero\n");
		return 1;
	}
	servidor->tablero = tablero;
	servidor->esta_conectado = false;
	return 0;
}

void enviar_al_cliente(servidor_t* servidor, const char* mensaje, size_t len){
	unsigned int long_int = htonl(len);
	socket_tcp_enviar(&servidor->socket_cliente, &long_int, LONG_MSJ_LONGITUD);
	socket_tcp_enviar(&servidor->socket_cliente, mensaje, len);
}

void enviar_tablero(servidor_t* servidor){
	char* representacion = tablero_obtener(&servidor->tablero);
	enviar_al_cliente(servidor, representacion, strlen(representacion));
	free(representacion);
}

//Recibe los comandos del cliente segun el protocolo. Si el socket 
//del cliente se cierra devuelve -1  para indicar que la conexion fue terminada
int recibir_comandos(servidor_t* servidor, char* comando){ 
	int recibidos;
	memset(comando, '\0', 4);
	recibidos = socket_tcp_recibir(&servidor->socket_cliente, comando, 1);
	if (recibidos < 0){
		return 1;
	}else if (recibidos == 0){ 
		return -1;
	}
	if (comando[0] == 'P'){
		recibidos = socket_tcp_recibir(&servidor->socket_cliente,\
			&comando[1], 3);
		if (recibidos < 0){ return 1;}
	}	
	return 0;
}

void verificar(servidor_t* servidor){
	if (tablero_verificar(&servidor->tablero)){
		enviar_al_cliente(servidor, MSJ_OK, LONG_MSJ_OK);
	}else{
		enviar_al_cliente(servidor, MSJ_ERROR, LONG_MSJ_ERROR);
	}
}

void poner(servidor_t* servidor, int fila, int col, int valor){
	//Si la celda no es modificable
	if (tablero_modificar_celda(&servidor->tablero, fila, col, valor) == 2){
			enviar_al_cliente(servidor, MSJ_CELDA_NO_MODIFICABLE, \
				LONG_MSJ_CELDA_NO_MOD);
	}else{
			enviar_tablero(servidor);
	}
}

void ejecutar_comando(servidor_t* servidor, const char* comando){
	if (comando[0] == 'V'){
		verificar(servidor);
	}else if (comando[0] == 'R'){
		tablero_reiniciar(&servidor->tablero);
		enviar_tablero(servidor);
	}else if (comando[0] == 'G'){
		enviar_tablero(servidor);
	}else if (comando[0] == 'P'){
		poner(servidor, comando[1] -1, comando[2] -1, comando[3]);
	}
}
int servidor_escuchar(servidor_t* servidor, const char* servicio){
	socket_tcp_crear(&servidor->socket_tcp);
	if (socket_tcp_bind_and_listen(&servidor->socket_tcp, servicio) != 0){
		fprintf(stderr, "Error al poner el socket en modo escucha\n");
		return 1;
	}
	servidor->esta_conectado = true;
	if (socket_tcp_aceptar(&servidor->socket_tcp, \
		&servidor->socket_cliente) < 0){ 
		fprintf(stderr, "Error en aceptar\n");
		return 2;
	}
	char comando[4];
	while (1) {	
		if (recibir_comandos(servidor, comando) < 0){
			break;
		}
		ejecutar_comando(servidor, comando);
	}
	servidor_destruir(servidor);
	return 0;
}

void servidor_destruir(servidor_t* servidor){
	tablero_destruir(&servidor->tablero);
	if (servidor->esta_conectado){
		socket_tcp_destruir(&servidor->socket_tcp);
	}
	return;
}
