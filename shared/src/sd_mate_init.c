#include "sd_mate_init.h"

t_carpincho_init des_carpincho_init(t_package paquete){
    t_carpincho_init data;
    void *buffer = paquete.buffer;
    memcpy( &data.estimacion_anterior, buffer, sizeof(double));
    //de ser necesario agregar campos necesario para mate_init
    return data;
}

t_package ser_respuesta_id(int pid){
    t_package paquete = ser_entero(pid, MATE_INIT);
    return paquete;
}

int des_respuesta_id(t_package paquete){
    int pid = des_entero(paquete);
    return pid;
}