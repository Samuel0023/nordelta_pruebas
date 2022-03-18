#include "../include/server_utils.h"

int iniciar_servidor(char* IP, char* PUERTO)
{
	int socket_servidor;

    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, PUERTO, &hints, &servinfo);

    // Creamos el socket de escucha del servidor

    // Asociamos el socket a un puerto
	for (p = servinfo; p != NULL; p = p->ai_next)
	{
		if ((socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
			continue;

		if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(socket_servidor);
			continue;
		}
		break;
	}
    // Escuchamos las conexiones entrantes
	listen(socket_servidor, SOMAXCONN);
	freeaddrinfo(servinfo);
    //log_trace(logger, "Listo para escuchar a mi cliente");

    return socket_servidor;
}

int esperar_cliente(int socket_servidor)
{
	struct sockaddr_in dir_cliente;
	socklen_t tam_direccion = sizeof(struct sockaddr_in);

	int socket_cliente = accept(socket_servidor, (void *)&dir_cliente, &tam_direccion);

	return socket_cliente;
}

int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if (recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) != 0)
		return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
}

void *recibir_buffer(int *size, int socket_cliente)
{
	void *buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

t_package recibir_mensaje_v2(int socket_cliente)
{
	t_package paquete;

	int state = 0;
	state = recv(socket_cliente, &(paquete.cod_operacion), sizeof(op_code), MSG_WAITALL);
	if(state == 0){
		paquete.cod_operacion = 0;
		paquete.buffer = malloc(1);
		return paquete;
	}
	state = recv(socket_cliente, &(paquete.tam_buffer), sizeof(int), MSG_WAITALL);
	//paquete.buffer = malloc(paquete.tam_buffer);
	state = recv(socket_cliente, paquete.buffer, paquete.tam_buffer, MSG_WAITALL);
	return paquete;
}


t_package recibir_mensaje(int socket_cliente)
{
	t_package paquete;

	int state = 0;
	state = recv(socket_cliente, &(paquete.cod_operacion), sizeof(op_code), MSG_WAITALL);
	if(state == 0){
		paquete.cod_operacion = 0;
		paquete.buffer = malloc(1);
		return paquete;
	}
	state = recv(socket_cliente, &(paquete.tam_buffer), sizeof(int), MSG_WAITALL);
	paquete.buffer = malloc(paquete.tam_buffer);
	state = recv(socket_cliente, paquete.buffer, paquete.tam_buffer, MSG_WAITALL);
	return paquete;
}

//podemos usar la lista de valores para poder hablar del for y de como recorrer la lista
t_list *recibir_paquete(int socket_cliente)
{
	int size;
	int desplazamiento = 0;
	void *buffer;
	t_list *valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);
	while (desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento += sizeof(int);
		char *valor = malloc(tamanio);
		memcpy(valor, buffer + desplazamiento, tamanio);
		desplazamiento += tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
	return NULL;
}
