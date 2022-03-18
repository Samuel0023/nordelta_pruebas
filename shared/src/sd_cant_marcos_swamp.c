#include "sd_cant_marcos_swamp.h"

void peticion_cant_marcos_swamp(int conexionSWAMP){
    if(conexionSWAMP<0) return;
    //Paquete con cod de op para swamp
    t_package paquete;
    paquete.buffer = malloc(1);
    paquete.cod_operacion = CANT_MARCOS_SWAMP;
    paquete.tam_buffer = 1;

    //envia mensaje a swamp
    sendMessage(paquete, conexionSWAMP);
}

t_package ser_cant_marcos_swamp(t_cant_marcos_swamp data_buffer){
    t_package paquete;
    int tam_buffer = sizeof(int);
    paquete.buffer = malloc(tam_buffer);
    paquete.cod_operacion = CANT_MARCOS_SWAMP;
    paquete.tam_buffer = tam_buffer;
    int offset = 0;

    memcpy(paquete.buffer + offset, &data_buffer.respuesta_cantidad_marcos, sizeof(int));
 
    return paquete;
}

int respuesta_cant_marcos_swamp(int conexionSWAMP){
    if(conexionSWAMP<0) return -1;
    int cant_marcos_swamp;
    t_package paquete;

    recv(conexionSWAMP, &(paquete.cod_operacion), sizeof(op_code), MSG_WAITALL);
    paquete.cod_operacion = CANT_MARCOS_SWAMP;
    recv(conexionSWAMP, &(paquete.tam_buffer), sizeof(int), MSG_WAITALL);
    paquete.buffer = malloc(paquete.tam_buffer);
    recv(conexionSWAMP, paquete.buffer, paquete.tam_buffer, MSG_WAITALL);

    return cant_marcos_swamp = (int) paquete.buffer; 
}
