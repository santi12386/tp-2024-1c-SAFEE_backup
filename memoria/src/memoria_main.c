#include <stdlib.h>
#include <stdio.h>
#include "memoria_main.h"

int Saludar(void) {
    logger = log_create("memoria.log", "memoria", 1, LOG_LEVEL_DEBUG);
	log_info(logger,"Hola! CPU");
    return 0;
}

int main(void) {

	pthread_t tid_kernel;
	pthread_t tid_cpu;
	pthread_t tid_io;
	char *ret_value;

	logger = log_create("memoria.log", "memoria", 1, LOG_LEVEL_DEBUG);
	config_global = config_create("../utils/config/config_global.config");
	
	//conexiones
	pthread_create(&tid_kernel, NULL, thread_crear_conexion_server, KERNEL_MEMORIA);
	pthread_create(&tid_cpu, NULL, thread_crear_conexion_server, CPU_MEMORIA);
	pthread_create(&tid_io, NULL, thread_crear_conexion_cliente, IO_MEMORIA);
	//conexiones

	//espero fin conexiones
	pthread_join(tid_kernel, ret_value);
	log_info(ret_value);
	pthread_join(tid_cpu, ret_value);
	log_info(ret_value);
	pthread_join(tid_io, ret_value);
	log_info(ret_value);
	//espero fin conexiones

	return 0;
}
void cliente_conexion_IO(char * puerto, char * ip){
	t_paquete* send_handshake;
	int conexion_IO;
	protocolo_socket op;
	int flag=1;

	sem_wait(server_io_memoria);
	//sleep?
	conexion_IO = crear_conexion(ip, puerto);
	send_handshake = crear_paquete(HANDSHAKE);
	agregar_a_paquete (send_handshake, "hola soy memoria", 16+1);

	while(flag){
		enviar_paquete(send_handshake, conexion_IO);
		sleep(1);
		op = recibir_operacion(conexion_IO);
		switch (op)
		{
		case HANDSHAKE:
			log_info("recibi handshake de IO");
			break;
		
		case TERMINATE:
			flag = 0;
			break;

		default:
			break;
		}
	}

	eliminar_paquete(send_handshake);
	liberar_conexion(conexion_IO);
}
void cliente_conexion_CPU(char * puerto, char * ip){
	t_paquete* send_handshake;
	int conexion_IO;
	protocolo_socket op;
	int flag=1;

	sem_wait(server_cpu_memoria);
	conexion_IO = crear_conexion(ip, puerto);
	send_handshake = crear_paquete(HANDSHAKE);
	agregar_a_paquete (send_handshake, "hola soy memoria", 16+1);

	while(flag){
		enviar_paquete(send_handshake, conexion_IO);
		sleep(1);
		op = recibir_operacion(conexion_IO);
		switch (op)
		{
		case HANDSHAKE:
			log_info("recibi handshake de CPU");
			break;
		
		case TERMINATE:
			flag = 0;
			break;

		default:
			break;
		}
	}

	eliminar_paquete(send_handshake);
	liberar_conexion(conexion_IO);
}
void cliente_conexion_KERNEL(char * puerto, char * ip){
	t_paquete* send_handshake;
	int conexion_IO;
	protocolo_socket op;
	int flag=1;

	sem_wait(server_kernel_memoria);
	conexion_IO = crear_conexion(ip, puerto);
	send_handshake = crear_paquete(HANDSHAKE);
	agregar_a_paquete (send_handshake, "hola soy memoria", 16+1);

	while(flag){
		enviar_paquete(send_handshake, conexion_IO);
		sleep(1);
		op = recibir_operacion(conexion_IO);
		switch (op)
		{
		case HANDSHAKE:
			log_info("recibi handshake de KERNEL");
			break;
		
		case TERMINATE:
			flag = 0;
			break;

		default:
			break;
		}
	}

	eliminar_paquete(send_handshake);
	liberar_conexion(conexion_IO);
}