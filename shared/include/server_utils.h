#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <time.h>
#include <netdb.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include "TAD.h"
#include <commons/string.h>

void *recibir_buffer(int *, int);
int iniciar_servidor(char *IP, char *PUERTO);
int esperar_cliente(int socket_servidor);
t_list *recibir_paquete(int socket);
t_package recibir_mensaje(int socket_cliente);
t_package recibir_mensaje_v2(int socket_cliente);
int recibir_operacion(int);

#endif