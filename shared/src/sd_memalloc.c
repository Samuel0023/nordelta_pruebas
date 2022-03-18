#include "../include/sd_memalloc.h"

t_package ser_msj_code_memalloc(t_msj_memalloc data_alloc){
    t_package paquete;
    int tam = sizeof(int) * 2;
    paquete.buffer = malloc(tam);// 2 enteros
    paquete.tam_buffer = tam;
    paquete.cod_operacion = MEMALLOC;
    int offset = 0;

    memcpy(paquete.buffer, &data_alloc.pid_carpincho, sizeof(int));
    offset+= sizeof(int);

    memcpy(paquete.buffer+offset, &data_alloc.tam_alloc, sizeof(int));

    return paquete;
}

t_msj_memalloc des_msj_code_memalloc(t_package paquete){
    t_msj_memalloc data_alloc;

    memcpy(&data_alloc.pid_carpincho, paquete.buffer, sizeof(int));
    paquete.buffer+= sizeof(int);

    memcpy(&data_alloc.tam_alloc, paquete.buffer, sizeof(int));

    return data_alloc;
}