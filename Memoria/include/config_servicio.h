#ifndef SERVIDOR_MEMORIA_H_
#define SERVIDOR_MEMORIA_H_

#include <commons/config.h>
#include "tad_memoria.h"

extern t_config * config;

char * get_ip();
char * get_puerto();
char * get_ip_swamp();
char * get_puerto_swamp();
int get_tamanio();
int get_tamanio_pagina();
char * get_tipo_asignacion();
int get_marcos_por_carpincho();
char * get_algoritmo_reemplazo_mmu();
int get_cantidad_entradas_tlb();
char * get_algoritmo_reemplazo_tlb();
int get_retardo_acierto_tlb();
int get_retardo_fallo_tlb();
char* get_path_dump_tlb();

#endif