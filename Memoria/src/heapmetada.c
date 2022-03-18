#include "heapmetadata.h"

void* serializarHeapMetaData(t_heap_meta_data item){
    void* streamHMD = malloc(9); //tamaño obligatorio por enunciado
    int offset = 0;

    memcpy(streamHMD, &item.prevAlloc, sizeof(int));
	offset += sizeof(int);

    memcpy(streamHMD+offset, &item.nextAlloc, sizeof(int));
    offset += sizeof(int);

    memcpy(streamHMD+offset, &item.isFree, sizeof(bool));
 
    return streamHMD;
}

t_heap_meta_data deserializarHeapMetaData(int start){
    t_heap_meta_data heapMetaData;

    void *buffer = stream_memoria;
    buffer += start;

    memcpy( &heapMetaData.prevAlloc, buffer, sizeof(int));
    buffer += sizeof(int);

    memcpy( &heapMetaData.nextAlloc, buffer, sizeof(int));
    buffer += sizeof(int);

    memcpy( &heapMetaData.isFree, buffer, sizeof(int));
    buffer += sizeof(int);

    return heapMetaData;
}

t_heap_meta_data crearHeapMetaData(int prevAlloc, int nextAlloc, bool isFree){
    t_heap_meta_data heapMetaData;
    heapMetaData.prevAlloc = prevAlloc;
    heapMetaData.nextAlloc = nextAlloc;
    heapMetaData.isFree = isFree;

    return heapMetaData;
}

void guardarHMDMP(int inicioMP, int tamanio, void* stream_heapmetadata){
    int offset = inicioMP;
    memcpy(stream_memoria+offset, stream_heapmetadata, tamanio);
}

int getMallocSize(int recorrido, t_heap_meta_data heapMetaData){
    int TAMANIO_PAGINA = get_tamanio_pagina();
    int limiteMaximo = getcantMarcosSWAMP() * TAMANIO_PAGINA;
    
    int limite = heapMetaData.nextAlloc;
	if(heapMetaData.nextAlloc == NULL)
		limite = limiteMaximo; 
	int size = limite - 9 - recorrido;
    return size;
}		

void guardarHMD(t_heap_meta_data hmd, int inicio){
    void* stream = serializarHeapMetaData(hmd);
    guardarHMDMP(inicio, 9, stream);
    free(stream);
}

t_heap_meta_data* getNextHeapMetaData(int offset){
    int contador = 0;
    t_heap_meta_data* nextHeapMetaData = malloc(sizeof(t_heap_meta_data));
    t_heap_meta_data heapMetaData;
    do{
        heapMetaData = deserializarHeapMetaData(offset);
        int malloc_size = getMallocSize(offset, heapMetaData);
        offset = 9 + malloc_size;
        if(contador == 1){
            nextHeapMetaData->isFree = heapMetaData.isFree;
            nextHeapMetaData->prevAlloc = heapMetaData.prevAlloc;
            nextHeapMetaData->nextAlloc = heapMetaData.nextAlloc;
            break;
        }
        contador++;
    }while(heapMetaData.nextAlloc != NULL);
    return nextHeapMetaData;
}