#include "../include/socket_client.h"

int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	//hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
	
	if (connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
	{
		//freeaddrinfo(server_info);
		return -1;
	}
	freeaddrinfo(server_info);
	return socket_cliente;
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}

t_package make_res(char *message){
    t_message res;
    res.message = message;
    res.long_message = strlen(message);
    return ser_message(res);
}
void sendMessage(t_package paquete, int socket_cliente)
{
	int bytes = 0;
	void *a_enviar = serializar_paquete(paquete, &bytes);
	send(socket_cliente, a_enviar, bytes, 0);

	free(paquete.buffer);
	free(a_enviar);
}

/*
 * Recibe un paquete a serializar, y un puntero a un int en el que dejar
 * el tamaño del stream de bytes serializados que devuelve
*/
void *serializar_paquete(t_package paquete, int *bytes)
{
	(*bytes) = sizeof(op_code) + sizeof(uint32_t) + paquete.tam_buffer;
	void * a_enviar = malloc((*bytes));
	int desplazamiento = 0;

	memcpy(a_enviar + desplazamiento, &(paquete.cod_operacion), sizeof(op_code));
	desplazamiento+= sizeof(op_code);
	memcpy(a_enviar + desplazamiento, &(paquete.tam_buffer), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(a_enviar + desplazamiento, paquete.buffer, paquete.tam_buffer);
	desplazamiento += paquete.tam_buffer;

	return a_enviar;
}
