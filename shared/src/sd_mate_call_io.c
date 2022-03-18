#include "../include/sd_mate_call_io.h"

t_package ser_msj_code_mete_call_io(t_mate_call_io data_buffer){
    t_package paquete;

    int tam_buffer = sizeof(uint32_t) + 2*sizeof(int) + data_buffer.tam_nombre_io + data_buffer.tam_mensaje;
    paquete.tam_buffer = tam_buffer;
    paquete.buffer = malloc(tam_buffer);
    paquete.cod_operacion = CALL_IO;
    int offset = 0;

    memcpy(paquete.buffer, &data_buffer.id_carpincho, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    memcpy(paquete.buffer+offset, &data_buffer.tam_nombre_io, sizeof(int));
    offset += sizeof(int);

    memcpy(paquete.buffer+offset, data_buffer.nombre_io, data_buffer.tam_nombre_io);
    offset+=data_buffer.tam_nombre_io;

    memcpy(paquete.buffer+offset, &data_buffer.tam_mensaje, sizeof(int));
    offset+=sizeof(int);

    memcpy(paquete.buffer+offset, data_buffer.mensaje, data_buffer.tam_mensaje);

    return paquete;
}

t_mate_call_io des_msj_code_mete_call_io(t_package paquete){
    t_mate_call_io data;
    int offset = 0;

    memcpy(&data.id_carpincho, paquete.buffer, sizeof(uint32_t));
    offset+=sizeof(uint32_t);

    memcpy(&data.tam_nombre_io, paquete.buffer+offset, sizeof(int));
    offset+=sizeof(int);

    data.nombre_io = malloc(data.tam_nombre_io+1);
    memcpy(data.nombre_io, paquete.buffer+offset, data.tam_nombre_io);
    data.nombre_io[data.tam_nombre_io]='\0';
    offset+=data.tam_nombre_io;

    memcpy(&data.tam_mensaje, paquete.buffer+offset, sizeof(int));
    offset+=sizeof(int);

    data.mensaje = malloc(data.tam_mensaje+1);
    memcpy(data.mensaje, paquete.buffer+offset, data.tam_mensaje);
    data.mensaje[data.tam_mensaje]='\0';

    return data;
}