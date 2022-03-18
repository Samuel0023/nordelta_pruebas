#include "../include/respuesta_memoria.h"

void enviar_respuesta_memalloc(int socket, int estado_res){
    t_package paquete;
    paquete = ser_res_memalloc(estado_res);

    sendMessage(paquete, socket);
}

void enviar_respuesta_memfree(int socket, int estado_res){
    t_package paquete;
    paquete = ser_res_memfree(estado_res);

    sendMessage(paquete, socket);
}

void enviar_respuesta_memread(int socket, int estado_res){
    t_package paquete;
    paquete = ser_res_memread(estado_res);

    sendMessage(paquete, socket);
}

void enviar_respuesta_memwrite(int socket, int estado_res){
    t_package paquete;
    paquete = ser_res_memwrite(estado_res);

    sendMessage(paquete, socket);
}