#ifndef RESPUESTA_KERNAL_SEM_H_
#define RESPUESTA_KERNAL_SEM_H_

#include "tad_kernel.h"
#include "global.h"
#include "socket_client.h"
#include "sd_res_semaforo_kernel.h"
#include "TAD.h"

void enviar_respuesta_mate_sem_init(int socket, int estado_res);
void enviar_respuesta_mate_sem_wait(int socket, int estado_res);
void enviar_respuesta_mate_sem_post(int socket, int estado_res);
void enviar_respuesta_mate_sem_destoy(int socket, int estado_res);

#endif