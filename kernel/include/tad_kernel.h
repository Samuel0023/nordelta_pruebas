#ifndef TAD_KERNEL_H
#define TAD_KERNEL_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <netdb.h>
#include <commons/bitarray.h>
#include <commons/collections/list.h>
#include "TAD.h"
#include "../include/config_kernel.h"

typedef struct
{
    int id;
    status_carpincho estado;
    double estimacion_anterior;
    double rafaga_anterior;
    long inicio_ejecucion_time;
    long inicio_listo_time;
    int socket_res;

    t_list* sem_retenidos; //[id_semaphores]
    int motivo_retencion;   //id_semaphore
} PCB_Carpincho;

typedef struct
{
    char* name;
	int duration;
    t_list* pcbs;   //[PCB_Carpincho]
    sem_t pcb_pendientes;           
    pthread_mutex_t pcbs_mutex;         
} t_device;


typedef struct{
    int id;
	char* name_sem;
    int valor_inicial;
	int value;
    sem_t sem;
}t_sem_global;

#endif