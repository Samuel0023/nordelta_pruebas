#include "../include/sd_res_memoria.h"

t_package ser_respuesta_memoria(int code_res, op_code code);

t_package ser_respuesta_memoria(int code_res, op_code code){
    t_package paquete;
    int tam_buffer = sizeof(int);
    paquete.buffer = malloc(tam_buffer);
    paquete.cod_operacion = code; 
    paquete.tam_buffer = tam_buffer;

    memcpy(paquete.buffer , &code_res , sizeof(int));
    
    return paquete;
}

int des_respuesta_memoria(t_package paquete){
    int res;

    memcpy(&res, paquete.buffer, sizeof(int));

    return res;
}

t_package ser_res_memalloc(int code_res){
    t_package paquete;
    paquete = ser_respuesta_memoria(code_res, MEMALLOC);

    return paquete;
}

t_package ser_res_memfree(int code_res){
    t_package paquete;
    paquete = ser_respuesta_memoria(code_res, MEMFREE);

    return paquete;
}

 t_package ser_res_memwrite(int code_res){
    t_package paquete;
    paquete = ser_respuesta_memoria(code_res, MEMWRITE);

    return paquete;
}

t_package ser_res_memread(int code_res){
    t_package paquete;
    paquete = ser_respuesta_memoria(code_res, MEMREAD);

    return paquete;
}