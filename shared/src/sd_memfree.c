#include "sd_memfree.h"

t_package ser_msj_code_memfree(t_msj_memfree data_free){
    t_package paquete;
    paquete.buffer = malloc(8);// 2 enteros
    paquete.tam_buffer = 8;
    paquete.cod_operacion = MEMFREE;
    int offset = 0;

    memcpy(paquete.buffer, &data_free.pid_carpincho, sizeof(int));
    offset+= sizeof(int);

    memcpy(paquete.buffer+offset, &data_free.dir_logica, sizeof(int));

    return paquete;
}

t_msj_memfree des_msj_code_memfree(t_package paquete){
    t_msj_memfree data;
    int offset = 0;

    memcpy(&data.pid_carpincho, paquete.buffer,  sizeof(int));
    offset+= sizeof(int);

    memcpy(&data.dir_logica, paquete.buffer+offset, sizeof(int));

    return data;
}