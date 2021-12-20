#define _POSIX_C_SOURCE 200112L
#include "socket_tcp.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>




void socket_tcp_crear(socket_tcp_t* self){
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1){
		return;
	}
	self->fd = fd;
}

void setear_addrinfo_tcp_ipv4(struct addrinfo* hints){
	memset(hints, 0, sizeof(struct addrinfo));
	hints->ai_family = AF_INET;
	hints->ai_socktype = SOCK_STREAM;
}

int socket_tcp_conectar(socket_tcp_t* self, const char* host,\
	const char* servicio){
	struct addrinfo hints;
	struct addrinfo* resultados, *dir_act;

	setear_addrinfo_tcp_ipv4(&hints);
	hints.ai_flags = 0;

	int estado = getaddrinfo(host, servicio, &hints, &resultados);
	if (estado != 0){
		freeaddrinfo(resultados);
		return 1;
	}

	for (dir_act = resultados; dir_act != NULL; dir_act = dir_act->ai_next){
		if (connect(self->fd, dir_act->ai_addr, dir_act->ai_addrlen) != -1){
			break;
		}	
	}
	if (!dir_act){
		freeaddrinfo(resultados);
		return 2;
	}
	freeaddrinfo(resultados);
	return 0;
}


int socket_tcp_bind_and_listen(socket_tcp_t* self, const char* servicio){
	struct addrinfo hints;
	struct addrinfo* resultados, *dir_act;
	setear_addrinfo_tcp_ipv4(&hints);
	hints.ai_flags = AI_PASSIVE;

	int variable = 1;
    int salida = setsockopt(self->fd, SOL_SOCKET, SO_REUSEADDR, \
    	&variable, sizeof(variable));
    if (salida < 0) {
	    return 4;
    }

	int estado = getaddrinfo(NULL, servicio, &hints, &resultados);
	if (estado != 0){
		freeaddrinfo(resultados);
		return 1;
	}
	//Podria ser una funcion auxiliar con un puntero a funcion
	for (dir_act = resultados; dir_act != NULL; dir_act = dir_act->ai_next){
		if (bind(self->fd, dir_act->ai_addr, dir_act->ai_addrlen) != -1){
			break;
		}
	}
	freeaddrinfo(resultados);
	if (!dir_act){
		return 2;
	}
	if (listen(self->fd, COLA_CONECCIONES) != 0){
		return 3;
	}

	return 0;
}

int socket_tcp_recibir(socket_tcp_t* self, void* buffer, size_t longitud){
	ssize_t recibidos = 0;
	do{
		char* buff_nuevo = (char*)buffer;
		ssize_t nuevos = recv(self->fd, &buff_nuevo[recibidos], \
			longitud-recibidos, 0);
		if (nuevos < 0){
			return -1;
		} else if (nuevos == 0) {  
			break;
		}
		recibidos += nuevos;	
	}while (recibidos < longitud);
	return recibidos;
}

int socket_tcp_enviar(socket_tcp_t* self, const void* buffer, size_t longitud){
	ssize_t enviados = 0;
	do{
		char* buff_nuevo = (char*) buffer;
		ssize_t nuevos = send(self->fd, &buff_nuevo[enviados],\
			longitud-enviados, MSG_NOSIGNAL);
		if (nuevos < 0){
			return -1;
		}
		enviados += nuevos;
	}while (enviados < longitud);
	return enviados;
}

int socket_tcp_aceptar(socket_tcp_t* self, socket_tcp_t* socket_cliente){
	int fd_cliente = accept(self->fd, NULL, NULL);
	if (fd_cliente < 0){
		return -1;
	}
	socket_cliente->fd = fd_cliente;
	return 0; 				
}

int socket_tcp_destruir(socket_tcp_t* self){
	shutdown(self->fd, 2);
	if (close(self->fd) == -1){
		return 1;
	}
	return 0;
}
