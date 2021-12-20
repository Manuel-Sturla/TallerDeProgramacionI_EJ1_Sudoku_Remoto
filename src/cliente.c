#define _POSIX_C_SOURCE 200809L
#include "cliente.h"
#include "socket_tcp.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <arpa/inet.h>

#define TAMANIO_BUFFER_COMANDOS 16
#define TAMANIO_MSJ_SERVIDOR 4
#define MSJ_VERIFICAR "V"
#define MSJ_REINICIAR "R"
#define MSJ_OBTENER "G"
#define MSJ_PONER "P"

#define MSJ_COMANDO_INVALIDO "Comando invalido, intente nuevamente\n"
#define MSJ_INDICE_INVALIDO "Error en los índices. Rango soportado: [1,9]\n"
#define MSJ_VALOR_INVAL "Error en el valor ingresado. Rango soportado: [1,9]\n"
#define MSJ_INGRESO_COMANDOS "Ingrese un comando: "


int cliente_crear(cliente_t* cliente, const char* host, const char* puerto){
	socket_tcp_crear(&cliente->socket_tcp);
	if (socket_tcp_conectar(&cliente->socket_tcp, host, puerto) != 0){
		fprintf(stderr, "No se pudo crear el cliente, error al conectar\n");
		socket_tcp_destruir(&cliente->socket_tcp);
		return 1;
	}
	return 0;
}

void enviar_al_servidor(cliente_t* cliente, const char* mensaje){
	socket_tcp_enviar(&cliente->socket_tcp, mensaje , strlen(mensaje));
}

char* recibir_mensaje_servidor(cliente_t* cliente){
	unsigned int tamanio;
	socket_tcp_recibir(&cliente->socket_tcp, &tamanio, TAMANIO_MSJ_SERVIDOR);
	tamanio = ntohl(tamanio);
	char* mensaje = malloc(sizeof(char)*tamanio + 1); 
	if (mensaje == NULL){
		return NULL;
	}
	socket_tcp_recibir(&cliente->socket_tcp, mensaje, tamanio);
	mensaje[tamanio] = '\0';
	return mensaje;
}



void realizar_comando(cliente_t* cliente, const char* mensaje_enviar){
	enviar_al_servidor(cliente, mensaje_enviar);
	char* mensaje_recibido = recibir_mensaje_servidor(cliente);
	if (mensaje_recibido == NULL) return;
	printf("%s", mensaje_recibido);
	free(mensaje_recibido);
}

int esta_en_rango(int numero){
	return (numero > 0) && (numero <= 9);
}

int verificar_valores(int valor, int fila, int columna){
	if (!esta_en_rango(fila) || !esta_en_rango(columna)){
		fprintf(stderr, "%s", MSJ_INDICE_INVALIDO);
		return 1;
	}else if (!esta_en_rango(valor)){
		fprintf(stderr, "%s", MSJ_VALOR_INVAL);
		return 1;
	}
	return 0;
}

char* crear_comando_poner(int valor, int fila, int columna){
	char* comando = malloc(sizeof(char) * 5);
	if (comando == NULL){
		return NULL;
	}
	comando[0] = 'P';
	comando[1] = (unsigned char) fila;
	comando[2] = (unsigned char) columna;
	comando[3] = (unsigned char) valor;
	comando[4] = '\0';
	return comando;
}


char* parsear_comando(char* comando){
	char* parametros[4];
	char* comando_poner = NULL;
	for (int i = 0; i < 4; i++){
		char* param = strtok_r(NULL, " ,", &comando);
		parametros[i] = param;
	}
	int valor = atoi(parametros[0]);
	int fila = atoi(parametros[2]);
	int columna = atoi(parametros[3]);
	if (verificar_valores(valor, fila, columna) == 0){
		comando_poner = crear_comando_poner(valor, fila, columna);
	}
	return comando_poner;
}
void cliente_recibir_comandos(cliente_t* cliente){
	char* comando = NULL;
	char* guardado;
	size_t tamanio;
	while (getline(&comando, &tamanio, stdin) > 0){
		if (strlen(comando) > TAMANIO_BUFFER_COMANDOS){
			fprintf(stderr, "%s\n", MSJ_COMANDO_INVALIDO);
			continue;
		}
		comando[0] = toupper(comando[0]);
		char * primera_palabra = strtok_r(comando, " ", &guardado);
		if (strcmp(primera_palabra, "Verify\n") == 0){
			realizar_comando(cliente, MSJ_VERIFICAR);
		}else if (strcmp(primera_palabra, "Get\n") == 0){
			realizar_comando(cliente, MSJ_OBTENER);
		}else if (strcmp(primera_palabra, "Put") == 0){
			char* comando_poner = parsear_comando(guardado);
			if (comando_poner == NULL) continue;
			realizar_comando(cliente, comando_poner);
			free(comando_poner);
		}else if (strcmp(primera_palabra, "Reset\n") == 0){
			realizar_comando(cliente, MSJ_REINICIAR);
		}else if (strcmp(primera_palabra, "Exit\n") == 0){
			break;
		}else{
			fprintf(stderr, "%s\n", MSJ_COMANDO_INVALIDO);
		}
	}
	free(comando);
}

void cliente_destruir(cliente_t* cliente){
	socket_tcp_destruir(&cliente->socket_tcp);
}
