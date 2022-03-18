#ifndef RESPUESTA_MEMORIA_H_
#define RESPUESTA_MEMORIA_H_

#include "tad_kernel.h"
#include "global.h"
#include "socket_client.h"
#include "sd_res_memoria.h"

void enviar_respuesta_memalloc(int socket, int estado_res);
void enviar_respuesta_memfree(int socket, int estado_res);
void enviar_respuesta_memread(int socket, int estado_res);
void enviar_respuesta_memwrite(int socket, int estado_res);

#endif