#include "sd_memwrite.h"

t_package ser_contenido_memoria(t_msj_memwrite data, op_code code);

/* ******** */
t_package ser_contenido_memoria(t_msj_memwrite data, op_code code){
    t_package paquete;

    int size = 3*sizeof(int) + data.tam_contenido;
    paquete.buffer = malloc(size);
    paquete.tam_buffer = size;
    paquete.cod_operacion = code;
    int offset = 0;

    memcpy(paquete.buffer, &data.pid_carpincho, sizeof(int));
    offset+= sizeof(int);

    memcpy(paquete.buffer+offset, &data.dir_logica, sizeof(int));
    offset+= sizeof(int);

    memcpy(paquete.buffer+offset, &data.tam_contenido, sizeof(int));
    offset+= sizeof(int);

    memcpy(paquete.buffer+offset, data.contenido, data.tam_contenido);

    return paquete;
}

/* ******** */

t_package ser_msj_code_memwrite(t_msj_memwrite data){
    t_package paquete = ser_contenido_memoria(data, MEMWRITE);
    
    return paquete;
}

t_package ser_msj_code_memread(t_msj_memwrite data){
    t_package paquete = ser_contenido_memoria(data, MEMREAD);
    
    return paquete;
}

t_msj_memwrite des_msj_code_memwrite(t_package paquete){
    t_msj_memwrite data;
    int offset = 0;

    memcpy(&data.pid_carpincho, paquete.buffer,  sizeof(int));
    offset+= sizeof(int);

    memcpy(&data.dir_logica, paquete.buffer+offset, sizeof(int));
    offset+= sizeof(int);

    memcpy(&data.tam_contenido, paquete.buffer+offset, sizeof(int));
    offset+= sizeof(int);

    data.contenido=malloc(data.tam_contenido);
    memcpy(data.contenido, paquete.buffer+offset, data.tam_contenido);

    return data;
}