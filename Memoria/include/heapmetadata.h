#ifndef HEAPMETADATA_H_
#define HEAPMETADATA_H_

#include "TAD.h"
#include "tad_memoria.h"
#include "config_servicio.h"
// #include "sd_pageswamp.h"
// #include "config_servicio.h"
// #include <commons/bitarray.h>
// #include "socket_client.h"
// #include "server_utils.h"
// #include <commons/collections/list.h>

void* serializarHeapMetaData(t_heap_meta_data item);
t_heap_meta_data deserializarHeapMetaData(int start);
t_heap_meta_data crearHeapMetaData(int prevAlloc, int nextAlloc, bool isFree);
void guardarHMDMP(int inicioMP, int tamanio, void* stream_heapmetadata);
int getMallocSize(int recorrido, t_heap_meta_data heapMetaData);
void guardarHMD(t_heap_meta_data hmd, int inicio);
t_heap_meta_data* getNextHeapMetaData(int offset);

extern void* stream_memoria;

#endif