#ifndef TAD_MEMORIA_H
#define TAD_MEMORIA_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <netdb.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <commons/bitarray.h>

/*
    1 proceso tiene 1 tabla de paginación
    1 tabla de paginación -> (de 1 a muchos) páginas
    1 página tiene 1 marco
*/

typedef struct
{
	int prevAlloc;
	int nextAlloc;
	bool isFree;
} t_heap_meta_data;

typedef struct
{
	int page_id;
	int frame_id;
	bool presencia;
	bool modificado;
	bool uso;
	int timestamp;
	int process_id;
} t_page;

typedef struct
{
	int process_id;
	bool libre; // ocupado libre = 0 ?
	int page_id;
	int frame_id;
	int timestamp;		   //para LRU
	int timestamp_entrada; //para FIFO
} t_entrada_tlb;
t_list *entradas_tlb;
typedef struct
{
	int process_id;
	t_list *pages; //[t_page]
	int amount_tlb_miss;
	int amount_tlb_hit;
} t_page_table;

//Variables globales
void *stream_memoria;
t_bitarray *bitmap;
t_list *pageTables; //[t_page_table]
int MARCOS_POR_CARPINCHO;
int conexionSWAMP;
t_list *reserved_bitmap; //[int]
int memory_timestamp;

//Config-logger
t_log *logger;
t_config *config;
pthread_mutex_t mutex_mem_alloc;
int puntero_clock;

//contadores de tlb hit/miss

int tlb_hit_totales; /*  */
int tlb_miss_totales;

#endif