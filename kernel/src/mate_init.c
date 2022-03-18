#include "mate_init.h"

int inicializar_carpincho_nuevo(double estimacion_anterior_config, int socket_carpincho){
    PCB_Carpincho * carpincho_nuevo = malloc(sizeof(PCB_Carpincho));

    pthread_mutex_lock(&MX_ID_PROCESO);
    carpincho_nuevo->id = id_proceso;
    id_proceso++;
    pthread_mutex_unlock(&MX_ID_PROCESO);

    carpincho_nuevo->estimacion_anterior = estimacion_anterior_config;
    carpincho_nuevo->rafaga_anterior = 0;
    //valor en 0??
    carpincho_nuevo->inicio_ejecucion_time = 0;
    carpincho_nuevo->inicio_listo_time = 0;
    carpincho_nuevo->socket_res = socket_carpincho;

    carpincho_nuevo->estado = NEW;
    carpincho_nuevo->sem_retenidos = list_create();
    pthread_mutex_lock(&MX_NEW);
    list_add(list_NEW, carpincho_nuevo);
    pthread_mutex_unlock(&MX_NEW);

    sem_post(&cantidad_procesos_nuevos_suspendidos);
    return carpincho_nuevo->id;
}


void enviar_id(int socket, int id_carpincho){
    t_package paquete;
    paquete = ser_respuesta_id(id_carpincho);

    sendMessage(paquete, socket);
}
