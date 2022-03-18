#include "../include/sd_res_semaforo_kernel.h"

t_package ser_respuesta_kernel_sem(int code_res, op_code code);

t_package ser_respuesta_kernel_sem(int code_res, op_code code){
    t_package paquete;
    int tam_buffer = sizeof(int);
    paquete.buffer = malloc(tam_buffer);
    paquete.cod_operacion = code; 
    paquete.tam_buffer = tam_buffer;

    memcpy(paquete.buffer , &code_res , sizeof(int));
    
    return paquete;
}

int des_respuesta_kernel_sem(t_package paquete){
    int res;

    memcpy(&res, paquete.buffer, sizeof(int));

    return res;
}

t_package ser_res_mate_sem_init(int code_res){
    t_package paquete = ser_respuesta_kernel_sem(code_res, SEM_INIT);

    return paquete;
}

t_package ser_res_mate_sem_wait(int code_res){
    t_package paquete = ser_respuesta_kernel_sem(code_res, SEM_WAIT);

    return paquete;
}
t_package ser_res_mate_sem_post(int code_res){
    t_package paquete = ser_respuesta_kernel_sem(code_res, SEM_POST);

    return paquete;
}

t_package ser_res_mate_sem_destroy(int code_res){
    t_package paquete = ser_respuesta_kernel_sem(code_res, SEM_DESTROY);

    return paquete;
}