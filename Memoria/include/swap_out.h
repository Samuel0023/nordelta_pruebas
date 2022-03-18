#ifndef SWAP_OUT_H_
#define SWAP_OUT_H_

#include "TAD.h"
#include "tad_memoria.h"
#include "sd_pageswamp.h"
#include "config_servicio.h"
#include <commons/bitarray.h>
#include "socket_client.h"
#include "server_utils.h"
#include "bitmap_utils.h"
#include "heapmetadata.h"
#include "sd_lectura_pagina.h"
#include <commons/collections/list.h>

extern int conexionSWAMP;
extern t_list *reserved_bitmap;
extern t_list *entradas_tlb;

void swap_out_process(int process_id);

#endif