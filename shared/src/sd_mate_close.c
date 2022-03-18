#include "../include/sd_mate_close.h"

t_package ser_msj_code_mete_close(t_mate_close data_buffer){
    t_package paquete;
    paquete.tam_buffer = sizeof(uint32_t);
    paquete.buffer = malloc(sizeof(uint32_t));
    paquete.cod_operacion = MATE_CLOSE;

    memcpy(paquete.buffer, &data_buffer.id_carpincho, sizeof(uint32_t));

    return paquete;
}

t_mate_close des_msj_code_mete_close(t_package paquete){
    t_mate_close data;

    memcpy(&data.id_carpincho, paquete.buffer, sizeof(uint32_t));

    return data;
}