#ifndef SERVER_H_
#define SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include "utils.h"
#include "server_utils.h"
#include "TAD.h"
#include "tad_kernel.h"
#include "sd_messages.h"
#include "socket_client.h"
#include <commons/string.h>
#include "tad_kernel.h"
#include "sd_memalloc.h"
#include "sd_memfree.h"
#include "sd_memwrite.h"
#include "sd_semaphore.h"
#include "semaforos_kernel.h"
#include "socket_client.h"
#include "sd_mate_close.h"
#include "sd_mate_call_io.h"
#include "mate_init.h"
#include "../include/respuesta_memoria.h"
#include "sd_res_memoria.h"
#include "sd_res_semaforo_kernel.h"

extern t_log* logger_kernel;
t_config_kernel *config_kernel;
void* iniciar_servidor_kernel();

#endif /* SERVER_H_ */
