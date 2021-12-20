#ifndef _SOCKET_TCP_H_
#define _SOCKET_TCP_H_
#include <stdlib.h>

#define COLA_CONECCIONES 20
typedef struct socket_tcp{
	int fd;
}socket_tcp_t;

//Crea un socket IPV4 del tipo TCP.
//POST: crea el socket, en caso de error imprime un mensaje y deja el
//socket recibido inalterado.
void socket_tcp_crear(socket_tcp_t* self);

//Envia tantos bytes como indique la longitud, de la informacion almacenada
//en el buffer. Lo envia el socket conectado en <socket_tcp_aceptar>
//PRE: el buffer debe contener tanto espacio como longitud del mensaje que se
//quiere mandar.
//POST: Devuelve la cantidad de bytes que fueron enviados o un numero negativo
//en caso de error.
int socket_tcp_enviar(socket_tcp_t* self, const void* buffer, size_t longitud);

//Recibe informacion hasta recibir <longitud> bytes. Esta debe ser
//menor o igual a la capacidad del buffer.
//PRE: el socket debe estar creado y debe ser resultado de una conexion
//aceptada y el buffer y la longitud deben ser suficientes para el mensaje
//que se quiera recibir.
//POST: devuelve la cantidad de bytes recibidos.
int socket_tcp_recibir(socket_tcp_t* self, void* buffer, size_t longitud);

//Conecta el socket a una direccion con el host y servicio especificado.
//PRE: el socket fue creado.
//POST: se conecta al host indicado y devuelve 0. En caso de error:
// * 1: Error en la obtencion de direcciones
// * 2: No se pudo conectar.
int socket_tcp_conectar(socket_tcp_t* self, const char* host, \
	const char* servicio);

//Crea un socket pasivo que escuche conecciones en el servicio/puerto
//indicado.
//PRE: el socket fue creado. Y el servicio debe ser válido para los permisos
//dados al programa.
//POST: se crea un socket escuchando a la direccion con una cola de conecciones
//de valor COLA_CONECCIONES.
int socket_tcp_bind_and_listen(socket_tcp_t* self, const char* servicio);

//Acepta una conceccion entrante en el socket recibido. Y guarda en un 
//socket nuevo el socket al cual se acepto
//PRE: este debe haber sido puesto a escuchar una direccion con bind_and_listen
//el socket cliente no es necesario que este inicializado, dado que se
//inicializara de forma correcta en caso de que se acepte correctamente 
//la coneccion.
//POST: devuelve 0 si se aceptó correctamente. -1 en caso de error
int socket_tcp_aceptar(socket_tcp_t* self, socket_tcp_t* socket_cliente);

//Cierra el socket liberando los recursos
//Devuelve 1 en caso de error, 0 si no lo hubo.
int socket_tcp_destruir(socket_tcp_t* self);

#endif
