#include "../include/respuesta_kernel_sem.h"

void enviar_respuesta_mate_sem_init(int socket, int estado_res){
    t_package paquete;
    paquete = ser_res_mate_sem_init(estado_res);

    sendMessage(paquete, socket);
}

void enviar_respuesta_mate_sem_wait(int socket, int estado_res){
    t_package paquete;
    paquete = ser_res_mate_sem_wait(estado_res);

    sendMessage(paquete, socket);
}
void enviar_respuesta_mate_sem_post(int socket, int estado_res){
    t_package paquete;
    paquete = ser_res_mate_sem_post(estado_res);

    sendMessage(paquete, socket);
}
void enviar_respuesta_mate_sem_destoy(int socket, int estado_res){
    t_package paquete;
    paquete = ser_res_mate_sem_destroy(estado_res);

    sendMessage(paquete, socket);
}