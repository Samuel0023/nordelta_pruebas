#ifndef SERVIDOR_MEMORIA_H_
#define SERVIDOR_MEMORIA_H_

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "TAD.h"
#include "tad_memoria.h"
#include "socket_client.h"
#include "server_utils.h"
#include "sd_messages.h"
#include "memoria_op.h"
#include "sd_memalloc.h"
#include "sd_memwrite.h"
#include "swap_out.h"
#include "sd_res_memoria.h"


extern pthread_mutex_t mutex_mem_alloc;
extern t_log * logger;

void iniciar_servidor_memoria(char * ip, char * puerto);

void* ejecutar_operacion_memoria(int cliente_fd);
#endif