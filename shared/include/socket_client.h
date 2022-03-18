#ifndef SOCKET_CLIENT_H_
#define SOCKET_CLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <netdb.h>
#include <string.h>
#include <stdbool.h>
#include <commons/log.h>
#include <commons/string.h>
#include "TAD.h"
#include "sd_messages.h"
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>

int crear_conexion(char *ip, char *puerto);
void *serializar_paquete(t_package paquete, int *bytes);
void liberar_conexion(int socket_cliente);
void sendMessage(t_package paquete, int socket_cliente);
t_package make_res(char *message);
#endif /* SOCKET_CLIENT_H_ */
