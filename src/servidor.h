#ifndef _SERVIDOR_H_
#define _SERVIDOR_H_
#include "tablero.h"
#include "socket_tcp.h"



typedef struct servidor{
	tablero_t tablero;
	socket_tcp_t socket_tcp;
	socket_tcp_t socket_cliente;
	bool esta_conectado;
}servidor_t;

//Crea e inicia un servidor de sudoku con el tablero inicial leido de
//el archivo localizado en <ruta archivo>.
//PRE: debe existir un archivo en <ruta_archivo>, y debe tener la configuracion
//válida para crear un sudoku.
//POST: crea el servidor. Devuelve distinto de 0 en caso de error.
int servidor_crear(servidor_t* servidor, const char* ruta_archivo);

//Deja el servidor en modo escucha para que espere los comandos del cliente.
//Utiliza el serivicio /puerto indicado en el parametro.
int servidor_escuchar(servidor_t* servidor, const char* servicio);

//Destruye el servidor liberando sus recursos.
void servidor_destruir(servidor_t* servidor);

#endif
