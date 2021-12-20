#include "tablero.h"
#include "celda.h"
#include "test.h"
#include "sector.h"
#include "socket_tcp.h"
#include "servidor.h"
#include "impresor.h"
#include <stdio.h>
#include <string.h>

#define SALIDA_TABLERO_INICIADO "U===========U===========U===========U\n\
U 3 |   | 5 U   |   | 8 U   | 1 |   U\n\
U---+---+---U---+---+---U---+---+---U\n\
U   |   | 7 U   |   |   U 5 |   | 8 U\n\
U---+---+---U---+---+---U---+---+---U\n\
U 1 | 2 |   U 7 | 5 |   U   | 9 |   U\n\
U===========U===========U===========U\n\
U   |   | 9 U   | 7 |   U   |   | 4 U\n\
U---+---+---U---+---+---U---+---+---U\n\
U   |   | 4 U 3 |   | 5 U 9 |   |   U\n\
U---+---+---U---+---+---U---+---+---U\n\
U 7 |   |   U   | 9 |   U 8 |   |   U\n\
U===========U===========U===========U\n\
U   | 3 |   U   | 4 | 6 U   | 5 | 7 U\n\
U---+---+---U---+---+---U---+---+---U\n\
U 4 |   | 6 U   |   |   U 1 |   |   U\n\
U---+---+---U---+---+---U---+---+---U\n\
U   | 7 |   U 5 |   |   U 6 |   |   U\n\
U===========U===========U===========U\n"

#define SALIDA_TABLERO_MODIFICADO "U===========U===========U===========U\n\
U 3 |   | 5 U   |   | 8 U   | 1 |   U\n\
U---+---+---U---+---+---U---+---+---U\n\
U   | 7 | 7 U   |   |   U 5 |   | 8 U\n\
U---+---+---U---+---+---U---+---+---U\n\
U 1 | 2 |   U 7 | 5 |   U   | 9 |   U\n\
U===========U===========U===========U\n\
U   |   | 9 U   | 7 |   U   |   | 4 U\n\
U---+---+---U---+---+---U---+---+---U\n\
U   |   | 4 U 3 |   | 5 U 9 |   |   U\n\
U---+---+---U---+---+---U---+---+---U\n\
U 7 |   |   U   | 9 |   U 8 |   |   U\n\
U===========U===========U===========U\n\
U   | 3 |   U   | 4 | 6 U   | 5 | 7 U\n\
U---+---+---U---+---+---U---+---+---U\n\
U 4 |   | 6 U   |   |   U 1 |   |   U\n\
U---+---+---U---+---+---U---+---+---U\n\
U   | 7 |   U 5 |   |   U 6 |   |   U\n\
U===========U===========U===========U\n"
void pruebas_de_test(){
	probar("Pruebo que funciona la funcion de prueba con true", true);
	probar("Pruebo que funciona la funcion de prueba con false", false);
	probar("La funcion de prueba con comparacion simple", 1==1);
	probar("La funcion de prueba con un numero", 10);
	probar("La funcion de prueba con 0 da error", !0);

}
void pruebas_celda_vacia(){
	printf("\n *** Inicio pruebas CELDA ***\n");

	celda_t celda;
	probar("Celda vacia (con 0) se crea correctamente", celda_crear(&celda, 0, 0) == 0);
	memset(&celda, 0, sizeof(celda_t));
	probar("Crear celda con numero invalido da error", celda_crear(&celda, 100, 0) == 1);
	memset(&celda, 0, sizeof(celda_t));
	probar("Crear celda con numeros negativo da error", celda_crear(&celda, -10,0) == 1);
	memset(&celda, 0, sizeof(celda_t));
	celda_crear(&celda, 1, 0);
	celda_poner_numero(&celda, 5);
	probar("Modificar una celda no modificable no cambia su valor", celda_obtener_numero(&celda) == 1);
	memset(&celda, 0, sizeof(celda_t));
	celda_crear(&celda, 1, 1);
	celda_poner_numero(&celda, 15);
	probar("Modificar una celda con un valor invalido no la modifica", celda_obtener_numero(&celda) == 1);
	memset(&celda, 0, sizeof(celda_t));
	celda_crear(&celda, 1, 1);
	celda_poner_numero(&celda, 5);
	probar("Modificar celda cambia su valor", celda_obtener_numero(&celda) == 5);
}

void pruebas_sector(){
	printf("\n *** Inicio pruebas SECTOR ***\n");
	sector_t sector;
	probar("Crear sector se crea correctamente", sector_crear(&sector) == 0);
	celda_t celda;
	celda_crear(&celda, 5, 1);
	probar("Agregar celda se agrega correctamente", sector_agregar_celda(&sector, &celda) == 0);
	sector_destruir(&sector);
	sector_crear(&sector);
	for (int i = 1; i <= 9; i++){
		sector_agregar_celda(&sector, &celda);
	}
	probar("Agregar celda en sector lleno, devuelve error", sector_agregar_celda(&sector, &celda) == 1);
	probar("Contar apariciones de un numero que no hay es 0", sector_contar_apariciones(&sector, 1) == 0);
	probar("Contar apariciones de un numero que hay es correcto", sector_contar_apariciones(&sector, 5) == 9);
	sector_destruir(&sector);

}

void pruebas_tablero(){
	printf("\n *** Inicio pruebas TABLERO ***\n");
	tablero_t tablero;
	probar("Crear tablero con archivo inexistente, da error", tablero_crear(&tablero, "Archivo_inexistente") == 1);
	memset(&tablero, 0, sizeof(tablero_t));
	probar("Crear tablero con archivo que existe no da error", tablero_crear(&tablero, "board.txt") == 0);
	
	FILE* archivo = fopen("board.txt","r");
	if (!archivo){printf("No se pudo abrir el archivo\n");}
	int fila = 0;
	int columna = 0;
	bool ok = true;
	while (!feof(archivo)) {
		char caracter = (char)fgetc(archivo);
		if ((caracter == ' ') || (caracter == '\n') || caracter == EOF){
			continue;
		}
		if (caracter - '0' != celda_obtener_numero(&tablero.celdas[fila][columna])){
			ok = false;
		} 
		if (columna == 8){
			fila++;
			columna = 0;
		}else{
			columna++;
		}
	}
	probar("Crear tablero desde archivo se crea con los valores correctos", ok);
	fclose(archivo);
	tablero_destruir(&tablero);

	tablero_crear(&tablero, "board.txt");
	probar("Modificar una celda NO modificable del tablero da error y no la modifica", \
		(tablero_modificar_celda(&tablero, 0, 0, 5) == 2) && (celda_obtener_numero(&tablero.celdas[0][0]) == 3));
	
	probar("Modificar celda en posicion invalida da error", tablero_modificar_celda(&tablero, -1, -1, 3) == 1);
	probar("Modificar celda en posicion invalida 2 da error", tablero_modificar_celda(&tablero, 150, 5, 3) == 1);

	tablero_modificar_celda(&tablero, 4, 0, 90);
	probar("Modifcar celda con valor invalido no la cambia", celda_obtener_numero(&tablero.celdas[4][0]) == 0);

	probar("Modificar una celda modificable no da error", tablero_modificar_celda(&tablero,0, 1, 4) == 0);
	tablero_modificar_celda(&tablero,0, 3, 5);
	probar("Modificar una celda cambia su valor", celda_obtener_numero(&tablero.celdas[0][3]) == 5);
	probar("Modificar celda en extremo no da error", tablero_modificar_celda(&tablero, 8, 8, 7) == 0);
	probar("Modificar celda en extremo cambia su valor", celda_obtener_numero(&tablero.celdas[8][8]) == 7);
	tablero_destruir(&tablero); 

	//Inicio pruebas del reiniciar
	tablero_crear(&tablero, "board.txt");
	tablero_modificar_celda(&tablero,0, 3, 7);
	tablero_reiniciar(&tablero);
	probar("Reiniciar el tablero lo reinicia", celda_obtener_numero(&tablero.celdas[0][3]) == 0);
	tablero_destruir(&tablero);

	//Inicio pruebas del verificar
	tablero_crear(&tablero, "board.txt");
	probar("Verificar un tablero recien creado (y valido) da True", tablero_verificar(&tablero));
	tablero_modificar_celda(&tablero, 0, 1, 9);
	probar("Verificar tablero con una modificacion valida da True", tablero_verificar(&tablero));
	tablero_reiniciar(&tablero);	
	
	tablero_modificar_celda(&tablero, 6, 2, 9);
	probar("Verificar tablero con repeticion en columna", !tablero_verificar(&tablero));
	tablero_reiniciar(&tablero);

	tablero_modificar_celda(&tablero, 0, 1, 8);
	probar("Verificar tablero con repeticion en fila", !tablero_verificar(&tablero));
	tablero_reiniciar(&tablero);

	tablero_modificar_celda(&tablero, 5, 7, 4);
	probar("Verificar tablero con repeticion en sector", !tablero_verificar(&tablero));
	tablero_destruir(&tablero);


	probar("Crear tablero con pistas vacias no explota", tablero_crear(&tablero, "board_vacio.txt") == 0);
	tablero_reiniciar(&tablero);
	probar("REiniciar tablero vacio anda", true);
	tablero_destruir(&tablero);

}


int recibir_mensaje(socket_tcp_t* s){
	char buffer[10];
	int recibido = socket_tcp_recibir(s, buffer, 10);
	printf("--- Recibi: %s\n", buffer);
	return recibido;
}

void pruebas_socket(){
	printf("\n *** Inicio pruebas SOCKET ***\n");
	socket_tcp_t s;
	socket_tcp_crear(&s);
	probar("Crear socket lo crea correctamente", s.fd != -1);
	socket_tcp_destruir(&s);

	// socket_tcp_crear(&s);
	// probar("Conectar a Localhost anda", socket_tcp_conectar(&s, "localhost", "5555") == 0);
	// char* cad = "123456789123456789";
	// probar("Enviar cadena no da error", socket_tcp_enviar(&s, cad, 10) > 0);
	// probar("Recibir cadena recibe algo", recibir_mensaje(&s) > 0);
	// socket_tcp_destruir(&s);

	socket_tcp_crear(&s);
	
	probar("Bind and listen en un puerto funciona", socket_tcp_bind_and_listen(&s, "5558") == 0);
	socket_tcp_t socket_cliente;
	probar("Aceptar una conexion entrante no da error", socket_tcp_aceptar(&s, &socket_cliente) == 0);	
	int recibidos = recibir_mensaje(&socket_cliente);
	probar("REcibir mensajes recibe algo", recibidos > 0);
	char* cad2 = "Holus\n";
	probar("Enviar mensaje manda algo", socket_tcp_enviar(&socket_cliente, cad2, 7) > 0);
	socket_tcp_destruir(&s);


}

void pruebas_servidor(){
	printf("\n *** Inicio pruebas SERVIDOR ***\n");
	servidor_t servidor;
	probar("Crear servidor con archivo valido no da error", servidor_crear(&servidor, "board.txt") == 0);
	servidor_destruir(&servidor);
	probar("Crear servidor con archivo invalido da error", servidor_crear(&servidor, "xx.txt") != 0);
	
	servidor_crear(&servidor, "board.txt");
	servidor_escuchar(&servidor, "5556");
	
}

void pruebas_impresor(){
	printf("\n *** Inicio pruebas IMPRESOR ***\n");

	tablero_t tablero;
	tablero_crear(&tablero, "board.txt");
	char* cadena = tablero_obtener(&tablero);
	//printf("%s\n", cadena );
	probar("El impresor imprime el tablero de forma correcta", strcmp( cadena, SALIDA_TABLERO_INICIADO) == 0);
	free(cadena);
	tablero_modificar_celda(&tablero, 1, 1, 7);
	char* cadena2 = tablero_obtener(&tablero);
	probar("Luego de modificar, la salida se modifica", strcmp( cadena2, SALIDA_TABLERO_MODIFICADO) == 0);
	free(cadena2);	
	tablero_destruir(&tablero);
}

int main(){
	pruebas_de_test();
	pruebas_celda_vacia();
	pruebas_sector();
	pruebas_tablero();
	//pruebas_socket();
	//pruebas_servidor();
	pruebas_impresor();
	return 0;
}

