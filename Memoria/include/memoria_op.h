#ifndef MEMORIA_OP_H_
#define MEMORIA_OP_H_
#include "config_servicio.h"
#include "utils.h"
#include <commons/config.h>
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
#include "swap_out.h"
#include "utils.h"
#include "sd_memwrite.h"
#include "TAD.h"
#include "tad_memoria.h"
#include "utils.h"
#include "heapmetadata.h"

void mem_mate_init(int process_id,int cliente_fd);
int memalloc(char*);
void memfree();
//void* memread(int);
int memwrite(t_msj_memwrite memwrite);
void* memread(t_msj_memwrite data);

#endif