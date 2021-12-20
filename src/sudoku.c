#include "servidor.h"
#include "cliente.h"
#include <stdio.h>

#define MSJ_ERROR_ARG "Modo no soportado, el primer parámetro debe ser "\
					"server o client\n"
int main(int argc, char **argv){
	if (argc == 4){
		cliente_t cliente;
		if (cliente_crear(&cliente, argv[2], argv[3]) != 0){
			return 1;
		}
		cliente_recibir_comandos(&cliente);
		cliente_destruir(&cliente);
	}else if (argc == 3){
		servidor_t servidor;
		if (servidor_crear(&servidor, "board.txt") != 0){
			return 1;
		}
		servidor_escuchar(&servidor, argv[2]);
	}else{
		printf("%s\n", MSJ_ERROR_ARG);
	}
	return 0;
}
