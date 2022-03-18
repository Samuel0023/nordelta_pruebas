#ifndef MEMORIA_H_
#define MEMORIA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/config.h>
#include "server_utils.h"
#include "TAD.h"
#include "servidor_memoria.h"
#include "heapmetadata.h"
#include "config_servicio.h"
#include "tad_memoria.h"
#include <signal.h>
#include <time.h>
#include "funciones_tlb.h"

void inicializacion_estructuras_memoria();

void inicializacion();

extern t_list *pageTables; //[t_page_table]
extern void *stream_memoria;
extern int conexionSWAMP;
extern pthread_mutex_t mutex_mem_alloc;

// variables que se usaran en la tlb

int timestamp;
pthread_mutex_t MX_TIMESTAMP;
// int tlb_hit_totales; /*  */
// int tlb_miss_totales;

#endif /* memoria */