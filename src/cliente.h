#ifndef _CLIENTE_H_
#define _CLIENTE_H_
#include "socket_tcp.h"

typedef struct cliente{
	socket_tcp_t socket_tcp;
}cliente_t;

//Crea un cliente y lo conecta al servidor en el puerto y host indicado.
//PRE: el servidor debe estar inciado y escuchando en el puerto y host que
//se intenta conectar.
//POST: crea y conceta el cliente.
int cliente_crear(cliente_t* cliente, const char* host, const char* puerto);

//Pone al cliente en modo escucha para recibir comandos por STDIN, hasta que
//se ingrese el comando de salida: "exit".
//PRE: el cliente debe estar creado correctamente.
//POST: verifica y ejecuta los comandos recibidos. Devuelve 1
void cliente_recibir_comandos(cliente_t* cliente);

//Destruye el cliente liberando los recursos.
void cliente_destruir(cliente_t* cliente);

#endif
