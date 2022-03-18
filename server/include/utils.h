#ifndef UTILS_H_
#define UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<time.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>

t_log* logger;

int esperar_cliente(int);
t_list* recibir_paquete_tp0(int);
void recibir_mensaje_tp0(int);

#endif /* UTILS_H_ */
