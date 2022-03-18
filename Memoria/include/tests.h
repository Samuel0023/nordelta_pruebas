#ifndef TESTS_H
#define TESTS_H
#include <CUnit/Basic.h>
#include <TAD.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "sd_pageswamp.h"
#include "heapmetadata.h"
#include "get_pagina.h"
#include "config_servicio.h"
#include "funciones_tlb.h"
#include "algoritmos_de_remplazo.h"
#include "memoria.h"
#include "tad_memoria.h"
#include "commons/temporal.h"
#include "time.h"
#include "string.h"

int run_tests();
void verificar_marcos_a_usar();
void verificar_bitmap_disponible();
void ser_des_pageswamp();
void ser_des_heapmetadata();
void verificar_malloc_size();
void verificar_search_pages();
void verificar_transferencia_swap();
void verificar_get_cant_marcos_swap();
void verificar_getTablaPaginas();
void verificar_DLconvertToDF();
void verificar_getNextHeapMetaData();
void verificar_hayAllocDisponible_con_tablaPaginaProceso_vacia();
void validar_find_page_in_pageTable();
void validar_get_paginas_id_memoria();
void validar_get_all_paginas_memoria();
void validar_algoritmo_MMU();
void verificar_mem_mate_init();
void verificar_algoritmos_tlb_Y_getPaginaTLB();

void verificar_inicializar_entradas_tlb();

void verificar_suma_tlb_hit_miss();
void verificar_limpiar_entradas_tlb();
void generar_dump_tlb_duplicado();
extern void *stream_memoria;
extern int conexionSWAMP;

#endif