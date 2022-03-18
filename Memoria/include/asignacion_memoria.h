#ifndef ASIGNACION_MEMORIA_H_
#define ASIGNACION_MEMORIA_H_

#include "TAD.h"
#include "tad_memoria.h"
#include "sd_pageswamp.h"
#include "config_servicio.h"
#include <commons/bitarray.h>
#include "socket_client.h"
#include "server_utils.h"
#include "bitmap_utils.h"
#include "heapmetadata.h"
#include <commons/collections/list.h>

int asignMemory(int alloc_size, int process_id);
void case_with_empty_pages(
    int alloc_size, int process_id, int FRAMES_PER_PROCESS, char* ASSIGNATION_TYPE,
    t_page_table* pageTable, int PAGE_SIZE
);
void case_with_not_empty_pages(
    int alloc_size, int process_id, int FRAMES_PER_PROCESS, char* ASSIGNATION_TYPE,
    t_page_table* pageTable, int PAGE_SIZE
);
#endif