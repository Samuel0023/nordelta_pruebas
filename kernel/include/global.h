#ifndef KERNEL
#define KERNEL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/config.h>
#include <pthread.h>
#include "../include/config_utils.h"
#include "../include/config_kernel.h"
#include <commons/collections/list.h>

t_list *list_NEW;
t_list *list_READY;
t_list *list_EXEC;
t_list *list_SUSPENDED_READY;
t_list *list_SUSPENDED_BLOCKED;
t_list *list_BLOCKED;
t_list *lista_EXIT;

//Semaforos para las listas de estados
pthread_mutex_t MX_NEW;
pthread_mutex_t MX_READY;
pthread_mutex_t MX_EXEC;
pthread_mutex_t MX_SUSPENDED_READY;
pthread_mutex_t MX_SUSPENDED_BLOCKED;
pthread_mutex_t MX_BLOCKED;
pthread_mutex_t MX_EXIT;

//Semaforos para multiprogramación y multiprocesamiento
pthread_mutex_t MX_PCB;
sem_t limite_grado_multiprocesamiento;
sem_t limite_grado_multiprogramacion;
sem_t cantidad_procesos_nuevos_suspendidos;
sem_t cant_procesos_listos;
sem_t cantidad_procesos_suspendidos;

//General
t_log *logger_kernel;

//Kernel
t_list* semaphores_kernel;          //[t_semaphore]
t_list* devices;                    //[t_device]

int id_proceso; //id autoincrementable
pthread_mutex_t MX_ID_PROCESO;

t_list *asignaciones; 
t_list *peticiones; 
#endif /* SERVER_H_ */
