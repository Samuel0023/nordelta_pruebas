#include "sd_entero.h"

t_package ser_entero(int value, op_code cod_operacion){
    t_package paquete;
    int tam_buffer = sizeof(int);
    paquete.buffer = malloc(tam_buffer);
    paquete.cod_operacion = cod_operacion; 
    paquete.tam_buffer = tam_buffer;

    memcpy(paquete.buffer , &value , sizeof(int));
    
    return paquete;
}

int des_entero(t_package paquete){
    int value;
    void *buffer = paquete.buffer;
    memcpy(&value, buffer, sizeof(int));
    
    return value;
}