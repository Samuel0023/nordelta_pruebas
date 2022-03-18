#ifndef FUNCIONES_TLB_H_
#define FUNCIONES_TLB_H_

#include "TAD.h"
#include "tad_memoria.h"
#include "sd_pageswamp.h"
#include "config_servicio.h"
#include <commons/bitarray.h>
#include "socket_client.h"
#include "server_utils.h"
#include "bitmap_utils.h"
#include "heapmetadata.h"
#include "get_pagina.h"
#include <commons/collections/list.h>
#include "commons/temporal.h"
#include "time.h"
#include "utils.h"
#include "fcntl.h"
#include <sys/mman.h>

//extern int tlb_hit_totales;
//extern int tlb_miss_totales;
extern pthread_mutex_t MX_TIMESTAMP;
extern int timestamp;

void inicializar_entradas_tlb();
t_page* getPaginaMMU(int pageID, int procesoID);
t_entrada_tlb *entrada_fifo_tlb(int *pos_reemplazo);
t_entrada_tlb *entrada_lru_tlb(int *pos_reemplazo);
void agregar_entrada_tlb(t_page* una_pagina);
int getPaginaTLB(int pageID, int procesoID);
void sumar_tlb_miss(int procesoID);
void sumar_tlb_hit(int procesoID);
void mostrar_tlb_hit_miss_globales();
void mostrar_tlb_hit_miss_por_proceso();

//funcion para SIGURS2
void limpiar_entradas_tlb();
void generar_dump_tlb();
#endif